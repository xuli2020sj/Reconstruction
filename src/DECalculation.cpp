#include <chrono>
#include "DECalculation.h"
#include "MA_Coef.h"
#include "DEPost.h"


DECalculation::DECalculation(void)
{
	ESampleHead=NULL;
	EHead=NULL;
//	INICalculation();
}

DECalculation::~DECalculation()
{
	if(ESampleHead) TriDataB.DeleteHead(ESampleHead);
	ESampleHead=NULL;
	if(EHead) TriDataB.DeleteHead(EHead);
	EHead=NULL;

}

//�����ʼ��
bool DECalculation::INICalculation()
{
	//��ȡ��������
	ESampleHead=TriDataB.ReadData(m_Folder+"E_Sample1.dat");//***1
	if(!ESampleHead||!ESampleHead->FirstNode) return false;
	//��Ʒ�������
	PSample.SetPoint(ESampleHead->FirstNode->x,ESampleHead->FirstNode->y,ESampleHead->FirstNode->z);//***2

	MA_Coef MAB;
	//�����������ݼ�¼����������������˥��ϵ��
	for(int n=0;n<ESampleHead->NuDataUsed;n++)
	{
		m_Energy[n]=ESampleHead->VARIABLESData[n];
		for(int no=0;no<11;no++)
		{
			m_mu[no][n]=MAB.MassAttenuation(no,m_Energy[n]);
		}
	}
	//���������
	auto Time = chrono::system_clock::now();
	time_t time = chrono::system_clock::to_time_t(Time);
	int seeddata= tolower(time);
	mt.seed(seeddata%60);
	MaxMT=int(floor(pow(2.0,19937)))-1;


	//��ȡ̽��������
	if(!ReadDetector()) {
		return false;
	}
	//��ȡ����������
	if(!ReadWaste()) return false;

	//����̽����
	SetCellDetector();
	//��������
	ESampleCal();
	//���÷���Դ
	if(!SetSource()) return false;
	NodeUsedx=EHead->FirstNode;
    NodeUsedy=EHead->FirstNode;
	NodeUsedz=EHead->FirstNode;

return true;
}
//��ȡ̽��������
bool DECalculation::ReadDetector(void)
{
return true;
}
//��ȡ����������
bool DECalculation::ReadWaste(void)
{
//�ڴ����÷������
return true;
}

