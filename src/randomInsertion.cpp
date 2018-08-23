#include "randomInsertion.h"
#include "utilities.h"
#include <iterator>
#include <cassert>

using namespace std;

RandomInsertion::RandomInsertion(Data *data) :
    SolutionOperator(data),
    employees_(data_->employees_.size(), nullptr),
    unassigned_shift_(data_->weeks_, vector<boost::tuple<int, ShiftType *>>()),
    weekend_indexes_(data_->weeks_)
{
    // initialize employees_
    for (int i = 0; i < employees_.size(); i++) {
        employees_[i] = &(data_->employees_[i]);
    }

    // reserve space
    for (int week = 0; week < data_->weeks_; week++)
        unassigned_shift_[week].reserve(data_->shift_types_.size() * 5);

    // create weekend_indexes_
    iota(weekend_indexes_.begin(), weekend_indexes_.end(), 0);
}

/** ------------------------------------------------------------------------------------------------ */

inline int RandomInsertion::countUnassignedShifts()
{
    return accumulate(unassigned_shift_.begin(), unassigned_shift_.end(), 0,
               [](int &a, vector<boost::tuple<int, ShiftType *>> &vec) {
                   return a + int(vec.size());
               });
}


/** ------------------------------------------------------------------------------------------------ */

void RandomInsertion::populateUnassignedShifts(Solution *solution, bool weekend)
{
    // clear vectors
    for (auto &vec : unassigned_shift_)
        vec.clear();

    // insert unassigned shifts
    for (auto day_it = data_->day_available_shifts_.cbegin(); day_it != data_->day_available_shifts_.cend(); day_it++)
    {
        day = distance(data_->day_available_shifts_.cbegin(), day_it);
        if ((weekend and day > 4) or ((not weekend) and day < 5))
        {
            for (auto shift_it = day_it->cbegin(); shift_it != day_it->cend(); shift_it++)
            {
                if (shift_it->second and solution->timetable_[day][shift_it->first->id_].empty())
                {
                    unassigned_shift_[getWeek(day)].push_back(boost::make_tuple(day, shift_it->first));
                }
            }
        }
    }
}


/** ------------------------------------------------------------------------------------------------ */

void RandomInsertion::repair(Solution *solution)
{
    assert(solution && "got nullptr as argument");

    for (int i = 0; i< 2; i++)
    {
        // first cycle repair weekends, second repair weeks
        populateUnassignedShifts(solution, i == 0);

        // shuffle shifts once only
        for (auto &vec : unassigned_shift_)
            random_shuffle(vec.begin(), vec.end(), randomGenerator);

        // start loop
        while (true) {
            // if all shifts have been assigned
            if (all_of(unassigned_shift_.begin(), unassigned_shift_.end(),
                       [](vector<boost::tuple<int, ShiftType *>> &vec) { return vec.empty(); }))
                break;

            // shuffle
            random_shuffle(weekend_indexes_.begin(), weekend_indexes_.end(), randomGenerator);
            random_shuffle(employees_.begin(), employees_.end(), randomGenerator);

            // auxiliary variables to define employee to assign shift to, in case primary condition is not satisfied
            Employee *candidate_two(nullptr), *candidate_three(nullptr);

            for (int week : weekend_indexes_) {
                if (not unassigned_shift_[week].empty()) { // continue only if there are shifts to be assigned
                    // get shift info
                    day = unassigned_shift_[week].back().get<0>();
                    shift = unassigned_shift_[week].back().get<1>();

                    // loop over employees (starting from a random point) till you find an available one
                    int random_start(Random::integer(0, employees_.size()-1));
                    int counter = 0;
                    for (; counter < employees_.size(); counter++) {
                        employee = employees_[(random_start + counter) % int(employees_.size())];
                        if (solution->isFreeOnDay(employee,day,shift))
                        {
                            candidate_three = employee;
                            if (solution->checkShiftsLoad(employee,week))
                            {
                                candidate_two = employee;
                                if (solution->checkShiftsTypes(employee,week,shift) and solution->weekendCheck(employee,day,shift))
                                {
                                    solution->assignShift(employee,day,shift);
                                    goto shift_assigned;
                                }
                            }
                        }
                    }

                    // if we got here, it means no employee was chosen, pick an employee that has available shifts on the week
                    solution->assignShift(candidate_two, day, shift);
                    goto shift_assigned;

                    // if we got here, pick employee that is not already busy on the day
                    solution->assignShift(candidate_three, day, shift);
                    goto shift_assigned;

                    shift_assigned:
                    // remove shift from stack
                    unassigned_shift_[week].pop_back();
                }
            }
        }
    }
}