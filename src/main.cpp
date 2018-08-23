#include <iostream>
#include <ctime>
#include <string>
#include "configParameters.h"
#include "data.h"
#include "solution.h"

using namespace std;

int main()
{	
	Data data;
	data.printInfo();
	cout << endl;
	data.printShiftTypes();
	cout << endl;
	data.printEmployees();
	cout << endl;

    Solution solution(&data);
	return 0;
}