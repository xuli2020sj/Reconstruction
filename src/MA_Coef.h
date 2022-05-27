#pragma once
#include <string>
#define AlDensity 2.699
#define FeDensity 7.874
#define GeDensity 5.323
#define PbDensity 11.72
#define CuDensity 8.96
#define ConcreteDensity 2.5
#define GlassDensity 2.5
#define AirDensity 0.0013
#define PolyethyleneDensity 0.94
#define WaterDensity 1
#define MaxNuOfMa 11
class MA_Coef
{
public:
	int NoOfMa;
	std::string NameOfMa[MaxNuOfMa];
	MA_Coef(void)
	{
		NameOfMa[0]="Ordinary";
		NameOfMa[1]="None";
		NameOfMa[2]="Water";
		NameOfMa[3]="Polyethylene";
		NameOfMa[4]="Concrete";
		NameOfMa[5]="Glass";
		NameOfMa[6]="Aluminum";
		NameOfMa[7]="Germanium";
		NameOfMa[8]="Iron";
		NameOfMa[9]="Copper";
		NameOfMa[10]="Lead";

	}
	~MA_Coef(void)
	{
	}
	double MassAttenuationAir(double E);
	double MassAttenuationWater(double  E);
	double MassAttenuationPolyethylene(double E);
	double MassAttenuationConcrete(double E);
	double MassAttenuationAl(double  E);
	double MassAttenuationGe(double  E);
	double MassAttenuationFe(double  E);
	double MassAttenuationPb(double E);
	double MassAttenuation(int No,double E);
	double Density(int No);
};

