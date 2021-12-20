#include "NormalController.h"
#include "Controller.h"
#include "Food.h"
#include <algorithm>
#include <vector>

void NormalController::sort_shelves(){
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
        for(auto i : shelf.vec){
            cout<<i->getName()<<" ";
        }
        cout<<endl;
    }
    
}

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
    sort_shelves();
    Food food_to_insert(name, foodSize, exp);
    bool inserted=false;
    //check if space avail on shelves
    int avail_x=0, avail_y=0;
    int avail_space_start_x=0, avail_space_end_x=0;
    int shelf_height=0;


    for(auto &each_shelf : shelves){//for each shelf
        avail_x=0; avail_y=0; avail_space_start_x=0; avail_space_end_x=0;
        if(each_shelf.vec.empty()){
            FoodPtr first_food = new FoodInFridge(food_to_insert, 0, shelf_height);
            each_shelf.vec.push_back(first_food);//add first food to shelf

            if(foodList.find(name)==foodList.end()){//food does not exist in foodlist
                    vector<FoodPtr> v{first_food};
                    foodList.insert(make_pair(name, v));//add the new food to foodlist
                }
                else{//food already exsists in foodList
                    foodList[name].push_back(first_food);//append foodptr to foodlist
                }
            return true;

        }
        int counter=0;
        for(auto &each_foodptr : each_shelf.vec){//for each foodptr/foodinfridge that is in the shelf
            if(counter==0){//if first item in shelf
                if(each_foodptr->getPos().first > foodSize.first){//if the food is not 밀착 with the left wall, and food fits
                    FoodPtr new_food=new FoodInFridge(food_to_insert, 0, shelf_height);//using heap memory
                    each_shelf.vec.push_back(new_food);//food added to shelf
                
                    if(foodList.find(name)==foodList.end()){//food does not exist in foodlist
                        vector<FoodPtr> v{new_food};
                        foodList.insert(make_pair(name, v));//add the new food to foodlist
                    }
                    else{//food already exsists in foodList
                        foodList[name].push_back(new_food);//append foodptr to foodlist
                    }
                    return true;
                }
                avail_space_start_x=each_foodptr->getSize().first;//starting avail x space starts from 0
            }
            else{//if not first item in shelf
                avail_space_start_x=each_foodptr->getPos().first+each_foodptr->getSize().first;//starting x pos starts
            }
            counter+=1;

            if(counter==each_shelf.vec.size()){//if last element of vector
                avail_space_end_x=size.first;//set end_x the 'wall' of fridge
            }
            else{
                avail_space_end_x=(*(&each_foodptr+1))->getPos().first;
            }
            avail_x=avail_space_end_x-avail_space_start_x;
            if(avail_x>=foodSize.first){//enough space in x, assume height of shelf>height of food always

                //insert food into shelf @ avail_space_start_x
                FoodPtr new_food=new FoodInFridge(food_to_insert, avail_space_start_x, shelf_height);//using heap memory
                each_shelf.vec.push_back(new_food);//food added to shelf
                
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
        shelf_height+=each_shelf.height;
    }
    return inserted;
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
    sort_shelves();
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
    for(auto &shelf_element : shelves){
        for(auto it = shelf_element.vec.begin(); it!=shelf_element.vec.end(); it++){
            if((*it)->getPos().second==food_y && (*it)->getPos().first==food_x){
                shelf_element.vec.erase(it);
            }
        }
    }
    
    //erase the food from foodList
    foodList[food_name].erase(food_name_iterator);
    return true;
}
