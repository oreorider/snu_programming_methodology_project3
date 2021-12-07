#include "NormalController.h"
#include "Controller.h"
#include "Food.h"


/**
 * Store a given food object into the refrigerator. 
 * 
 * This function should follows the 'Modified Shelf First Fit (Shelf-FF) algorithm, which is given in the PPT slide.
 * Your implemented algorithm should find the proper position of the new food object.
 * The new food object should be initialized in the heap memory and referenced from both food_list and shelves.
 * 
 * @param name The name of the food to be stored
 * @param size width and height of the food
 * @param exp the expire date of the food
 * @return true if inserting the food for both food_list and shelves has been succeed
 */
bool NormalController::stackFood(const string name, intPair foodSize, int exp)
{
    Food food_to_insert(name, foodSize, exp);
    bool inserted=false;
    //check if space avail on shelves
    int avail_x, avail_y;
    int avail_space_start_x, avail_space_end_x;
    int shelf_height=0;
    for(auto &each_shelf : shelves){//for each shelf
        for(auto &each_foodptr : each_shelf.vec){//for each foodptr/foodinfridge that is in the shelf
            avail_space_start_x=each_foodptr->getPos().first+(each_foodptr->getSize().first);
            avail_space_end_x;

            auto nextElement=*(&each_foodptr+1);
            if(&nextElement==&each_shelf.vec.back()){//if last element of vector
                avail_space_end_x=size.first;//set end_x the 'wall' of fridge
            }
            else{
                avail_space_end_x=nextElement->getPos().first;
            }
            avail_x=avail_space_start_x-avail_space_end_x;
            if(avail_x>foodSize.first){//enough space in x, assume height of shelf>height of food always
                //insert food into shelf @ avail_space_start_x
                FoodPtr new_food=new FoodInFridge(food_to_insert, avail_space_end_x, shelf_height);
                each_shelf.vec.push_back(new_food);//food added to shelf
                if(foodList.find(name)==foodList.end()){//food does not exist in foodlist
                    vector<FoodPtr> v{new_food};
                    foodList.insert(make_pair(name, v));//add the new food to foodlist
                }
                else{//food already exsists in foodList
                    foodList[name].push_back(new_food);//append foodptr to foodlist
                }
                inserted=true;
            }
        }
        shelf_height+=each_shelf.height;
    }
    if(inserted){
        return true;
    }
    else{
        return false;
    }
}


/**
 * Pop a food with shortest expire date from both foodList and shelves.
 * This function does not return the object.
 * The pop process should follows the algorithm in PPT slide.
 * 
 * @param food_name a name of food to be popped
 * @return 
 */
bool NormalController::popFood(const string food_name)
{
    if(foodList.find(food_name)==foodList.end()){//food not in foodList.
        return false;
    }
    //assuming now the food is inside the foodList
    auto v = foodList[food_name];
    /**
     * ===============================================
     * ======== TODO: Implement this function ========
     * ===============================================
     */
    return false;
}
