#include "MA_Coef.h"
#include <math.h>
using namespace std;

#define Power6(a) pow(a,6.0)
#define Power5(a) pow(a,5.0)
#define Power4(a) pow(a,4.0)
#define Power3(a) pow(a,3.0)
#define Power2(a) pow(a,2.0)
#define SumPower6(a,b0,b1,b2,b3,b4,b5,b6) (b6*Power6(a)+b5*Power5(a)+b4*Power4(a)+b3*Power3(a)+b2*Power2(a)+b1*a+b0)

double MA_Coef::MassAttenuationAir(double E) 
{
	if(E>20) E/=1000.0;
	return pow(10.0,SumPower6(log10(fabs(E)),-1.1911,-0.524,-0.1535,0.1594,0.1011,-0.0603,-0.0123));
}
double MA_Coef::MassAttenuationWater(double E) 
{
	if(E>20) E/=1000.0;
	return pow(10.0,SumPower6(log10(fabs(E)),-1.1455,-0.5228,-0.1496,0.1484,0.0878,-0.0555,-0.0086));
}
double MA_Coef::MassAttenuationConcrete(double E) 
{
	if(E>20) E/=1000.0;
	return pow(10.0,SumPower6(log10(fabs(E)),-1.1734,-0.5237,-0.2496,0.2038,0.2863,-0.1077,-0.0617));
}
double MA_Coef::MassAttenuationFe(double E) 
{
	if(E>20) E/=1000.0;
	return pow(10.0,SumPower6(log10(fabs(E)),-1.2305,-0.4217,0.032,-0.1471,0.2817,0.0004,-0.0501));
}
double MA_Coef::MassAttenuationAl(double E) 
{
	if(E>20) E/=1000.0;
	return pow(10.0,SumPower6(log10(fabs(E)),-1.1973,-0.5185,-0.2449,0.2095,0.3142,-0.1131,-0.0687));
}
double MA_Coef::MassAttenuationGe(double E)
{
	if(E>20) E/=1000.0;
	return pow(10.0,SumPower6(log10(fabs(E)),-1.24153,-0.45999,0.0071,-0.12833,0.48508,-0.0306,-0.12888));
}
double MA_Coef::MassAttenuationPolyethylene(double E)
{
	if(E>20) E/=1000.0;
	return pow(10.0,SumPower6(log10(fabs(E)),-1.1406,-0.5015,-0.0856,0.0607,-0.0137,-0.0081,0.0196));
}
double MA_Coef::MassAttenuationPb(double E)
{
	if(E>20) E/=1000.0;
	if((E)<1) return pow(10.0,SumPower6(log10(fabs(E)),-1.15024,-0.9646,0.35863,-1.48405,-0.34391,1.15888,0.58996));
	else return pow(10.0,SumPower6(log10(fabs(E)),-1.14857,-0.98454,1.45959,-1.0453,0.5958,-0.19745,0.01703));
}
//1 真空 2water 3Polyethylene 4Concrete 5Glass 用Concrete代替  6Al 7Ge 8Fe 9Cu用Fe代替，10Pb 其他水
double MA_Coef::MassAttenuation(int No,double E)
{
	double Coef;
	switch(No)
	{
	case 1:
		Coef=0;
		break;
	case 2:
		Coef=MassAttenuationWater(E);
		break;
	case 3:
		Coef=MassAttenuationPolyethylene(E);
		break;
	case 4:
		Coef=MassAttenuationConcrete(E);
		break;
	case 5:
		Coef=MassAttenuationConcrete(E);
		break;
	case 6:
		Coef=MassAttenuationAl(E);
		break;
	case 7:
		Coef=MassAttenuationGe(E);
		break;
	case 8:
		Coef=MassAttenuationFe(E);
		break;
	case 9:
		Coef=MassAttenuationFe(E);
		break;
	case 10:
		Coef=MassAttenuationPb(E);
		break;
	default:
		Coef=MassAttenuationWater(E);
		break;
	}
	return Coef;
}
//1 真空 2water 3Polyethylene 4Concrete 5Glass 6Al 7Ge 8Fe 9Cu 10Pb 其他水
double MA_Coef::Density(int No)
{
	double Density;
	switch(No)
	{
	case 1:
		Density=0;
		break;
	case 2:
		Density=WaterDensity;
		break;
	case 3:
		Density=PolyethyleneDensity;
		break;
	case 4:
		Density=ConcreteDensity;
		break;
	case 5:
		Density=GlassDensity;
		break;
	case 6:
		Density=AlDensity;
		break;
	case 7:
		Density=GeDensity;
		break;
	case 8:
		Density=FeDensity;
		break;
	case 9:
		Density=CuDensity;
		break;
	case 10:
		Density=PbDensity;
		break;
	default:
		Density=1;
		break;
	}
	return Density;
}