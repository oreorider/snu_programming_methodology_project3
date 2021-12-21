#include "SmartController.h"
#include "Controller.h"
#include "Food.h"
#include <algorithm>


void SmartController::sort_shelves(){
    int count=0;
    for(auto& shelf : shelves){
        //sort shelf.vec
        auto& vector = shelf.vec;
        int min=99;
        auto min_iter=vector.begin();
        for(auto it = vector.begin(); it!=vector.end(); it++){
            min = (*it)->getPos().first;
            min_iter=it;
            for(auto inner = it+1; inner!=vector.end(); inner++){
                if((*inner)->getPos().first < min){
                    min = ((*inner)->getPos().first);
                    min_iter = inner;
                }
            }          
            iter_swap(it, min_iter);
            //cout<<"swapped"<<endl;
        }
        cout<<"shelf "<<count<<" holds ";
        for(auto i : shelf.vec){
            cout<<i->getName()<<" ";
        }
        count+=1;
        cout<<endl;
    }
}


/**
 * Get maximum height among the foods in this shelf
 * 
 * @return maximum height of the foods.
 */
int SmartController::maxHeight(Shelf &shelf)
{
    if (shelf.vec.empty())
        return 0;
    auto comp = [](const FoodPtr a, const FoodPtr b)
    {
        return (a->getSize().second < b->getSize().second);
    };
    auto it_shelf = max_element(shelf.vec.begin(), shelf.vec.end(), comp);

    return (*it_shelf)->getSize().second;
}

/**
 * Store a given food object into the refrigerator
 * 
 * This function should follows the 'Shelf First Fit (Shelf-FF) algorithm, which is given in the PPT slide.
 * Your implemented algorithm should find the proper position of the new food.
 * Also, the given food object may in the stack memory. You should save it in the heap memory and reference
 * it from both food_list and shelves.
 * 
 * @param name The name of the food to be stored
 * @param size width and height of the food
 * @param exp the expire date of the food
 * @return true if inserting the food for both food_list and shelves has been succeed
 */
bool SmartController::stackFood(const string name, intPair foodSize, int exp)
{   
    sort_shelves();
    if(foodSize.first > size.first || foodSize.second > size.second){//if item is bigger than shelf size or fridge height
        return false;
    }
    Food food_to_insert(name, foodSize, exp);

    if(shelves.empty()){//if no shelves at all
        shelves.push_back(Shelf(foodSize.second));//create first shelf with height of object being added
        FoodPtr first_food = new FoodInFridge(food_to_insert, 0, 0);//create foodptr in heap
        shelves[0].vec.push_back(first_food);//add to shelf
        vector<FoodPtr> v{first_food};
        foodList.insert(make_pair(name, v));//add to foodList
        return true;
    }

    int shelf_level_counter=0;
    int totalHeight=0;
    for(auto &shelf : shelves){

        //if food taller than shelf AND top most shelf
        if(foodSize.second > shelf.height && shelf_level_counter == shelves.size() -1){
            int inputHeight = shelf.height;
            shelves.push_back(Shelf(foodSize.second));//create new shelf with height of the food
            FoodPtr newFood = new FoodInFridge(food_to_insert, 0, inputHeight+totalHeight);
            shelves.back().vec.push_back(newFood);
            if(foodList.find(name) == foodList.end()){//add to foodList
                vector<FoodPtr> v{newFood};
                foodList.insert(make_pair(name, v));
            }
            else{
                foodList[name].push_back(newFood);
            }
            return true;
        }

        if(foodSize.second > shelf.height){//if food is taller than this shelf
            totalHeight += shelf.height;
            shelf_level_counter++;
            continue;
        }


        //if food is smaller than shelf, same as normalcontroller
        int avail_x=0, avail_y=0;
        int avail_space_start_x=0, avail_space_end_x=0;
        int counter=0;
        for(auto &each_foodptr : shelf.vec){//for each foodptr/foodinfridge that is in the shelf
            if(counter==0){//if first item in shelf
                avail_space_start_x=each_foodptr->getSize().first;//starting avail x space starts from end of first item
            }
            else{//if not first item in shelf
                avail_space_start_x=each_foodptr->getPos().first+each_foodptr->getSize().first;//starting x pos starts
            }

            counter+=1;
            if(counter==shelf.vec.size()){//if last element of vector
                avail_space_end_x=size.first;//set end_x the 'wall' of fridge
            }
            else{
                avail_space_end_x=(*(&each_foodptr+1))->getPos().first;
            }
            avail_x=avail_space_end_x-avail_space_start_x;
            if(avail_x>=foodSize.first){//enough space in x, assume height of shelf>height of food always

                //insert food into shelf @ avail_space_start_x
                FoodPtr new_food=new FoodInFridge(food_to_insert, avail_space_start_x, totalHeight);//using heap memory
                shelf.vec.push_back(new_food);//food added to shelf
                
                if(foodList.find(name)==foodList.end()){//food does not exist in foodlist
                    vector<FoodPtr> v{new_food};
                    foodList.insert(make_pair(name, v));//add the new food to foodlist
                }
                else{//food already exsists in foodList
                    foodList[name].push_back(new_food);//append foodptr to foodlist
                }
                return true;
            }
        }
        shelf_level_counter+=1;
        totalHeight+=shelf.height;
    }

    if(foodSize.second + totalHeight < size.second){//if still space avail for the food to be inserted
        shelves.push_back(Shelf(foodSize.second));//create new shelf with height of the food
        FoodPtr newFood = new FoodInFridge(food_to_insert, 0, totalHeight);
        shelves.back().vec.push_back(newFood);
        if(foodList.find(name) == foodList.end()){//add to foodList
            vector<FoodPtr> v{newFood};
        }
        else{
            foodList[name].push_back(newFood);
        }
        return true;
    }

    return false;
}

