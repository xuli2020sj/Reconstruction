#pragma once
#include "TriData.h"
#include "CellCalculation.h"

class DEPost
{
public:
	DEPost(void);
	~DEPost(void);
	TriData TriDataB;
	CellCalculation CellB;
	TriDNodeHeadDefine *EHead;
	std::string m_Folder;
	GPoint _PA;
	GPoint _PB;
	GPoint _PGe;
	GPoint _PWaste;
	GCell _WD1;
	GCell _WD2;
	bool Process();
	void EProcess2(int key);
    std::string m_SavingFolder;

};

