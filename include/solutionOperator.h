#pragma once

#include "data.h"
#include "solution.h"
#include <string>
#include <unordered_map>


/** This class represent an operator that can destroy and repair a solution. */


class SolutionOperator {
    /** CONSTRUCTORS */
public:
    SolutionOperator(Data *data): data_(data) {};

    /** METHODS */
public:
    /** repair a solution on the given week */
    virtual void repair(Solution *solution) = 0;

    /** destroy part of a solution on the given week */
    virtual void destroy(Solution *solution) = 0;

//    /** remove a set of shifts (determined through to to_remove_) from solution on given week. */
//    virtual void removeShifts(Solution *solution, int week);

    /** name of the move */
    std::string getName() {return name_;};



    /** VARIABLES */
public:
    Data *data_;
    std::string name_;

//    /** [shift.id] = if shift has to be removed */
//    std::vector<bool> to_remove_;


};

