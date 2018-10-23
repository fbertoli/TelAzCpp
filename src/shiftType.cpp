#include "shiftType.h"
#include "utilities.h"

using namespace std;

ShiftType::ShiftType() :
        id_(0),
        mandatory_(true),
        name_(""),
        start_time_{},
        end_time_{},
        on_weekdays_(7, true),
        number_employees_(-1),
        min_operators_(0),
        max_operators_(100000),
        max_per_week_(0)
{}

std::ostream& operator<<(std::ostream &out, const ShiftType shift)
{
    out << "name = " << shift.name_ << ", id = " << shift.id_;
    out << ",time = " << printTime(shift.start_time_);
    out << " - " << printTime(shift.end_time_);
    out << endl;
    out << "\tweekdays = ";
    printVector(shift.on_weekdays_);
    out << endl;
    out << "\tnumber employees = (" << shift.number_employees_ << ", " << shift.min_operators_ << ", " << shift.max_operators_ << ")" << endl;
    return out;
}

/** comparison operator */
bool operator== (const ShiftType &s1, const ShiftType &s2) {return s1.id_ == s2.id_;}
bool operator!= (const ShiftType &s1, const ShiftType &s2) {return s1.id_ != s2.id_;}