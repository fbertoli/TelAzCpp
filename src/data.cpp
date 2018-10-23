#include "data.h"
#include "configParameters.h"
#include "utilities.h"
#include <ctime>
#include <numeric>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;


/** ------------------------------------------------------------------------------------------------
 * CONSTRUCTOR */

Data::Data() :
	start_date_(config_parameters.start_date_),
	end_date_(config_parameters.end_date_),
	employees_(),
	shift_types_(),
	shift_name_map_(),
	employee_name_map_()
{
	days_ = difftime(mktime(&end_date_), mktime(&start_date_)) / (60*60*24) + 1;
	weeks_ = days_ / 7;
	is_holiday_ = vector<bool>(days_, false);
	day_available_shifts_ = vector<map<ShiftType *, bool>>(days_);

	// read files
    readHolidayFile(config_parameters.file_holiday_);
    readShiftFile(config_parameters.file_shift_);
	readEmployeeFile(config_parameters.file_employee_);

	// create adjacency relations
    createAdjacencyMatrix();
}



/** ------------------------------------------------------------------------------------------------ */

void Data::readHolidayFile(string file_path)
{
	// read
	vector<vector<string> > lines{readLinesFile(file_path)};

	// parse
	tm holiday;
	for (auto &line : lines) {
		if (line.size() == 0)
			continue;
		removeChar(&line.front(), ',');
		readDate(&line.front(), &holiday);

		int day_number = difftime(mktime(&holiday), mktime(&start_date_)) / (60 * 60 * 24);
		if ((day_number < days_) && (day_number >= 0))
			is_holiday_[day_number] = true;
	}
	
}


/** ------------------------------------------------------------------------------------------------ */

void Data::readEmployeeFile(string file_path)
{
	// read
	vector<vector<string>> lines(readLinesFile(file_path));

	// parse
	vector<vector<string>>::iterator line = lines.begin();
	while (line != lines.end())
	{
		if (line->size())
		{
			// create object
			employees_.push_back(Employee{});
			Employee &employee = employees_.back();

			// name
			// TODO improve name concatenation to add space in between name and surname
			employee.name_ = accumulate(line->begin(), line->end(), string(""));
			line++;

			//id
			employee.id_ = employees_.size() -1;

			//ccnl contract
			if (line->at(0).compare("ccnl") == 0)
			{
				employee.ccnl_ = true;
				line++;
			}
			
			// shifts per week
			employee.shift_per_week_ = stoi(line->back());
			employee.full_time_ = (employee.shift_per_week_ >= 5);
			line++;

			// default unavailable map
			employee.unavailable_= vector<vector<bool>>(days_, vector<bool >(shift_types_.size(), false));
			for (int i = 0; i < days_; i++)
			{
				for (auto it = day_available_shifts_[i].cbegin(); it!= day_available_shifts_[i].cend(); ++it)
					employee.unavailable_[i][it->first->id_] = not it->second; // set as unavailable shifts that are not available at all on that day
			}
			
			//read optional line until you find an empty line
			while (line->size())
			{
				// non available days
				if (line->at(0).compare("giorni") == 0 && line->at(1).compare("non") == 0 && line->at(2).compare("disponibile") == 0)
				{
					for (auto token = line->begin() + 4; token != line->end(); token++)
					{
						tm day;
						// remove commas and read date
						removeChar(&*token, ',');
						readDate(&*token, &day);

						int day_number = difftime(mktime(&day), mktime(&start_date_)) / (60 * 60 * 24);
						if ((day_number < days_) && (day_number >= 0))
						{
							for (auto it = day_available_shifts_[day_number].cbegin(); it!= day_available_shifts_[day_number].cend(); ++it)
								employee.unavailable_[day_number][it->first->id_] = true;
						}
					}
				}
				// turni non disponibile
				else if (line->at(0).compare("turni") == 0 && line->at(1).compare("non") == 0 && line->at(2).compare("disponibile") == 0)
				{
					vector<string>::iterator token = line->begin() + 3;
					while ((++token) != line->end())
					{
						string shift_type = *token;
						advance(token, 2);
						tm day;
						// remove commas and read date
						removeChar(&*token, ',');
						readDate(&*token, &day);

						int day_number = difftime(mktime(&day), mktime(&start_date_)) / (60 * 60 * 24);
						if ((day_number < days_) && (day_number >= 0))
						{
							employee.unavailable_[day_number][shift_name_map_[shift_type]->id_] = true;
						}
					}
				}
				// turni specifici
				else if (line->at(0).compare("turni") == 0 && line->at(1).compare("specifici") == 0)
				{
					// change default implementation
					for (int i = 0; i < days_; i++)
					{
						for (auto it = day_available_shifts_[i].cbegin(); it!= day_available_shifts_[i].cend(); ++it)
							employee.unavailable_[i][it->first->id_] = true;
					}
					// read specific shifts
					vector<string>::iterator token = line->begin() + 2;
					while ((++token) != line->end())
					{
						ShiftType *shift = shift_name_map_[*token];
						advance(token, 2);
						// get day of the week (no need to remove comma as function only uses first 3 letters
						int day_of_week = translateDay(&*token);
						for (int week = 0; week < weeks_; week++)
						{
							employee.unavailable_[week*7 + day_of_week][shift->id_] = false;
						}

					}
				}

				// increment line
				++line;
			}
		}
		++line;
	}

	// add to map
	for (auto &em : employees_)
		employee_name_map_[string(em.name_)] = &em;
}

