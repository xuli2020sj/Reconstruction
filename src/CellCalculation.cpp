#include "CellCalculation.h"
#include "MA_Coef.h"
#include <math.h>
#include <string.h>
#include <fstream>

using namespace std;
#define Mindk 0.00000001

//说明：
/*
任何一根直线可以表示为参数方程：
(x-xA)/(xB-xA)=(y-yA)/(yB-yA)=(z-zA)/(zB-zA)=k
展开为三个方程：
x=k(xB-xA)+xA
y=k(yB-yA)+yA
z=k(zB-zA)+zA
上述方程中的k为参数，物理意义为：任意点到A的长度与AB长度的比值

KDefine类的作用：记录直线与任何体交点的K值
                 如果是均质实心体，即首尾交点的K
				 如果存在空心部分，直线与空心交点，存除在附加kp内

GPoint类的作用：点
GPlane类的作用：面
GCell类的作用：体
CellCalculation类的作用：直线与体素的计算
MA_Coef.h：典型材料的衰减系数及密度的经验公式或值
*/
////////////////////////////////////KDefine////////////////////////
//进行kp值的分析，一般是k1,k2有新值后，分析kp是否改变
void KDefine::AnalyzeKP(void)
{
	if(kp.empty()) return;//
	int n,N=kp.size();
	vector<KpDefine>::iterator it,it2;
	it=kp.begin();
	for(n=0;n<N&&it!=kp.end();n++)
	{
		if(kp.empty()) break;
		if(k1>=it->k1&&k1<=it->k2&&k2>=it->k1&&k2<=it->k2) //在圆内
			{kp.clear();k1=0;k2=0;break;}
		if(k2<=it->k1)
		{
			kp.erase(it,kp.end());break;
		}
		if(k2<=it->k2)
		{
			k2=it->k1;
			kp.erase(it,kp.end());break;
		}
		if(k1>=it->k1)
		{
			if(k1<=it->k2) k1=it->k2;
			it=kp.erase(it);
		}
		else it++;		
	}
}
//附加K的添加
//所谓附加K的意思是：在首尾两个交点中间，如果存在空心部分，直线与空心的交点在这里记录
void KDefine::pushback_kp2(double kt1,double kt2)
{
	//保证大小顺序
	if(kt1>kt2) 
	{
		double t=kt1;
		kt1=kt2;kt2=t;	
	}
	if((kt2-kt1)<Mindk) return;//太小了就不管
	//如果新加入圆在AB之外，就不改变
	if(kt2<=k1) return;
	if(kt1>=k2) return;
	if(kt1<=k1&&kt2>=k2) //AB在新圆内部
	{
		kp.clear();k1=0;k2=0;return;
	}
	//如果新圆k1小于K1，
	if(kt1<=k1)
	{
		k1=kt2;
		AnalyzeKP();
		return;
	}
	//如果新圆k2大于K2，
	if(kt2>=k2)
	{
		k2=kt1;
		AnalyzeKP();
		return;
	}
	//以下情况是新加圆在AB之内
	KpDefine temp;
	temp.k1=kt1;
	temp.k2=kt2;
	//如果原来没有附加的k，直接添加
	if(kp.empty()) {kp.push_back(temp);return;}

	vector<KpDefine>::iterator it1=kp.begin(),it2=kp.end()-1;
	//新圆包括了所有圆，删除所有圆，添加新圆
	if(kt1<=it1->k1&&kt2>=it2->k2)
	{
		kp.clear();
		kp.push_back(temp);	
		return;
	}
	//如果新圆在A与第一个圆之间
	if(kt2<it1->k1)
	{
		kp.insert(it1,temp);return;
	}
	if(kt2<=it1->k2)//如果新圆与第一个圆重合
	{
		it1->k1=kt1;return;
	}
	//如果新圆在B与最后一个圆之间
	if(kt1>it2->k2)
	{
		kp.push_back(temp);return;
	}
	if(kt1>=it2->k1)//如果新圆与最后一个圆重合
	{
		it2->k2=kt2;return;
	}

	//以下情况是新圆在第一个圆和最后一个圆之间
	vector<KpDefine>::iterator it;
	//定位k1，找到k1后面的圆
	for(it=kp.begin();it!=kp.end();it++)
	{
		if(kt1<=it->k1) {it1=it;break;}
		else if(kt1<=it->k2) 
		{
			kt1=it->k1;it1=it;break;
		}
	}
	//定位k2
	for(it=it1;it!=kp.end();it++)
		if(kt2<it->k1)
		{
			it2=it;	
			if((it-1)->k2>=kt2) kt2=(it-1)->k2;			
			break;		
		}
		if(it==kp.end()) it2=it;
		if((it-1)->k2>=kt2) kt2=(it-1)->k2;	
	//如果圆在两个圆之间，直接插入
	//此时不存在与其它圆相交的情况,也不可能在末尾
	if(it1==it2)
	{
		temp.k1=kt1;
		temp.k2=kt2;
		kp.insert(it1,temp);
		return;	
	}
	//如果只相差一个圆，更改这个圆
	if((it1+1)==it2)
	{
		it1->k1=kt1;
		it1->k2=kt2;
		return;
	}
	//如果相差一个圆以上
	it1->k1=kt1;//更改第一个圆
	it1->k2=kt2;
	kp.erase(it1+1,it2);//后面删掉
}

