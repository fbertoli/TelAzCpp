#pragma once
#include <iostream>
#include <ctime>
#include <string>
#include <vector>

class ShiftType {
    // CONSTRUCTORS
public:
    ShiftType();

    // METHODS
public:
    friend std::ostream& operator<<(std::ostream &out, const ShiftType shift);

    /** comparison operator */
    friend bool operator== (const ShiftType &s1, const ShiftType &s2);
    friend bool operator!= (const ShiftType &s1, const ShiftType &s2);

    // VARIABLES
public:
    int id_; 				// equals the position of shift_type in data.shift_types_
    bool mandatory_;		// whether the shift is mandatory
    std::string name_;		// the name as given in the input
    std::tm start_time_, end_time_;
    std::vector<bool> on_weekdays_; 	// [day] = can the turn take place on day
    int number_employees_;	// if not 0, the number of employees to have in this turn
    int min_operators_;		// if not 0, the minimum number of employees to be assigned to this turn
    int max_operators_;		// if not 0, the maximum number of employees to be assigned to this turn
    int max_per_week_;      // if not 0, the max number of times an employee can take the shift on a week
};