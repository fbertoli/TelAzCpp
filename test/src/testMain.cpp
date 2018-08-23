#include <iostream>
#include <ctime>
#include <string>
#include "configParameters.h"
#include "data.h"
#include "solution.h"
#include "randomInsertion.h"

using namespace std;

int main()
{
    Data data;

    /** DATA CLASS TESTS */
    data.printInfo();
    cout << endl;
    data.printShiftTypes();
    cout << endl;
    data.printEmployees();
    cout << endl;


    /** SOLUTION CLASS TESTS */
    Solution solution(&data);
    RandomInsertion rand_insertion(&data);
    rand_insertion.repair(&solution);
    solution.write("test/output/solution.txt");

    return 0;
}