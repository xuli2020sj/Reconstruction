//
// Created by X on 2022/5/27.
//

#ifndef DETECTION_GRIDCAL_H
#define DETECTION_GRIDCAL_H


#include <iostream>
#include <string>
#include "Eigen"
using namespace std;

struct cell {
    size_t index;

};

class Grid {
private:
    double distanceFromPackage = 500;
    double packageXLen = 2438;
    double packageYLen = 1926;
    double packageZLen = 1331;
    int gridNumOfX = 5, gridNumOfY = 4, gridNumOfZ = 3;

public:
    Grid() {

    }
};

#endif //DETECTION_GRIDCAL_H
