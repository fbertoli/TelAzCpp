#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <ctime>

// remove c from string str
void removeChar(std::string *str, char c);

// read file and return lines (throw excpetion if file cannot be open)
std::vector<std::vector<std::string> > readLinesFile(std::string file_path);

// print functions
template<typename T>
void printVector(const T& t) {
    std::copy(t.cbegin(), t.cend(), std::ostream_iterator<typename T::value_type>(std::cout, ", "));
}

std::string printDate(const tm &date);
std::string printTime(const tm &time);

// date reading and formatting
void readDate(std::string *s, tm *date);
inline int formatYear(int year) {return year + ((year < 100) ? 2000 : 0);}

// translate day of the week (use monday = 0)
int translateDay(std::string *day_of_the_week);
std::string translateDay(int day_of_the_week);
