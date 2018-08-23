#include "configParameters.h"
#include "utilities.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <iterator>
#include <cstdio>
#include <cstdlib>

using namespace std;

// Declare the global configuration class:
const ConfigParameters config_parameters("config/config.txt");



// Auixiliary variables that will be used to initialize ConfigParameters members
string file_holiday;
string file_shift;
string file_employee;
tm start_date;
tm end_date;
int min_rest_hours;
int max_weekends_in_a_row;

// keep track if we have already read the file
bool already_read_file = false;

// read the config file and initialize auxiliariy variables 
void readConfigFile(const string & file_path)
{
	// record we read file
	already_read_file = true;

	// read file
	string new_line;
	ifstream config_file(file_path);
	vector<vector<string>> lines;
	if (config_file.is_open()) {
		// save lines
		while (getline(config_file, new_line)) {
			istringstream iss(new_line);
			vector<string> tokens{ istream_iterator<string>{iss},
				istream_iterator<string>{} };
			lines.push_back(tokens);
		}
		config_file.close();
	}
	else {
		// throw exception
		throw runtime_error("Could not open config file");
	}

	// initialize auxiliary variables
	for (auto line : lines) {
		if (line.size() == 0)
			continue;
		
		// parse date
		if (line[0].compare("giorno") == 0)
		{
			if (line[1].compare("iniziale") == 0)
			{
				readDate(&line.back(), &start_date);
				if (start_date.tm_wday != 1)
				{
					cout << "Giorno iniziale non è un Lunedì" << endl;
					exit(EXIT_FAILURE);
				}

			}
			else if (line[1].compare("finale") == 0)
			{
				readDate(&line.back(), &end_date);
				if (end_date.tm_wday != 0)
				{
					cout << "Giorno finale non è una Domenica" << endl;
					exit(EXIT_FAILURE);
				}
			}
		}
		// parse files name
		else if (line[0].compare("file") == 0)
		{
			if (line[1].compare("turni") == 0)
			{
				file_shift = line.back();
			}
			else if (line[1].compare(0, 4, "fest") == 0)
			{
				file_holiday = line.back();
			}
			else if (line[1].compare("operatori") == 0)
			{
				file_employee = line.back();
			}
		}
		// parse min_rest_hour
		else if (line[0].compare("minimo") == 0 && line[1].compare("ore") == 0) 
		{
			min_rest_hours = stoi(line.back());
		}
		// parse max_weekends_in_a_row
		else if (line[0].compare("weekend") == 0 && line[1].compare("di") == 0 && line[1].compare("fila") == 0)
		{
			max_weekends_in_a_row = stoi(line.back());
		}
	}

}


// return the value of var by lookup
int getIntConfigVar(const string & config_file_path, string var_name)
{
	// check if we have already read file
	if (!already_read_file)
		readConfigFile(config_file_path);

	// return variable value
	if (var_name.compare("min_rest_hours_") == 0)
		return min_rest_hours;
	else if (var_name.compare("max_weekends_in_a_row") == 0)
		return max_weekends_in_a_row;
	return 0; // default return
}


// return the value of var by lookup
string getStringConfigVar(const string & config_file_path, string var_name)
{
	// check if we have already read file
	if (!already_read_file)
		readConfigFile(config_file_path);

		// return variable value
	if (var_name.compare("file_holiday_") == 0)
		return file_holiday;
	else if (var_name.compare("file_shift_") == 0)
		return file_shift;
	else if (var_name.compare("file_employee_") == 0)
		return file_employee;
	return ""; // default return
}

// return the value of var by lookup
tm getDateConfigVar(const string & config_file_path, string var_name)
{
	// check if we have already read file
	if (!already_read_file)
		readConfigFile(config_file_path);

	// return variable value
	if (var_name.compare("start_date_") == 0)
		return start_date;
	else if (var_name.compare("end_date_") == 0 )
		return end_date;
	return tm{}; // default return
}


// constructor
ConfigParameters::ConfigParameters(const std::string & file_path) :
	file_holiday_(getStringConfigVar(file_path, "file_holiday_")),
	file_shift_(getStringConfigVar(file_path, "file_shift_")),
	file_employee_(getStringConfigVar(file_path, "file_employee_")),
	start_date_(getDateConfigVar(file_path, "start_date_")),
	end_date_(getDateConfigVar(file_path, "end_date_")),
	min_rest_hours_(getIntConfigVar(file_path, "min_rest_hours_")),
	max_weekends_in_a_row_(getIntConfigVar(file_path, "max_weekends_in_a_row"))
{
}