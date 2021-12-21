#include "SmartRefrigerator.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
#include <string>
#include <limits>

#include "Food.h"
#include "Recipe.h"

/**
 * A constructor of the class
 * SmartController should be initialized in this constructor
 */
SmartRefrigerator::SmartRefrigerator()
{
    controller = new SmartController(size, foodList);
};

/**
 * add recipes from a text file (.txt)
 * This function should add a new recipe object with the given info from the
 * file to the private variable recipe (vector)
 */
void SmartRefrigerator::addRecipeFromFile()
{
    string recipeFile;
    cout << "Enter your file name : ";
    cin >> recipeFile;
    ifstream recipe_list(recipeFile);

    if (!recipe_list)
    {
        cout << "There is no recipe file!" << endl;
        return;
    }

    string line;
    vector<string> recipe;
    while (getline(recipe_list, line))
    {
        int pos = 0;
        std::string token;
        while ((pos = line.find(' ')) != std::string::npos)
        {
            token = line.substr(0, pos);
            std::cout << token << std::endl;
            line.erase(0, pos + 1);
            recipe.push_back(token);
        }

        // tomatoPasta bacon 1 onion 1 pasta 1 tomato 1 / 7
        // Recipe(string, vector<strIntPair>, double);
        std::vector<std::pair<std::string, int>> ingredients;
        for (int i = 1; i < recipe.size() - 2; i += 2)
        {
            ingredients.push_back(std::make_pair(recipe.at(i), std::stoi(recipe.at(i+1))));
        }
        this->recipes.push_back(Recipe(recipe.at(0), ingredients, stoi(line)));
        recipe.clear();
    }
    recipe_list.close();
}

/**
 * show the current recipes of the smart refrigetrator
 * e.g. recipe name : [ingredient1, # of ingredient1]...[] / score 7
 *
 */
void SmartRefrigerator::showRecipe()
{
    recipeVec ::iterator iter;
    if (recipes.empty())
        cout << "There is no recipe yet." << endl;
    else
    {
        for (iter = recipes.begin(); iter != recipes.end(); ++iter)
        {
            cout << iter->getName() << " : ";
            for (int i = 0; i < iter->getIngredients().size(); i++)
            {
                cout << '[' << iter->getIngredients()[i].first << ',' << iter->getIngredients()[i].second << ']';
            }
            cout << " / score " << iter->getScore() << endl;
        }
    }
}

/**
 * recommend the best possible combination (with repetition) of three recipes
 * Below are some rules about the recommendation
 * 1) the number of each ingredient required by a combination should be less
 * than the number of each ingredient in the refrigerator in order to be
 * considered as a possible combination 2) the sum of recipe scores of a
 * combination should be normalized by the highest score sum of the possible
 * combination 3) the exp sum of the foods in the refrigerator considering of a
 * combination should be normalized by the longest exp (the greatest number)
 * from a possible combination
 */

struct greater_equal_key_foods
{
    inline bool operator() (const FoodPtr& food1, const FoodPtr& food2)
    {
        return (food1->getExp() >= food2->getExp());
    }
};

struct greater_equal_key_recipes
{
    inline bool operator() (const Recipe& recipe1, const Recipe& recipe2)
    {
        return (recipe1.getScore() >= recipe2.getScore());
    }
};

int SmartRefrigerator::calculateExpirationScore(Recipe & s){
    int expiration_score=0;
    vector<string> ingredient_names;
    for(auto & ingredient : s.getIngredients()){
        ingredient_names.push_back(ingredient.first);
        if(foodList.find(ingredient.first)==foodList.end()){
            return -999;
        }
        int avail_ingred = foodList[ingredient.first].size()-ingredient.second;
        if(avail_ingred < 0){
            return -999;
        }
        if(avail_ingred >= 0){
            auto vec = foodList[ingredient.first];
            for(int i=0; i<vec.size()-ingredient.second; i++){
                expiration_score+=vec[i]->getExp();//expiration score for that specific ingredient
            }
        }
    }
    for(auto & foodlist_element : foodList){
        for(auto & foodptr_element : foodlist_element.second){
            if(find(ingredient_names.begin(), ingredient_names.end(), foodptr_element->getName()) == ingredient_names.end()){//foodptr not part of ingredient list
                expiration_score+=foodptr_element->getExp();
            }
        }
    }
    return expiration_score;
}

int SmartRefrigerator::calculateExpirationScore(Recipe & s, map<string, int>& used_ingredients){
    int expiration_score;
    for(auto & ingredient : s.getIngredients()){
        if(foodList.find(ingredient.first)==foodList.end()){//ingredient not in foodlist
            return -999;
        }
        int avail_ingred;
        if(used_ingredients.find(ingredient.first)==used_ingredients.end()){//if no ingredients used so far
            avail_ingred = foodList[ingredient.first].size();
            
        }
        else{//if some ingredients used
            avail_ingred = foodList[ingredient.first].size() - used_ingredients[ingredient.first];
        }
        if(avail_ingred <= 0){//not enough ingrident in fridge
            return -999;
        }
        else{//if enough ingriedents in foodList
            if(used_ingredients.find(ingredient.first)==used_ingredients.end()){//if first time ing being used
                used_ingredients.insert(make_pair(ingredient.first, ingredient.second));
            }
            else{//if ing has been used in the past
                used_ingredients[ingredient.first]=avail_ingred-ingredient.second;
            }
            auto vec = foodList[ingredient.first];
            for(int i=0; i<vec.size() - used_ingredients[ingredient.first]; i++){
                expiration_score+=vec[i]->getExp();//expiration score for that specific ingredient
            }
        }
    }
    return expiration_score;
}