//�������� *************************** 3
//ע�⣺
void DECalculation::SetCellDetector()
{
	//���е�λΪcm
	//׼ֱ��
	GPoint PCollimator(45,0,0);//׼ֱ��ǰ��������
	int NoMaOfCollimator=10;//����;
	double DeOfCollimator=11.32;
	//double HFeWall=1;//���ĺ�ȣ���λcm;
	double W1OfCollimator=36;//׼ֱ����ȣ���λcm;
	double H1OfCollimator=26;//׼ֱ���߶ȣ���λcm;
	double W2OfCollimator=20;//׼ֱ�ڿ�ȣ���λcm;
	double H2OfCollimator=9;//׼ֱ�ڸ߶ȣ���λcm;
	double D1OfCollimator=30;//׼ֱ��ֱ������λcm;
	double D2OfCollimator=10;//׼ֱ��ֱ������λcm;
	double LOfCollimator=25;//׼ֱ����ȣ���λcm;
	bool IsShield=false;//�Ƿ������ο�;
	double LOfShield=8;//���κ�ȣ���λcm;
	int NoMaOfShield=8;//���β��ϱ��
	double DeOfShield=7.8;//���β����ܶȣ�g/cc;
	//̽����
	GPoint PAl;PAl.SetPoint(PCollimator,LOfCollimator,0,0);//��ñǰ������
	int NoMaOfCrystal=7;//�������
	double DeOfCrystal=5.35;//�����ܶȣ�g/cm3
	double ROfCrystal=3.045;//����뾶����λcm;
	double hOfCrystalDead=0.07;//�����ȣ���λcm;
	double LOfCrystal=5.18;//���峤�ȣ���λcm;
	int NoMaOfCup=6;//ñ����
	double ROfCup=3.225;//ñ�뾶��cm
	double hOfCup=0.15;//ñ��ȣ�cm
	double DeOfCup=2.7;//ñ�ܶȣ�g/cm3
	double dL_CupGe=0.5;//������Ge֮��ļ�϶
	int NoMaOfCold=9;//�������
	double ROfCold=0.435;//��о�뾶����λcm;
	double LOfCold=3.82;//��о���ȣ���λcm;
	double DeOfCold=8.96;//��о�ܶȣ�g/cm3
	GPoint PEnd;PEnd.SetPoint(PAl,2*(dL_CupGe+hOfCup+hOfCrystalDead)+LOfCrystal,0,0);//���Ǻ��

	//��������
	//���ϱ�ţ�1 ��� 2water 3Polyethylene 4Concrete 5Glass ��Concrete����  6Al 7Ge 8Fe 9Cu��Fe���棬10Pb ����ˮ
	int no=0;
	//0�������壬��ָ����ͭ����ע�����ʱ�۳�ͭ��
	if(!Cell[no].Plane.empty())Cell[no].Plane.clear();
	PGe.SetPoint(PAl,hOfCup+dL_CupGe+hOfCrystalDead+0.5*LOfCrystal,0,0);
	Cell[no].Cylinder(PGe,'x',ROfCrystal,LOfCrystal);
	Cell[no].NoM=NoMaOfCrystal;
	Cell[no].Density=DeOfCrystal;
	m_RGe=ROfCrystal;
	m_LGe=LOfCrystal;
	no++;
	//1��ͭ��
	if(!Cell[no].Plane.empty())Cell[no].Plane.clear();
	GPoint PCu;PCu.SetPoint(PAl,hOfCup+dL_CupGe+hOfCrystalDead+LOfCrystal-LOfCold*0.5,0,0);
	Cell[no].Cylinder(PCu,'x',ROfCold,LOfCold);
	Cell[no].NoM=NoMaOfCold;
	Cell[no].Density=DeOfCold;
	no++;
	//2��������,�������Բ������ȥ������
	if(!Cell[no].Plane.empty())Cell[no].Plane.clear();
	Cell[no].Cylinder(PGe,'x',ROfCrystal+hOfCrystalDead,LOfCrystal+hOfCrystalDead*2);
	Cell[no].NoM=NoMaOfCrystal;
	Cell[no].Density=DeOfCrystal;
	no++;
	//3������,�������Բ������ȥ���Ĳ���
	if(!Cell[no].Plane.empty())Cell[no].Plane.clear();
	GPoint PAl0;PAl0.SetPoint(PAl,0.5*(PEnd.x-PAl.x),0,0);//�����������ģ�����ǰ������
	Cell[no].Cylinder(PAl0,'x',ROfCup,PEnd.x-PAl.x);
	Cell[no].NoM=NoMaOfCup;
	Cell[no].Density=DeOfCup;
	no++;
	//4�����ǿ��Ĳ���
	if(!Cell[no].Plane.empty())Cell[no].Plane.clear();
	Cell[no].Cylinder(PAl0,'x',ROfCup-hOfCup,PEnd.x-PAl.x-2*hOfCup);
	Cell[no].NoM=NoMaOfCup;
	Cell[no].Density=DeOfCup;
	no++;
	//5��׼ֱ��
	GPoint PPb0;PPb0.SetPoint(PCollimator,0.5*(PEnd.x-PCollimator.x),0,0);
	if(!Cell[no].Plane.empty())Cell[no].Plane.clear();
	Cell[no].Cuboid(PPb0,PEnd.x-PCollimator.x,W1OfCollimator,H1OfCollimator);
	Cell[no].NoM=NoMaOfCollimator;
	Cell[no].Density=DeOfCollimator;
	no++;
	//6��׼ֱ���м�۳����֣�׼ֱ��
	if(!Cell[no].Plane.empty())Cell[no].Plane.clear();
	Cell[no].Cuboid(PPb0,PEnd.x-PCollimator.x,W2OfCollimator,H2OfCollimator);
	Cell[no].NoM=1;
	Cell[no].Density=0;
	no++;
	//7��׼ֱ�����ο�
	if(IsShield)
	{
		if(!Cell[no].Plane.empty())Cell[no].Plane.clear();
		GPoint PShield;PShield.SetPoint(PCollimator,0.5*LOfShield,0,0);
		Cell[no].Cuboid(PShield,LOfShield,W2OfCollimator,H2OfCollimator);
		Cell[no].NoM=NoMaOfShield;
		Cell[no].Density=DeOfShield;
	}
	else Cell[no].NoM=-999;
	no++;
	//����8������
	NuCellDec=no;
}
//���÷�����
void DECalculation::SetCellWaste()
{
	//���е�λΪcm
	//������
	PWaste.SetPoint(0,0,0);//��������������
	double RWaste1=35;//����Ͱ�ڰ뾶������뾶
	double RWaste2=35.15;//����Ͱ��뾶

	int no=NuCellDec;
	//����Ͱ��10��ʼ���
	if(!Cell[no].Plane.empty())Cell[no].Plane.clear();
	Cell[no].Cylinder(PWaste,'z',RWaste1,RWaste2,2000);//����Ͱ�ǣ��������ýϴ󣬸��ǲ�������
	no++;

	//��������
	NuCellTotal=no;
}
//����Դ������
bool DECalculation::SetSource()//////////////***********4
{
	if(EHead) TriDataB.DeleteHead(EHead);
	//������Դ�ڵ㣬x,y,z����������������������һ����

	EHead=TriDataB.CreatNodes(100,1,1,ESampleHead);
	if(!EHead) return false;
	EHead->Title="EData.dat";


	TriDNodeDefine *nodex,*nodey,*nodez;
	nodez=EHead->FirstNode;
	for(int k=0;k<EHead->NuNodesInZ;k++,nodez=nodez->NextZ)
	{
		nodey=nodez;
		for(int j=0;j<EHead->NuNodesInY;j++,nodey=nodey->NextY)
		{
			nodex=nodey;
			for(int i=0;i<EHead->NuNodesInX;i++,nodex=nodex->NextX)
			{
				nodex->x=-35+0.5*i;
				nodex->y=0;
				nodex->z=0;
			}
		}
	}
	return true;
}


