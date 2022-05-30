#include <string>
#include <algorithm>
#include <numeric>
#include "spdlog/spdlog.h"
#include "gridCal.h"
#include <Eigen/IterativeLinearSolvers>
#include <iostream>

using namespace std;

Grid::Grid() {
    std::vector<double> splitX = {1,1,1,1,1};
    std::vector<double> splitY = {1,1,2,3};
    std::vector<double> splitZ = {1,1,1};
    gridNumOfX = splitX.size();
    gridNumOfY = splitY.size();
    gridNumOfZ = splitZ.size();
    cellNum = gridNumOfX * gridNumOfY * gridNumOfX;
    effi = vector<vector<double>>(4, vector<double>(gridNumOfY, 0));

    spdlog::info("Grid are separated by : {0} {1} {2} ", gridNumOfX, gridNumOfY, gridNumOfZ);
    initCellLenList(splitX, cellXLenList, packageXLen);
    initCellLenList(splitY, cellYLenList, packageYLen);
    initCellLenList(splitZ, cellZLenList, packageZLen);
    spdlog::info("init cell len success");
    initCellPosList();
    spdlog::info("init cell pos success");
    initCellList();
    spdlog::info("init cell list success");
}

Grid::Grid(std::vector<double> splitX, std::vector<double> splitY, std::vector<double> splitZ, std::vector<double> detYPosList) {
    gridNumOfX = splitX.size();
    gridNumOfY = splitY.size();
    gridNumOfZ = splitZ.size();
    cellNum = gridNumOfX * gridNumOfY * gridNumOfX;

    spdlog::info("Grid are separated by : {0} {1} {2} ", gridNumOfX, gridNumOfY, gridNumOfZ);
    initCellLenList(splitX, cellXLenList, packageXLen);
    initCellLenList(splitY, cellYLenList, packageYLen);
    initCellLenList(splitZ, cellZLenList, packageZLen);
    spdlog::info("init cell len success");
    initCellPosList();
    spdlog::info("init cell pos success");
    initCellList();
    spdlog::info("init cell list success");
    initDetList(detYPosList);
    spdlog::info("init det list success");
}

void Grid::initCellList() {
    cellList.resize(cellNum);
    int i = 0;
    for (int x = 0; x < gridNumOfX; x++) {
        for (int y = 0; y < gridNumOfY; y++) {
            for (int z = 0; z < gridNumOfZ; z++) {
                auto temp = new cell(i, {x,y,z},
                                {cellXPosList[x], cellXPosList[y], cellXPosList[z]},
                                {cellXLenList[x], cellYLenList[y], cellZLenList[z]});
                cellList[i] = temp;
                ++i;
            }
        }
    }
}

void Grid::initCellLenList(std::vector<double>& ratio, std::vector<double>& cellLenList, double sumLen) {
    size_t len = ratio.size();
    cellLenList.resize(len);
    double sum = accumulate(ratio.begin(), ratio.end(), 0.);
    for (int i = 0; i < len; i++) {
        cellLenList[i] = ratio[i] / sum * sumLen;
    }
}

void Grid::initCellPosList() {
    cellXPosList.resize(gridNumOfX);
    cellYPosList.resize(gridNumOfY);
    cellZPosList.resize(gridNumOfZ);
    cellXPosList[0] = cellXLenList[0] / 2 + pos[0];
    cellYPosList[0] = cellYLenList[0] / 2 + pos[1];
    cellZPosList[0] = cellZLenList[0] / 2 + pos[2];
    spdlog::debug("cell pos {0} {1} {2}", cellXPosList[0], cellYPosList[0], cellZPosList[0]);
    for (int i = 1; i < gridNumOfX; i++) {
        cellXPosList[i] = cellXPosList[i-1] + cellXLenList[i];
    }
    for (int i = 1; i < gridNumOfY; i++) {
        cellYPosList[i] = cellYPosList[i-1] + cellYLenList[i];
    }
    for (int i = 1; i < gridNumOfZ; i++) {
        cellZPosList[i] = cellZPosList[i-1] + cellZLenList[i];
    }
}

void Grid::initDetList(std::vector<double> detYPosList) {
    detList.resize(cellNum);
    int i = 0;

    assert(detYPosList.size() == gridNumOfY);
    for (int x = 0; x < gridNumOfX; x++) {
        for (int y = 0; y < gridNumOfY; y++) {
            for (int z = 0; z < gridNumOfZ; z++) {
                auto temp = new det(i, {x,y,z},
                                     {cellXPosList[x], detYPosList[y], cellZPosList[z]},
                                    vector<double>(cellNum, 0.));
                detList[i] = temp;
                ++i;
            }
        }
    }
    spdlog::info("init DetList success");
}

void Grid::reconstruction_BiCGSTAB() {
    Eigen::MatrixXd C = Eigen::MatrixXd::Zero(cellNum, 1);
    Eigen::MatrixXd A = Eigen::MatrixXd::Zero(cellNum, 1);
//    Eigen::MatrixXd E = Eigen::MatrixXd::Zero(cellNum, cellNum);
    Eigen::SparseMatrix<double> E(cellNum, cellNum);
    for (int i = 0; i < cellNum; i++) {
        C(i) = detList[i]->countRate;
    }
    spdlog::info("count rate init success");
    for (int i = 0; i < cellNum; i++) {
        for (int j = 0; j < cellNum; j++) {
            E.insert(i,j) = detList[i]->detectionEfficiency[j];
        }
    }
    spdlog::info("Detection Efficiency init success");
    Eigen::BiCGSTAB<Eigen::SparseMatrix<double>> solver;
    solver.compute(E);
    A = solver.solve(C);
    A.resize(1, cellNum);
    totalActivity_BiCGSTAB = A.sum();
    std::cout << "#iterations:     " << solver.iterations() << std::endl;
    std::cout << "estimated error: " << solver.error()      << std::endl;
    std::cout << "The activity matrix is:\n" << A << std::endl;
    std::cout << "The total activity calculated by BiCGSTAB is: " << A.sum() << std::endl;
}

void Grid::showAllCell() {
    for (auto c : cellList) {
        spdlog::info("cell index {0}, 3d index {1} {2} {3} ", c->index, c->index3d[0], c->index3d[1], c->index3d[2]);
        spdlog::info("pos {:.4f} {:.4f} {:.4f}, len {:.4f} {:.4f} {:.4f}", c->pos3d[0], c->pos3d[1], c->pos3d[2], c->len3d[0], c->len3d[1], c->len3d[2]);
    }
}

void Grid::showAllDet() {
    for (auto c : detList) {
        spdlog::info("dec index {0}, 3d index {1} {2} {3} ", c->index, c->index3d[0], c->index3d[1], c->index3d[2]);
        spdlog::info("pos {:.4f} {:.4f} {:.4f}", c->pos3d[0], c->pos3d[1], c->pos3d[2]);
    }
}

void Grid::reconstruction_MLEM() {

}

void Grid::setEffi(std::vector<std::vector<double>> eff) {
    assert(eff.size() == effi.size());
    effi = std::move(eff);
}

int main() {
    spdlog::info("Welcome to Calculation!");
    auto grid = Grid();
    auto grid1 = Grid({1,1,1,1,1}, {1,1,2,3}, {1,1,1},{0, -50, -100, -150});
    grid.showAllDet();
    grid.showAllCell();
    grid.reconstruction_BiCGSTAB();
    spdlog::info("Calculation finished");
}