////////////////////////////////////GCell类////////////////////////
GCell::GCell(int noM)
{
	MA_Coef MA;
	if(noM<6&&noM>=0)
	{
		NoM=noM;
		Density=MA.Density(noM);
	}
	else
	{
		NoM=-1;
		Density=0;
	}
	No=0;
	Activity=0;
}
GCell::GCell(int noM,double activity)
{
	MA_Coef MA;
	if(noM<6&&noM>=0)
	{
		NoM=noM;
		Density=MA.Density(noM);
	}
	else
	{
		NoM=-1;
		Density=0;
	}
	No=0;
	Activity=activity;
}
GCell::GCell(int noM,double density, double activity)
{
	MA_Coef MA;
	NoM=noM;
	Density=density;
	No=0;
	Activity=activity;
}
GCell::GCell(int noM,int no,double density, double activity)
{
	MA_Coef MA;
	NoM=noM;
	Density=density;
	No=no;
	Activity=activity;
}
void GCell::SetMaterial(int noM)
{
	MA_Coef MA;
	if(noM<6&&noM>=0)
	{
		NoM=noM;
		Density=MA.Density(noM);
	}
	else
	{
		NoM=-1;
		Density=0;
	}
}
void GCell::SetMaterial(int noM,double activity)
{
	SetMaterial(noM);
	Activity=activity;
}
void KDefine::OutputInf()
{
	printf("K: %g ->",k1);
	for(int i=0;i<kp.size();i++)
	{
		printf("%g  ",kp[i].k1);
		printf("%g->",kp[i].k2);	
	}
	printf("%g\n",k2);
}
void GCell::OutPutInf()
{
	std::string Inf,Temp;
	printf("No:%d, NoM:%d, Density=%g\r\n",No,NoM,Density);
	P0.OutputPoint();
	for(int n=0;n<Plane.size();n++)
	{
		Plane[n].OutputPlane();
		if(PM[n]=='+') printf("+\n");	
		else printf("-\n");	
	}
}

////////////////////////////////////CellCalculation类////////////////////////
CellCalculation::CellCalculation(void)
{
}

CellCalculation::~CellCalculation(void)
{
}
//返回衰减系数，//1 真空 2water 3Polyethylene 4Concrete 5Glass 6Al 7Ge 8Fe 9Cu 10Pb 其他水
double CellCalculation::Attenuation(int noM,double E)
{
	MA_Coef MA;
	double ma=MA.MassAttenuation(noM,E);
	double density=MA.Density(noM);
	return ma*density;
}
double CellCalculation::Attenuation(int noM,double density,double E)
{
	MA_Coef MA;
	double ma=MA.MassAttenuation(noM,E);
	return ma*density;
}
//计算两点之间的距离
double CellCalculation::L_PointsAB(GPoint PA, GPoint PB)
{
	return sqrt(pow(PA.x-PB.x,2)+pow(PA.y-PB.y,2)+pow(PA.z-PB.z,2));
}

