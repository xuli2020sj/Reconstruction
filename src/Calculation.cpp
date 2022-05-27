#include <synchapi.h>
#include "Calculation.h"


Calculation::Calculation():
IsOnCal(false),
IsOnPause(false),
MaxStep(100),
nStep(0)
{
		TypeOfCal=0;
}


Calculation::~Calculation()
{
}


bool Calculation::StartDoing()
{
	DECalculationB.m_Folder=m_Folder;
	if(!DECalculationB.INICalculation())
	{
        cout << "init failed" << endl;
		return false;
	}

	if(TypeOfCal==1)
	{
		MaxStep=1;
		DECalculationB.PostProcess(m_FileName);
		nStep=1;
		IsOnCal=false;
		return true;
	}

	MaxStep=DECalculationB.EHead->NuNodesTotal;
	for(nStep=0;nStep<MaxStep&&IsOnCal;nStep++)
	{
		if(!DECalculationB.ECal_OnePoint()) break;
		while(IsOnPause) Sleep(1000);
//		Sleep(500);
	}
	DECalculationB.Output("");
	IsOnCal=false;
	return true;
}