/** ------------------------------------------------------------------------------------------------ */



void Data::readShiftFile(string file_path) {
	// read
	vector<vector<string>> lines(readLinesFile(file_path));

	// parse
	vector<vector<string>>::iterator line = lines.begin();
	while (line != lines.end())
	{
		bool reading_mandatory_shifts = true;
		if (line->size() > 0)
		{
			// check if we are reading section title
			if (line->size() == 1)
			{
				if (line->at(0).compare("OBBLIGATORI") == 0)
					reading_mandatory_shifts = true;
				else if (line->at(0).compare("INTERMEDI") == 0)
					reading_mandatory_shifts = false;
				++line;
				continue;
			}

			// create shift
			shift_types_.push_back(ShiftType());
			ShiftType *shift =  &(shift_types_.back());

			// parse name
			shift->name_ = line->at(0);

			// get id
			shift->id_ = shift_types_.size() - 1;

			//mandatory shift
			shift->mandatory_ = reading_mandatory_shifts;

			// parse start and end time
			std::istringstream st(line->at(1));
			st >> get_time(&shift->start_time_, "%H:%M");
			std::istringstream et(line->at(2));
			et >> get_time(&shift->end_time_, "%H:%M");

			// if there are no optional fields, go to next line
			if (line->size() < 4)
			{
				++line;
				continue;
			}

			// parse optional fields
			bool reading_excluded_days = false;
			vector<string>::iterator token = line->begin() + 3;
			while ((token) != line->end())
			{
				if (token->compare("minimo_operatori") == 0)
				{
					advance(token, 2);
					removeChar(&*token, ',');
					shift -> min_operators_ = stoi(*token);
				}
				else if (token->compare("massimo_operatori") == 0)
				{
					advance(token, 2);
					removeChar(&*token, ',');
					shift -> max_operators_ = stoi(*token);
				}
				else if (token->compare("numero_operatori") == 0)
				{
					advance(token, 2);
					removeChar(&*token, ',');
					shift -> number_employees_ = stoi(*token);
				}
				else if (token->compare("per_settimana") == 0)
				{
					advance(token, 2);
					removeChar(&*token, ',');
					shift -> max_per_week_ = stoi(*token);
				}
				else if (token->compare("giorni") == 0)
				{
					shift->on_weekdays_ = vector<bool>(7, false);
					reading_excluded_days = false;
				}
				else if (token->compare("escluso") == 0)
				{
					reading_excluded_days = true;
				}
				else if (token->compare("=") != 0)
				{
					// no need to remove comma from day name as translateDay uses only first three letters
					shift->on_weekdays_[translateDay(&*token)] = (not reading_excluded_days);
				}
				advance(token, 1);
			}
		}
		++line;
	}

	// create name map
	for (auto &shift : shift_types_) {
		shift_name_map_[shift.name_] = &shift;
	}

	// create day_available_shifts_
	for (int day =0; day < days_; day++)
	{
		day_available_shifts_[day] = map<ShiftType *, bool>();
		for (auto &shift : shift_types_)
		{
			day_available_shifts_[day][&shift] = (shift.on_weekdays_[day % 7]);
		}
	}

}


/** ------------------------------------------------------------------------------------------------ */


void Data::createAdjacencyMatrix()
{
    // TODO implement the creation of the adjacency matrix
}


/** ------------------------------------------------------------------------------------------------
 * PRINTING */
void Data::printEmployees() {
	cout << "employees: " << endl;
	for (auto employee : employees_)
	{
		cout << employee << endl << endl;
	}
}

/** ------------------------------------------------------------------------------------------------ */


void Data::printInfo()
{
	cout << "GENERAL INFO" << endl;
	cout << "days = " << days_ << ", weeks = " << weeks_;
	cout << ", start_date = " << printDate(start_date_);
	cout << ", end_date = " << printDate(end_date_);
	cout << endl;
	cout << "holidays: ";
	for (int i = 0; i < days_; i++)
	{
		if (is_holiday_[i])
		{
			tm day = start_date_;
			day.tm_mday += i;
			mktime(&day);
			cout << printDate(day);
			cout << ", ";
		}
	}
	cout << endl;
}

/** ------------------------------------------------------------------------------------------------ */


void Data::printShiftTypes()
{
	cout << "SHIFT TYPES" << endl;
	for (auto &s : shift_types_)
		cout << s << endl;

	cout << endl;
	cout << "Map: " << endl;
	for (auto it = shift_name_map_.cbegin(); it != shift_name_map_.cend(); ++it )
	{
		cout << it->first  << ": " << it->second->name_  << std::endl ;
	}

	cout << endl;
	cout << "Available shifts: " << endl;
	for (int i = 0; i < days_; i++)
	{
		std::cout << i << ": ";
		for (auto &shift : shift_types_)
		{
			if (day_available_shifts_[i][&shift])
				cout << shift.name_ << ", ";
		}
		cout << endl;
	}

}

/** ------------------------------------------------------------------------------------------------ */



tm Data::getDate(int day)
{
    tm date = start_date_;
    date.tm_mday += day;
    mktime(&date);
    return date;
}