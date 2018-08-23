#pragma once

#include "utilities.h"
#include <string>
#include <vector>
#include <iostream>

/** This (abstract) class implements an abstract source of cost. It is purely an interface */


class Solution;
class Data;

class Cost {

    /** CONSTRUCTORS */
public:
    Cost(Data &data) : data_(data) {}
    virtual ~Cost() {};


    /** METHODS */
public:

    /** compute the cost of the given solution and update solution.cost_map_ */
    virtual double computeCost(Solution *solution) = 0;

    /** get the name of the cost */
    std::string getName() {return name_;};

    /** print the cost info */
    virtual void printCostInfo() = 0;



    /**VARIABLES */
public:
    /** reference to data of the instance */
    Data &data_;

    /** name of the cost (to debug) */
    std::string name_;

    /** id used to identify the type of child class */
    int id_;

    /** auxiliary variable used in computation */
    double delta_, day_delta_;

    /** the penalty */
    double penalty_;

    /** only for printing */
    int format_ = 20;
};