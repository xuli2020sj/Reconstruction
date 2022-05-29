#include <string>
#include <Eigen/Dense>
#include <Eigen/Cholesky>
#include <Eigen/LU>
#include <Eigen/QR>
#include <Eigen/SVD>
#include <Eigen/IterativeLinearSolvers>
#include <utility>

#ifndef DETECTION_GRIDCAL_H
#define DETECTION_GRIDCAL_H

struct cell {
    size_t index;
    double activity{0.};
    double efficiency{0.};
    std::vector<double> len3d;
    std::vector<double> pos3d;
    std::vector<int> index3d;
    cell(int _index, std::vector<int>  _index3d, std::vector<double> _pos3d, std::vector<double> _len3d)
    : index(_index), index3d(std::move(_index3d)), len3d(std::move(_len3d)), pos3d(std::move(_pos3d)) {};
};

struct det {
    size_t index;
    std::vector<double> pos3d;
    std::vector<int> index3d;
    std::vector<double> detectionEfficiency;
    double countRate;
    det(int _index, std::vector<int> _index3d, std::vector<double> _pos3d, std::vector<double> _detectionEfficiency)
            : index(_index), index3d(std::move(_index3d)), pos3d(std::move(_pos3d)), countRate(0) {};
};

class Grid {
private:
    double distanceFromPackage = 500;
    double packageXLen = 2438;
    double packageYLen = 1926. / 2;
    double packageZLen = 1331;
    size_t gridNumOfX = 5, gridNumOfY = 4, gridNumOfZ = 3;
    size_t cellNum;
    std::vector<double> cellXLenList, cellYLenList, cellZLenList;
    std::vector<double> cellXPosList, cellYPosList, cellZPosList;
    std::vector<cell*> cellList;

    void initCellList();
    void initCellPosList();
    static void initCellLenList(std::vector<double>& ratio, std::vector<double>& cellLenList, double sumLen);

    std::vector<det*> detList;
    void initDetList();
public:
    Grid();
    void showAllCell();
    void reconstruction_MLEM();
    void reconstruction_BiCGSTAB();
};

#endif //DETECTION_GRIDCAL_H
