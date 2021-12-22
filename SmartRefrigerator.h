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
    tuple<vector<Recipe>, double, double> makeCourse(vector<pair<Recipe, double>>);
    bool possibleDish(Recipe & r, map<string, int> & m);
    double calculateExpirationScore(Recipe & s);
    double calculateExpirationScore(map<string, int> &m);
    void addRecipeFromFile();
    void showRecipe();
    void recommendMealCourses();
    int menuSelect();

private:
    recipeVec recipes;
};

#endif
