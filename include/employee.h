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

    // VARIABLES
public:
	// name of employee
	std::string name_;

	// id used to identify operator
	int id_;

	// whether he/she has a ccnl contract
	bool ccnl_;

	// how many shift per week he/she has to do
	int shift_per_week_;

	// defined unavailabilities. [day][shift_type] = bool
	std::vector<std::map<ShiftType *, bool >> unavailable_;


};