////////////////////////////////////////////
//����ΪЧ�ʼ���
///////////////////////////////////////////

//�������ݵļ���
void DECalculation::ESampleCal()
{
	CalculationOnePoint(0,PSample,PGe);
}
//�����һ��ļ��㣬��Calculation�����
bool DECalculation::ECal_OnePoint()
{
	if(!NodeUsedx) return false;
	m_PA.SetPoint(NodeUsedx->x,NodeUsedx->y,NodeUsedx->z);
	ECal_OnePoint(m_PA);
	
	NodeUsedx=NodeUsedx->NextX;
	if(!NodeUsedx)
	{
		if(NodeUsedy)NodeUsedy=NodeUsedy->NextY;	
		if(!NodeUsedy)
		{
			if(NodeUsedz) NodeUsedz=NodeUsedz->NextZ;	
			NodeUsedy=NodeUsedz;	
		}	
		NodeUsedx=NodeUsedy;
	}
//	std::cout << "ECal_OnePoint success" << std::endl;
	return true;
}
//�����������
void DECalculation::Output(std::string FileName)
{
	if(FileName.length()<1) //��ָ���ļ�������Ĭ���ļ���
		TriDataB.outputdata_3d(EHead,m_Folder+EHead->Title,"xyz");
	else
	TriDataB.outputdata_3d(EHead,FileName,"xyz");//����ָ���ļ������
	//����
	PostProcess();
}
//����
void DECalculation::PostProcess(std::string FileName)
{
	if(EHead) TriDataB.DeleteHead(EHead);
	EHead=TriDataB.ReadData(FileName);
	PostProcess();
}
//�����������
void DECalculation::PostProcess()
{
	if(EHead==NULL) return;
	//���ú���
	DEPost DEPostB;
	DEPostB.m_Folder=m_Folder;
	DEPostB.EHead=EHead;
	DEPostB._PGe.SetPoint(PGe);
	DEPostB.Process();
}


void DECalculation::ECal_OnePoint(GPoint _PA)
{
	//Ч�ʼ���
	if(!NodeUsedx) return;
	CalculationOnePoint(1,_PA,PGe);
	for(int n=0;n<ESampleHead->NuDataUsed;n++)
	{
		NodeUsedx->data[n]=ESampleHead->FirstNode->data[n]*SumCEGeo[n]/SumCEGeo_0[n];
	}
}

//�����ڳ��ȼ���,̽����
void DECalculation::LInCellCalculationDec(GPoint PA, GPoint PB)
{
	CellCalculation TempCCal;

	GPoint PB1=TempCCal.ExtendP(PA, PB,1.1);
	GPoint PB2=TempCCal.GetP2(PA, PB1,Cell[6]);

	double L=TempCCal.L_PointsAB(PA,PB2);//����֮��ĳ���
	//CString Ctemp,CTemp;

	for(int n=0;n<NuCellDec;n++)
	{
		if(Cell[n].NoM==-999) LInCell[n]=0;
		else LInCell[n]=TempCCal.L_Cell(PA, PB2,Cell[n],L);	
		if(n==0&&LInCell[0]<0.01) {LInCell[0]=0;break;}
		// Ctemp.Format("%g\t",LInCell[n]);CTemp+=Ctemp;
	}
	//AfxMessageBox(CTemp);
}
//�����ڳ��ȼ���,����
void DECalculation::LInCellCalculationWaste(GPoint PA,GPoint PB)
{
	CellCalculation TempCCal;
	double L=TempCCal.L_PointsAB(PA,PB);//����֮��ĳ���
	for(int n=NuCellDec;n<NuCellTotal;n++)
	{
		 LInCell[n]=TempCCal.L_Cell(PA, PB,Cell[n],L);	
	}
}

