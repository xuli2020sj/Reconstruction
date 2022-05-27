#pragma once

#define LineChar_MaxLength 3000//字符辨认时的最大个数;
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
	double volume;//网格体积
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
	int DimensionOfNodes;//维数，一维值为1，2维为2，3维为3;
	int NuNodesInX;//X方向节点个数;
	int NuNodesInY;//Y方向节点个数;
	int NuNodesInZ;//Z方向节点个数;
	int NuNodesTotal;//节点总数;
	int NuData;
	std::string Title;//数据空间的名称;
	std::string Inf;//该数据空间的注释;
	vector <std::string> VARIABLES;//double数组内各个物理量的名称;
	std::string TitleXYZ[3];
	vector <double> VARIABLESData;//每个节点的数据说明;
	vector <double> MaxData;
	vector <double> MinData;
	TriDNodeDefine *FirstNode;
	TriDNodeDefine *EndNode;
	int NuDataUsed;//data数组内数组空间的使用个数，应小于MaxNuDataInTriDNode;
	int NuNodesUsed;//使用节点的总数;
	int NuDecimal;//小数点位数;
	bool IsScientific;//是否科学计数法;
	TriDNodeHeadDefine()
	{
		DimensionOfNodes=0;//维数，创建数据空间时根据个方向节点数是否大于1自动计算;
		NuNodesInX=0;//x方向节点数，初始为零;
		NuNodesInY=0;//y方向节点数，初始为零;
		NuNodesInZ=0;//z方向节点数，初始为零;
		NuNodesTotal=0;//总节点数，初始为零;
		Title="DataTemp.dat";//数据空间的名称，可修改;
		FirstNode=nullptr;//首节点;
		EndNode=nullptr;//末节点;
		TitleXYZ[0]="x";//x坐标名称;
		TitleXYZ[1]="y";//y坐标名称;
		TitleXYZ[2]="z";//z坐标名称;
		NuDataUsed=0;//数据空间使用数据的个数;
		Inf="No Remark";//空间的备注说明;
		IsScientific=true;//数据输出时，是否科学计数法，不修改时即不采用;
		NuDecimal=4;//数据输出时，小数点位数，不修改为8位;
	}
};

class TriData
{
public:
	int KeyDirection[3];
	int NuIJK;//读取data文件时,IJK存在的个数，都存在为3;
	int IsExistI;//读取data文件时，是否存在I=,不存在为-1，存在为IJK排列次序编号，零开始;
	int IsExistJ;//读取data文件时，是否存在I=,不存在为-1，存在为IJK排列次序编号，零开始;
	int IsExistK;//读取data文件时，是否存在I=,不存在为-1，存在为IJK排列次序编号，零开始;
	TriDNodeHeadDefine *TriDataUsing;
	TriDNodeHeadDefine *TriDataUsingTemp;
	double m_RelaxFactor;
	//三维数据空间使用初始化;
	void TriDataInitialization(void);
	//创建节点,x,y,z三个方向上的数量，初始数据都是零;
	TriDNodeHeadDefine * CreatNodes(int NuX, int NuY, int NuZ);
	//创建头节点;
	TriDNodeHeadDefine * CreatNodeHead(int NuData);
	TriDNodeHeadDefine * CreatNodeHead();
	//复制head节点;
	void CopyHead(TriDNodeHeadDefine *nodehead1,TriDNodeHeadDefine *nodehead2);
	//将head1中的信息复制给head2，但是节点数不变;
	void CopyHeadInf(TriDNodeHeadDefine *nodehead1,TriDNodeHeadDefine *nodehead2);
	//将head1复制给head2;
	TriDNodeHeadDefine * CopyNodes(TriDNodeHeadDefine *nodehead1,TriDNodeHeadDefine *nodehead2);
	//生成并初始化节点,节点号、坐标置零，链接指针置NULL;
	TriDNodeDefine * CreatNode();
	TriDNodeDefine * CreatNode(int NuData);
	TriDNodeDefine * CreatNode(int NuData,double data0);
	//创建节点无头结点,x,y,z三个方向上的数量，初始数据都是零;
	TriDNodeDefine * CreatNodesWithoutHead(int NuX, int NuY, int NuZ);
	//创建节点,x,y,z三个方向上的数量，并对数据进行初始化;
    TriDNodeHeadDefine *CreatNodes(int NuX, int NuY, int NuZ,int NDataUsed,double data);
    TriDNodeHeadDefine *CreatNodes(int NuX, int NuY, int NuZ,TriDNodeHeadDefine *Sample);
	//移动节点，移动一步,当前节点，移动方向，返回移动后的节点指针，方向1、2、3分别为x，y，z方向，负号表示负方向;
	TriDNodeDefine * MoveNode(TriDNodeDefine *node,int direction);
	//两个节点之间的链接，node1指向node2,direction=1 x方向，2 y方向，3 z方向;
	void ConnectTwoNodes(TriDNodeDefine *node1,TriDNodeDefine *node2,int direction);
	//两行节点之间的链接，node1所在的行指向node2所在的行,direction=1 x方向，2 y方向，3 z方向;
	void ConnectTwoLineNodes(TriDNodeDefine *node1,TriDNodeDefine *node2,int directionMove,int directionLink);
	//两面节点之间的链接，node1所在面指向node2所在面,direction=1 x方向，2 y方向，3 z方向;
	void ConnectTwoFaceNodes(TriDNodeDefine *node1,TriDNodeDefine *node2,int directionLink);
	//针对头指针删除节点;
	void DeleteNodes(TriDNodeHeadDefine *nodehead);
	void DeleteHead(TriDNodeHeadDefine *nodehead);
	//将节点的数据全部置零;
	void SetNodesData(TriDNodeHeadDefine *nodehead,double data);
	//将节点的某个数据置零;
	void SetNodesData(TriDNodeHeadDefine *nodehead,int no,double data);
	//将节点的某个数据乘以零;
	void MultiplyNodesData(TriDNodeHeadDefine *nodehead,int no,double data);
	//输出节点数据到文件（三维格式，即包括IJK，即使为1）,格式可被tecplot读入，变量为：头指针，全名（包括路径），数据存储的优先方向（如“XYZ”，“ZYX”等）;
	void outputdata_3d(TriDNodeHeadDefine *nodehead,std::string FullName,std::string Direction);
	//针对 1 半径，2水平角度，3竖直角度的球坐标输出笛卡尔坐标形式
   void  outputdata_3d_ZB(TriDNodeHeadDefine *nodehead,std::string FullName,std::string Direction);
   //针对 1 角度，2半径，3高度的柱坐标输出笛卡尔坐标形式
   void  outputdata_3d_ZB2(TriDNodeHeadDefine *nodehead,std::string FullName,std::string Direction);
	//从文件FullName读取数据，该文件格式同tecplot读取格式;
	TriDNodeHeadDefine * ReadData(std::string FullName);
	//读数据文件，按照方向以及IJK的数量读取
	TriDNodeHeadDefine * ReadData(std::string FullName,std::string direction);
	//从字符数组linetext分解double数据至数组datatemp，返回识别数据的个数;
	//对VARIABLES存储的字符进行数值辨认;
	void ATOF_VARIABLES(TriDNodeHeadDefine *nodehead);
	int IdentifyData(char linetext[LineChar_MaxLength],double datatemp[]);
	int IdentifyData2(char linetext[LineChar_MaxLength],double datatemp[]);
	//数据文件第一行处理，即提取文件名;
	std::string Process1stLine(char linetext[LineChar_MaxLength]);
	//数据文件第二行处理，提取变量名;
	int Process2ndLine(char linetext[LineChar_MaxLength],vector <std::string> &VARIABLESTemp,vector <std::string> &VARIABLESDataTemp);
	//数据文件第三行处理，提取IJK信息;
	bool Process3rdLine(char linetext[LineChar_MaxLength]);
	//读取数据时，根据新读取节点与上一节点判别存储的xyz方向;
	int ProcessDirection(TriDNodeDefine *nodenew,TriDNodeDefine *nodeold);
	int ProcessDirection(double x,double y,double z,TriDNodeDefine *nodeold);
	//将node1中data的数据及坐标拷贝给node2;
	bool CopyDataNode1toNode2(TriDNodeDefine *node1,TriDNodeDefine *node2);

