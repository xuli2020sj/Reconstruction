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

   //դԪ��ֱ����������Ĳ���ֵ������kp��¼
	struct KpDefine
	{
		double k1;
		double k2;
	};
   //�������k�����ڵ㣬����դԪ��������ʱ����ڵ�
   //����դԪ����ʱ������k1��k2���Լ��ڲ���kpֵ�����д�С��������
	struct KpDefine2
	{
		double k;//kֵ
       //λ�÷��ţ���λ��ABCDE��
		//A��1��ǰ�ϵ�2�Ǻ�˵㣬B��ÿ���˵��k��ʶ��1��k1��2��k2��
		//CDE��kp�ı�ţ�1��999��0�Ǳ߽ڵ㣨k1��k2��
		int pos;
	    //ָ��
		KpDefine2 *next;
		KpDefine2 *last;
	};
	class KDefine//��ֱ��AB�ϵ�������Ĳ�����0<=k1<=K2<=1
	{
	public:
		bool IsExist;
		double k1;//����A�ĵ�
		double k2;//����B�ĵ�
		vector<KpDefine> kp;//���ӵ㣬��ĩ�����м�Ķϵ�

		double dk;//kֵ�Ĳ����AB���Ⱦ��Ǵ�͸����
		KDefine(void)//���캯��
		{
			k1=0;k2=1;dk=1;IsExist=false;
		}
		KDefine(double K1,double K2)//���캯��
		{
			k1=K1;k2=K2;dk=K2-K1;IsExist=false;
		}
		~KDefine(void)
		{
			if(!kp.empty()) kp.clear();
		}
		//����
		void AnalyzeKP(void);//����kpֵ�ķ�����һ����k1,k2����ֵ�󣬷���kp���ڵı�Ҫ��
		void pushback_kp(KpDefine data)//���K�������Ǹ��ӵ㣬���������K����
		{kp.push_back(data);}
		void pushback_kp(double kt1,double kt2)//���K�������Ǹ��ӵ㣬����������kֵ
		{
			KpDefine temp;
			temp.k1=kt1;
			temp.k2=kt2;
			kp.push_back(temp);
		}
		void pushback_kp2(double kt1,double kt2);//���K�����Ǹ��ӵ㣬����������kֵ
		void pushback_kp2(vector<KpDefine> kpt)//���K�����Ǹ��ӵ㣬���������K����
		{
			pushback_kp2(kpt[0].k1,kpt[0].k2);
		}

		void OutputInf();
	};
	/////////////�ඨ�壺��
	class GPoint
	{
	public:
		double x;
		double y;
		double z;
		double activity;
		GPoint(void)//Ĭ��Ϊԭ��
		{
			x=0;	y=0;	z=0;activity=1;
		}
		GPoint(double X, double Y,double Z)//���캯��
		{
			x=X;	y=Y;	z=Z;activity=1;
		}
		GPoint(double X, double Y,double Z,double A)//���캯��
		{
			x=X;	y=Y;	z=Z;  activity=A;
		}
		~GPoint(void)
		{	
		}
		void SetPoint(GPoint P)//���õ㣬�������ο���
		{
			x=P.x;	y=P.y;	z=P.z; activity=P.activity;
		}
		void SetPoint(double X, double Y,double Z)//���õ㣬��������������
		{
			x=X;	y=Y;	z=Z;activity=1;
		}
		void SetPoint(double X, double Y,double Z,double A)//���õ㣬��������������
		{
			x=X;	y=Y;	z=Z;activity=A;
		}
		void SetPoint(GPoint p0,double dX, double dY,double dZ)//���õ㣬��������������
		{
			x=p0.x+dX;	y=p0.y+dY;	z=p0.z+dZ;activity=0;
		}
		void OutputPoint()
		{			
			printf("Point(%g, %g, %g)\n",x,y,z);			
		}
	};
	/////////////�ඨ�壺��
	class GPlane :
		public GPoint
	{
	public:
		GPlane(void)//Ĭ�ϴ���ԭ�����
		{
			type='S';A=1;B=1;C=1;R=1;
		}
		GPlane(char Type,double A1,double B1,double C1,double D1)//ƽ��
		{
			type=Type;A=A1;B=B1;C=C1;R=D1;
		}
		GPlane(GPoint P,double R1)//����
		{
			type='S';P0.SetPoint(P);R=R1;A=1;B=1;C=1;
		}
		GPlane(double R1)//���棬������ԭ��
		{
			type='S';R=R1;A=1;B=1;C=1;
		}
		GPlane(GPoint P,char axis,double R1)//����
		{
			type='C';
			if(axis=='x'||axis=='X') 
			{P0.SetPoint(P);A=0;B=1;C=1;R=R1;}
			else if(axis=='y'||axis=='Y') 
			{P0.SetPoint(P);A=1;B=0;C=1;R=R1;}		
			else if(axis=='z'||axis=='Z') 
			{P0.SetPoint(P);A=1;B=1;C=0;R=R1;}				
		}
		GPlane(char axis,double R1)//���棬������ԭ��
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
		char type; //'P'ƽ�棬'C'���棬'S'����
		/*
		ƽ�淽�̣�Ax+By+Cz=R
		�����PX  A=1��B=0��C=0�ȵ�

		���淽�̣�A(x-P0.x)^2+B(y-P0.y)^2+C(z-P0.z)^2=R^2��A=1,B=1,C=1

		��������淽�̣�
		�����ƽ����x�ᣬA=0,B=1,C=1, A(x-P0.x)^2+B(y-P0.y)^2+C(z-P0.z)^2=R^2
		�����ƽ����y�ᣬA=1,B=0,C=1, A(x-P0.x)^2+B(y-P0.y)^2+C(z-P0.z)^2=R^2
		�����ƽ����z�ᣬA=1,B=1,C=0, A(x-P0.x)^2+B(y-P0.y)^2+C(z-P0.z)^2=R^2
		*/
		GPoint P0;
		double A;
		double B;
		double C;
		double R;
		void SetPlane(char Type,double A1,double B1,double C1,double D1)//����ƽ�棬���в�����Ĭ��ԭ��
		{
			type=Type;	A=A1;B=B1;C=C1;R=D1;
		}
		void SetPlane(char Type,GPoint P,double A1,double B1,double C1,double D1)//����ƽ�棬��ͨ��ģʽ�����в���
		{
			type=Type;	P0.SetPoint(P);A=A1;B=B1;C=C1;R=D1;
		}
		void CopyPlane(GPlane PTemp)//����ƽ�棬�������ο�ƽ��
		{
			type=PTemp.type;	P0.SetPoint(PTemp.P0);
			A=PTemp.A;B=PTemp.B;C=PTemp.C;R=PTemp.R;
		}
		void PlaneX(double x)//����ƽ�� ��x�ᴹֱ�������������ཻ����
		{
			type='P';A=1;B=0;C=0;R=x;
		}
		void PlaneY(double y)//����ƽ�� ��y�ᴹֱ�������������ཻ����
		{
			type='P';A=0;B=1;C=0;R=y;
		}
		void PlaneZ(double z)//����ƽ�� ��z�ᴹֱ�������������ཻ����
		{
			type='P';A=0;B=0;C=1;R=z;
		}
		void PlaneC(GPoint P,double A1,double B1,double C1,double R1)//�������� ���������߾����Ĳο��㣬ͨ�÷����ĸ�����
		{
			type='C';	P0.SetPoint(P);A=A1;B=B1;C=C1;R=R1;
		}
		void PlaneC(double A1,double B1,double C1,double R1)//�������� ���߾����Ĳο���Ϊԭ�㣬������ͨ�÷����ĸ�����
		{
			type='C';A=A1;B=B1;C=C1;R=R1;
		}
		void PlaneCX(GPoint P,double R1)//�������� ������xƽ�У����������߾����Ĳο��㣬�뾶
		{
			type='C';	P0.SetPoint(P);A=0;B=1;C=1;R=R1;
		}
		void PlaneCY(GPoint P,double R1)//�������� ������yƽ�У����������߾����Ĳο��㣬�뾶
		{
			type='C';	P0.SetPoint(P);A=1;B=0;C=1;R=R1;
		}
		void PlaneCZ(GPoint P,double R1)//�������� ������zƽ�У����������߾����Ĳο��㣬�뾶
		{
			type='C';	P0.SetPoint(P);A=1;B=1;C=0;R=R1;
		}
		void PlaneS(GPoint P,double R1)//����Բ�����������ĵ㣬�뾶
		{
			type='S';	P0.SetPoint(P);A=1;B=1;C=1;R=R1;
		}
		void PlaneS(double R1)//����Բ���������뾶
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
	/////////////�ඨ�壺��
	class GCell :
	public GPlane
	{
	public:
		GCell(void)//���캯��
		{
			NoM=-1;//��ʾ���
			No=0;
			Density=0;
			Activity=0;
			next=NULL;
		}
		GCell(int NoM);//���캯��
		GCell(int noM,double activity);
		GCell(int noM,double density,double activity);
		GCell(int noM,int no,double density, double activity);
		~GCell(void)//��������
		{
			if(!Plane.empty())Plane.clear(); 
			if(!PM.empty()) PM.clear(); 
		}
		GPoint P0;//���ص�����ϵԭ��
		vector <GPlane> Plane;
		vector <char> PM;
		int No;//���ر��
		int NoM;//���ϱ��
		double Density;
		double Activity;
		GCell *next;
		void SetCellMaterial(int nom,double d,double A)
		{
			NoM=nom;Density=d;Activity=A;
		}
		//���ƽ��
		void AddPlane(GPlane plane,char pm)
		{
			Plane.push_back(plane);
			PM.push_back(pm);		
		}
		int PlaneNu(void)
		{return Plane.size();}
		//xyz���������,����˵�������������꣬x������ʼλ�ã�y������ʼλ�ã�z������ʼλ�ã�ע��ͬһ����ֵ��С���
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
		//xyz���������,����˵�������������꣬x���򳤶ȣ�y���򳤶ȣ�z���򳤶ȣ������Ǹ�ֵ
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
		//Բ��,����˵�������������꣬���᷽���⾶���߶�
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
		//Բ��,����˵�������������꣬���᷽���ھ����⾶���߶�
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
		//��,����˵�������������꣬�뾶
		void Ball(GPoint p0,double radius)
		{
			if(!Plane.empty()) Plane.clear();
			if(!PM.empty()) PM.clear();
			P0.SetPoint(p0);
			GPlane ps(p0,radius);AddPlane(ps,'-');
		}
		//��,����˵�������������꣬�ھ����⾶
		void Ball(GPoint p0,double radius1,double radius2)
		{
			if(!Plane.empty()) Plane.clear();
			if(!PM.empty()) PM.clear();
			P0.SetPoint(p0);
			GPlane ps1(p0,radius1);AddPlane(ps1,'+');
			GPlane ps2(p0,radius2);AddPlane(ps2,'-');
		}
		//�������Բ���
		void SetMaterial(int noM);
		void SetMaterial(int noM,double activity);
		void OutPutInf();
		};


class CellCalculation
{
public:
	CellCalculation(void);
	~CellCalculation(void);
	double Attenuation(int noM,double E);//����˥��ϵ��������˥��ϵ����noM��Eȷ�����ܶ���noMȷ��Ĭ��ֵ��E��λMeV
	double Attenuation(int noM,double density,double E);//����˥��ϵ��������˥��ϵ����noM��Eȷ����E��λMeV���Լ������ܶȣ���λg/cc
	KDefine k_InterPoint(GPoint PA, GPoint PB, GPlane P,char PM);//����AB��������Ľ���
	KDefine K_Cell(GPoint PA, GPoint PB,GCell Cell);//����AB���������صĽ���
	KDefine ADeductB(GPoint PA, GPoint PB,GCell CellA,GCell CellB);//����AB����������A�п۳�B��Ľ���
	double L_PointsAB(GPoint PA, GPoint PB);//����AB���߳���
	double L_Cell(GPoint PA, GPoint PB,GCell Cell);//����AB�����������ڵĴ�͸����
	double L_Cell(GPoint PA, GPoint PB,KDefine KA);//����K�ڵ���͸����
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