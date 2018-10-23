#include <iostream>
#include <ctime>
#include <string>
#include "configParameters.h"
#include "data.h"
#include "solution.h"
#include "randomInsertion.h"
#include <cassert>

using namespace std;

int main()
{
    Data data;

    /** DATA CLASS TESTS */
//    data.printInfo();
//    cout << endl;
//    data.printShiftTypes();
//    cout << endl;
//    data.printEmployees();
//    cout << endl;



    /** SOLUTION CLASS TESTS */
    Solution solution(&data);
    RandomInsertion rand_insertion(&data);
    rand_insertion.repair(&solution);
    assert(solution.checkConsistency());
    solution.write("test/output/solution.txt");

    Solution solution2(&data);
    solution2.read("test/output/solution.txt");
    assert(solution2.checkConsistency());
    solution2.write("test/output/solution2.txt");

    cout << ((solution == solution2) ? "true" : "false") << endl;

    return 0;
}