//关于平面法向正负的说明
//平面方程 Ax+By+Cz=D,法向向量为（A,B,C）,在定义平面时需要注意 -Ax-By-Cz=-D的法向向量（-A,-B,-C）为前面方程背面的法向方向
//判断直线AB方向与平面法向向量（A,B,C）是同方向还是逆方向,求它与向量（PB-PA）的内积，如果大于零，夹角小于90度，表示两者同方向，否则逆方向	

//计算直线AB与曲面（含平面）相交后，存在体素内的两个点的K值（即重新计算AB点内新的起始与终点）
//PA是A点坐标，PB是B点坐标，P是平面，PM是曲面正负侧标识，体素在与法向侧相同侧为+，相逆侧为负（注意相同平面有两个一正一负的法向）
KDefine CellCalculation::k_InterPoint(GPoint PA, GPoint PB, GPlane P,char PM)
{
	KDefine KData;
	if(P.type=='P'||P.type=='p')//平面
	{//
		double Left=P.A*(PB.x-PA.x)+P.B*(PB.y-PA.y)+P.C*(PB.z-PA.z);//内积
		double Right=P.R-P.A*PA.x-P.B*PA.y-P.C*PA.z;
		double PAD=PA.x*P.A+PA.y*P.B+PA.z*P.C-P.R;
		double PBD=PB.x*P.A+PB.y*P.B+PB.z*P.C-P.R;
		//直线AB全部体外
		if(PAD<=0&&PBD<=0&&PM=='+') //如果AB在平面负法线侧，但是体是正法线侧
				{KData.k1=1;KData.k2=1;}
		else if(PAD>=0&&PBD>=0&&PM=='-') //如果AB在平面正法向线侧，但是体是负法线侧
				{KData.k1=0;KData.k2=0;}	
		//以下直线AB全部或部分在体内
		else if(fabs(Left)>1e-8)
		{///
			double kk=Right/Left;		
			//全部在体内，k值不变
			//以下是部分在体内，k值视情况赋值
			if(kk>0&&kk<1)
			{////
				if(Left>0) //如果A->B与平面P法向同方向（夹角小于90°）
				{/////
					//PM为+，表示体在平面正法向方向侧，如果AB与法向同方向，交点替换A，值赋给k1
					//PM为-，表示体在平面负法向方向侧，如果AB与法向同方向，交点替换B，值赋给k2
					if(PM=='+') // A |-> B
						KData.k1=kk;
					else// A <-| B 
						KData.k2=kk;		
				}/////
				else //如果A->B与平面P法向逆方向（夹角大于90°）
				{/////
					//PM为+，表示体在平面正法向方向侧，如果AB与法向逆方向，交点替换B，值赋给k2
					//PM为-，表示体在平面负法向方向侧，如果AB与法向逆方向，交点替换A，值赋给k1
					if(PM=='+') // B |-> A
						KData.k2=kk;	
					else 	
						KData.k1=kk;	
				}/////	
			}////	
		}///
	}//
	else if(P.type=='S'||P.type=='s'||P.type=='C'||P.type=='c')//球圆柱
	{//
		double C1=P.A,C2=P.B,C3=P.C;
		//圆表面向外为法向方向，向内为负法向方向，PM为+是圆外，PM为-是圆内
		//计算一元2次方程 ax2+bx+c=0
		double a=C1*pow(PB.x-PA.x,2.0)+C2*pow(PB.y-PA.y,2.0)+C3*pow(PB.z-PA.z,2.0);
		double b=2*(C1*(PB.x-PA.x)*(PA.x-P.P0.x)+C2*(PB.y-PA.y)*(PA.y-P.P0.y)+C3*(PB.z-PA.z)*(PA.z-P.P0.z));
		double c=C1*pow(PA.x-P.P0.x,2.0)+C2*pow(PA.y-P.P0.y,2.0)+C3*pow(PA.z-P.P0.z,2.0)-P.R*P.R;
		double d=b*b-4*a*c;
		if(PM=='-')//体素在圆内
		{
			if(d<0)//直线与圆无交点
				{KData.k1=0;KData.k2=0;}
			else//有交点
			{
				double k1=(-1*b-sqrt(d))/(2*a);
				double k2=(-1*b+sqrt(d))/(2*a);
				KData.k1=k1>0?k1:0;
				KData.k2=k2<1?k2:1;
			}		
		}
		else//体素在圆外
		{
			if(d<0)//直线与圆无交点
				{ }
			else//有交点
			{
				double k1=(-1*b-sqrt(d))/(2*a);
				double k2=(-1*b+sqrt(d))/(2*a);

				if(k1>0&&k2<1)//表示圆在AB之间,比较特殊，是两段直线
				{
					KData.k1=0;KData.k2=1;
					KData.pushback_kp(k1,k2);
				}
				else if(k1<=0&&k2<1)//表示A在圆内
				{
					KData.k1=k2;KData.k2=1;
				}
				else if(k1>0&&k2>=1)//表示B在圆内
				{
					KData.k1=0;KData.k2=k1;
				}
				else//k1<=0&&k2>=1表示AB在圆内
				{
					KData.k1=0;KData.k2=0;			
				}
			}			
		}
	}//
	return KData;
}

