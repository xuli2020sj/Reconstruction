#include <iostream>
#include <string>
#include "gridCal.h"
using namespace std;


int main() {
    string CTemp;
    int i = 1;
    CTemp = "VARIABLE" + to_string(i);
    cout << CTemp.find("VDAR") << endl;
    return 0;
}