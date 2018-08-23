#pragma once

#include "random.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <ctime>

/** remove c from string str */
void removeChar(std::string *str, char c);

/** tranasform day to week */
inline int getWeek(int day) {return day / 7;}

/** determine if day is on weekend */
inline bool isOnWeekend(int day) {return (day % 7) > 4;}

/** return the string + blank space to reach size n. IF s.size() > n return s */
inline std::string format(std::string s, int n) {return (s.size() < n) ? (s + std::string(n - s.size(), ' ')) : s.substr(0, n);}
std::string format(double d, int n);
std::string format(int d, int n);


/** join vector of strings like python (pass by copy so you can define on-the-fly) */
std::string joinStrings(std::vector<std::string> vec, std::string delimiter);

/** read file and return lines (throw excpetion if file cannot be open) */
std::vector<std::vector<std::string> > readLinesFile(std::string file_path);

/** print functions */
template<typename T>
void printVector(const T& t) {
    std::copy(t.cbegin(), t.cend(), std::ostream_iterator<typename T::value_type>(std::cout, ", "));
}

std::string printDate(const tm &date);
std::string printTime(const tm &time);

/** date reading and formatting */
void readDate(std::string *s, tm *date);
inline int formatYear(int year) {return year + ((year < 100) ? 2000 : 0);}

/** translate day of the week (use monday = 0) */
int translateDay(std::string *day_of_the_week);
std::string translateDay(int day_of_the_week);

/** random generator needed for random_shuffle */
inline int randomGenerator(int i) {return (i > 1 ) ? Random::integer(0, i-1) : 0;}
