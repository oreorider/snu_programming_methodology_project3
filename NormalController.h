#ifndef __NORMALCONTROLLER_H_
#define __NORMALCONTROLLER_H_
#include "Controller.h"

#define MAX_HEIGHT 6
typedef FoodInFridge *FoodPtr;
typedef map<string, vector<FoodPtr>> foodListType;

class NormalController : public Controller
{
public:
    NormalController(intPair &size_, foodListType &foodList_) : Controller(size_, foodList_) {
        int numberShelves = size.second / maxHeight;
        for(int i=0; i<numberShelves; i++){
            shelves.push_back(Shelf(maxHeight));
        }
    }
    void sort_shelves();
    bool stackFood(const string, intPair, int);
    bool popFood(const string);

private:
    //assumption
    int maxHeight = MAX_HEIGHT;
};

#endif
