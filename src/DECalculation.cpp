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

//计算初始化
bool DECalculation::INICalculation()
{
	//读取样本数据
	ESampleHead=TriDataB.ReadData(m_Folder+"E_Sample1.dat");//***1
	if(!ESampleHead||!ESampleHead->FirstNode) return false;
	//样品点的坐标
	PSample.SetPoint(ESampleHead->FirstNode->x,ESampleHead->FirstNode->y,ESampleHead->FirstNode->z);//***2

	MA_Coef MAB;
	//根据样本数据记录能量，并计算质量衰减系数
	for(int n=0;n<ESampleHead->NuDataUsed;n++)
	{
		m_Energy[n]=ESampleHead->VARIABLESData[n];
		for(int no=0;no<11;no++)
		{
			m_mu[no][n]=MAB.MassAttenuation(no,m_Energy[n]);
		}
	}
	//随机数设置
	auto Time = chrono::system_clock::now();
	time_t time = chrono::system_clock::to_time_t(Time);
	int seeddata= tolower(time);
	mt.seed(seeddata%60);
	MaxMT=int(floor(pow(2.0,19937)))-1;


	//读取探测器数据
	if(!ReadDetector()) {
		return false;
	}
	//读取废物箱数据
	if(!ReadWaste()) return false;

	//设置探测器
	SetCellDetector();
	//计算样本
	ESampleCal();
	//设置放射源
	if(!SetSource()) return false;
	NodeUsedx=EHead->FirstNode;
    NodeUsedy=EHead->FirstNode;
	NodeUsedz=EHead->FirstNode;

return true;
}
//读取探测器数据
bool DECalculation::ReadDetector(void)
{
return true;
}
//读取废物箱数据
bool DECalculation::ReadWaste(void)
{
//在此设置废物参数
return true;
}

