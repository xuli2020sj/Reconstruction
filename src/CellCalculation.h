#pragma once
#ifndef CellCal_H
#define CellCal_H
#include<vector>
#include <iostream>

using namespace std;
//namespace GCELL_2019

	#ifdef __cplusplus
	#define NULL 0
	#else
	#define NULL ((void *)0)
	#endif

   //栅元与直线两个交点的参数值，用于kp记录
	struct KpDefine
	{
		double k1;
		double k2;
	};
   //焦点参数k的链节点，用于栅元布尔计算时排序节点
   //两个栅元计算时，交点k1，k2，以及内部的kp值，进行从小到大排序
	struct KpDefine2
	{
		double k;//k值
       //位置符号，五位数ABCDE，
		//A：1是前断点2是后端点，B：每个端点的k标识，1是k1，2是k2，
		//CDE是kp的编号，1至999，0是边节点（k1或k2）
		int pos;
	    //指针
		KpDefine2 *next;
		KpDefine2 *last;
	};
	class KDefine//在直线AB上的两个点的参数，0<=k1<=K2<=1
	{
	public:
		bool IsExist;
		double k1;//靠近A的点
		double k2;//靠近B的点
		vector<KpDefine> kp;//附加点，首末交点中间的断点

		double dk;//k值的差，乘以AB长度就是穿透距离
		KDefine(void)//构造函数
		{
			k1=0;k2=1;dk=1;IsExist=false;
		}
		KDefine(double K1,double K2)//构造函数
		{
			k1=K1;k2=K2;dk=K2-K1;IsExist=false;
		}
		~KDefine(void)
		{
			if(!kp.empty()) kp.clear();
		}
		//函数
		void AnalyzeKP(void);//进行kp值的分析，一般是k1,k2有新值后，分析kp存在的必要性
		void pushback_kp(KpDefine data)//添加K，不考虑附加点，参数：添加K的类
		{kp.push_back(data);}
		void pushback_kp(double kt1,double kt2)//添加K，不考虑附加点，参数：两个k值
		{
			KpDefine temp;
			temp.k1=kt1;
			temp.k2=kt2;
			kp.push_back(temp);
		}
		void pushback_kp2(double kt1,double kt2);//添加K，考虑附加点，参数：两个k值
		void pushback_kp2(vector<KpDefine> kpt)//添加K，考虑附加点，参数：添加K的类
		{
			pushback_kp2(kpt[0].k1,kpt[0].k2);
		}

		void OutputInf();
	};
	/////////////类定义：点
	class GPoint
	{
	public:
		double x;
		double y;
		double z;
		double activity;
		GPoint(void)//默认为原点
		{
			x=0;	y=0;	z=0;activity=1;
		}
		GPoint(double X, double Y,double Z)//构造函数
		{
			x=X;	y=Y;	z=Z;activity=1;
		}
		GPoint(double X, double Y,double Z,double A)//构造函数
		{
			x=X;	y=Y;	z=Z;  activity=A;
		}
		~GPoint(void)
		{	
		}
		void SetPoint(GPoint P)//设置点，参数：参考点
		{
			x=P.x;	y=P.y;	z=P.z; activity=P.activity;
		}
		void SetPoint(double X, double Y,double Z)//设置点，参数：三个坐标
		{
			x=X;	y=Y;	z=Z;activity=1;
		}
		void SetPoint(double X, double Y,double Z,double A)//设置点，参数：三个坐标
		{
			x=X;	y=Y;	z=Z;activity=A;
		}
		void SetPoint(GPoint p0,double dX, double dY,double dZ)//设置点，参数：三个坐标
		{
			x=p0.x+dX;	y=p0.y+dY;	z=p0.z+dZ;activity=0;
		}
		void OutputPoint()
		{			
			printf("Point(%g, %g, %g)\n",x,y,z);			
		}
	};
	/////////////类定义：面
	class GPlane :
		public GPoint
	{
	public:
		GPlane(void)//默认处于原点的球
		{
			type='S';A=1;B=1;C=1;R=1;
		}
		GPlane(char Type,double A1,double B1,double C1,double D1)//平面
		{
			type=Type;A=A1;B=B1;C=C1;R=D1;
		}
		GPlane(GPoint P,double R1)//球面
		{
			type='S';P0.SetPoint(P);R=R1;A=1;B=1;C=1;
		}
		GPlane(double R1)//球面，中心是原点
		{
			type='S';R=R1;A=1;B=1;C=1;
		}
		GPlane(GPoint P,char axis,double R1)//柱面
		{
			type='C';
			if(axis=='x'||axis=='X') 
			{P0.SetPoint(P);A=0;B=1;C=1;R=R1;}
			else if(axis=='y'||axis=='Y') 
			{P0.SetPoint(P);A=1;B=0;C=1;R=R1;}		
			else if(axis=='z'||axis=='Z') 
			{P0.SetPoint(P);A=1;B=1;C=0;R=R1;}				
		}
		GPlane(char axis,double R1)//柱面，中心是原点
		{
			type='C';
			if(axis=='x'||axis=='X') 
			{A=0;B=1;C=1;R=R1;}
			else if(axis=='y'||axis=='Y') 
			{A=1;B=0;C=1;R=R1;}		
			else if(axis=='z'||axis=='Z') 
			{A=1;B=1;C=0;R=R1;}				
		}
		~GPlane(void)
		{
		}
		char type; //'P'平面，'C'柱面，'S'球面
		/*
		平面方程：Ax+By+Cz=R
		如果是PX  A=1，B=0，C=0等等

		球面方程：A(x-P0.x)^2+B(y-P0.y)^2+C(z-P0.z)^2=R^2，A=1,B=1,C=1

		如果是柱面方程：
		如果轴平行于x轴，A=0,B=1,C=1, A(x-P0.x)^2+B(y-P0.y)^2+C(z-P0.z)^2=R^2
		如果轴平行于y轴，A=1,B=0,C=1, A(x-P0.x)^2+B(y-P0.y)^2+C(z-P0.z)^2=R^2
		如果轴平行于z轴，A=1,B=1,C=0, A(x-P0.x)^2+B(y-P0.y)^2+C(z-P0.z)^2=R^2
		*/
		GPoint P0;
		double A;
		double B;
		double C;
		double R;
		void SetPlane(char Type,double A1,double B1,double C1,double D1)//设置平面，所有参数，默认原点
		{
			type=Type;	A=A1;B=B1;C=C1;R=D1;
		}
		void SetPlane(char Type,GPoint P,double A1,double B1,double C1,double D1)//设置平面，最通用模式，所有参数
		{
			type=Type;	P0.SetPoint(P);A=A1;B=B1;C=C1;R=D1;
		}
		void CopyPlane(GPlane PTemp)//拷贝平面，参数：参考平面
		{
			type=PTemp.type;	P0.SetPoint(PTemp.P0);
			A=PTemp.A;B=PTemp.B;C=PTemp.C;R=PTemp.R;
		}
		void PlaneX(double x)//设置平面 与x轴垂直，参数：与轴相交坐标
		{
			type='P';A=1;B=0;C=0;R=x;
		}
		void PlaneY(double y)//设置平面 与y轴垂直，参数：与轴相交坐标
		{
			type='P';A=0;B=1;C=0;R=y;
		}
		void PlaneZ(double z)//设置平面 与z轴垂直，参数：与轴相交坐标
		{
			type='P';A=0;B=0;C=1;R=z;
		}
		void PlaneC(GPoint P,double A1,double B1,double C1,double R1)//设置柱面 参数：轴线经过的参考点，通用方程四个参数
		{
			type='C';	P0.SetPoint(P);A=A1;B=B1;C=C1;R=R1;
		}
		void PlaneC(double A1,double B1,double C1,double R1)//设置柱面 轴线经过的参考点为原点，参数：通用方程四个参数
		{
			type='C';A=A1;B=B1;C=C1;R=R1;
		}
		void PlaneCX(GPoint P,double R1)//设置柱面 中心轴x平行，参数：轴线经过的参考点，半径
		{
			type='C';	P0.SetPoint(P);A=0;B=1;C=1;R=R1;
		}
		void PlaneCY(GPoint P,double R1)//设置柱面 中心轴y平行，参数：轴线经过的参考点，半径
		{
			type='C';	P0.SetPoint(P);A=1;B=0;C=1;R=R1;
		}
		void PlaneCZ(GPoint P,double R1)//设置柱面 中心轴z平行，参数：轴线经过的参考点，半径
		{
			type='C';	P0.SetPoint(P);A=1;B=1;C=0;R=R1;
		}
		void PlaneS(GPoint P,double R1)//设置圆，参数：中心点，半径
		{
			type='S';	P0.SetPoint(P);A=1;B=1;C=1;R=R1;
		}
		void PlaneS(double R1)//设置圆，参数：半径
		{
			type='S';	A=1;B=1;C=1;R=R1;
		}
		void OutputPlane()
		{
			if(type=='P')
			{

				printf("Plane: %g x + %g y + %g z = %g ",A,B,C,R);			
			}
			else if(type=='C')
			{
				printf("Plane: %g*(x-%g)^2+%g*(y-%g)^2+%g*(z-%g)^2=%g^2 ",A,P0.x,B,P0.y,C,P0.z,R);			
			}
			else 
			{
				printf("Plane: %g*(x-%g)^2+%g*(y-%g)^2+%g*(z-%g)^2=%g^2 ",A,P0.x,B,P0.y,C,P0.z,R);		
			}
		}
	};
	/////////////类定义：体
	class GCell :
	public GPlane
	{
	public:
		GCell(void)//构造函数
		{
			NoM=-1;//表示真空
			No=0;
			Density=0;
			Activity=0;
			next=NULL;
		}
		GCell(int NoM);//构造函数
		GCell(int noM,double activity);
		GCell(int noM,double density,double activity);
		GCell(int noM,int no,double density, double activity);
		~GCell(void)//析构函数
		{
			if(!Plane.empty())Plane.clear(); 
			if(!PM.empty()) PM.clear(); 
		}
		GPoint P0;//体素的坐标系原点
		vector <GPlane> Plane;
		vector <char> PM;
		int No;//体素编号
		int NoM;//材料编号
		double Density;
		double Activity;
		GCell *next;
		void SetCellMaterial(int nom,double d,double A)
		{
			NoM=nom;Density=d;Activity=A;
		}
		//添加平面
		void AddPlane(GPlane plane,char pm)
		{
			Plane.push_back(plane);
			PM.push_back(pm);		
		}
		int PlaneNu(void)
		{return Plane.size();}
		//xyz方向的箱体,参数说明：体中心坐标，x方向起始位置，y方向起始位置，z方向起始位置，注意同一方向值由小变大
		void Cuboid(GPoint p0,double x1,double x2,double y1,double y2,double z1,double z2)
		{
			if(!Plane.empty()) Plane.clear();
			if(!PM.empty()) PM.clear();
			P0.SetPoint(p0);
			GPlane px1('P',1,0,0,x1);AddPlane(px1,'+');
			GPlane px2('P',1,0,0,x2);AddPlane(px2,'-');
			GPlane py1('P',0,1,0,y1);AddPlane(py1,'+');
			GPlane py2('P',0,1,0,y2);AddPlane(py2,'-');
			GPlane pz1('P',0,0,1,z1);AddPlane(pz1,'+');
			GPlane pz2('P',0,0,1,z2);AddPlane(pz2,'-');
		}
		//xyz方向的箱体,参数说明：体中心坐标，x方向长度，y方向长度，z方向长度，不能是负值
		void Cuboid(GPoint p0,double dx,double dy,double dz)
		{
			if(!Plane.empty()) Plane.clear();
			if(!PM.empty()) PM.clear();
			P0.SetPoint(p0);
			GPlane px1('P',1,0,0,p0.x-dx*0.5);AddPlane(px1,'+');
			GPlane px2('P',1,0,0,p0.x+dx*0.5);AddPlane(px2,'-');
			GPlane py1('P',0,1,0,p0.y-dy*0.5);AddPlane(py1,'+');
			GPlane py2('P',0,1,0,p0.y+dy*0.5);AddPlane(py2,'-');
			GPlane pz1('P',0,0,1,p0.z-dz*0.5);AddPlane(pz1,'+');
			GPlane pz2('P',0,0,1,p0.z+dz*0.5);AddPlane(pz2,'-');
		}
		//圆柱,参数说明：体中心坐标，主轴方向，外径，高度
		void Cylinder(GPoint p0,char axis,double radius,double height)
		{
			if(!Plane.empty()) Plane.clear();
			if(!PM.empty()) PM.clear();
			P0.SetPoint(p0);
			if(axis=='x'||axis=='X')
			{
				GPlane px1('P',1,0,0,p0.x-height*0.5);AddPlane(px1,'+');
				GPlane px2('P',1,0,0,p0.x+height*0.5);AddPlane(px2,'-');				
			}
			else if(axis=='y'||axis=='Y')
			{
				GPlane py1('P',0,1,0,p0.y-height*0.5);AddPlane(py1,'+');
				GPlane py2('P',0,1,0,p0.y+height*0.5);AddPlane(py2,'-');
			}	
			else if(axis=='z'||axis=='Z')
			{
				GPlane pz1('P',0,0,1,p0.z-height*0.5);AddPlane(pz1,'+');
				GPlane pz2('P',0,0,1,p0.z+height*0.5);AddPlane(pz2,'-');		
			}
				GPlane pc(p0,axis,radius);AddPlane(pc,'-');
		}
		//圆管,参数说明：体中心坐标，主轴方向，内径，外径，高度
		void Cylinder(GPoint p0,char axis,double radius1,double radius2,double height)
		{
			if(!Plane.empty()) Plane.clear();
			if(!PM.empty()) PM.clear();
			P0.SetPoint(p0);
			if(axis=='x'||axis=='X')
			{
				GPlane px1('P',1,0,0,p0.x-height*0.5);AddPlane(px1,'+');
				GPlane px2('P',1,0,0,p0.x+height*0.5);AddPlane(px2,'-');
			}
			else if(axis=='y'||axis=='Y')
			{
				GPlane py1('P',0,1,0,p0.y-height*0.5);AddPlane(py1,'+');
				GPlane py2('P',0,1,0,p0.y+height*0.5);AddPlane(py2,'-');
			}	
			else if(axis=='z'||axis=='Z')
			{
				GPlane pz1('P',0,0,1,p0.z-height*0.5);AddPlane(pz1,'+');
				GPlane pz2('P',0,0,1,p0.z+height*0.5);AddPlane(pz2,'-');	
			}
			GPlane pc1(p0,axis,radius1);AddPlane(pc1,'+');
			GPlane pc2(p0,axis,radius2);AddPlane(pc2,'-');
		}
		//球,参数说明：体中心坐标，半径
		void Ball(GPoint p0,double radius)
		{
			if(!Plane.empty()) Plane.clear();
			if(!PM.empty()) PM.clear();
			P0.SetPoint(p0);
			GPlane ps(p0,radius);AddPlane(ps,'-');
		}
		//球,参数说明：体中心坐标，内径，外径
		void Ball(GPoint p0,double radius1,double radius2)
		{
			if(!Plane.empty()) Plane.clear();
			if(!PM.empty()) PM.clear();
			P0.SetPoint(p0);
			GPlane ps1(p0,radius1);AddPlane(ps1,'+');
			GPlane ps2(p0,radius2);AddPlane(ps2,'-');
		}
		//设置物性参数
		void SetMaterial(int noM);
		void SetMaterial(int noM,double activity);
		void OutPutInf();
		};


