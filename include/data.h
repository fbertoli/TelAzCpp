#pragma once

#include <string>
#include <vector>
#include <ctime>
#include <map>
#include "employee.h"
#include "shiftType.h"

class Data {
	// CONSTRUCTOR - DESTRUCTOR
public:
	Data();

	// METHODS
private:
	// reading info from files
	void readHolidayFile(std::string file_path);
	void readShiftFile(std::string file_path);
	void readEmployeeFile(std::string file_path);

	// populate the adjancecy vars
	void createAdjacencyMatrix();

public:
	// printing
	void printEmployees();
	void printInfo();
	void printShiftTypes();

	// VARIABLES
public:
	// number of days and weeks in the horizon
	int days_, weeks_;

	// start and end date
	std::tm start_date_, end_date_;

	// [day] = True if day is holiday
	std::vector<bool> is_holiday_;

	// all operators
	std::vector<Employee> employees_;

	// all shift types. The position will be identified with the type
	std::vector<ShiftType> shift_types_;

	// map shifts names to shifts ref
	std::map<std::string, ShiftType *> shift_name_map_;

	// [day][shift] = shifts is available on a day
	std::vector<std::map<ShiftType *, bool>> day_available_shifts_;

};