//����һ�����Ч��
//A�㷢��������̽�������Ծ�������OΪԭ�㣬��һ��Բ��Բƽ����AO��ֱ��
//��Բ��������ɵ�B������AB���ߵ�Ч��
//key=0����������key=1��������㣬��������_PA,_PB�����������ĵ�Dec
#define NuOfRandomData 1000
void DECalculation::CalculationOnePoint(int key,GPoint _PA,GPoint Dec)
{
	double Rs;
	double dx=_PA.x-Dec.x;
	double dy=_PA.y-Dec.y;
	double dz=_PA.z-Dec.z;
	double r=sqrt(dy*dy+dz*dz);
	double rGe=m_RGe;//��뾶
	double L2Ge=m_LGe*0.5;//�ྦྷ�峤��һ��
	double LAO=sqrt(r*r+dx*dx);//AO֮��ĳ���
	double LTJ;
	GPoint PA(_PA.x,_PA.y,_PA.z);
	GPoint PB;
	//���������
	if(key==0)//����ϵ�ڲο��㷢�䣬���������Ϊx��
	{
		Rs=(1+L2Ge-dx)/(-L2Ge-dx)*rGe;
		PB.SetPoint(Dec.x+L2Ge+1,Dec.y,Dec.z);
		double LA3=Dec.x+L2Ge+1-_PA.x;
		LTJ=0.5*(1-sqrt(LA3*LA3-Rs*Rs)/LA3);//�����
	}
	else//�ռ�����㷢�䣬����AO��ֱԲ�棬���������ΪAO����
	{ 
		double k=r/dx;
		double x1=(-k*rGe-L2Ge)/(k*k+1);
		double y1=k*x1;
		double L1=sqrt((x1+L2Ge)*(x1+L2Ge)+(y1+rGe)*(y1+rGe));
		double LA1=sqrt((x1-dx)*(x1-dx)+(r-y1)*(r-y1));
		double x2=(k*rGe+L2Ge)/(k*k+1);
		double y2=k*x2;  
		double L2=sqrt((x2-L2Ge)*(x2-L2Ge)+(y2-rGe)*(y2-rGe));
		double LA2=sqrt((x2-dx)*(x2-dx)+(r-y2)*(r-y2));
		double xx3=L2Ge+1;
		double yy3=-rGe;
		double x3=(k*yy3+xx3)/(k*k+1);
		double y3=k*x3;
		double LA3=sqrt((x3-dx)*(x3-dx)+(r-y3)*(r-y3));//A��Բ������ľ���

		double L31=LA3/LA1*L1;
		double L32=LA3/LA2*L2;
		Rs=L32;
		if(L31>L32) Rs=L31;
		double kt=LA3/LAO;
		PB.SetPoint(kt*(Dec.x-_PA.x)+_PA.x,
			         kt*(Dec.y-_PA.y)+_PA.y,
					 kt*(Dec.z-_PA.z)+_PA.z);
		LTJ=0.5*(1-sqrt(LA3*LA3-Rs*Rs)/LA3);//�����
	}
	double rr;
	double angle;
	GPoint PT;
	int no;

	if(key==0)	for(no=0;no<MaxData;no++) SumCEGeo_0[no]=0;
	else for(no=0;no<MaxData;no++) SumCEGeo[no]=0;
	for(int n=0;n<NuOfRandomData;n++)
	{
		rr=Rs*sqrt(1.0*(mt()%MaxMT)/MaxMT);
		angle=360.0*(mt()%MaxMT)/MaxMT;
		PT=CoordinatesTrasnfer(PA,PB,rr,angle);
		//_PB.SetPoint(PT.x,PT.y,PT.z);
		DetectorGeo(key,_PA,PT);
		if(key==0)
		for(no=0;no<ESampleHead->NuDataUsed&&no<MaxData;no++)
		{
			SumCEGeo_0[no]+=CEGeo_0[no];	
		}
		else
		for(no=0;no<ESampleHead->NuDataUsed&&no<MaxData;no++)
		{
			SumCEGeo[no]+=CEGeo[no];				
		}

	}
		if(key==0) 
		for(no=0;no<ESampleHead->NuDataUsed&&no<MaxData;no++)
		{
			SumCEGeo_0[no]=LTJ*SumCEGeo_0[no]/NuOfRandomData;
		}
		else
		for(no=0;no<ESampleHead->NuDataUsed&&no<MaxData;no++)
		{
			SumCEGeo[no]=LTJ*SumCEGeo[no]/NuOfRandomData;
		}
}

