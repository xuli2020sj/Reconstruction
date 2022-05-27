#pragma once

#define LineChar_MaxLength 3000//�ַ�����ʱ��������;
#define PI 3.14159265358979
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

class TriDNodeDefine
{
public:
	int NoX;
	int NoY;
	int NoZ;
	double x;
	double y;
	double z;
	double volume;//�������
	vector <double> data;
	TriDNodeDefine *NextX;
	TriDNodeDefine *LastX;
	TriDNodeDefine *NextY;
	TriDNodeDefine *LastY;
	TriDNodeDefine *NextZ;
	TriDNodeDefine *LastZ;
	TriDNodeDefine()
	{
		NoX=-1;NoY=-1;NoZ=-1;
		x=-1;y=-1;z=-1;
		volume=0;
		NextX=nullptr;LastX=NULL;
		NextY=NULL;LastY=NULL;
		NextZ=NULL;LastZ=NULL;
	}
};

class TriDNodeHeadDefine
{
protected:

public:
	int DimensionOfNodes;//ά����һάֵΪ1��2άΪ2��3άΪ3;
	int NuNodesInX;//X����ڵ����;
	int NuNodesInY;//Y����ڵ����;
	int NuNodesInZ;//Z����ڵ����;
	int NuNodesTotal;//�ڵ�����;
	int NuData;
	std::string Title;//���ݿռ������;
	std::string Inf;//�����ݿռ��ע��;
	vector <std::string> VARIABLES;//double�����ڸ���������������;
	std::string TitleXYZ[3];
	vector <double> VARIABLESData;//ÿ���ڵ������˵��;
	vector <double> MaxData;
	vector <double> MinData;
	TriDNodeDefine *FirstNode;
	TriDNodeDefine *EndNode;
	int NuDataUsed;//data����������ռ��ʹ�ø�����ӦС��MaxNuDataInTriDNode;
	int NuNodesUsed;//ʹ�ýڵ������;
	int NuDecimal;//С����λ��;
	bool IsScientific;//�Ƿ��ѧ������;
	TriDNodeHeadDefine()
	{
		DimensionOfNodes=0;//ά�����������ݿռ�ʱ���ݸ�����ڵ����Ƿ����1�Զ�����;
		NuNodesInX=0;//x����ڵ�������ʼΪ��;
		NuNodesInY=0;//y����ڵ�������ʼΪ��;
		NuNodesInZ=0;//z����ڵ�������ʼΪ��;
		NuNodesTotal=0;//�ܽڵ�������ʼΪ��;
		Title="DataTemp.dat";//���ݿռ�����ƣ����޸�;
		FirstNode=nullptr;//�׽ڵ�;
		EndNode=nullptr;//ĩ�ڵ�;
		TitleXYZ[0]="x";//x��������;
		TitleXYZ[1]="y";//y��������;
		TitleXYZ[2]="z";//z��������;
		NuDataUsed=0;//���ݿռ�ʹ�����ݵĸ���;
		Inf="No Remark";//�ռ�ı�ע˵��;
		IsScientific=true;//�������ʱ���Ƿ��ѧ�����������޸�ʱ��������;
		NuDecimal=4;//�������ʱ��С����λ�������޸�Ϊ8λ;
	}
};