//计算直线AB穿过体素Cell的两个k值
//PA是A点坐标，PB是B点坐标，Cell是定义的体素
KDefine CellCalculation::K_Cell(GPoint PA, GPoint PB,GCell Cell)
{
	KDefine KData;
	KDefine PK;
	GPlane PTemp;
	char PM;
	for(int i=0;i<Cell.PlaneNu();i++)
	{
		KData.IsExist=true;
		PTemp=Cell.Plane[i];
		PM=Cell.PM[i];
		PK=k_InterPoint(PA, PB, PTemp, PM);
		//同时大于1或小于0或差值太小,直接结束
		if((PK.k1<=0&&PK.k2<=0)||(PK.k1>=1&&PK.k2>=1)||fabs(PK.k1-PK.k2)<Mindk)
		{
			KData.k1=0;
			KData.k2=0;
			KData.dk=0;
			if(!KData.kp.empty()) KData.kp.clear();
			return KData;
		}			
		if(KData.k1<PK.k1) KData.k1=PK.k1;
		if(KData.k2>PK.k2) KData.k2=PK.k2;
		if(!KData.kp.empty())//如果原来有附加的k，表示有空心圆
			KData.AnalyzeKP();
		if(!PK.kp.empty())//如果新计算有附加k，添加
			KData.pushback_kp2(PK.kp);
		if(KData.k1>=KData.k2||fabs(KData.k1-KData.k2)<Mindk)
		{
			KData.k1=0;
			KData.k2=0;
			KData.dk=0;
			if(!KData.kp.empty()) KData.kp.clear();
			return KData;
		}
	}
	KData.dk=KData.k2-KData.k1;
	if(!KData.kp.empty())
	{
	   for (auto it=KData.kp.begin();it!=KData.kp.end();it++)
		 KData.dk-=(it->k2-it->k1);
	}
	return KData;
}
//返回体素交点（不考虑内部空心）的第1个交点坐标
GPoint CellCalculation::GetP1(GPoint PA, GPoint PB,GCell Cell)
{
	KDefine temp=K_Cell(PA,PB,Cell);//找到交点	
	GPoint Temp;
	if(temp.IsExist) 
		Temp.SetPoint(PA.x+temp.k1*(PB.x-PA.x),PA.y+temp.k1*(PB.y-PA.y),PA.z+temp.k1*(PB.z-PA.z));
	return Temp;
}
//返回体素交点（不考虑内部空心）的第2个交点坐标
GPoint CellCalculation::GetP2(GPoint PA, GPoint PB,GCell Cell)
{
	KDefine temp=K_Cell(PA,PB,Cell);//找到交点	
	GPoint Temp;
	if(temp.IsExist) 
		Temp.SetPoint(PA.x+temp.k2*(PB.x-PA.x),PA.y+temp.k2*(PB.y-PA.y),PA.z+temp.k2*(PB.z-PA.z));
	return Temp;
}
GPoint CellCalculation::ExtendP(GPoint PA, GPoint PB,double times)
{
	GPoint Temp;
		Temp.SetPoint(PA.x+(PB.x-PA.x)*times,PA.y+(PB.y-PA.y)*times,PA.z+(PB.z-PA.z)*times);
	return Temp;
}
//计算穿透距离,PA:A点坐标，PB：B点坐标，Cell体
double CellCalculation::L_Cell(GPoint PA, GPoint PB,GCell Cell)
{
	KDefine temp=K_Cell(PA,PB,Cell);//找到交点
	return L_PointsAB(PA,PB)*temp.dk;
}
//计算穿透距离,PA:A点坐标，PB：B点坐标，Cell体
double CellCalculation::L_Cell(GPoint PA, GPoint PB,GCell Cell,double L)
{
	KDefine temp=K_Cell(PA,PB,Cell);//找到交点
	if(temp.IsExist) return L*temp.dk;
	else return 0;
}

