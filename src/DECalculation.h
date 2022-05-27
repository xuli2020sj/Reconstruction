#pragma once
#include "TriData.h"
#include "CellCalculation.h"
#include <random>
#include <fstream>
#include <string>

using namespace std;
#define MaxCell 100
#define MaxData 50
class DECalculation
{
protected:
	std::mt19937 mt;
	long long MaxMT;
	double m_Energy[MaxData];
	double m_mu[11][MaxData];

public:
	DECalculation(void);
	~DECalculation(void);
	std::string m_Folder;
	TriData TriDataB;
	TriDNodeHeadDefine *ESampleHead;
	TriDNodeHeadDefine *EHead;
	TriDNodeDefine *NodeUsedx,*NodeUsedy,*NodeUsedz;

	bool INICalculation(void);
	bool ReadDetector(void);
	bool ReadWaste(void);

	//体素的定义
	GPoint P0;//零坐标轴
	GPoint PWaste;//零坐标轴
	GPoint PGe;//林敏体的中心坐标
	GPoint PSample;
	GPoint m_PA;
	GPoint m_PB;
	double m_RGe,m_LGe;
	int NuCellDec;
	int NuCellTotal;
	GCell Cell[MaxCell];
	double LInCell[MaxCell];
	void SetCellDetector();
	void SetCellWaste();
	bool SetSource();
	void LInCellCalculationDec(GPoint PA,GPoint PB);
	void LInCellCalculationWaste(GPoint PA,GPoint PB);
	void CalculationOnePoint(int key,GPoint _PA,GPoint Dec);
	double  CEGeo[MaxData];
	double  CEGeo_0[MaxData];
	double  CEM[MaxData];
	double  SumCEGeo[MaxData];
	double  SumCEGeo_0[MaxData];
	double EData[MaxData];
	GPoint CoordinatesTrasnfer(GPoint PA,GPoint PB,GPoint PC);
	GPoint CoordinatesTrasnfer(GPoint PA,GPoint PB,double r,double angle);
	void DetectorGeo(int key,GPoint _PA,GPoint _PB);
	void ESampleCal();
	bool ECal_OnePoint();
	void ECal_OnePoint(GPoint _PA);
	void Output(string FileName);
	void PostProcess();
	void PostProcess(string FileName);
};

