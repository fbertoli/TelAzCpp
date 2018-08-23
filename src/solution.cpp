#include "solution.h"
#include "configParameters.h"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <string>

using namespace std;


/** CONSTRUCTORS */

Solution::Solution(Data *data):
	data_{data},
	timetable_(data->days_, vector<vector<Employee*>>(data->shift_types_.size(), vector<Employee *>())),
	schedule_(data->employees_.size(), vector<ShiftType*>(data->days_, nullptr)),
	weekends_(data->employees_.size(), vector<int>(data_->weeks_, 0)),
	shift_count_(data->employees_.size(), vector<vector<int>>(data_->weeks_, vector<int>(data_->shift_types_.size(),0))),
	available_shifts_(data->employees_.size(),vector<int>(data_->weeks_, 0))
{
	// fill available_shifts_
	for (auto & em : data_->employees_)
		fill(available_shifts_[em.id_].begin(), available_shifts_[em.id_].end(), em.shift_per_week_);
}



/** ------------------------------------------------------------------------------------------------ */

void Solution::removeShift(Employee *employee, int day)
{
	// safety check
	assert(employee && "employee is nullptr");

	// check if employee has an assigned shift on given day
	ShiftType *shift{schedule_[employee->id_][day]};
	if (shift)
	{
		// remove from timetable_
		vector<Employee *> &vec = timetable_[day][shift->id_];
		vec.erase(std::remove(vec.begin(), vec.end(), employee), vec.end());

		// remove from schedule
		schedule_[employee->id_][day] = nullptr;

		// if shift was on the weekend adjust weekends_
		if (isOnWeekend(day))
		{
			--weekends_[employee->id_][getWeek(day)];

			// safety check
			assert(weekends_[employee->id_][getWeek(day)] >= 0);
		}

		// update shift_count_
		--shift_count_[employee->id_][getWeek(day)][shift->id_];

		// update available_shifts_
		++available_shifts_[employee->id_][getWeek(day)];
	}
}


/** ------------------------------------------------------------------------------------------------ */

void Solution::removeAllShifts(Employee *employee)
{
	for (int day = 0; day < data_->days_; day++)
	{
		removeShift(employee, day);
	}
}



/** ------------------------------------------------------------------------------------------------ */

void Solution::assignShift(Employee *employee, int day, ShiftType *shift)
{
	// safety check
	assert(employee && "employee is nullptr");
	assert(shift && "shift is nullptr");

	// safety check
	if (schedule_[employee->id_][day])
	{
		cout << "ERROR!! Assigning shift to employee on already busy day.";
		throw -1;
	}



	// add to timetable if not already there
	vector<Employee *> &vec = timetable_[day][shift->id_];
	cout << int(vec.size()) << ", " << int(timetable_[day][shift->id_].size()) << endl;
	if (vec.empty() or find(vec.begin(), vec.end(), employee) != vec.end())
		timetable_[day][shift->id_].push_back(employee);

	// add to schedule
	schedule_[employee->id_][day] = shift;

	// if shift is on the weekend adjust weekends_
	if (isOnWeekend(day))
	{
		weekends_[employee->id_][getWeek(day)]++;
	}

	// update shift_count_
	++shift_count_[employee->id_][getWeek(day)][shift->id_];

	// update available_shifts_
	--available_shifts_[employee->id_][getWeek(day)];
}


/** ------------------------------------------------------------------------------------------------ */

void Solution::write(std::string file_name, std::vector<Cost *> *costs)
{
	ofstream out_file;
	out_file.open (file_name);
	string indent = " ";

	out_file << "TIMETABLE" << endl;
	for (int day = 0; day < data_->days_; day++)
	{
		out_file << "DAY " << day << endl;
		for (auto it = data_->day_available_shifts_[day].cbegin(); it !=  data_->day_available_shifts_[day].cend(); it++)
		{
			if (it->second) {
				out_file << indent << format(it->first->name_, 11) << ": ";
				out_file << accumulate(timetable_[day][it->first->id_].begin(), timetable_[day][it->first->id_].end(), string(),
								   [](string &ss, Employee *e)
								   {
									   return ss.empty() ? e->name_ : ss + ", " + e->name_;
								   });
				out_file << endl;
			}
		}
	}
	out_file << endl << endl;
	out_file << "PERSONAL SCHEDULE" << endl;
	for (auto & employee : data_->employees_)
	{
		out_file << employee.name_ << endl;
		for (int day = 0; day < data_->days_; day++)
		{
			if (day % 7 == 0)
				out_file << indent << "- " << "week " << day / 7 << " -" << endl;
			if (schedule_[employee.id_][day])
			{
				out_file << indent << indent << format(printDate(data_->getDate(day)), 8) << ": " << schedule_[employee.id_][day]->name_ << endl;
			}
		}
		out_file << endl;
	}
}