//计算穿透距离，扣除空心距离
//KA为体素内空体素的K值集合，
//PA:A点坐标，PB：B点坐标，KA及AB连线上所有空心的k值
double CellCalculation::L_Cell(GPoint PA, GPoint PB,KDefine KA)
{
	double dk=KA.k2-KA.k1;//头尾交点
	if(!KA.kp.empty())//如果存在空心
	{
	   for (auto it=KA.kp.begin();it!=KA.kp.end();it++)
		dk-=(it->k2-it->k1);//减去所有空心
	}
	return L_PointsAB(PA,PB)*dk;//返回实心长度
}
//从体素A中扣除B
KDefine CellCalculation::ADeductB(GPoint PA, GPoint PB,GCell CellA,GCell CellB)
{
	KDefine KA=K_Cell(PA, PB,CellA);
	KDefine KB=K_Cell(PA, PB,CellB);
	KA.OutputInf();
	KB.OutputInf();
	KA.pushback_kp2(KB.k1,KB.k2);
	KA.dk=KA.k2-KA.k1;
	if(!KA.kp.empty())
	{
	   for (auto it=KA.kp.begin();it!=KA.kp.end();it++)
		 KA.dk-=(it->k2-it->k1);
	}
	KA.OutputInf();
	return KA;
}

KpDefine2 *CellCalculation::CreatNode(double k,int pos)
{
	KpDefine2 *temp;
	temp=new KpDefine2;
	temp->k=k;
	temp->pos=pos;
	temp->next=NULL;
	temp->last=NULL;
	return temp;
}
KpDefine2 *CellCalculation::CreatNode(KpDefine2 *last,double k,int pos)
{
	KpDefine2 *temp=CreatNode(k,pos);
	last->next=temp;
	temp->last=last;
	return temp;
}
KpDefine2 *CellCalculation::InsertNode(KpDefine2 *start,double k,int pos)
{
	KpDefine2 *temp=CreatNode(k,pos);
	KpDefine2 *temp2=start,*last=NULL;
	while(temp2&&temp2->k<=k) 
	{
		last=temp2;temp2=temp2->next;
	}
	if(last==NULL) {temp->next=start;start->last=temp;}
	else if(temp2==NULL) {last->next=temp;temp->last=last;}
	else {temp->last=last;last->next=temp;temp->next=temp2;temp2->last=temp;}
	return temp;
}
KpDefine2 *CellCalculation::DeleteNode(KpDefine2 *start,KpDefine2 *node)
{
	if(node==NULL||start==NULL) return NULL;
	KpDefine2 *temp=node->next;
	if(start==node) {start=temp;start->last=NULL;delete node;}
	else {temp->last=node->last;node->last->next=temp;delete node;}
	return start;
}
KpDefine2 *CellCalculation::DeleteNodes(KpDefine2 *start,KpDefine2 *node1,KpDefine2 *node2)
{
	if(start==NULL) return NULL;
	if(node1==NULL||node2==NULL) return start;

	
	return start;
}
void CellCalculation::DeleteNodes(KpDefine2 *start)
{
	KpDefine2 *temp=start;
	while(temp)
	{
		start=temp->next;
		delete temp;
		temp=start;
	}
}
//计算直线AB穿过两个相加体素的k值及kp值
//K1是AB直线穿过体素1的交点，K2是AB直线穿过体素2的交点
//加法计算，返回值：加法以后的交点
KDefine CellCalculation::K_Add(KDefine K1,KDefine K2)