//��PA��PB������ֱ���Ҵ�����PB��ƽ���ϣ��ֲ�����PC(PBΪԭ��)��������������ϵ������
GPoint DECalculation::CoordinatesTrasnfer(GPoint PA,GPoint PB,GPoint PC)
{
	GPoint PD,PT;
	double dx=PB.x-PA.x;
	double dy=PB.y-PA.y;
	double dz=PB.z-PA.z;
	double lab=sqrt(dx*dx+dy*dy+dz*dz);//AB����
	if(fabs(lab)<1e-8) //���AB������̫��������ת��ԭ���귵��
	{
		PD.x=PC.x+PB.x;
		PD.y=PC.y+PB.y;
		PD.z=PC.z+PB.z;
		return PD;
	}
	double lyz=sqrt(dy*dy+dz*dz);//AB��YZƽ��ͶӰ����
	if(fabs(lyz)<1e-8) //���AB��������x��������������ת��ԭ���귵��
	{
		PD.x=PC.x+PB.x;
		PD.y=PC.y+PB.y;
		PD.z=PC.z+PB.z;
		return PD;
	}
	double ux,uy,uz;
	ux=lyz/lab;
	uy=-dx*dy/lyz/lab;
	uz=-dx*dz/lyz/lab;
	double vx,vy,vz;
	vx=0;
	vy=dz/lyz;
	vz=-dy/lyz;
	double nx,ny,nz;
	nx=dx/lab;
	ny=dy/lab;
	nz=dz/lab;
	PD.x=PC.x*ux+PC.y*vx+PC.z*nx+PB.x;
	PD.y=PC.x*uy+PC.y*vy+PC.z*ny+PB.y;
	PD.z=PC.x*uz+PC.y*vz+PC.z*nz+PB.z;
	return PD;
}
//��PA��PB������ֱ���Ҵ�����PB��ƽ���ϣ����Ƕ�PBΪԭ��ļ����꣬�뾶�ͽǶȣ���λ��
GPoint DECalculation::CoordinatesTrasnfer(GPoint PA,GPoint PB,double r,double angle)
{
	GPoint PC(r*cos(angle*3.1415926/180),r*sin(angle*3.1415926/180),0);
	return CoordinatesTrasnfer(PA,PB,PC);
}

//���㼸��Ч��
void DECalculation::DetectorGeo(int key,GPoint _PA,GPoint _PB)
{
	int no;
	if(key==0)	for(no=0;no<MaxData;no++) CEGeo_0[no]=0;
	else for(no=0;no<MaxData;no++) CEGeo[no]=0;
	
	LInCellCalculationDec(_PA,_PB);
	//��ļ���
	if(LInCell[0]<=0.00000001) return;//���û�о����������
	double LGe=LInCell[0]-LInCell[1];
	//double uGe=
	//������ļ���
	double LGeDead=(LInCell[2]-LInCell[0]);
	//���ǵļ���
	double LAl=(LInCell[3]-LInCell[4]);
	//׼ֱ������
	double LPb=LInCell[5]-LInCell[6];
	//���ο�
	double LShield=LInCell[7];
	std::string Ctemp;
	double att;
	double LAtt=0;
	double LAbs=0;
	for(int n=0;n<ESampleHead->NuDataUsed&&n<MaxData;n++)
	{
		LAtt=m_mu[Cell[5].NoM][n]*Cell[5].Density*LPb;
		LAtt+=m_mu[Cell[3].NoM][n]*Cell[3].Density*LAl;
		LAtt+=m_mu[Cell[0].NoM][n]*Cell[0].Density*LGeDead;
		if(key==1&&Cell[7].NoM>=0)LAtt+=m_mu[Cell[7].NoM][n]*Cell[7].Density*LShield;
		//LAtt=0;
		LAbs=m_mu[Cell[0].NoM][n]*Cell[0].Density*LGe;
		//LAbs=1;
		att=(1-exp(-LAbs))*exp(-LAtt);	
		if(att<0)
		{
//			Ctemp.Format("%g %g %g %g (%g %g %g)",LAtt,LAbs,LInCell[0],LInCell[1],_PB.x,_PB.y,_PB.z);
//			AfxMessageBox(Ctemp);
		}
		if(key==0) CEGeo_0[n]=att;
		else CEGeo[n]=att;
	}
}

