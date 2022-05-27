#include "CellCalculation.h"
#include "MA_Coef.h"
#include <math.h>
#include <string.h>
#include <fstream>

using namespace std;
#define Mindk 0.00000001

//˵����
/*
�κ�һ��ֱ�߿��Ա�ʾΪ�������̣�
(x-xA)/(xB-xA)=(y-yA)/(yB-yA)=(z-zA)/(zB-zA)=k
չ��Ϊ�������̣�
x=k(xB-xA)+xA
y=k(yB-yA)+yA
z=k(zB-zA)+zA
���������е�kΪ��������������Ϊ������㵽A�ĳ�����AB���ȵı�ֵ

KDefine������ã���¼ֱ�����κ��彻���Kֵ
                 ����Ǿ���ʵ���壬����β�����K
				 ������ڿ��Ĳ��֣�ֱ������Ľ��㣬����ڸ���kp��

GPoint������ã���
GPlane������ã���
GCell������ã���
CellCalculation������ã�ֱ�������صļ���
MA_Coef.h�����Ͳ��ϵ�˥��ϵ�����ܶȵľ��鹫ʽ��ֵ
*/
////////////////////////////////////KDefine////////////////////////
//����kpֵ�ķ�����һ����k1,k2����ֵ�󣬷���kp�Ƿ�ı�
void KDefine::AnalyzeKP(void)
{
	if(kp.empty()) return;//
	int n,N=kp.size();
	vector<KpDefine>::iterator it,it2;
	it=kp.begin();
	for(n=0;n<N&&it!=kp.end();n++)
	{
		if(kp.empty()) break;
		if(k1>=it->k1&&k1<=it->k2&&k2>=it->k1&&k2<=it->k2) //��Բ��
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
//����K�����
//��ν����K����˼�ǣ�����β���������м䣬������ڿ��Ĳ��֣�ֱ������ĵĽ����������¼
void KDefine::pushback_kp2(double kt1,double kt2)
{
	//��֤��С˳��
	if(kt1>kt2) 
	{
		double t=kt1;
		kt1=kt2;kt2=t;	
	}
	if((kt2-kt1)<Mindk) return;//̫С�˾Ͳ���
	//����¼���Բ��AB֮�⣬�Ͳ��ı�
	if(kt2<=k1) return;
	if(kt1>=k2) return;
	if(kt1<=k1&&kt2>=k2) //AB����Բ�ڲ�
	{
		kp.clear();k1=0;k2=0;return;
	}
	//�����Բk1С��K1��
	if(kt1<=k1)
	{
		k1=kt2;
		AnalyzeKP();
		return;
	}
	//�����Բk2����K2��
	if(kt2>=k2)
	{
		k2=kt1;
		AnalyzeKP();
		return;
	}
	//����������¼�Բ��AB֮��
	KpDefine temp;
	temp.k1=kt1;
	temp.k2=kt2;
	//���ԭ��û�и��ӵ�k��ֱ�����
	if(kp.empty()) {kp.push_back(temp);return;}

	vector<KpDefine>::iterator it1=kp.begin(),it2=kp.end()-1;
	//��Բ����������Բ��ɾ������Բ�������Բ
	if(kt1<=it1->k1&&kt2>=it2->k2)
	{
		kp.clear();
		kp.push_back(temp);	
		return;
	}
	//�����Բ��A���һ��Բ֮��
	if(kt2<it1->k1)
	{
		kp.insert(it1,temp);return;
	}
	if(kt2<=it1->k2)//�����Բ���һ��Բ�غ�
	{
		it1->k1=kt1;return;
	}
	//�����Բ��B�����һ��Բ֮��
	if(kt1>it2->k2)
	{
		kp.push_back(temp);return;
	}
	if(kt1>=it2->k1)//�����Բ�����һ��Բ�غ�
	{
		it2->k2=kt2;return;
	}

	//�����������Բ�ڵ�һ��Բ�����һ��Բ֮��
	vector<KpDefine>::iterator it;
	//��λk1���ҵ�k1�����Բ
	for(it=kp.begin();it!=kp.end();it++)
	{
		if(kt1<=it->k1) {it1=it;break;}
		else if(kt1<=it->k2) 
		{
			kt1=it->k1;it1=it;break;
		}
	}
	//��λk2
	for(it=it1;it!=kp.end();it++)
		if(kt2<it->k1)
		{
			it2=it;	
			if((it-1)->k2>=kt2) kt2=(it-1)->k2;			
			break;		
		}
		if(it==kp.end()) it2=it;
		if((it-1)->k2>=kt2) kt2=(it-1)->k2;	
	//���Բ������Բ֮�䣬ֱ�Ӳ���
	//��ʱ������������Բ�ཻ�����,Ҳ��������ĩβ
	if(it1==it2)
	{
		temp.k1=kt1;
		temp.k2=kt2;
		kp.insert(it1,temp);
		return;	
	}
	//���ֻ���һ��Բ���������Բ
	if((it1+1)==it2)
	{
		it1->k1=kt1;
		it1->k2=kt2;
		return;
	}
	//������һ��Բ����
	it1->k1=kt1;//���ĵ�һ��Բ
	it1->k2=kt2;
	kp.erase(it1+1,it2);//����ɾ��
}

////////////////////////////////////GCell��////////////////////////
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

////////////////////////////////////CellCalculation��////////////////////////
CellCalculation::CellCalculation(void)
{
}

CellCalculation::~CellCalculation(void)
{
}
//����˥��ϵ����//1 ��� 2water 3Polyethylene 4Concrete 5Glass 6Al 7Ge 8Fe 9Cu 10Pb ����ˮ
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
//��������֮��ľ���
double CellCalculation::L_PointsAB(GPoint PA, GPoint PB)
{
	return sqrt(pow(PA.x-PB.x,2)+pow(PA.y-PB.y,2)+pow(PA.z-PB.z,2));
}

//����ƽ�淨��������˵��
//ƽ�淽�� Ax+By+Cz=D,��������Ϊ��A,B,C��,�ڶ���ƽ��ʱ��Ҫע�� -Ax-By-Cz=-D�ķ���������-A,-B,-C��Ϊǰ�淽�̱���ķ�����
//�ж�ֱ��AB������ƽ�淨��������A,B,C����ͬ�������淽��,������������PB-PA�����ڻ�����������㣬�н�С��90�ȣ���ʾ����ͬ���򣬷����淽��	

//����ֱ��AB�����棨��ƽ�棩�ཻ�󣬴��������ڵ��������Kֵ�������¼���AB�����µ���ʼ���յ㣩
//PA��A�����꣬PB��B�����꣬P��ƽ�棬PM�������������ʶ���������뷨�����ͬ��Ϊ+�������Ϊ����ע����ͬƽ��������һ��һ���ķ���
KDefine CellCalculation::k_InterPoint(GPoint PA, GPoint PB, GPlane P,char PM)
{
	KDefine KData;
	if(P.type=='P'||P.type=='p')//ƽ��
	{//
		double Left=P.A*(PB.x-PA.x)+P.B*(PB.y-PA.y)+P.C*(PB.z-PA.z);//�ڻ�
		double Right=P.R-P.A*PA.x-P.B*PA.y-P.C*PA.z;
		double PAD=PA.x*P.A+PA.y*P.B+PA.z*P.C-P.R;
		double PBD=PB.x*P.A+PB.y*P.B+PB.z*P.C-P.R;
		//ֱ��ABȫ������
		if(PAD<=0&&PBD<=0&&PM=='+') //���AB��ƽ�渺���߲࣬�������������߲�
				{KData.k1=1;KData.k2=1;}
		else if(PAD>=0&&PBD>=0&&PM=='-') //���AB��ƽ���������߲࣬�������Ǹ����߲�
				{KData.k1=0;KData.k2=0;}	
		//����ֱ��ABȫ���򲿷�������
		else if(fabs(Left)>1e-8)
		{///
			double kk=Right/Left;		
			//ȫ�������ڣ�kֵ����
			//�����ǲ��������ڣ�kֵ�������ֵ
			if(kk>0&&kk<1)
			{////
				if(Left>0) //���A->B��ƽ��P����ͬ���򣨼н�С��90�㣩
				{/////
					//PMΪ+����ʾ����ƽ����������࣬���AB�뷨��ͬ���򣬽����滻A��ֵ����k1
					//PMΪ-����ʾ����ƽ�渺������࣬���AB�뷨��ͬ���򣬽����滻B��ֵ����k2
					if(PM=='+') // A |-> B
						KData.k1=kk;
					else// A <-| B 
						KData.k2=kk;		
				}/////
				else //���A->B��ƽ��P�����淽�򣨼нǴ���90�㣩
				{/////
					//PMΪ+����ʾ����ƽ����������࣬���AB�뷨���淽�򣬽����滻B��ֵ����k2
					//PMΪ-����ʾ����ƽ�渺������࣬���AB�뷨���淽�򣬽����滻A��ֵ����k1
					if(PM=='+') // B |-> A
						KData.k2=kk;	
					else 	
						KData.k1=kk;	
				}/////	
			}////	
		}///
	}//
	else if(P.type=='S'||P.type=='s'||P.type=='C'||P.type=='c')//��Բ��
	{//
		double C1=P.A,C2=P.B,C3=P.C;
		//Բ��������Ϊ����������Ϊ��������PMΪ+��Բ�⣬PMΪ-��Բ��
		//����һԪ2�η��� ax2+bx+c=0
		double a=C1*pow(PB.x-PA.x,2.0)+C2*pow(PB.y-PA.y,2.0)+C3*pow(PB.z-PA.z,2.0);
		double b=2*(C1*(PB.x-PA.x)*(PA.x-P.P0.x)+C2*(PB.y-PA.y)*(PA.y-P.P0.y)+C3*(PB.z-PA.z)*(PA.z-P.P0.z));
		double c=C1*pow(PA.x-P.P0.x,2.0)+C2*pow(PA.y-P.P0.y,2.0)+C3*pow(PA.z-P.P0.z,2.0)-P.R*P.R;
		double d=b*b-4*a*c;
		if(PM=='-')//������Բ��
		{
			if(d<0)//ֱ����Բ�޽���
				{KData.k1=0;KData.k2=0;}
			else//�н���
			{
				double k1=(-1*b-sqrt(d))/(2*a);
				double k2=(-1*b+sqrt(d))/(2*a);
				KData.k1=k1>0?k1:0;
				KData.k2=k2<1?k2:1;
			}		
		}
		else//������Բ��
		{
			if(d<0)//ֱ����Բ�޽���
				{ }
			else//�н���
			{
				double k1=(-1*b-sqrt(d))/(2*a);
				double k2=(-1*b+sqrt(d))/(2*a);

				if(k1>0&&k2<1)//��ʾԲ��AB֮��,�Ƚ����⣬������ֱ��
				{
					KData.k1=0;KData.k2=1;
					KData.pushback_kp(k1,k2);
				}
				else if(k1<=0&&k2<1)//��ʾA��Բ��
				{
					KData.k1=k2;KData.k2=1;
				}
				else if(k1>0&&k2>=1)//��ʾB��Բ��
				{
					KData.k1=0;KData.k2=k1;
				}
				else//k1<=0&&k2>=1��ʾAB��Բ��
				{
					KData.k1=0;KData.k2=0;			
				}
			}			
		}
	}//
	return KData;
}

//����ֱ��AB��������Cell������kֵ
//PA��A�����꣬PB��B�����꣬Cell�Ƕ��������
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
		//ͬʱ����1��С��0���ֵ̫С,ֱ�ӽ���
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
		if(!KData.kp.empty())//���ԭ���и��ӵ�k����ʾ�п���Բ
			KData.AnalyzeKP();
		if(!PK.kp.empty())//����¼����и���k�����
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
//�������ؽ��㣨�������ڲ����ģ��ĵ�1����������
GPoint CellCalculation::GetP1(GPoint PA, GPoint PB,GCell Cell)
{
	KDefine temp=K_Cell(PA,PB,Cell);//�ҵ�����	
	GPoint Temp;
	if(temp.IsExist) 
		Temp.SetPoint(PA.x+temp.k1*(PB.x-PA.x),PA.y+temp.k1*(PB.y-PA.y),PA.z+temp.k1*(PB.z-PA.z));
	return Temp;
}
//�������ؽ��㣨�������ڲ����ģ��ĵ�2����������
GPoint CellCalculation::GetP2(GPoint PA, GPoint PB,GCell Cell)
{
	KDefine temp=K_Cell(PA,PB,Cell);//�ҵ�����	
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
//���㴩͸����,PA:A�����꣬PB��B�����꣬Cell��
double CellCalculation::L_Cell(GPoint PA, GPoint PB,GCell Cell)
{
	KDefine temp=K_Cell(PA,PB,Cell);//�ҵ�����
	return L_PointsAB(PA,PB)*temp.dk;
}
//���㴩͸����,PA:A�����꣬PB��B�����꣬Cell��
double CellCalculation::L_Cell(GPoint PA, GPoint PB,GCell Cell,double L)
{
	KDefine temp=K_Cell(PA,PB,Cell);//�ҵ�����
	if(temp.IsExist) return L*temp.dk;
	else return 0;
}

//���㴩͸���룬�۳����ľ���
//KAΪ�����ڿ����ص�Kֵ���ϣ�
//PA:A�����꣬PB��B�����꣬KA��AB���������п��ĵ�kֵ
double CellCalculation::L_Cell(GPoint PA, GPoint PB,KDefine KA)
{
	double dk=KA.k2-KA.k1;//ͷβ����
	if(!KA.kp.empty())//������ڿ���
	{
	   for (auto it=KA.kp.begin();it!=KA.kp.end();it++)
		dk-=(it->k2-it->k1);//��ȥ���п���
	}
	return L_PointsAB(PA,PB)*dk;//����ʵ�ĳ���
}
//������A�п۳�B
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
//����ֱ��AB��������������ص�kֵ��kpֵ
//K1��ABֱ�ߴ�������1�Ľ��㣬K2��ABֱ�ߴ�������2�Ľ���
//�ӷ����㣬����ֵ���ӷ��Ժ�Ľ���
KDefine CellCalculation::K_Add(KDefine K1,KDefine K2)

{
	K1.OutputInf();//**������
	K2.OutputInf();//**������

	if(K1.k2-K1.k1<Mindk)//���K1û�н���
		return K2;	

	if(K2.k2-K2.k1<Mindk)//���K2û�н���
		return K1;	

	KDefine KData;
	KpDefine Temp;
	if(K1.k1<=K2.k1) KData.k1=K1.k1;
	else  KData.k1=K2.k1;
	if(K1.k2<=K2.k2) KData.k2=K2.k2;
	else  KData.k2=K1.k2;

	if(K1.k1>=K2.k2)//K1�����K2��
	{
		for (int i=0;i<(int)K2.kp.size();i++)//��¼K2
		{
			Temp.k1=K2.kp[i].k1;Temp.k2=K2.kp[i].k2;
			KData.kp.push_back(Temp);		  
		}
		if(K1.k1-K2.k2>Mindk)//�������û�н���
		{
			Temp.k1=K2.k2;Temp.k2=K1.k1;
			KData.kp.push_back(Temp);		  
		}
		for (int i=0;i<(int)K1.kp.size();i++)//��¼K1
		{
			Temp.k1=K1.kp[i].k1;Temp.k2=K1.kp[i].k2;
			KData.kp.push_back(Temp);		  
		}	
	}
	else if(K1.k2<=K2.k1)//K2�����K1��
	{
		for (int i=0;i<(int)K1.kp.size();i++)
		{
			Temp.k1=K1.kp[i].k1;Temp.k2=K1.kp[i].k2;
			KData.kp.push_back(Temp);		  
		}
		if(K2.k1-K1.k2>Mindk)//�������û�н���
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
	else//K1,K2�ص�
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
			if(A==1&&B==2) key1=true;//K1��ʼʵ��
			else if(A==1&&B==1) key1=false;//K1ʵ�����
			if(A==2&&B==2) key2=true;//K2��ʼʵ��
			else if(A==2&&B==1) key2=false;//K2ʵ�����

			if(B==2&&key)//B=2����϶�Ҷˣ������϶��˴��ڣ����¼
			{
				Temp.k1=k1;Temp.k2=k;
				if(Temp.k2-Temp.k1>Mindk)
				KData.kp.push_back(Temp);
				key=false;
			}

			if(key1||key2)//ĳһ��ʵ��������������Ҫ���Ǽ���
			{kpt=kpt->next;continue;}

			if(B==1)//��ʾʵ����ұ߽�
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
    //����dk
	KData.dk=KData.k2-KData.k1;
	if(!KData.kp.empty())
	{
	   for (auto it=KData.kp.begin();it!=KData.kp.end();it++)
		 KData.dk-=(it->k2-it->k1);
	}
	return KData;
}

//����ֱ��AB�����������ز�����kֵ��kpֵ
//K1��ABֱ�ߴ�������1�Ľ��㣬K2��ABֱ�ߴ�������2�Ľ���
//�������㣬����ֵ�������Ľ���
KDefine CellCalculation::K_Inter(KDefine K1,KDefine K2)

{
	K1.OutputInf();//**������
	K2.OutputInf();//**������

	KDefine KData(0,0);

	if((K1.k2-K1.k1)<Mindk||(K2.k2-K2.k1)<Mindk)//���K1û�н���
		return  KData;	
	if(K1.k1>=K2.k2||K1.k2<=K2.k1) return  KData;//���岻�غ�

	//kֵ��������
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
    //�������

       //��������	
		//ԭ��ABCDE
		//��¼ʵ�壬��¼k1��k2������ҵ��ڶ����ཻʵ�壬��¼kp������k2
		KpDefine Temp;
		int Alast,Blast;
		int A,B;
		double k,klast;
		int NNodes=0;
		Alast=kp0->pos/10000;Blast=(kp0->pos/1000)%10;klast=kp0->k;
		int sumB=1;//�׸���bֵ��2
		kpt=kp0->next;
		while(kpt)
		{
			A=kpt->pos/10000;B=(kpt->pos/1000)%10;k=kpt->k;
			if(B==2) sumB++;
			else sumB--;
			if((k-klast>Mindk)&&((Blast==2&&B==1)&&(Alast!=A//��������ʵ���ཻ�����
				||sumB==1)))//��������ʵ�����ݵ����
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
	
    //����dk
	KData.dk=KData.k2-KData.k1;
	if(!KData.kp.empty())
	{
	   for (auto it=KData.kp.begin();it!=KData.kp.end();it++)
		 KData.dk-=(it->k2-it->k1);
	}
	return KData;
}

//����ֱ��AB��������������ص�kֵ��kpֵ
//K1��ABֱ�ߴ�������1�Ľ��㣬K2��ABֱ�ߴ�������2�Ľ���
//�������㣬����1��ȥ����2������ֵ������1��ȥ����2��ʣ�²��ֵĽ���
KDefine CellCalculation::K_1Minus2(KDefine K1,KDefine K2)
{
	K1.OutputInf();//**������
	K2.OutputInf();//**������

	KDefine KData(0,0);
	if(K1.k2-K1.k1<Mindk) return KData;	//���K1û�н���
	if(K2.k2-K2.k1<Mindk) return K1;	//���K1û�н���
	if(K1.k1>=K2.k2||K1.k2<=K2.k1) return K1;//���岻�غ�		

	KpDefine Temp;

	//kֵ��������
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
    //�������

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
			if(A==1&&B==2) {k2=k;key1=true;}//K1��ʼʵ��
			else if(A==1&&B==1) {k1=k;key1=false;}//K1ʵ�����
			if(A==2&&B==2) {k1=k;key2=true;}//K2��ʼʵ��
			else if(A==2&&B==1) {k2=k;key2=false;}//K2ʵ�����

			if((key1&&key2)||//���K1��ʵ�壬���K2Ҳ��ʵ��
				(!key1&&!key2))//���K1����ʵ�壬���K2Ҳ����ʵ��
			{
				//���1��ʵ�嶼���ڣ����K2�Ǻ�ʵ�壬K1ʵ�����,���������ֵ��һ��
				//���2���������ڣ�K1�ڽ���ʵ��ʱ���������ʼֵ��һ������ʼֵ������K1��ʼ��Ҳ������K2���������ģ�
					if(k1-k2>Mindk)//���K2�Ǻ�ʵ�壬K1ʵ�����,���������ֵ��һ��
					{
						if(NNnodes==0)//���û��ʵ��
						{
							KData.k1=k2;
							KData.k2=k1;
							NNnodes++;
						}
						else//�����ʵ��
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

    //����dk
	KData.dk=KData.k2-KData.k1;
	if(!KData.kp.empty())
	{
	   for (auto it=KData.kp.begin();it!=KData.kp.end();it++)
		 KData.dk-=(it->k2-it->k1);
	}
	return KData;
}