{
	K1.OutputInf();//**调试用
	K2.OutputInf();//**调试用

	if(K1.k2-K1.k1<Mindk)//如果K1没有交点
		return K2;	

	if(K2.k2-K2.k1<Mindk)//如果K2没有交点
		return K1;	

	KDefine KData;
	KpDefine Temp;
	if(K1.k1<=K2.k1) KData.k1=K1.k1;
	else  KData.k1=K2.k1;
	if(K1.k2<=K2.k2) KData.k2=K2.k2;
	else  KData.k2=K1.k2;

	if(K1.k1>=K2.k2)//K1整体比K2大
	{
		for (int i=0;i<(int)K2.kp.size();i++)//记录K2
		{
			Temp.k1=K2.kp[i].k1;Temp.k2=K2.kp[i].k2;
			KData.kp.push_back(Temp);		  
		}
		if(K1.k1-K2.k2>Mindk)//如果两个没有交集
		{
			Temp.k1=K2.k2;Temp.k2=K1.k1;
			KData.kp.push_back(Temp);		  
		}
		for (int i=0;i<(int)K1.kp.size();i++)//记录K1
		{
			Temp.k1=K1.kp[i].k1;Temp.k2=K1.kp[i].k2;
			KData.kp.push_back(Temp);		  
		}	
	}
	else if(K1.k2<=K2.k1)//K2整体比K1大
	{
		for (int i=0;i<(int)K1.kp.size();i++)
		{
			Temp.k1=K1.kp[i].k1;Temp.k2=K1.kp[i].k2;
			KData.kp.push_back(Temp);		  
		}
		if(K2.k1-K1.k2>Mindk)//如果两个没有交集
		{
			Temp.k1=K1.k2;Temp.k2=K2.k1;
			KData.kp.push_back(Temp);		  
		}
		for (int i=0;i<(int)K2.kp.size();i++)
		{
			Temp.k1=K2.kp[i].k1;Temp.k2=K2.kp[i].k2;
			KData.kp.push_back(Temp);		  
		}	
	}
	else//K1,K2重叠
	{
		KpDefine2 *kp0=NULL,*kpt,*kplast;
		//K1
		kp0=CreatNode(K1.k1,12000);kplast=kp0;	
		for(int i=0;i<(int)K1.kp.size();i++)
		{
			kplast=CreatNode(kplast,K1.kp[i].k1,11000+i+1);
			kplast=CreatNode(kplast,K1.kp[i].k2,12000+i+1);		
		}
		kplast=CreatNode(kplast,K1.k2,11000);
		//K2
		kplast=InsertNode(kp0,K2.k1,22000);if(kplast->last==NULL)kp0=kplast;
		for(int i=0;i<(int)K2.kp.size();i++)
		{
			kplast=InsertNode(kplast,K2.kp[i].k1,21000+i+1);
			kplast=InsertNode(kplast,K2.kp[i].k2,22000+i+1);		
		}
		kplast=InsertNode(kplast,K2.k2,21000);

		bool key1,key2,key;		
		int A1,B1,A,B;
		double k1,k;
		key=false;
		key1=false;
		key2=false;
		kpt=kp0;
		while(kpt)
		{
			A=kpt->pos/10000;B=(kpt->pos/1000)%10;k=kpt->k;
			if(A==1&&B==2) key1=true;//K1开始实体
			else if(A==1&&B==1) key1=false;//K1实体结束
			if(A==2&&B==2) key2=true;//K2开始实体
			else if(A==2&&B==1) key2=false;//K2实体结束

			if(B==2&&key)//B=2，空隙右端，如果空隙左端存在，则记录
			{
				Temp.k1=k1;Temp.k2=k;
				if(Temp.k2-Temp.k1>Mindk)
				KData.kp.push_back(Temp);
				key=false;
			}

			if(key1||key2)//某一个实体在延续，不需要考虑计算
			{kpt=kpt->next;continue;}

			if(B==1)//表示实体的右边界
			{
				A1=A;B1=B;k1=k;	key=true;
			}
			kpt=kpt->next;
		}

		kpt=kp0;
		while(kpt)
		{
			printf("%d %f\n",kpt->pos,kpt->k);
			kpt=kpt->next;
		}
	 DeleteNodes(kp0);
	}
    //计算dk
	KData.dk=KData.k2-KData.k1;
	if(!KData.kp.empty())
	{
	   for (auto it=KData.kp.begin();it!=KData.kp.end();it++)
		 KData.dk-=(it->k2-it->k1);
	}
	return KData;
}

