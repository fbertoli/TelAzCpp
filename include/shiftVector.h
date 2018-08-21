#pragma once
#include <string>
#include <vector>


// Use a struct for a sinle shift
struct Shift {
	std::string name;
	bool mandatory;
	int day;
	int week;
};

// Implement a class to be uase as a contained for shifts
class ShiftVector 
{
	// Constructor
public:
	ShiftList();

	// Methods

	// Variables
	std::vector<Shift> list;


	
};