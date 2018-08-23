#pragma once

#include "solutionOperator.h"
#include "solution.h"
#include "data.h"
#include "random.h"
#include <vector>
#include "boost/tuple/tuple.hpp"



class RandomInsertion : public SolutionOperator
{
    /** CONSTRUCTORS */
public:
    RandomInsertion(Data *data);

    /** METHODS */
public:
    /** repair a solution on the given week */
    void repair(Solution *solution);

    /** destroy part of a solution on the given week */
    void destroy(Solution *solution) {};

private:
    /** scan solution to populate the unassigned shift variables */
    void populateUnassignedShifts(Solution *solution, bool weekend);

    /** count the unassigned shifts left */
    inline int countUnassignedShifts();




    /** VARIABLES */
private:
    /** Internal variable to shuffle employees */
    std::vector<Employee *> employees_;

    /** [week] = vector of manatory shifts (day, shift_type) that have to be yet to be covered. */
    std::vector<std::vector<boost::tuple<int, ShiftType *>>> unassigned_shift_;

    /** internal variable to shuffle the weekends */
    std::vector<int> weekend_indexes_;

    /** auxiliary variables */
    int day;
    ShiftType *shift;
    Employee *employee;



};