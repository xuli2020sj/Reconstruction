#include <iostream>
#include <string>
#include <algorithm>
#include <numeric>
#include "spdlog/spdlog.h"
#include "spdlog/fmt/fmt.h"
#include "gridCal.h"

using namespace std;


void Grid::initCellList() {
    cellNum = gridNumOfX*gridNumOfY*gridNumOfZ;
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

Grid::Grid() {
    std::vector<double> splitX= {1,1,1,1,1};
    std::vector<double> splitY= {1,1,2,3};
    std::vector<double> splitZ= {1,1,1};
    gridNumOfX = splitX.size();
    gridNumOfY = splitY.size();
    gridNumOfZ = splitZ.size();
    cellNum = gridNumOfX * gridNumOfY * gridNumOfX;
    countList.resize(cellNum);

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

void Grid::initCellLenList(std::vector<double>& ratio, std::vector<double>& cellLenList, double sumLen) {
    size_t len = ratio.size();
    cellLenList.resize(len);
    double sum = accumulate(ratio.begin(), ratio.end(), 0.);
    for (int i = 0; i < len; i++) {
        cellLenList[i] = ratio[i] / sum * sumLen;
    }
}

void Grid::initCellPosList() {
    size_t xLen = cellXLenList.size();
    size_t yLen = cellYLenList.size();
    size_t zLen = cellZLenList.size();
    cellXPosList.resize(xLen);
    cellYPosList.resize(zLen);
    cellZPosList.resize(yLen);
    spdlog::debug("cell list len {0} {1} {2}", xLen, yLen, zLen);
    cellXPosList[0] = cellXLenList[0];
    cellYPosList[0] = cellYLenList[0] + distanceFromPackage;
    cellZPosList[0] = cellZLenList[0];
    spdlog::debug("cell pos {0} {1} {2}", cellXPosList[0], cellYPosList[0], cellZPosList[0]);
    for (int i = 1; i < xLen; i++) {
        cellXPosList[i] = cellXPosList[i-1] + cellXLenList[i];
    }
    for (int i = 1; i < yLen; i++) {
        cellYPosList[i] = cellYPosList[i-1] + cellYLenList[i];
    }
    for (int i = 1; i < zLen; i++) {
        cellZPosList[i] = cellZPosList[i-1] + cellZLenList[i];
    }
}

void Grid::showAllCell() {
    for (auto c : cellList) {
        spdlog::info("cell index {0}, 3d index {1} {2} {3} ", c->index, c->index3d[0], c->index3d[1], c->index3d[2]);
        spdlog::info("pos {:.4f} {:.4f} {:.4f}, len {:.4f} {:.4f} {:.4f}", c->pos3d[0], c->pos3d[1], c->pos3d[2], c->len3d[0], c->len3d[1], c->len3d[2]);
    }
}

void Grid::initDetList() {
    int i = 0;

    //todo 暴露下面参数接口
    std::vector<double> detYPosList = {0, -50, -100, -150};
    //todo 建立效率矩阵
    std::vector<double> effi = vector<double>(cellNum, 0.);

    assert(detYPosList.size() == gridNumOfY);
    for (int x = 0; x < gridNumOfX; x++) {
        for (int y = 0; y < gridNumOfY; y++) {
            for (int z = 0; z < gridNumOfZ; z++) {
                auto temp = new det(i, {x,y,z},
                                     {cellXPosList[x], detYPosList[y], detYPosList[z]},
                                    effi);
                detList[i] = temp;
                ++i;
            }
        }
    }
}




int main() {
    spdlog::info("Welcome to Calculation!");
    auto grid = Grid();
    grid.showAllCell();

    spdlog::info("Calculation finished");
}