class TriData
{
public:
	int KeyDirection[3];
	int NuIJK;//��ȡdata�ļ�ʱ,IJK���ڵĸ�����������Ϊ3;
	int IsExistI;//��ȡdata�ļ�ʱ���Ƿ����I=,������Ϊ-1������ΪIJK���д����ţ��㿪ʼ;
	int IsExistJ;//��ȡdata�ļ�ʱ���Ƿ����I=,������Ϊ-1������ΪIJK���д����ţ��㿪ʼ;
	int IsExistK;//��ȡdata�ļ�ʱ���Ƿ����I=,������Ϊ-1������ΪIJK���д����ţ��㿪ʼ;
	TriDNodeHeadDefine *TriDataUsing;
	TriDNodeHeadDefine *TriDataUsingTemp;
	double m_RelaxFactor;
	//��ά���ݿռ�ʹ�ó�ʼ��;
	void TriDataInitialization(void);
	//�����ڵ�,x,y,z���������ϵ���������ʼ���ݶ�����;
	TriDNodeHeadDefine * CreatNodes(int NuX, int NuY, int NuZ);
	//����ͷ�ڵ�;
	TriDNodeHeadDefine * CreatNodeHead(int NuData);
	TriDNodeHeadDefine * CreatNodeHead();
	//����head�ڵ�;
	void CopyHead(TriDNodeHeadDefine *nodehead1,TriDNodeHeadDefine *nodehead2);
	//��head1�е���Ϣ���Ƹ�head2�����ǽڵ�������;
	void CopyHeadInf(TriDNodeHeadDefine *nodehead1,TriDNodeHeadDefine *nodehead2);
	//��head1���Ƹ�head2;
	TriDNodeHeadDefine * CopyNodes(TriDNodeHeadDefine *nodehead1,TriDNodeHeadDefine *nodehead2);
	//���ɲ���ʼ���ڵ�,�ڵ�š��������㣬����ָ����NULL;
	TriDNodeDefine * CreatNode();
	TriDNodeDefine * CreatNode(int NuData);
	TriDNodeDefine * CreatNode(int NuData,double data0);
	//�����ڵ���ͷ���,x,y,z���������ϵ���������ʼ���ݶ�����;
	TriDNodeDefine * CreatNodesWithoutHead(int NuX, int NuY, int NuZ);
	//�����ڵ�,x,y,z���������ϵ��������������ݽ��г�ʼ��;
    TriDNodeHeadDefine *CreatNodes(int NuX, int NuY, int NuZ,int NDataUsed,double data);
    TriDNodeHeadDefine *CreatNodes(int NuX, int NuY, int NuZ,TriDNodeHeadDefine *Sample);
	//�ƶ��ڵ㣬�ƶ�һ��,��ǰ�ڵ㣬�ƶ����򣬷����ƶ���Ľڵ�ָ�룬����1��2��3�ֱ�Ϊx��y��z���򣬸��ű�ʾ������;
	TriDNodeDefine * MoveNode(TriDNodeDefine *node,int direction);
	//�����ڵ�֮������ӣ�node1ָ��node2,direction=1 x����2 y����3 z����;
	void ConnectTwoNodes(TriDNodeDefine *node1,TriDNodeDefine *node2,int direction);
	//���нڵ�֮������ӣ�node1���ڵ���ָ��node2���ڵ���,direction=1 x����2 y����3 z����;
	void ConnectTwoLineNodes(TriDNodeDefine *node1,TriDNodeDefine *node2,int directionMove,int directionLink);
	//����ڵ�֮������ӣ�node1������ָ��node2������,direction=1 x����2 y����3 z����;
	void ConnectTwoFaceNodes(TriDNodeDefine *node1,TriDNodeDefine *node2,int directionLink);
	//���ͷָ��ɾ���ڵ�;
	void DeleteNodes(TriDNodeHeadDefine *nodehead);
	void DeleteHead(TriDNodeHeadDefine *nodehead);
	//���ڵ������ȫ������;
	void SetNodesData(TriDNodeHeadDefine *nodehead,double data);
	//���ڵ��ĳ����������;
	void SetNodesData(TriDNodeHeadDefine *nodehead,int no,double data);
	//���ڵ��ĳ�����ݳ�����;
	void MultiplyNodesData(TriDNodeHeadDefine *nodehead,int no,double data);
	//����ڵ����ݵ��ļ�����ά��ʽ��������IJK����ʹΪ1��,��ʽ�ɱ�tecplot���룬����Ϊ��ͷָ�룬ȫ��������·���������ݴ洢�����ȷ����硰XYZ������ZYX���ȣ�;
	void outputdata_3d(TriDNodeHeadDefine *nodehead,std::string FullName,std::string Direction);
	//��� 1 �뾶��2ˮƽ�Ƕȣ�3��ֱ�Ƕȵ�����������ѿ���������ʽ
   void  outputdata_3d_ZB(TriDNodeHeadDefine *nodehead,std::string FullName,std::string Direction);
   //��� 1 �Ƕȣ�2�뾶��3�߶ȵ�����������ѿ���������ʽ
   void  outputdata_3d_ZB2(TriDNodeHeadDefine *nodehead,std::string FullName,std::string Direction);
	//���ļ�FullName��ȡ���ݣ����ļ���ʽͬtecplot��ȡ��ʽ;
	TriDNodeHeadDefine * ReadData(std::string FullName);
	//�������ļ������շ����Լ�IJK��������ȡ
	TriDNodeHeadDefine * ReadData(std::string FullName,std::string direction);
	//���ַ�����linetext�ֽ�double����������datatemp������ʶ�����ݵĸ���;
	//��VARIABLES�洢���ַ�������ֵ����;
	void ATOF_VARIABLES(TriDNodeHeadDefine *nodehead);
	int IdentifyData(char linetext[LineChar_MaxLength],double datatemp[]);
	int IdentifyData2(char linetext[LineChar_MaxLength],double datatemp[]);
	//�����ļ���һ�д�������ȡ�ļ���;
	std::string Process1stLine(char linetext[LineChar_MaxLength]);
	//�����ļ��ڶ��д�����ȡ������;
	int Process2ndLine(char linetext[LineChar_MaxLength],vector <std::string> &VARIABLESTemp,vector <std::string> &VARIABLESDataTemp);
	//�����ļ������д�����ȡIJK��Ϣ;
	bool Process3rdLine(char linetext[LineChar_MaxLength]);
	//��ȡ����ʱ�������¶�ȡ�ڵ�����һ�ڵ��б�洢��xyz����;
	int ProcessDirection(TriDNodeDefine *nodenew,TriDNodeDefine *nodeold);
	int ProcessDirection(double x,double y,double z,TriDNodeDefine *nodeold);
	//��node1��data�����ݼ����꿽����node2;
	bool CopyDataNode1toNode2(TriDNodeDefine *node1,TriDNodeDefine *node2);

