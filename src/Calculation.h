#include "DECalculation.h"

class Calculation
{
protected:
	DECalculation DECalculationB;

public:
	Calculation();
	~Calculation();
	int TypeOfCal;
	bool IsOnCal;
	bool IsOnPause;
	std::string m_Folder;
    std::string m_FileName;
	bool StartDoing();
	int MaxStep;
	int nStep;
    std::string Information;

};

