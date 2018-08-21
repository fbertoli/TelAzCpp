#pragma once
#include "data.h"
#include "employee.h"


class Solution
{
	// CONSTRUCTOR - DESTRUCTOR
public:
	Solution(Data *data);

	// METHODS
public:


	// VARIABLES
public:
	// the data the solution refers to
	Data * data_;

	// the timetable. [day][shift (encoded as int)] = vector op employees pointers assiged to shift
	std::vector<std::vector<std::vector<*Employee>>> timetable_;

	// the personal schedule. [employee.id_][day] = Shift_type assigned
	std::vector<vector<*Shift_type>> schedule_;
};