class CellCalculation
{
public:
	CellCalculation(void);
	~CellCalculation(void);
	double Attenuation(int noM,double E);//计算衰减系数，质量衰减系数由noM和E确定，密度由noM确定默认值，E单位MeV
	double Attenuation(int noM,double density,double E);//计算衰减系数，质量衰减系数由noM和E确定，E单位MeV，自己输入密度，单位g/cc
	KDefine k_InterPoint(GPoint PA, GPoint PB, GPlane P,char PM);//计算AB连线与面的交点
	KDefine K_Cell(GPoint PA, GPoint PB,GCell Cell);//计算AB连线在体素的交点
	KDefine ADeductB(GPoint PA, GPoint PB,GCell CellA,GCell CellB);//计算AB连线在体素A中扣除B后的交点
	double L_PointsAB(GPoint PA, GPoint PB);//计算AB连线长度
	double L_Cell(GPoint PA, GPoint PB,GCell Cell);//计算AB连线在体素内的穿透长度
	double L_Cell(GPoint PA, GPoint PB,KDefine KA);//根据K节点求穿透长度
	double L_Cell(GPoint PA, GPoint PB,GCell Cell,double L);
	GPoint GetP1(GPoint PA, GPoint PB,GCell Cell);
	GPoint GetP2(GPoint PA, GPoint PB,GCell Cell);
	GPoint ExtendP(GPoint PA, GPoint PB,double times);
    KDefine K_Add(KDefine K1,KDefine K2);
    KDefine K_Inter(KDefine K1,KDefine K2);
    KDefine K_1Minus2(KDefine K1,KDefine K2);
    KpDefine2 *CreatNode(double k,int pos);
    KpDefine2 *CreatNode(KpDefine2 *last,double k,int pos);
    KpDefine2 *InsertNode(KpDefine2 *start,double k,int pos);
    void DeleteNodes(KpDefine2 *start);
    KpDefine2 *DeleteNode(KpDefine2 *start,KpDefine2 *node);
    KpDefine2 *DeleteNodes(KpDefine2 *start,KpDefine2 *node1,KpDefine2 *node2);
};

#endif