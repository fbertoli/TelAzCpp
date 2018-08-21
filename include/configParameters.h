#pragma once
#include <string>
#include <ctime>

struct ConfigParameters {
	// CONSTRUCTOR (load stuff from a configuration file)
	ConfigParameters(const std::string & file_path);

	// VARIABLES
public:
	const std::string file_holiday_;
	const std::string file_shift_;
	const std::string file_employee_;
	const std::tm start_date_;
	const std::tm end_date_;
	const int min_rest_hours_;
};

// The global that you will use to fetch the configurations during runtime.
extern const ConfigParameters config_parameters;