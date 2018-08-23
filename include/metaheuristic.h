#pragma once


#include "data.h"
#include "cost.h"
#include <vector>
#include <string>

class Metaheuristic {

/** CONSTRUCTORS */
public:
    Metaheuristic(std::vector<Cost*> *costs, std::string output_folder = "") :
            costs_(costs),
            iterations_(0),
            idle_iterations_(0),
            time_(0)
    {
        output_folder_ = output_folder;
    }



    /** METHODS */
public:
    /** run the methaheuristic */
    virtual void run() = 0;

    /** print information */
    virtual void printInfo() = 0;


    /** VARIABLES */
public:

    /** the current iteration number */
    int iterations_;

    /** the current number of idle iterations */
    int idle_iterations_;

    /** the cost components */
    std::vector<Cost*> *costs_;

    /** time take by the metaheuristic */
    double time_;

    /** output folder */
    std::string output_folder_;
};