/**
 * Pop food with shortest expire date from both foodList and shelves.
 * This function does not return the object.
 * The pop process should follows the algorithm in PPT slide.
 * 
 * @param food_name a name of food to be popped
 * @return 
 */
bool SmartController::popFood(const string food_name) // void
{
    auto food_name_iterator=findMinExpFood(food_name);
    if(food_name_iterator==foodList[food_name].end()){//food not found in shelves
        return false;
    }
    //if food found, and
    //assuming now the food is inside the foodList
    //find location of food from food_name_iterator

    auto food_location = (*food_name_iterator)->getPos();
    //get the food location in shelves
    int food_x = food_location.first;
    int food_y = food_location.second;

    //erase the food at the location from shelves
    int deleted_shelf_height;
    bool shelf_empty=false;
    auto shelf_iter = shelves.begin();
    for(auto &shelf_element : shelves){
        for(auto it = shelf_element.vec.begin(); it!=shelf_element.vec.end(); it++){
            if((*it)->getPos().second==food_y && (*it)->getPos().first==food_x){
                deleted_shelf_height=shelf_element.height;
                shelf_element.vec.erase(it);//erase from shelf
                foodList[food_name].erase(food_name_iterator);//erase from foodList
                if(shelf_element.vec.empty()){
                    shelf_empty=true;
                    shelves.erase(shelf_iter);
                }
                goto end;
            }
        }
        shelf_iter++;
    }

    end://food popped form shelves and foodList
        //must re - arange objects in shelves.

    //if shelf deleted, shift foods down
    if(shelf_empty){
        for(auto & shelf : shelves){
            for(auto & element : shelf.vec){
                if(element->getPos().second > food_y){//if food obj is above the deleted obj
                    auto newPos = make_pair(element->getPos().first, element->getPos().second-deleted_shelf_height);//shift food down the height of deleted shelf
                    element->setPos(newPos);
                }
            }
        }
    }

    //push all objects to the left
    for(auto &shelf : shelves){
        int new_x=food_x;
        for(auto &element : shelf.vec){
            if(element->getPos().second == food_y && element->getPos().first > food_x){//on the same level and to the right of removed object
                auto newPosition = make_pair(new_x, element->getPos().second);
                element->setPos(newPosition);
                new_x+=element->getSize().first;
            }
        }
    }

    //change height of all objects in shelf
    int totalHeight = 0;
    for(auto &shelf : shelves){
        int newHeight = maxHeight(shelf);
        shelf.height=newHeight;
        for(auto &element : shelf.vec){
            auto newLocation = make_pair(element->getPos().first, totalHeight);
            element->setPos(newLocation);
        }
        totalHeight += shelf.height;
    }

    
    return false;
}