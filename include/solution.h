#pragma once

#include "data.h"
#include "employee.h"
#include "shiftType.h"
#include "cost.h"

class Solution
{
	// CONSTRUCTOR - DESTRUCTOR
public:
	Solution(Data *data);

	// METHODS
public:
    /** remove employee from shift on given day */
    void removeShift(Employee *employee, int day);

    /** remove employee from all shifts */
    void removeAllShifts(Employee *employee);

    /** assign a shift to an employee. Employee MUST not have any shift assigned on that day*/
    void assignShift(Employee *employee, int day, ShiftType *shift);

   /** check weekend conditions */
	inline bool weekendCheck(Employee *employee, int day, ShiftType *shift);

	/** check if employee is available and not already assigned a shift on day */
	inline bool isFreeOnDay(Employee *employee, int day, ShiftType *shift);

	/** check number of shifts in week  */
	inline bool checkShiftsLoad(Employee *employee, int week);

	/** check if employee has already assigned max shift of same time */
	inline bool checkShiftsTypes(Employee *employee, int week, ShiftType *shift);

	/** check all the above at once */
	inline bool canTakeShift(Employee *employee, int day, ShiftType *shift);


	/** read a solution from file */
	void read(std::string file_name);

	/** write the Solution to the specified file */
    void write(std::string file_name, std::vector<Cost *> *costs = nullptr);

    /** check functions. Return true if it is feasible*/
    bool checkShiftCoverage();
    bool checkWeekends();
    bool checkAdjacency();
    bool checkEmployeeShiftWeekly();

    /** check consistency of variables */
    bool checkConsistency() {return true;};


	// VARIABLES
public:
	// the data the solution refers to
	Data * data_;

	// the timetable. [day][shift.id] = employees assigned to shift  (note: all shift are considered for all days, not only the available ones)
	std::vector<std::vector<std::vector<Employee *>>> timetable_;

	// the personal schedule. [employee.id_][day] = Shift_type assigned
	std::vector<std::vector<ShiftType *>> schedule_;

	// [employee.id_][week] =  number of shifts employee works on the weekend
	std::vector<std::vector<int>> weekends_;

	// weekly shift count. [employee.id][week][shift.id] = number of shift type employed is assigned to during the week.
	std::vector<std::vector<std::vector<int>>> shift_count_;

	// [employee.id][week] = number of shift employee still has to be assigned to
	std::vector<std::vector<int>> available_shifts_;
};



/** ------------------------------------------------------------------------------------------------ */

inline bool Solution::weekendCheck(Employee *employee, int day, ShiftType *shift)
{
	return 	(day % 7 < 5) or	// if day is not on weekend
			(
				(getWeek(day) == 0 or weekends_[employee->id_][getWeek(day) - 1] == 0) // previous weekend
				and
				(getWeek(day) == (data_->weeks_-1) or weekends_[employee->id_][getWeek(day) + 1] == 0)  // next weekend
				and
				(employee->full_time_ or weekends_[employee->id_][getWeek(day)] == 0) // number of shifts on this weekend
			);

}

/** ------------------------------------------------------------------------------------------------ */

inline bool Solution::isFreeOnDay(Employee *employee, int day, ShiftType *shift)
{
	return (!schedule_[employee->id_][day]) and !employee->unavailable_[day][shift->id_];
}

/** ------------------------------------------------------------------------------------------------ */

inline bool Solution::checkShiftsLoad(Employee *employee, int week)
{
	return available_shifts_[employee->id_][week] > 0;
}

/** ------------------------------------------------------------------------------------------------ */

inline bool Solution::checkShiftsTypes(Employee *employee, int week, ShiftType *shift)
{
	return shift_count_[employee->id_][week][shift->id_] < shift->max_per_week_;
}


/** ------------------------------------------------------------------------------------------------ */

inline bool Solution::canTakeShift(Employee *employee, int day, ShiftType *shift)
{
	return isFreeOnDay(employee,day,shift) and checkShiftsLoad(employee, getWeek(day)) and
			checkShiftsTypes(employee, getWeek(day), shift) and weekendCheck(employee,day, shift);
}
