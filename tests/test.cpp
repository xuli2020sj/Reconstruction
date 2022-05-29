#include <iostream>
#include <string>

using namespace std;

struct cell {
    size_t index;
    double activity;

};

class Grid {
private:
    double distanceFromPackage = 500;
    double packageXLen = 2438;
    double packageYLen = 1926;
    double packageZLen = 1331;
    int gridNumOfX = 5, gridNumOfY = 4, gridNumOfZ = 3;


};

int main() {
    string CTemp;
    int i = 1;
    CTemp = "VARIABLE" + to_string(i);
    cout << CTemp.find("VDAR") << endl;
    return 0;
}