//计算直线AB穿过两个体素并集的k值及kp值
//K1是AB直线穿过体素1的交点，K2是AB直线穿过体素2的交点
//并集计算，返回值：并集的交点
KDefine CellCalculation::K_Inter(KDefine K1,KDefine K2)

{
	K1.OutputInf();//**调试用
	K2.OutputInf();//**调试用

	KDefine KData(0,0);

	if((K1.k2-K1.k1)<Mindk||(K2.k2-K2.k1)<Mindk)//如果K1没有交点
		return  KData;	
	if(K1.k1>=K2.k2||K1.k2<=K2.k1) return  KData;//整体不重合

	//k值创建链表
		KpDefine2 *kp0=NULL,*kpt,*kplast,*kpnext;
		//K1
		kp0=CreatNode(K1.k1,12000);kplast=kp0;	
		for(int i=0;i<(int)K1.kp.size();i++)
		{
			kplast=CreatNode(kplast,K1.kp[i].k1,11000+i+1);
			kplast=CreatNode(kplast,K1.kp[i].k2,12000+i+1);		
		}
		kplast=CreatNode(kplast,K1.k2,11000);
		//K2
		kplast=InsertNode(kp0,K2.k1,22000);if(kplast->last==NULL)kp0=kplast;	
		for(int i=0;i<(int)K2.kp.size();i++)
		{
			kplast=InsertNode(kplast,K2.kp[i].k1,21000+i+1);
			kplast=InsertNode(kplast,K2.kp[i].k2,22000+i+1);		
		}
		kplast=InsertNode(kplast,K2.k2,21000);
    //链表结束

       //并集计算	
		//原理：ABCDE
		//记录实体，记录k1，k2，如果找到第二个相交实体，记录kp，更新k2
		KpDefine Temp;
		int Alast,Blast;
		int A,B;
		double k,klast;
		int NNodes=0;
		Alast=kp0->pos/10000;Blast=(kp0->pos/1000)%10;klast=kp0->k;
		int sumB=1;//首个是b值是2
		kpt=kp0->next;
		while(kpt)
		{
			A=kpt->pos/10000;B=(kpt->pos/1000)%10;k=kpt->k;
			if(B==2) sumB++;
			else sumB--;
			if((k-klast>Mindk)&&((Blast==2&&B==1)&&(Alast!=A//两个体素实体相交的情况
				||sumB==1)))//两个体素实体相容的情况
			{
				if(NNodes==0)
				{
					KData.k1=klast;
					KData.k2=k;				
				}
				else
				{
					Temp.k1=KData.k2;Temp.k2=klast;
					if(Temp.k2-Temp.k1>Mindk)
					{
						 KData.kp.push_back(Temp);	
						 KData.k2=k;
					}
				}
				NNodes++;
			}
			Blast=B;
			Alast=A;
			klast=k;
			kpt=kpt->next;
		}

		kpt=kp0;
		while(kpt)
		{
			printf("%d %f\n",kpt->pos,kpt->k);
			kpt=kpt->next;
		}
	    DeleteNodes(kp0);
	
    //计算dk
	KData.dk=KData.k2-KData.k1;
	if(!KData.kp.empty())
	{
	   for (auto it=KData.kp.begin();it!=KData.kp.end();it++)
		 KData.dk-=(it->k2-it->k1);
	}
	return KData;
}

