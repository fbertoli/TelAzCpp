#include <iostream>
#include "employee.h"
#include "shiftType.h"

using namespace std;

ostream& operator<< (ostream &out, const Employee &employee)
{
    // Since operator<< is a friend of the Point class, we can access Point's members directly.
    out << "" << employee.name_ << ".";
    out << " " << ((employee.full_time_) ? " full-time" : "part-time");
    if (employee.ccnl_)
        out << " [ccnl]";
    out << ", shifts per week = " << employee.shift_per_week_ << endl;
    out << "Unavailable on: ";
    for (int i = 0; i < employee.unavailable_.size(); i++)
    {
        for (auto item = employee.unavailable_[i].cbegin(); item != employee.unavailable_[i].cend(); item++)
        {
            if (*item)
                out << "(" <<  i << ", shift: " << distance(employee.unavailable_[i].cbegin(), item) << "), ";
        }
    }

    return out;
}
 