	//样条插值,样本数组x，样本数组y，样本总数，插值x数组，插值存储的空间y，插值个数;
	bool  SplineInterpolation(double xAxis[], double yAxis[], int NYT, double x[],double y[],int Nyt);
	//样条插值,样本数组x，样本数组y，样本总数，插值x数组，插值存储的空间y，插值个数，是否y值指数变换;
	bool  SplineInterpolation(double xAxis[], double yAxis[], int NYT, double x[],double y[],int Nyt,bool IsExponent);
	void Interpolation(TriDNodeDefine *node0,int direction,int Start,int End,int noData,double xdata[],double ydata[],int Nu);
	double Interpolation(TriDNodeDefine *node0,int direction,int Start,int End,int noData,double xdata);
	//拉格朗日插值,样本数组x，样本数组y，样本总数，插值x数组，插值存储的空间y，插值个数;
	bool  LagrangeInterpolation(double xAxis[], double yAxis[], int NYT, double x[],double y[],int Nyt);
	//从一单调数组中，找到x所在的位置，如介于i与i+1之间，返回i，不合规定返回-2，最左边返回-1,最右边返回NYT-1;
	int LocationInLineArray(double xAxis[],int NYT,double x);
	//线性插值,样本数组x，样本数组y，样本总数，插值x数组，插值存储的空间y，插值个数;
	bool LineInterpolation(double xAxis[], double yAxis[], int NYT, double x[],double y[],int Nyt);

	//最大似然法
bool MLEM_Iteration(TriDNodeHeadDefine *headA,std::string DirectionA,TriDNodeHeadDefine *headB,std::string DirectionB,TriDNodeHeadDefine *headX,std::string DirectionX,double  Resid[]);
//向量A与B做内积;
double ABVectorsProduct(TriDNodeDefine *nodeA,int directionA,TriDNodeDefine *nodeB,int directionB,int NoData);
//向量A的平方和平均;
double VectorsMeanSquare(TriDNodeDefine *nodeA,int directionA,int NoData);
//求方程残差,RMS(B-AX),系数矩阵A的头节点，方程右边B头节点，未知数X头节点，计算残差的第NoData个数据;
bool ResidualOfABX(TriDNodeHeadDefine *headA,int directionAI,int directionAJ,TriDNodeHeadDefine *headB,int directionB,TriDNodeHeadDefine *headX,int directionX,double Resid[]);
//X赋初值,X=AT*B,系数矩阵A，矩阵坐标平面（如“XY”指X为纵向，Y为横向），结果数据B，方向，未知数数据空间X，方向;
bool InitializationX(TriDNodeHeadDefine *headA,std::string DirectionA,TriDNodeHeadDefine *headB,std::string DirectionB,TriDNodeHeadDefine *headX,std::string DirectionX);

};