	//������ֵ,��������x����������y��������������ֵx���飬��ֵ�洢�Ŀռ�y����ֵ����;
	bool  SplineInterpolation(double xAxis[], double yAxis[], int NYT, double x[],double y[],int Nyt);
	//������ֵ,��������x����������y��������������ֵx���飬��ֵ�洢�Ŀռ�y����ֵ�������Ƿ�yֵָ���任;
	bool  SplineInterpolation(double xAxis[], double yAxis[], int NYT, double x[],double y[],int Nyt,bool IsExponent);
	void Interpolation(TriDNodeDefine *node0,int direction,int Start,int End,int noData,double xdata[],double ydata[],int Nu);
	double Interpolation(TriDNodeDefine *node0,int direction,int Start,int End,int noData,double xdata);
	//�������ղ�ֵ,��������x����������y��������������ֵx���飬��ֵ�洢�Ŀռ�y����ֵ����;
	bool  LagrangeInterpolation(double xAxis[], double yAxis[], int NYT, double x[],double y[],int Nyt);
	//��һ���������У��ҵ�x���ڵ�λ�ã������i��i+1֮�䣬����i�����Ϲ涨����-2������߷���-1,���ұ߷���NYT-1;
	int LocationInLineArray(double xAxis[],int NYT,double x);
	//���Բ�ֵ,��������x����������y��������������ֵx���飬��ֵ�洢�Ŀռ�y����ֵ����;
	bool LineInterpolation(double xAxis[], double yAxis[], int NYT, double x[],double y[],int Nyt);

	//�����Ȼ��
bool MLEM_Iteration(TriDNodeHeadDefine *headA,std::string DirectionA,TriDNodeHeadDefine *headB,std::string DirectionB,TriDNodeHeadDefine *headX,std::string DirectionX,double  Resid[]);
//����A��B���ڻ�;
double ABVectorsProduct(TriDNodeDefine *nodeA,int directionA,TriDNodeDefine *nodeB,int directionB,int NoData);
//����A��ƽ����ƽ��;
double VectorsMeanSquare(TriDNodeDefine *nodeA,int directionA,int NoData);
//�󷽳̲в�,RMS(B-AX),ϵ������A��ͷ�ڵ㣬�����ұ�Bͷ�ڵ㣬δ֪��Xͷ�ڵ㣬����в�ĵ�NoData������;
bool ResidualOfABX(TriDNodeHeadDefine *headA,int directionAI,int directionAJ,TriDNodeHeadDefine *headB,int directionB,TriDNodeHeadDefine *headX,int directionX,double Resid[]);
//X����ֵ,X=AT*B,ϵ������A����������ƽ�棨�硰XY��ָXΪ����YΪ���򣩣��������B������δ֪�����ݿռ�X������;
bool InitializationX(TriDNodeHeadDefine *headA,std::string DirectionA,TriDNodeHeadDefine *headB,std::string DirectionB,TriDNodeHeadDefine *headX,std::string DirectionX);

};


