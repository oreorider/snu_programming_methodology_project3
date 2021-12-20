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

    std::string line;
    std::vector<std::string> recipe;
    while (std::getline(recipe_list, line))
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
        this->recipes.push_back({ recipe.at(0), ingredients, std::stoi(recipe.at(recipe.size() - 1)) });
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

void SmartRefrigerator::recommendMealCourses()

{
    // Preprocessing: Sort all food vectors by expiration score, higher score at start of vector.
    // nlogn complexity
    for (auto iter = this->foodList.begin(); iter != this->foodList.end(); ++iter)
        std::sort(iter->second.begin(), iter->second.end(), greater_equal_key_foods());
    std::sort(this->recipes.begin(), this->recipes.end(), greater_equal_key_recipes());
    // Preprocessing Finished.

    
    // Greedy Algorithm for Meal Course Selection.
    std::vector<std::string> candidateRecipes;
    for (int i = 0; i < recipes.size() && candidateRecipes.size() < 4; ++i)
    {
        // Can we make this recipe?
        bool isPossible = true;
        auto& recipe = recipes.at(i);
        auto ingredients = recipe.getIngredients();

        std::map<std::string, int> dp;

        for (auto& ingredient : ingredients)
        {
            int needed = ingredient.second + dp.at(ingredient.first);
            auto& foodVector = this->foodList.at(ingredient.first);
            for (auto& food : foodVector)
            {
                needed -= food->getSize().first + food->getSize().second;
                if (needed <= 0)
                {
                    needed = 0;
                    break;
                }
            }

            if (needed > 0)
            {
                isPossible = false;
                break;
            }
        }

        if (isPossible) // We have enough ingredients for this recipe
        {
            // Update DP for future recipe calculations.
            for (auto& ingredient : ingredients)
                dp[ingredient.first] += ingredient.second;
            candidateRecipes.push_back(recipes.at(i).getName());
        }
    }

    if (candidateRecipes.size() < 3)
    {
        // Not enough ingredients for any course.
        std::cout << "Not enough foods for any course." << std::endl;
    }
    else if (candidateRecipes.size() == 3)
    {
        // Enough ingredients for only one course.
        
    }
    else
    {
        // Enough ingredients for three courses.
        
    }
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