//设置体素 *************************** 3
//注意：
void DECalculation::SetCellDetector()
{
	//所有单位为cm
	//准直器
	GPoint PCollimator(45,0,0);//准直器前端面坐标
	int NoMaOfCollimator=10;//材料;
	double DeOfCollimator=11.32;
	//double HFeWall=1;//铁衬厚度，单位cm;
	double W1OfCollimator=36;//准直器宽度，单位cm;
	double H1OfCollimator=26;//准直器高度，单位cm;
	double W2OfCollimator=20;//准直口宽度，单位cm;
	double H2OfCollimator=9;//准直口高度，单位cm;
	double D1OfCollimator=30;//准直器直径，单位cm;
	double D2OfCollimator=10;//准直口直径，单位cm;
	double LOfCollimator=25;//准直器深度，单位cm;
	bool IsShield=false;//是否有屏蔽块;
	double LOfShield=8;//屏蔽厚度，单位cm;
	int NoMaOfShield=8;//屏蔽材料编号
	double DeOfShield=7.8;//屏蔽材料密度，g/cc;
	//探测器
	GPoint PAl;PAl.SetPoint(PCollimator,LOfCollimator,0,0);//铝帽前端坐标
	int NoMaOfCrystal=7;//晶体材料
	double DeOfCrystal=5.35;//晶体密度，g/cm3
	double ROfCrystal=3.045;//晶体半径，单位cm;
	double hOfCrystalDead=0.07;//死层厚度，单位cm;
	double LOfCrystal=5.18;//晶体长度，单位cm;
	int NoMaOfCup=6;//帽材料
	double ROfCup=3.225;//帽半径，cm
	double hOfCup=0.15;//帽厚度，cm
	double DeOfCup=2.7;//帽密度，g/cm3
	double dL_CupGe=0.5;//铝壳与Ge之间的间隙
	int NoMaOfCold=9;//晶体材料
	double ROfCold=0.435;//冷芯半径，单位cm;
	double LOfCold=3.82;//冷芯长度，单位cm;
	double DeOfCold=8.96;//冷芯密度，g/cm3
	GPoint PEnd;PEnd.SetPoint(PAl,2*(dL_CupGe+hOfCup+hOfCrystalDead)+LOfCrystal,0,0);//铝壳后端

	//设置体素
	//材料编号：1 真空 2water 3Polyethylene 4Concrete 5Glass 用Concrete代替  6Al 7Ge 8Fe 9Cu用Fe代替，10Pb 其他水
	int no=0;
	//0是灵敏体，冷指阱是铜棒，注意计算时扣除铜棒
	if(!Cell[no].Plane.empty())Cell[no].Plane.clear();
	PGe.SetPoint(PAl,hOfCup+dL_CupGe+hOfCrystalDead+0.5*LOfCrystal,0,0);
	Cell[no].Cylinder(PGe,'x',ROfCrystal,LOfCrystal);
	Cell[no].NoM=NoMaOfCrystal;
	Cell[no].Density=DeOfCrystal;
	m_RGe=ROfCrystal;
	m_LGe=LOfCrystal;
	no++;
	//1是铜棒
	if(!Cell[no].Plane.empty())Cell[no].Plane.clear();
	GPoint PCu;PCu.SetPoint(PAl,hOfCup+dL_CupGe+hOfCrystalDead+LOfCrystal-LOfCold*0.5,0,0);
	Cell[no].Cylinder(PCu,'x',ROfCold,LOfCold);
	Cell[no].NoM=NoMaOfCold;
	Cell[no].Density=DeOfCold;
	no++;
	//2是死层锗,仅定义大圆柱，减去灵敏体
	if(!Cell[no].Plane.empty())Cell[no].Plane.clear();
	Cell[no].Cylinder(PGe,'x',ROfCrystal+hOfCrystalDead,LOfCrystal+hOfCrystalDead*2);
	Cell[no].NoM=NoMaOfCrystal;
	Cell[no].Density=DeOfCrystal;
	no++;
	//3是铝壳,仅定义大圆柱，减去空心部分
	if(!Cell[no].Plane.empty())Cell[no].Plane.clear();
	GPoint PAl0;PAl0.SetPoint(PAl,0.5*(PEnd.x-PAl.x),0,0);//铝壳体素中心，不是前端中心
	Cell[no].Cylinder(PAl0,'x',ROfCup,PEnd.x-PAl.x);
	Cell[no].NoM=NoMaOfCup;
	Cell[no].Density=DeOfCup;
	no++;
	//4是铝壳空心部分
	if(!Cell[no].Plane.empty())Cell[no].Plane.clear();
	Cell[no].Cylinder(PAl0,'x',ROfCup-hOfCup,PEnd.x-PAl.x-2*hOfCup);
	Cell[no].NoM=NoMaOfCup;
	Cell[no].Density=DeOfCup;
	no++;
	//5是准直器
	GPoint PPb0;PPb0.SetPoint(PCollimator,0.5*(PEnd.x-PCollimator.x),0,0);
	if(!Cell[no].Plane.empty())Cell[no].Plane.clear();
	Cell[no].Cuboid(PPb0,PEnd.x-PCollimator.x,W1OfCollimator,H1OfCollimator);
	Cell[no].NoM=NoMaOfCollimator;
	Cell[no].Density=DeOfCollimator;
	no++;
	//6是准直器中间扣除部分，准直孔
	if(!Cell[no].Plane.empty())Cell[no].Plane.clear();
	Cell[no].Cuboid(PPb0,PEnd.x-PCollimator.x,W2OfCollimator,H2OfCollimator);
	Cell[no].NoM=1;
	Cell[no].Density=0;
	no++;
	//7是准直孔屏蔽块
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
	//共计8个体素
	NuCellDec=no;
}
//设置废物箱
void DECalculation::SetCellWaste()
{
	//所有单位为cm
	//废物箱
	PWaste.SetPoint(0,0,0);//废物箱中心坐标
	double RWaste1=35;//废物桶内半径，废物半径
	double RWaste2=35.15;//废物桶外半径

	int no=NuCellDec;
	//废物桶从10开始编号
	if(!Cell[no].Plane.empty())Cell[no].Plane.clear();
	Cell[no].Cylinder(PWaste,'z',RWaste1,RWaste2,2000);//废物桶壳，长度设置较大，覆盖测量区域
	no++;

	//体素总数
	NuCellTotal=no;
}
//设置源的坐标
bool DECalculation::SetSource()//////////////***********4
{
	if(EHead) TriDataB.DeleteHead(EHead);
	//创建点源节点，x,y,z的数量，参照样本（能量一样）

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
//以下为效率计算
///////////////////////////////////////////

//样本数据的计算
void DECalculation::ESampleCal()
{
	CalculationOnePoint(0,PSample,PGe);
}
//任意点一点的计算，供Calculation类调用
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
//输出结果与后处理
void DECalculation::Output(std::string FileName)
{
	if(FileName.length()<1) //不指定文件名，用默认文件名
		TriDataB.outputdata_3d(EHead,m_Folder+EHead->Title,"xyz");
	else
	TriDataB.outputdata_3d(EHead,FileName,"xyz");//按照指定文件名输出
	//后处理
	PostProcess();
}
//后处理
void DECalculation::PostProcess(std::string FileName)
{
	if(EHead) TriDataB.DeleteHead(EHead);
	EHead=TriDataB.ReadData(FileName);
	PostProcess();
}
//输出结果与后处理
void DECalculation::PostProcess()
{
	if(EHead==NULL) return;
	//调用后处理
	DEPost DEPostB;
	DEPostB.m_Folder=m_Folder;
	DEPostB.EHead=EHead;
	DEPostB._PGe.SetPoint(PGe);
	DEPostB.Process();
}


void DECalculation::ECal_OnePoint(GPoint _PA)
{
	//效率计算
	if(!NodeUsedx) return;
	CalculationOnePoint(1,_PA,PGe);
	for(int n=0;n<ESampleHead->NuDataUsed;n++)
	{
		NodeUsedx->data[n]=ESampleHead->FirstNode->data[n]*SumCEGeo[n]/SumCEGeo_0[n];
	}
}

//体素内长度计算,探测器
void DECalculation::LInCellCalculationDec(GPoint PA, GPoint PB)
{
	CellCalculation TempCCal;

	GPoint PB1=TempCCal.ExtendP(PA, PB,1.1);
	GPoint PB2=TempCCal.GetP2(PA, PB1,Cell[6]);

	double L=TempCCal.L_PointsAB(PA,PB2);//两点之间的长度
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
//体素内长度计算,废物
void DECalculation::LInCellCalculationWaste(GPoint PA,GPoint PB)
{
	CellCalculation TempCCal;
	double L=TempCCal.L_PointsAB(PA,PB);//两点之间的长度
	for(int n=NuCellDec;n<NuCellTotal;n++)
	{
		 LInCell[n]=TempCCal.L_Cell(PA, PB,Cell[n],L);	
	}
}

//计算一个点的效率
//A点发生粒子向探测器，以晶体中心O为原点，画一个圆（圆平面与AO垂直）
//在圆内随机生成点B，计算AB射线的效率
//key=0计算样本，key=1计算任意点，两点坐标_PA,_PB，灵敏体中心点Dec
#define NuOfRandomData 1000
void DECalculation::CalculationOnePoint(int key,GPoint _PA,GPoint Dec)
{
	double Rs;
	double dx=_PA.x-Dec.x;
	double dy=_PA.y-Dec.y;
	double dz=_PA.z-Dec.z;
	double r=sqrt(dy*dy+dz*dz);
	double rGe=m_RGe;//锗半径
	double L2Ge=m_LGe*0.5;//锗晶体长度一半
	double LAO=sqrt(r*r+dx*dx);//AO之间的长度
	double LTJ;
	GPoint PA(_PA.x,_PA.y,_PA.z);
	GPoint PB;
	//计算立体角
	if(key==0)//坐标系内参考点发射，立体角中心为x轴
	{
		Rs=(1+L2Ge-dx)/(-L2Ge-dx)*rGe;
		PB.SetPoint(Dec.x+L2Ge+1,Dec.y,Dec.z);
		double LA3=Dec.x+L2Ge+1-_PA.x;
		LTJ=0.5*(1-sqrt(LA3*LA3-Rs*Rs)/LA3);//立体角
	}
	else//空间任意点发射，计算AO垂直圆面，立体角中心为AO连线
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
		double LA3=sqrt((x3-dx)*(x3-dx)+(r-y3)*(r-y3));//A到圆所在面的距离

		double L31=LA3/LA1*L1;
		double L32=LA3/LA2*L2;
		Rs=L32;
		if(L31>L32) Rs=L31;
		double kt=LA3/LAO;
		PB.SetPoint(kt*(Dec.x-_PA.x)+_PA.x,
			         kt*(Dec.y-_PA.y)+_PA.y,
					 kt*(Dec.z-_PA.z)+_PA.z);
		LTJ=0.5*(1-sqrt(LA3*LA3-Rs*Rs)/LA3);//立体角
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

//与PA至PB向量垂直，且垂足是PB的平面上，局部坐标PC(PB为原点)，求在世界坐标系的坐标
GPoint DECalculation::CoordinatesTrasnfer(GPoint PA,GPoint PB,GPoint PC)
{
	GPoint PD,PT;
	double dx=PB.x-PA.x;
	double dy=PB.y-PA.y;
	double dz=PB.z-PA.z;
	double lab=sqrt(dx*dx+dy*dy+dz*dz);//AB距离
	if(fabs(lab)<1e-8) //如果AB两个点太近，不旋转，原坐标返回
	{
		PD.x=PC.x+PB.x;
		PD.y=PC.y+PB.y;
		PD.z=PC.z+PB.z;
		return PD;
	}
	double lyz=sqrt(dy*dy+dz*dz);//AB在YZ平面投影长度
	if(fabs(lyz)<1e-8) //如果AB两个点是x方向向量，不旋转，原坐标返回
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
//与PA至PB向量垂直，且垂足是PB的平面上，点是对PB为原点的极坐标，半径和角度，单位°
GPoint DECalculation::CoordinatesTrasnfer(GPoint PA,GPoint PB,double r,double angle)
{
	GPoint PC(r*cos(angle*3.1415926/180),r*sin(angle*3.1415926/180),0);
	return CoordinatesTrasnfer(PA,PB,PC);
}

//计算几何效率
void DECalculation::DetectorGeo(int key,GPoint _PA,GPoint _PB)
{
	int no;
	if(key==0)	for(no=0;no<MaxData;no++) CEGeo_0[no]=0;
	else for(no=0;no<MaxData;no++) CEGeo[no]=0;
	
	LInCellCalculationDec(_PA,_PB);
	//锗的计算
	if(LInCell[0]<=0.00000001) return;//如果没有经过灵敏体积
	double LGe=LInCell[0]-LInCell[1];
	//double uGe=
	//死层锗的计算
	double LGeDead=(LInCell[2]-LInCell[0]);
	//铝壳的计算
	double LAl=(LInCell[3]-LInCell[4]);
	//准直器长度
	double LPb=LInCell[5]-LInCell[6];
	//屏蔽块
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