//计算直线AB穿过两个相减体素的k值及kp值
//K1是AB直线穿过体素1的交点，K2是AB直线穿过体素2的交点
//减法计算，体素1减去体素2，返回值：体素1减去体素2后剩下部分的交点
KDefine CellCalculation::K_1Minus2(KDefine K1,KDefine K2)
{
	K1.OutputInf();//**调试用
	K2.OutputInf();//**调试用

	KDefine KData(0,0);
	if(K1.k2-K1.k1<Mindk) return KData;	//如果K1没有交点
	if(K2.k2-K2.k1<Mindk) return K1;	//如果K1没有交点
	if(K1.k1>=K2.k2||K1.k2<=K2.k1) return K1;//整体不重合		

	KpDefine Temp;

	//k值创建链表
	KpDefine2 *kp0=NULL,*kpt,*kplast;
	//K1
		kp0=CreatNode(K1.k1,12000);kplast=kp0;	
		for(int i=0;i<(int)K1.kp.size();i++)
		{
			kplast=CreatNode(kplast,K1.kp[i].k1,11000+i+1);
			kplast=CreatNode(kplast,K1.kp[i].k2,12000+i+1);		
		}
		kplast=CreatNode(kplast,K1.k2,11000);
		//K2
		kplast=InsertNode(kp0,K2.k1,22000);if(kplast->last==NULL)kp0=kplast;
		for(int i=0;i<(int)K2.kp.size();i++)
		{
			kplast=InsertNode(kplast,K2.kp[i].k1,21000+i+1);
			kplast=InsertNode(kplast,K2.kp[i].k2,22000+i+1);		
		}
		kplast=InsertNode(kplast,K2.k2,21000);
    //链表结束

		bool key1,key2;		
		int A1,B1,A,B;
		double k2,k1,k;
		key1=false;
		key2=false;
		kpt=kp0;
		int NNnodes=0;
		while(kpt)
		{
			A=kpt->pos/10000;B=(kpt->pos/1000)%10;k=kpt->k;
			if(A==1&&B==2) {k2=k;key1=true;}//K1开始实体
			else if(A==1&&B==1) {k1=k;key1=false;}//K1实体结束
			if(A==2&&B==2) {k1=k;key2=true;}//K2开始实体
			else if(A==2&&B==1) {k2=k;key2=false;}//K2实体结束

			if((key1&&key2)||//如果K1是实体，如果K2也是实体
				(!key1&&!key2))//如果K1不是实体，如果K2也不是实体
			{
				//情况1：实体都存在，如果K2是后实体，K1实体结束,如果这两个值不一样
				//情况2：都不存在，K1在结束实体时，如果跟起始值不一样（起始值可以是K1开始，也可以是K2结束产生的）
					if(k1-k2>Mindk)//如果K2是后实体，K1实体结束,如果这两个值不一样
					{
						if(NNnodes==0)//如果没有实体
						{
							KData.k1=k2;
							KData.k2=k1;
							NNnodes++;
						}
						else//如果有实体
						{
							Temp.k1=KData.k2;
							Temp.k2=k2;
							if(Temp.k2-Temp.k1>Mindk)
							{KData.kp.push_back(Temp);NNnodes++;KData.k2=k1;}								
						}					
					}
			}			
			kpt=kpt->next;
		}

		kpt=kp0;
		while(kpt)
		{
			printf("%d %f\n",kpt->pos,kpt->k);
			kpt=kpt->next;
		}
	 DeleteNodes(kp0);

    //计算dk
	KData.dk=KData.k2-KData.k1;
	if(!KData.kp.empty())
	{
	   for (auto it=KData.kp.begin();it!=KData.kp.end();it++)
		 KData.dk-=(it->k2-it->k1);
	}
	return KData;
}