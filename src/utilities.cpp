#include "utilities.h"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iterator>
#include <iostream>


using namespace std;

// remove c from string str
void removeChar(string *str, char c) {
	char *cstr = new char[str->length() + 1];
	strcpy(cstr, str->c_str());
	char *pr = cstr, *pw = cstr;
	while (*pr) {
		*pw = *pr++;
		pw += (*pw != c);
	}
	*pw = '\0';
	*str = string(cstr);
}

// read file and return lines (throw excpetion if file cannot be open)
vector<vector<string>> readLinesFile(string file_path)
{
	string new_line;
	ifstream file(file_path);
	vector<vector<string>> lines;
	if (file.is_open()) {
		// save lines
		while (getline(file, new_line)) {
			istringstream iss(new_line);
			vector<string> tokens{ istream_iterator<string>{iss},
				istream_iterator<string>{} };
			lines.push_back(tokens);
		}
		file.close();
	}
	else {
		// throw exception
		throw runtime_error("Could not open file: " + file_path);
	}
	return lines;
}

//printing
std::string printDate(const tm &date) {return  to_string(date.tm_mday) + "/" + to_string(date.tm_mon+1)  + "/" + to_string(date.tm_year + 1900);}
std::string printTime(const tm &time) {return  to_string(time.tm_hour) + ":" + ((time.tm_min < 10) ? "0" : "" ) + to_string(time.tm_min);}


// date reading
void readDate(std::string *s, tm *date)
{

	sscanf(s->c_str(), "%2d/%2d/%4dd", &(date->tm_mday), &(date->tm_mon), &(date->tm_year));
	date->tm_year = formatYear(date->tm_year) - 1900;
	--date->tm_mon;
	date->tm_hour = 12; // to avoid midnight / next day problems
	date->tm_min = 0;
	date->tm_sec = 0;
	mktime(date);
	date->tm_hour = 12; // to avoid problems when obtaining days difference
	date->tm_min = 0;
	date->tm_sec = 0;
}

// translate day of the week
int translateDay(std::string *day_of_the_week)
{
	// get the first thre letters in lower case
	std::string str = day_of_the_week->substr(0, 3);
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	if (str.compare("lun") == 0 || str.compare("mon") == 0)
		return 0;
	else if (str.compare("mar") == 0 || str.compare("tue") == 0)
		return 1;
	else if (str.compare("mer") == 0 || str.compare("wed") == 0)
		return 2;
	else if (str.compare("gio") == 0 || str.compare("thu") == 0)
		return 3;
	else if (str.compare("ven") == 0 || str.compare("fri") == 0)
		return 4;
	else if (str.compare("sab") == 0 || str.compare("sab") == 0)
		return 5;
	else if (str.compare("dom") == 0 || str.compare("sun") == 0)
		return 6;

	return -1; // keeps compiler happy

}

std::string translateDay(int day_of_the_week)
{
	switch (day_of_the_week)
	{
		case 0: return "lun";
		case 1: return "mar";
		case 2: return "mer";
		case 3: return "gio";
		case 4: return "ven";
		case 5: return "sab";
		case 6: return "dom";
	}
	return "???"; // keeps compiler happy
}