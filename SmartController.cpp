#include "SmartController.h"
#include "Controller.h"
#include "Food.h"
#include <algorithm>

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
            shelves.push_back(Shelf(foodSize.second));//create new shelf with height of the food
            FoodPtr newFood = new FoodInFridge(food_to_insert, 0, totalHeight);
            if(foodList.find(name) == foodList.end()){//add to foodList
                vector<FoodPtr> v{newFood};
            }
            else{
                foodList[name].push_back(newFood);
            }
            return true;
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
    /**
     * ===============================================
     * ======== TODO: Implement this function ========
     * ===============================================
     */
    return false;
}