/** ------------------------------------------------------------------------------------------------ */

void Solution::read(string file_name)
{
	// read
	vector<vector<string> > lines{readLinesFile(file_name)};

	// auxiliary variable
	int day;
	Employee *employee;
	ShiftType *shift;

	// parse
	vector<vector<string>>::iterator line = lines.begin();
	while (line != lines.end()) {
		if (line.empty())
			continue;

		if (line->at(0).compare("TIMETABLE") == 0)
		{
			++line;
			while (line != lines.end())
			{
				if (line->at(0).compare("DAY") == 0)
					day = stoi(line->back());
				else
				{
					shift = data_->shift_name_map_[removeChar(&(line->at(0))];
					employee = data_->employee_name_map_[removeChar(&(line->at(2))];
				}
				++line;
			}
		}
		++line;
	}

	for (auto &line : lines) {
		if (line.size() == 0)
			continue;
		if ()
		removeChar(&line.front(), ',');
	}


}


/** ------------------------------------------------------------------------------------------------
 * CHECKING */


bool Solution::checkShiftCoverage()
{
	bool result = true;
	for (int day = 0; day < data_->days_; day++) {
		for (auto it = data_->day_available_shifts_[day].cbegin(); it != data_->day_available_shifts_[day].cend(); it++) {
			if (it->second and it->first->mandatory_ and timetable_[day][it->first->id_].empty()) { // if shift is available, is mandatory and no one is assigned to it
				result = false;
				cout << "Shift " << it->first->name_ << " is not covered on " << printDate(data_->getDate(day)) << endl;
			}
		}
	}
	return result;
}

/** ------------------------------------------------------------------------------------------------ */

bool Solution::checkWeekends() {
	bool result = true;
	for (auto &employee : data_->employees_)
	{
		int weekends_in_a_row = 0;
		for (int week = 0; week < data_->weeks_; week++)
		{
			if (weekends_[employee.id_][week] > 0)
			{
				++weekends_in_a_row;
				if (weekends_[employee.id_][week] > 1 and (not employee.full_time_ ))
				{
					cout << employee.name_ << " worked the entire weekends on week " << week << endl;
					result = false;
				}
			}
			else
				weekends_in_a_row = 0;

			// check number of weekends in a row
			if (weekends_in_a_row > config_parameters.max_weekends_in_a_row_) {
				cout << employee.name_ << " worked " << weekends_in_a_row << " weekends in a row starting on week "
					 << week << endl;
				result = false;
			}
		}
	}
	return result;
}

/** ------------------------------------------------------------------------------------------------ */

bool Solution::checkAdjacency()
{
	bool result = true;
	for (auto &employee : data_->employees_)
	{
		for (auto it =  schedule_[employee.id_].cbegin(); distance(it, schedule_[employee.id_].cend()) > 1; it++)
		{
			if (*it and *(next(it)) and not data_->adjacency_matrix_[(*it)->id_][(*next(it))->id_])
			{
				cout << "Employee " << employee.name_ << "breaks adjancency matrix by working shift " << (*it)->name_;
				cout << " on day " << distance(schedule_[employee.id_].cbegin(),it) << " and shift " << (*next(it))->name_ << " on next day" << endl;
				result = false;
			}
		}
	}
	return result;
}


/** ------------------------------------------------------------------------------------------------ */

bool Solution::checkEmployeeShiftWeekly()
{
	bool result = true;
	for (auto &employee : data_->employees_) {
		for (int week = 0; week < data_->weeks_; week++)
		{
			// check number of shift per week
			if (accumulate(shift_count_[employee.id_][week].begin(), shift_count_[employee.id_][week].end(), 0) != employee.shift_per_week_)
			{
				result = false;
				cout << employee.name_ << " has " << accumulate(shift_count_[employee.id_][week].begin(), shift_count_[employee.id_][week].end(), 0);
				cout << " shifts assigned on week " << week << endl;
			}

			// check maximum shifts
			for (auto &shift : data_->shift_types_)
			{
				if (shift_count_[employee.id_][week][shift.id_] > shift.max_per_week_)
				{
					result = false;
					cout << employee.name_ << " has " << shift_count_[employee.id_][week][shift.id_] << " " << shift.name_ << " shifts assigned on week " << week << endl;
				}
			}
		}
	}
	return result;
}


