#ifndef __SMARTREFRIGERATOR_H_
#define __SMARTREFRIGERATOR_H_
#include <map>

#include "Food.h"
#include "Recipe.h"
#include "Refrigerator.h"
#include "SmartController.h"


typedef vector<Recipe> recipeVec;

class SmartRefrigerator : public Refrigerator {
public:
    SmartRefrigerator();
    bool possibleDish(Recipe & r, map<string, int> & m);
    int calculateExpirationScore(Recipe & s);
    int calculateExpirationScore(Recipe & s, map<string, int> &m);
    void addRecipeFromFile();
    void showRecipe();
    void recommendMealCourses();
    int menuSelect();

private:
    recipeVec recipes;
};

#endif
