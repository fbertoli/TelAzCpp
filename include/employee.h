#pragma once

#include <string>
#include <vector>
#include <map>

class ShiftType;


class Employee {
	// COSTRUCTOR
public:
	Employee() {};
	//Employee(std::string name, bool ccnl, int shift_per_week) : name_{ name }, ccnl_{ ccnl }, shift_per_week_{ shift_per_week } {}

	// METHODS
    friend std::ostream& operator<< (std::ostream &out, const Employee &employee);

	/** comparison operator */
	friend bool operator== (const Employee &e1, const Employee &e2);
	friend bool operator!= (const Employee &e1, const Employee &e2);

    // VARIABLES
public:
	// name of employee
	std::string name_;

	// id used to identify operator
	int id_;

	// whether he/she has a ccnl contract
	bool ccnl_;

	// whether is a full-time employee
	bool full_time_;

	// how many shift per week he/she has to do
	int shift_per_week_;

	// define unavailabilities. [day][shift.id] = bool
	std::vector<std::vector<bool >> unavailable_;


};