void SmartRefrigerator::recommendMealCourses()

{
    // Preprocessing: Sort all food vectors by expiration score, higher score at start of vector.
    // nlogn complexity
    for (auto iter = this->foodList.begin(); iter != this->foodList.end(); ++iter)
        std::sort(iter->second.begin(), iter->second.end(), greater_equal_key_foods());//foodList sorted by expr date
    std::sort(this->recipes.begin(), this->recipes.end(), greater_equal_key_recipes());//vector of recipies sorted by satisfaction score
    //std::sort()//vector of recipes sorted by expiration score
    // Preprocessing Finished.
    vector<int> expiration_scores;
    map <string, int> used_ingredients;
    for(auto &recipe : recipes){
        expiration_scores.push_back(calculateExpirationScore(recipe));
    }
    /*
    for(int i=0; i<expiration_scores.size(); i++){
        cout<<recipes[i].getName()<<" has exp score of "<<expiration_scores[i]<<endl;
    }
    cout<<endl;*/

    //find normalized satisfaction
    map<Recipe, double> normalized_satisfaction;
    double max_satisfaction = 0;
    for(auto & recipe : recipes){
        if(recipe.getScore() > max_satisfaction){
            max_satisfaction = recipe.getScore();
        }
    }
    for(auto & recipe : recipes){
        auto newelemnet = make_pair(recipe, recipe.getScore()/max_satisfaction);
        normalized_satisfaction.insert(newelemnet);
    }
    for(auto element : normalized_satisfaction){
        cout<<element.first.getName()<<" has normalized satisfaction score of "<<element.second<<endl;
    }
    cout<<endl;

    //find normalized expire score
    map<Recipe, double> normalized_expire;
    double max_expiration = 0;
    for(auto exp_score : expiration_scores){
        if(exp_score > max_expiration){
            max_expiration = exp_score;
        }
    }
    int exp_score_counter=0;
    for(auto & recipe : recipes){
        auto newelement = make_pair(recipe, expiration_scores[exp_score_counter]/max_expiration);
        exp_score_counter++;
        normalized_expire.insert(newelement);
    }
    for(auto element : normalized_expire){
        cout<<element.first.getName()<<" has normalized expire score of "<<element.second<<endl;
    }
    cout<<endl;

    //weigh 1/2 each, find total
    map<Recipe, double> weighted_score;
    for(int i=0; i<normalized_expire.size(); i++){
        auto newelement = make_pair(recipes[i], normalized_expire[recipes[i]]*0.5 + normalized_satisfaction[recipes[i]]*0.5);
        weighted_score.insert(newelement);
    }
    for(auto element : weighted_score){
        cout<<element.first.getName()<<" has weighted score of "<<element.second<<endl;
    }
    cout<<endl;
}

/**
 * base function of the smart refrigerator
 * User should type a number between 1 and 4 to call a function of the smart
 * regrigerator Then, this function utilizes the other functions of the smart
 * refrigerator as the user's input Below is the list of options that menuSelect
 * should support 1) display 2) insert food 2-1) insert a list of foods from a
 * text file (.txt) 2-2) insert a food directly 3) pop food 4) add recipe 5)
 * show recipe 6) recommend meals 7) end program Unless the user ends the
 * program ( 7) end program ), this function does not terminate and keeps
 * requiring the user to type a number
 * @return 0 when the user ends the program by typing else 1
 */

int SmartRefrigerator::menuSelect()
{
    int i;
    while (1)
    {
        cout << "************************************************************" << endl;
        cout << "************************   Menu   **************************" << endl;
        cout << "************************************************************" << endl;

        cout << "1. display" << endl
             << "2. insert food" << endl
             << "3. pop food" << endl
             << "4. add recipe" << endl
             << "5. show recipe" << endl
             << "6. recommend meal courses" << endl
             << "7. end program" << endl
             << "Select the function you want : ";
        cin >> i;
        if (i < 1 || i > 7){
            cout << "Type an integer from 1 to 7!" << endl;
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
        else {break;}
    }
    if (i == 7)
        return 0;
    else
    {
        switch (i)
        {
            case 1:
                display();
                break;
            case 2:
            {
                int j;
                while (1)
                {
                    cout << "1. Insert a list of foods from a text file" << endl;
                    cout << "2. Insert a food directly" << endl;
                    cout << "Enter how to insert your food : ";
                    cin >> j;
                    if (!(j == 1 or j == 2))
                    {
                        cout << "Type 1 or 2 only" << endl;
                        if (cin.fail())
                        {
                            cin.clear();
                            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                if (j == 1)
                    insertFoodFromFile();
                else
                    insertFoodDirectly();
                break;
            }
            case 3:
            {
                popFood();
                break;
            }
            case 4:
                addRecipeFromFile();
                break;
            case 5:
                showRecipe();
                break;
            case 6:
                recommendMealCourses();
                break;
        }
        return 1;
    }
}