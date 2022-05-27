#include "TriData.h"
#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <math.h>
#include <cstring>

using namespace std;
#define MaxNuDataInTriDNode 100

	//��ά���ݿռ�ʹ�ó�ʼ��;
void TriData::TriDataInitialization(void)
{
	TriDataUsing=nullptr;
}

//���ɲ���ʼ���ڵ�,�ڵ�š��������㣬����ָ����NULL;
TriDNodeDefine * TriData::CreatNode()
{
	return CreatNode(0,0);
}
TriDNodeDefine * TriData::CreatNode(int NuData)
{
	return CreatNode(NuData,0);
}
TriDNodeDefine * TriData::CreatNode(int NuData,double data0)
{
	TriDNodeDefine *node=new TriDNodeDefine;
	for(int i=0;i<NuData;i++)
		node->data.push_back(data0);
	return node;
}
//�ƶ��ڵ㣬�ƶ�һ��,��ǰ�ڵ㣬�ƶ����򣬷����ƶ���Ľڵ�ָ�룬����1��2��3�ֱ�Ϊx��y��z���򣬸��ű�ʾ������;
TriDNodeDefine * TriData::MoveNode(TriDNodeDefine *node,int direction)
{
	if(!node) return NULL;
	else if(direction==1)
	{
		if(node->NextX&&node->NoX<node->NextX->NoX) return node->NextX;//����1Ϊx�������ƶ�һ��;
		else return NULL;
	}
	else if(direction==-1)
	{
		if(node->LastX&&node->NoX>node->LastX->NoX) return node->LastX;//����-1Ϊx�������ƶ�һ��;
		else return NULL;
	}
	else if(direction==2)
	{
		if(node->NextY&&node->NoY<node->NextY->NoY) return node->NextY;//����2Ϊy�������ƶ�һ��;
		else return NULL;
	}
	else if(direction==-2)
	{
		if(node->LastY&&node->NoY>node->LastY->NoY) return node->LastY;//����-2Ϊy�������ƶ�һ��;
		else return NULL;
	}
	else if(direction==3)
	{
		if(node->NextZ&&node->NoZ<node->NextZ->NoZ) return node->NextZ;//����3Ϊz�������ƶ�һ��;
		else return NULL;
	}
	else if(direction==-3)
	{
		if(node->LastZ&&node->NoZ>node->LastZ->NoZ) return node->LastZ;//����-3Ϊz�������ƶ�һ��;
		else return NULL;
	}
	else return NULL;
}

//�����ڵ�֮������ӣ�node1ָ��node2,direction=1 x����2 y����3 z����;
void TriData::ConnectTwoNodes(TriDNodeDefine *node1,TriDNodeDefine *node2,int direction)
{
	if(!node1||!node2) return;
	if(direction==1)
	{		node1->NextX=node2;node2->LastX=node1;	}
	else if(direction==2)
	{		node1->NextY=node2;node2->LastY=node1;	}
	else
	{		node1->NextZ=node2;node2->LastZ=node1;	}
}
//���нڵ�֮������ӣ�node1���ڵ���ָ��node2���ڵ���,direction=1 x����2 y����3 z����;
void TriData::ConnectTwoLineNodes(TriDNodeDefine *node1,TriDNodeDefine *node2,int directionMove,int directionLink)
{
	while(node1||node2)
	{
		ConnectTwoNodes(node1,node2,directionLink);
		node1=MoveNode(node1,directionMove);
		node2=MoveNode(node2,directionMove);
	}
}
//����ڵ�֮������ӣ�node1������ָ��node2������,direction=1 x����2 y����3 z����;
void TriData::ConnectTwoFaceNodes(TriDNodeDefine *node1,TriDNodeDefine *node2,int directionLink)
{
	int direction1,direction2;
	if(directionLink==1) {direction1=2;direction2=3;}
	else if(directionLink==2) {direction1=1;direction2=3;}
	else {direction1=1;direction2=2;}
	while(node1||node2)
	{
		ConnectTwoLineNodes(node1,node2,direction2,directionLink);
		node1=MoveNode(node1,direction1);
		node2=MoveNode(node2,direction1);
	}
}

//����ͷ�ڵ�;
TriDNodeHeadDefine * TriData::CreatNodeHead(int NuData)
{
	TriDNodeHeadDefine *nodehead;
    std::string CTemp;
	nodehead=new TriDNodeHeadDefine;
	for(int i=0;i<NuData;i++)
	{
		nodehead->VARIABLESData.push_back(i);//�ڵ����ݿռ�ĺ����꣬���������˵�������ݣ��Զ�ʶ���double����;
//		CTemp.Format(_T("VARIABLE%d"),i);
        CTemp = "VARIABLE" + i;
		nodehead->VARIABLES.push_back(CTemp);//�ڵ�����˵�������������;
	}
	nodehead->NuDataUsed=NuData;
	return nodehead;
}
TriDNodeHeadDefine * TriData::CreatNodeHead()
{
	return CreatNodeHead(0);
}


//��head1�е���Ϣ���Ƹ�head2������ָ�벻����;
void TriData::CopyHead(TriDNodeHeadDefine *nodehead1,TriDNodeHeadDefine *nodehead2)
{
	nodehead2->DimensionOfNodes=nodehead1->DimensionOfNodes;//ά�����������ݿռ�ʱ���ݸ�����ڵ����Ƿ����1�Զ�����;
	nodehead2->NuNodesInX=nodehead1->NuNodesInX;//x����ڵ�������ʼΪ��;
	nodehead2->NuNodesInY=nodehead1->NuNodesInY;//y����ڵ�������ʼΪ��;
	nodehead2->NuNodesInZ=nodehead1->NuNodesInZ;//z����ڵ�������ʼΪ��;
	nodehead2->NuNodesTotal=nodehead1->NuNodesTotal;//�ܽڵ�������ʼΪ��;
	nodehead2->NuNodesUsed=nodehead1->NuNodesUsed;//ʹ�õ��ܽڵ�������ʼΪ��;
	nodehead2->Title=nodehead1->Title;//���ݿռ�����ƣ����޸�;

	copy(nodehead1->VARIABLESData.begin(),nodehead1->VARIABLESData.end(),nodehead2->VARIABLESData.begin());
	copy(nodehead1->VARIABLES.begin(),nodehead1->VARIABLES.end(),nodehead2->VARIABLES.begin());

	nodehead2->TitleXYZ[0]=nodehead1->TitleXYZ[0];//x��������;
	nodehead2->TitleXYZ[1]=nodehead1->TitleXYZ[1];//y��������;
	nodehead2->TitleXYZ[2]=nodehead1->TitleXYZ[2];//z��������;
	nodehead2->NuDataUsed=nodehead1->NuDataUsed;//���ݿռ�ʹ�����ݵĸ���;
	nodehead2->Inf=nodehead1->Inf;//�ռ�ı�ע˵��;
	nodehead2->IsScientific=nodehead1->IsScientific;//�������ʱ���Ƿ��ѧ�����������޸�ʱ��������;
	nodehead2->NuDecimal=nodehead1->NuDecimal;//�������ʱ��С����λ�������޸�Ϊ8λ;
}
//��head1���Ƹ�head2;
TriDNodeHeadDefine * TriData::CopyNodes(TriDNodeHeadDefine *nodehead1,TriDNodeHeadDefine *nodehead2)
{
	if(nodehead2==NULL)
	{
		nodehead2=CreatNodes(nodehead1->NuNodesInX, nodehead1->NuNodesInY, nodehead1->NuNodesInZ);
		CopyHead(nodehead1,nodehead2);
	}
	int i,j,k;
	TriDNodeDefine *node11,*node12,*node13;
	TriDNodeDefine *node21,*node22,*node23;
	node11=nodehead1->FirstNode;
	node21=nodehead2->FirstNode;
	for(i=0;i<nodehead1->NuNodesInZ&&node11&&node21;i++,node11=node11->NextZ,node21=node21->NextZ)
	{
		node22=node21;node12=node11;
		for(j=0;j<nodehead1->NuNodesInY&&node12&&node22;j++,node12=node12->NextY,node22=node22->NextY)
		{
			node23=node22;node13=node12;
			for(k=0;k<nodehead1->NuNodesInX&&node13&&node23;k++,node13=node13->NextX,node23=node23->NextX)
			{
				CopyDataNode1toNode2(node13,node23);
			}
		}
	}
	return nodehead2;
}
//��head1�е���Ϣ���Ƹ�head2�����ǽڵ�������;
void TriData::CopyHeadInf(TriDNodeHeadDefine *nodehead1,TriDNodeHeadDefine *nodehead2)
{
	//nodehead2->DimensionOfNodes=nodehead1->DimensionOfNodes;//ά�����������ݿռ�ʱ���ݸ�����ڵ����Ƿ����1�Զ�����;
	//nodehead2->Title=nodehead1->Title;//���ݿռ�����ƣ����޸�;
	//copy(nodehead1->VARIABLESData.begin(),nodehead1->VARIABLESData.end(),nodehead2->VARIABLESData.begin());
	//copy(nodehead1->VARIABLES.begin(),nodehead1->VARIABLES.end(),nodehead2->VARIABLES.begin());

	nodehead2->VARIABLESData.clear();
	nodehead2->VARIABLES.clear();
	for(int n=0;n<nodehead1->NuDataUsed;n++)
	{
		nodehead2->VARIABLESData.push_back(nodehead1->VARIABLESData[n]);
		nodehead2->VARIABLES.push_back(nodehead1->VARIABLES[n]);
	}
	nodehead2->TitleXYZ[0]=nodehead1->TitleXYZ[0];//x��������;
	nodehead2->TitleXYZ[1]=nodehead1->TitleXYZ[1];//y��������;
	nodehead2->TitleXYZ[2]=nodehead1->TitleXYZ[2];//z��������;
	nodehead2->NuDataUsed=nodehead1->NuDataUsed;//���ݿռ�ʹ�����ݵĸ���;
	nodehead2->Inf=nodehead1->Inf;//�ռ�ı�ע˵��;
	nodehead2->IsScientific=nodehead1->IsScientific;//�������ʱ���Ƿ��ѧ�����������޸�ʱ��������;
	nodehead2->NuDecimal=nodehead1->NuDecimal;//�������ʱ��С����λ�������޸�Ϊ8λ;
}
//�����ڵ�,x,y,z���������ϵ���������ʼ���ݶ�����;
TriDNodeHeadDefine * TriData::CreatNodes(int NuX, int NuY, int NuZ)
{
	if(NuX>1000000||NuY>1000000||NuZ>1000000) { return NULL;}
	if(NuX<1||NuY<1||NuZ<1) {  return NULL;}
	int i,j,k;
	TriDNodeDefine *node1,*node2,*nodeLine1,*nodeLine2,*nodeFace1,*nodeFace2;
	TriDNodeHeadDefine *nodehead;
	nodehead=CreatNodeHead();
	nodehead->NuNodesInX=NuX;
	nodehead->NuNodesInY=NuY;
	nodehead->NuNodesInZ=NuZ;
	nodehead->NuNodesTotal=NuX*NuY*NuZ;
	nodehead->NuNodesUsed=nodehead->NuNodesTotal;
	nodehead->DimensionOfNodes=3;
	if(NuX==1) nodehead->DimensionOfNodes--;
	if(NuY==1) nodehead->DimensionOfNodes--;
	if(NuZ==1) nodehead->DimensionOfNodes--;

	for(i=0;i<NuZ;i++)
	{
		for(j=0;j<NuY;j++)
		{
			for(k=0;k<NuX;k++)
			{
				node2=CreatNode();
				node2->NoX=k;
				node2->NoY=j;
				node2->NoZ=i;
				node2->x=node2->NoX;
				node2->y=node2->NoY;
				node2->z=node2->NoZ;
				if(i==0&&j==0&&k==0) nodehead->FirstNode=node2;
				if(k==0) nodeLine2=node2;
				else ConnectTwoNodes(node1,node2,1);
				node1=node2;
			}
			if(j==0) nodeFace2=nodeLine2;
			else ConnectTwoLineNodes(nodeLine1,nodeLine2,1,2);
			nodeLine1= nodeLine2;
		}
		if(i>0) ConnectTwoFaceNodes(nodeFace1,nodeFace2,3);
		nodeFace1=nodeFace2;
	}
	nodehead->EndNode=node2;
	return nodehead;
}

//�����ڵ�,x,y,z���������ϵ��������������ݽ��г�ʼ��;
TriDNodeHeadDefine * TriData::CreatNodes(int NuX, int NuY, int NuZ,int NDataUsed,double data)
{
	if(NuX>1000000||NuY>1000000||NuZ>1000000) { return NULL;}
	if(NuX<1||NuY<1||NuZ<1) {  return NULL;}
	int i,j,k;
	TriDNodeDefine *node1,*node2,*nodeLine1,*nodeLine2,*nodeFace1,*nodeFace2;
	TriDNodeHeadDefine *nodehead;
	nodehead=CreatNodeHead(NDataUsed);
	nodehead->NuNodesInX=NuX;
	nodehead->NuNodesInY=NuY;
	nodehead->NuNodesInZ=NuZ;
	nodehead->NuNodesTotal=NuX*NuY*NuZ;
	nodehead->NuNodesUsed=nodehead->NuNodesTotal;
	nodehead->DimensionOfNodes=3;
	if(NuX==1) nodehead->DimensionOfNodes--;
	if(NuY==1) nodehead->DimensionOfNodes--;
	if(NuZ==1) nodehead->DimensionOfNodes--;

	for(i=0;i<NuZ;i++)
	{
		for(j=0;j<NuY;j++)
		{
			for(k=0;k<NuX;k++)
			{
				node2=CreatNode(NDataUsed, data);
				node2->NoX=k;
				node2->NoY=j;
				node2->NoZ=i;
				node2->x=node2->NoX;
				node2->y=node2->NoY;
				node2->z=node2->NoZ;
				if(i==0&&j==0&&k==0) nodehead->FirstNode=node2;
				if(k==0) nodeLine2=node2;
				else ConnectTwoNodes(node1,node2,1);
				node1=node2;
			}
			if(j==0) nodeFace2=nodeLine2;
			else ConnectTwoLineNodes(nodeLine1,nodeLine2,1,2);
			nodeLine1= nodeLine2;
		}
		if(i>0) ConnectTwoFaceNodes(nodeFace1,nodeFace2,3);
		nodeFace1=nodeFace2;
	}
	nodehead->EndNode=node2;

	return nodehead;
}

//�����ڵ�,x,y,z���������ϵ��������������ݽ��г�ʼ��;
TriDNodeHeadDefine * TriData::CreatNodes(int NuX, int NuY, int NuZ,TriDNodeHeadDefine *Sample)
{
	TriDNodeHeadDefine *nodehead=CreatNodes(NuX, NuY, NuZ,Sample->NuDataUsed,0);
	CopyHeadInf(Sample,nodehead);
	return nodehead;
}

//�����ڵ���ͷ���,x,y,z���������ϵ���������ʼ���ݶ�����;
TriDNodeDefine * TriData::CreatNodesWithoutHead(int NuX, int NuY, int NuZ)
{
	if(NuX>1000000||NuY>1000000||NuZ>1000000) {  return NULL;}
	if(NuX<1||NuY<1||NuZ<1) {  return NULL;}
	int i,j,k;
	TriDNodeDefine *node0,*node1,*node2,*nodeLine1,*nodeLine2,*nodeFace1,*nodeFace2;

	for(i=0;i<NuZ;i++)
	{
		for(j=0;j<NuY;j++)
		{
			for(k=0;k<NuX;k++)
			{
				node2=CreatNode();
				node2->NoX=k;
				node2->NoY=j;
				node2->NoZ=i;
				node2->x=node2->NoX;
				node2->y=node2->NoY;
				node2->z=node2->NoZ;
				if(i==0&&j==0&&k==0) node0=node2;
				if(k==0) nodeLine2=node2;
				else ConnectTwoNodes(node1,node2,1);
				node1=node2;
			}
			if(j==0) nodeFace2=nodeLine2;
			else ConnectTwoLineNodes(nodeLine1,nodeLine2,1,2);
			nodeLine1= nodeLine2;
		}
		if(i>0) ConnectTwoFaceNodes(nodeFace1,nodeFace2,3);
		nodeFace1=nodeFace2;
	}
	return node0;
}

void TriData::DeleteHead(TriDNodeHeadDefine *nodehead)
{
	if(!nodehead) return;
	nodehead->VARIABLES.clear();
	nodehead->VARIABLESData.clear();
	delete nodehead;
}
//���ͷָ��ɾ���ڵ�;
void TriData::DeleteNodes(TriDNodeHeadDefine *nodehead)
{
	if(!nodehead) return;
	TriDNodeDefine *nodex,*nodey,*nodez,*node;
	nodez=nodehead->FirstNode;
	int i,j,k;
	for(i=0;i<nodehead->NuNodesInZ;i++)
	{
		nodey=nodez;
		nodez=nodez->NextZ;
		for(j=0;j<nodehead->NuNodesInY;j++)
		{
			nodex=nodey;
			nodey=nodey->NextY;
			for(k=0;k<nodehead->NuNodesInX;k++)
			{
				node=nodex;
				nodex=nodex->NextX;
				if(node) node->data.clear();
				delete node;
			}
		}
	}
	DeleteHead(nodehead);
}

//���ڵ������ȫ������;
void TriData::SetNodesData(TriDNodeHeadDefine *nodehead,double data)
{
	if(!nodehead) return;
	TriDNodeDefine *nodex,*nodey,*nodez;
	int i,j,k,ndata;
	for(i=0,nodez=nodehead->FirstNode;i<nodehead->NuNodesInZ&&nodez;i++,nodez=nodez->NextZ)
		for(j=0,nodey=nodez;j<nodehead->NuNodesInY&&nodey;j++,nodey=nodey->NextY)
			for(k=0,nodex=nodey;k<nodehead->NuNodesInX&&nodex;k++,nodex=nodex->NextX)
				for(ndata=0;ndata<nodex->data.size();ndata++)
					nodex->data[i]=data;
}

//���ڵ��ĳ����������;
void TriData::SetNodesData(TriDNodeHeadDefine *nodehead,int no,double data)
{
	if(!nodehead) return;
	TriDNodeDefine *nodex,*nodey,*nodez;
	int i,j,k;
	for(i=0,nodez=nodehead->FirstNode;i<nodehead->NuNodesInZ&&nodez;i++,nodez=nodez->NextZ)
		for(j=0,nodey=nodez;j<nodehead->NuNodesInY&&nodey;j++,nodey=nodey->NextY)
			for(k=0,nodex=nodey;k<nodehead->NuNodesInX&&nodex;k++,nodex=nodex->NextX)
					nodex->data[no]=data;
}
//���ڵ��ĳ�����ݳ�����;
void TriData::MultiplyNodesData(TriDNodeHeadDefine *nodehead,int no,double data)
{
	if(!nodehead) return;
	TriDNodeDefine *nodex,*nodey,*nodez;
	int i,j,k;
	for(i=0,nodez=nodehead->FirstNode;i<nodehead->NuNodesInZ&&nodez;i++,nodez=nodez->NextZ)
		for(j=0,nodey=nodez;j<nodehead->NuNodesInY&&nodey;j++,nodey=nodey->NextY)
			for(k=0,nodex=nodey;k<nodehead->NuNodesInX&&nodex;k++,nodex=nodex->NextX)
					nodex->data[no]*=data;
}
//����ڵ����ݵ��ļ�����ά��ʽ��������IJK����ʹΪ1��,��ʽ�ɱ�tecplot���룬����Ϊ��ͷָ�룬ȫ��������·���������ݴ洢�����ȷ����硰XYZ������ZYX���ȣ�;
void TriData::outputdata_3d(TriDNodeHeadDefine *nodehead,std::string FullName,std::string Direction)
{
		if(!nodehead) return;
		if(!nodehead->FirstNode) return;
		int i,j,k,l;
		int Direction1,Direction2,Direction3;
        std::string CTemp;
		char IJK[3]={'I','J','K'};
		double Dxyz[3];
		int NuXYZ[3];
		NuXYZ[0]=nodehead->NuNodesInX;
		NuXYZ[1]=nodehead->NuNodesInY;
		NuXYZ[2]=nodehead->NuNodesInZ;

		if(Direction.find("XYZ")>=0||Direction.find(("xyz"))>=0) {Direction1=1;Direction2=2;Direction3=3;}
		if(Direction.find("XYZ")>=0||Direction.find(("xyz"))>=0) {Direction1=1;Direction2=2;Direction3=3;}
        
		else if(Direction.find("XZY")>=0||Direction.find(("xzy"))>=0) {Direction1=1;Direction2=3;Direction3=2;}
		else if(Direction.find(("YXZ"))>=0||Direction.find(("yxz"))>=0) {Direction1=2;Direction2=1;Direction3=3;}
		else if(Direction.find(("YZX"))>=0||Direction.find(("yzx"))>=0) {Direction1=2;Direction2=3;Direction3=1;}
		else if(Direction.find(("ZXY"))>=0||Direction.find(("zxy"))>=0) {Direction1=3;Direction2=1;Direction3=2;}
		else if(Direction.find(("ZYX"))>=0||Direction.find(("zyx"))>=0) {Direction1=3;Direction2=2;Direction3=1;}
		else {std::cerr << " Direction selection is wrong in 3D output.";return;}

		ofstream output_data;
		output_data.open(FullName,ios::trunc);
		//if(nodehead->IsScientific) output_data.setf(ios::scientific);
		//else output_data.setf(ios::fixed,ios::floatfield);
		//output_data.precision(nodehead->NuDecimal);

//		CTemp.Format(("TITLE=\"%s\""),nodehead->Title);
        CTemp = "TITLE=\"" + nodehead->Title +"\"";
	//	output_data<<"TITLE=\""<<nodehead->Title<<"\"\n";
		output_data<<CTemp<<endl;
	//	AfxMessageBox(nodehead->Title);
		CTemp.clear();
//		CTemp.Format(("VARIABLES= \"%s\" \"%s\" \"%s\" "),nodehead->TitleXYZ[0],nodehead->TitleXYZ[1],nodehead->TitleXYZ[2]);
        CTemp = "VARIABLES= \"" + nodehead->TitleXYZ[0] + "\" " +  nodehead->TitleXYZ[1] + "\" "  + nodehead->TitleXYZ[2] + "\" ";

		output_data<<CTemp;
		for(i=0;i<nodehead->VARIABLES.size();i++)
		{
//			CTemp.Format(("\"%s\" "),nodehead->VARIABLES[i]);
            CTemp = "\"" + nodehead->VARIABLES[i] + "\" ";
			output_data<<CTemp;
		}
		output_data<<endl;
        CTemp.clear();
//		CTemp.Format(("ZONE T=\"3D Data\" %c=%d %c=%d %c=%d F=POINT\n"),IJK[0],NuXYZ[0],IJK[1],NuXYZ[1],IJK[2],NuXYZ[2]);
        CTemp = "ZONE T=\"3D Data\" "+ to_string(IJK[0]) + "=" + to_string(NuXYZ[0]) + to_string(IJK[1]) + "=" + to_string(NuXYZ[1]) + to_string(IJK[2]) + "=" + to_string(NuXYZ[2]) +"%c=%d %c=%d %c=%d F=POINT\n";

//        CTemp = "ZONE T=\"3D Data\" "+ +"=%d %c=%d %c=%d F=POINT\n"
		output_data<<CTemp;

		TriDNodeDefine *nodez=nodehead->FirstNode,*nodey,*nodex;
		while(nodez)
		{
			nodey=nodez;
			while(nodey)
			{
				nodex=nodey;
				while(nodex)
				{
					Dxyz[0]=nodex->x;Dxyz[1]=nodex->y;Dxyz[2]=nodex->z;
//					CTemp.Format("%.4e %.4e %.4e",Dxyz[0],Dxyz[1],Dxyz[2]);
                    CTemp = to_string(Dxyz[0]) + " " + to_string(Dxyz[1]) + " " + to_string(Dxyz[2]) + " ";
					output_data<<CTemp<<" ";
					for(l=0;l<nodex->data.size();l++)
					{
//                        CTemp.Format("%.4e",nodex->data[l]);
                        CTemp = to_string(nodex->data[l]);
						output_data<<CTemp<<" ";
					}
					 output_data<<endl;
					 nodex=MoveNode(nodex,Direction1);
				}
				 nodey=MoveNode(nodey,Direction2);
			}
			nodez=MoveNode(nodez,Direction3);
		}
		output_data<<"END";
		output_data.close();
}
//��� 1 �뾶��2ˮƽ�Ƕȣ�3��ֱ�Ƕȵ�����������ѿ���������ʽ
void TriData::outputdata_3d_ZB(TriDNodeHeadDefine *nodehead,std::string FullName,std::string Direction)
{
		if(!nodehead) return;
		if(!nodehead->FirstNode) return;
		int i,k,l;
		int Direction1,Direction2,Direction3;
		std::string CTemp;
		char IJK[3]={'I','J','K'};
		double Dxyz[3];
		int NuXYZ[3];
		NuXYZ[0]=nodehead->NuNodesInX;
		NuXYZ[1]=nodehead->NuNodesInY;
		NuXYZ[2]=nodehead->NuNodesInZ;
		if(Direction.find(("XYZ"))>=0||Direction.find(("xyz"))>=0) {Direction1=1;Direction2=2;Direction3=3;}
		else if(Direction.find(("XZY"))>=0||Direction.find(("xzy"))>=0) {Direction1=1;Direction2=3;Direction3=2;}
		else if(Direction.find(("YXZ"))>=0||Direction.find(("yxz"))>=0) {Direction1=2;Direction2=1;Direction3=3;}
		else if(Direction.find(("YZX"))>=0||Direction.find(("yzx"))>=0) {Direction1=2;Direction2=3;Direction3=1;}
		else if(Direction.find(("ZXY"))>=0||Direction.find(("zxy"))>=0) {Direction1=3;Direction2=1;Direction3=2;}
		else if(Direction.find(("ZYX"))>=0||Direction.find(("zyx"))>=0) {Direction1=3;Direction2=2;Direction3=1;}
		else {std::cerr << " Direction selection is wrong in 3D output." << std::endl;return;}

		ofstream output_data;
		output_data.open(FullName,ios::trunc);
		if(nodehead->IsScientific) output_data.setf(ios::scientific);
		else output_data.setf(ios::fixed,ios::floatfield);
		output_data.precision(nodehead->NuDecimal);

//		CTemp.Format(("TITLE=\"%s\""),nodehead->Title);
        CTemp = "TITLE=\"" + nodehead->Title + "\"";
	//	output_data<<"TITLE=\""<<nodehead->Title<<"\"\n";
		output_data<<CTemp<<endl;
	//	AfxMessageBox(nodehead->Title);
		CTemp.clear();
//		CTemp.Format(("VARIABLES= \"%s\" \"%s\" \"%s\" "),"x","y","z");
        CTemp = "VARIABLES= x y z ";

		output_data<<CTemp;
		for(i=0;i<nodehead->VARIABLES.size();i++)
		{
//			CTemp.Format(("\"%s\" "),nodehead->VARIABLES[i]);
            CTemp = "\"" + nodehead->VARIABLES[i] + "\" ";
			output_data<<CTemp;
		}
		output_data<<endl;
		CTemp.clear();
//		CTemp.Format(("ZONE T=\"3D Data\" %c=%d %c=%d %c=%d F=POINT\n"),IJK[0],NuXYZ[0],IJK[1],NuXYZ[1],IJK[2],NuXYZ[2]);
        CTemp = "ZONE T=\"3D Data\" "+ to_string(IJK[0]) + "=" + to_string(NuXYZ[0]) + to_string(IJK[1]) + "=" + to_string(NuXYZ[1]) + to_string(IJK[2]) + "=" + to_string(NuXYZ[2]) +"%c=%d %c=%d %c=%d F=POINT\n";
		output_data<<CTemp;

		TriDNodeDefine *nodez=nodehead->FirstNode,*nodey,*nodex;
		while(nodez)
		{
			nodey=nodez;
			while(nodey)
			{
				nodex=nodey;
				while(nodex)
				{
					Dxyz[0]=nodex->x*cos(nodex->z*PI/180)*cos(nodex->y*PI/180);
					Dxyz[1]=nodex->x*cos(nodex->z*PI/180)*sin(nodex->y*PI/180);
					Dxyz[2]=nodex->x*sin(nodex->z*PI/180);

//					CTemp.Format("%.4e %.4e %.4e",Dxyz[0],Dxyz[1],Dxyz[2]);
                    CTemp = to_string(Dxyz[0]) + " " + to_string(Dxyz[1]) + " " + to_string(Dxyz[2]) + " ";
					output_data<<CTemp<<" ";
					for(l=0;l<nodex->data.size();l++)
					{
//						CTemp.Format("%.4e",nodex->data[l]);
                        CTemp = to_string(nodex->data[l]);
						output_data<<CTemp<<" ";
					}
					 output_data<<endl;
					 nodex=MoveNode(nodex,Direction1);
				}
				 nodey=MoveNode(nodey,Direction2);
			}
			nodez=MoveNode(nodez,Direction3);
		}
		output_data<<"END";
		output_data.close();
}
//��� 1 �Ƕȣ�2�뾶��3�߶ȵ�����������ѿ���������ʽ
void TriData::outputdata_3d_ZB2(TriDNodeHeadDefine *nodehead,std::string FullName,std::string Direction)
{
		if(!nodehead) return;
		if(!nodehead->FirstNode) return;
		int i,j,k,l;
		int Direction1,Direction2,Direction3;
		string CTemp;
		char IJK[3]={'I','J','K'};
		double Dxyz[3];
		int NuXYZ[3];
		NuXYZ[0]=nodehead->NuNodesInX;
		NuXYZ[1]=nodehead->NuNodesInY;
		NuXYZ[2]=nodehead->NuNodesInZ;
		if(Direction.find(("XYZ"))>=0||Direction.find(("xyz"))>=0) {Direction1=1;Direction2=2;Direction3=3;}
		else if(Direction.find(("XZY"))>=0||Direction.find(("xzy"))>=0) {Direction1=1;Direction2=3;Direction3=2;}
		else if(Direction.find(("YXZ"))>=0||Direction.find(("yxz"))>=0) {Direction1=2;Direction2=1;Direction3=3;}
		else if(Direction.find(("YZX"))>=0||Direction.find(("yzx"))>=0) {Direction1=2;Direction2=3;Direction3=1;}
		else if(Direction.find(("ZXY"))>=0||Direction.find(("zxy"))>=0) {Direction1=3;Direction2=1;Direction3=2;}
		else if(Direction.find(("ZYX"))>=0||Direction.find(("zyx"))>=0) {Direction1=3;Direction2=2;Direction3=1;}
        else {std::cerr << " Direction selection is wrong in 3D output." << std::endl; return;}
//		else {MessageBox(NULL,(" Direction selection is wrong in 3D output."),_T("Warning"),MB_ICONWARNING | MB_OK);return;}

		ofstream output_data;
		output_data.open(FullName,ios::trunc);
		if(nodehead->IsScientific) output_data.setf(ios::scientific);
		else output_data.setf(ios::fixed,ios::floatfield);
		output_data.precision(nodehead->NuDecimal);

//		CTemp.Format(("TITLE=\"%s\""),nodehead->Title);
        CTemp = "TITLE=\"" + nodehead->Title + "\"";

	//	output_data<<"TITLE=\""<<nodehead->Title<<"\"\n";
		output_data<<CTemp<<endl;
	//	AfxMessageBox(nodehead->Title);
		CTemp.clear();
//		CTemp.Format(("VARIABLES= \"%s\" \"%s\" \"%s\" "),"x","y","z");
        CTemp = "VARIABLES= x y z ";
		output_data<<CTemp;
		for(i=0;i<(int)nodehead->VARIABLES.size();i++)
		{
//			CTemp.Format(("\"%s\" "),nodehead->VARIABLES[i]);
            CTemp = "\"" + nodehead->VARIABLES[i] + "\" ";
			output_data<<CTemp;
		}
		output_data<<endl;
		CTemp.clear();
//		CTemp.Format(("ZONE T=\"3D Data\" %c=%d %c=%d %c=%d F=POINT\n"),IJK[0],NuXYZ[0],IJK[1],NuXYZ[1],IJK[2],NuXYZ[2]);
        CTemp = "ZONE T=\"3D Data\" "+ to_string(IJK[0]) + "=" + to_string(NuXYZ[0]) + to_string(IJK[1]) + "=" + to_string(NuXYZ[1]) + to_string(IJK[2]) + "=" + to_string(NuXYZ[2]) +"%c=%d %c=%d %c=%d F=POINT\n";
		output_data << CTemp;

		TriDNodeDefine *nodex,*nodey,*nodez;
		for(i=0,nodez=nodehead->FirstNode;i<nodehead->NuNodesInZ&&nodez;i++)
		{
			for(j=0,nodey=nodez;j<nodehead->NuNodesInY&&nodey;j++)
			{
				for(k=0,nodex=nodey;k<nodehead->NuNodesInX&&nodex;k++)
				{
					Dxyz[0]=nodex->y*cos(nodex->x*PI/180);
					Dxyz[1]=nodex->y*sin(nodex->x*PI/180);
					Dxyz[2]=nodex->z;
//					CTemp.Format("%.4e %.4e %.4e",Dxyz[0],Dxyz[1],Dxyz[2]);
                    CTemp = to_string(Dxyz[0]) + " " + to_string(Dxyz[1]) + " " + to_string(Dxyz[2]) + " ";
					output_data<< CTemp<< " ";
					for(l=0;l<(int)nodex->data.size();l++)
					{
//						CTemp.Format("%.4e",nodex->data[l]);
                        CTemp = to_string(nodex->data[l]);
						output_data<< CTemp <<" ";
					}
					 output_data<<endl;
					 nodex=MoveNode(nodex,Direction1);
				}
				 nodey=MoveNode(nodey,Direction2);
			}
			nodez=MoveNode(nodez,Direction3);
		}
		output_data<< "END";
		output_data.close();
}
//���ַ�����linetext�ֽ�double����������datatemp������ʶ�����ݵĸ���;
int TriData::IdentifyData(char linetext[LineChar_MaxLength],double datatemp[])
{
	char CharData[30];
    string CData[LineChar_MaxLength];
	int NuCData=0;
	int i,j;
	for(j=0;j<30;j++) CharData[j]='\0';
	for(i=0,j=0;i<LineChar_MaxLength;i++)
	{
		if((linetext[i]>='0'&&linetext[i]<='9')||linetext[i]=='e'||linetext[i]=='E'||linetext[i]=='+'||linetext[i]=='-'||linetext[i]=='.')
		{CharData[j++]=linetext[i];CharData[j]='\0';}
		else
		{
			if(j>0) CData[NuCData++]=CharData;
			for(j=0;j<30;j++) CharData[j]='\0';
			j=0;
		}
	}
	if(NuCData>0)
	{
		for(i=0;i<NuCData;i++)
		{
			datatemp[i]=stof(CData[i]);
		}
	}
	return NuCData;
}

int TriData::IdentifyData2(char linetext[LineChar_MaxLength],double datatemp[])
{
    char linetext2[LineChar_MaxLength];
    int i,j;
    for(i=0,j=0;i<LineChar_MaxLength&&linetext[i]!='\0';i++)
	{
        if((linetext[i]>='0'&&linetext[i]<='9')||linetext[i]=='e'||linetext[i]=='E'||linetext[i]=='+'||linetext[i]=='-'||linetext[i]=='.')
		{
            linetext2[j++]=linetext[i];	linetext2[j]='\0';
        } else {
            linetext[i]=' ';
            if(i==0&&linetext[i]==' ') continue;
            if(linetext[i-1]==' '&&linetext[i]==' ') continue;
            linetext2[j++]=linetext[i];	linetext2[j]='\0';
        }
    }
    char *p=linetext2;
    int NuCData=0;
    string CTemp;
//    cout << linetext2 << endl;
    p = strtok(linetext2," ");// Nuclide  *

    while(p&&*p!='\0')
	{
        CTemp=p;
		 if(CTemp.length()>0)
		 {
			 datatemp[NuCData++]=stof(CTemp);
		 }
		p=strtok(nullptr," ");//Peak Channel
	}
	return NuCData;
}

//�����ļ���һ�д�������ȡ�ļ���;
string TriData::Process1stLine(char linetext[LineChar_MaxLength])
{
	char chartemp[100]={'\0'};
	int i,j;
	int key=0;
	for(i=0,j=0;i<LineChar_MaxLength;i++)
	{
		if(linetext[i]=='"') key++;
		else if(key==1&&linetext[i]!=' ') {chartemp[j++]=linetext[i];chartemp[j]='\0';}
		if(key==2) break;
	}
	return chartemp;
}
//�����ļ��ڶ��д�����ȡ������;
int TriData::Process2ndLine(char linetext[LineChar_MaxLength], vector<string> &VARIABLESTemp,vector <string> &VARIABLESDataTemp)
{
	char chartemp[50]={'\0'};
	char chartemp2[50]={'\0'};
	int i,j,jj,k;
	int key=0;
	for(i=0,j=0,jj=0,k=0;i<LineChar_MaxLength;i++)
	{
		if(linetext[i]=='"') key++;
		else if(key>0&&(linetext[i]!=' '||linetext[i]!='\\t'))
		{
			chartemp[j++]=linetext[i];chartemp[j]='\0';
			if((linetext[i]>='0'&&linetext[i]<='9')||linetext[i]=='.'||linetext[i]=='+'||linetext[i]=='-')
			{chartemp2[jj++]=linetext[i];chartemp2[jj]='\0';}
		}
		if(key==2)
		{
			VARIABLESTemp.push_back(chartemp);VARIABLESDataTemp.push_back(chartemp2);k++;
//            std::cout << chartemp << std::endl;
			key=0;
			for(j=0;j<50;j++) {chartemp[j]='\0';chartemp2[j]='\0';}
			j=0;jj=0;
		}
	}
	return k;
}
//�����ļ������д�����ȡIJK��Ϣ;
bool TriData::Process3rdLine(char linetext[LineChar_MaxLength])
{
	char chartemp[32]={'\0'};
	char chartemp2[500]={'\0'};
	std::string CTemp,CTemp2;
	int i,j;
	int IData=0,JData=0,KData=0;
	int key;
	int DimensionIJK=0;
		for(i=0,j=0;i<LineChar_MaxLength;i++)
		{

			if(linetext[i]=='I') {chartemp2[j++]=linetext[i];chartemp2[j]='\0';}
			if(linetext[i]=='J') {chartemp2[j++]=linetext[i];chartemp2[j]='\0';}
			if(linetext[i]=='K') {chartemp2[j++]=linetext[i];chartemp2[j]='\0';}
			if(linetext[i]=='=') {chartemp2[j++]=linetext[i];chartemp2[j]='\0';}
			if(linetext[i]>='0'&&linetext[i]<='9') {chartemp2[j++]=linetext[i];chartemp2[j]='\0';}
		}

	CTemp=chartemp2;
	key=0;
	IsExistI=-1;	IsExistJ=-1;	IsExistK=-1;
	for(i=0;i<499&&i<=CTemp.length();i++)
	{
		if(chartemp2[i]=='I'&&chartemp2[i+1]=='=') {IData=i;IsExistI=DimensionIJK;DimensionIJK++;}
		if(chartemp2[i]=='J'&&chartemp2[i+1]=='=') {JData=i;IsExistJ=DimensionIJK;DimensionIJK++;}
		if(chartemp2[i]=='K'&&chartemp2[i+1]=='=') {KData=i;IsExistK=DimensionIJK;DimensionIJK++;}
	}
	if(DimensionIJK==0) return false;
	if(DimensionIJK>3) return false;
	NuIJK=DimensionIJK;//IJK���ڵĸ���;

	if(IData>0)
	{
		for(i=0;i<32;i++)
			chartemp[i]='\0';
		for(i=IData+2,j=0,key=0;i<499&&i<=CTemp.length();i++)
		{
			if(chartemp2[i]>='0'&&chartemp2[i]<='9'){chartemp[j++]=chartemp2[i];chartemp[j]='\0';}
			else break;
		}
		CTemp2.clear();CTemp2=chartemp;IData=stoi(CTemp2);
	}
	if(JData>0)
	{
		for(i=0;i<32;i++)
			chartemp[i]='\0';
		for(i=JData+2,j=0,key=0;i<499&&i<=CTemp.length();i++)
		{
			if(chartemp2[i]>='0'&&chartemp2[i]<='9'){chartemp[j++]=chartemp2[i];chartemp[j]='\0';}
			else break;
		}
		CTemp2.clear();CTemp2=chartemp;JData=stoi(CTemp2);
	}
	if(KData>0)
	{
		for(i=0;i<32;i++)
			chartemp[i]='\0';
		for(i=KData+2,j=0,key=0;i<499&&i<=CTemp.length();i++)
		{
			if(chartemp2[i]>='0'&&chartemp2[i]<='9'){chartemp[j++]=chartemp2[i];chartemp[j]='\0';}
			else break;
		}
		CTemp2.clear();CTemp2=chartemp;KData=stoi(CTemp2);
	}
	if(IData<1) IData=1;
	if(JData<1) JData=1;
	if(KData<1) KData=1;
	TriDataUsingTemp=CreatNodes(IData,JData, KData);
	return true;
}
//��ȡ����ʱ�������¶�ȡ�ڵ�����һ�ڵ��б�洢��xyz����;
int TriData::ProcessDirection(double x,double y,double z,TriDNodeDefine *nodeold)
{
	if(!nodeold) return -1;
	double a,b,c;
	double key=0.00000001;
	a=fabs(x-nodeold->x);
	b=fabs(y-nodeold->y);
	c=fabs(z-nodeold->z);
	if(KeyDirection[0]==0)//ȷ��KeyDirection0���ƶ�����
	{
		if((a>key&&b>key)||(a>key&&c>key)||(c>key&&b>key)) return 0;
		if(a>key) KeyDirection[0]=1;
		else if(b>key) KeyDirection[0]=2;
		else KeyDirection[0]=3;
	}
	if(KeyDirection[1]==0)//ȷ��KeyDirection1��KeyDirection2���ƶ�����
	{
		if(a>key&&b>key&&c>key) return 0;
		if(a>key&&b>key)
		{
			if(KeyDirection[0]==1) {KeyDirection[1]=2;KeyDirection[2]=3;}
			else if(KeyDirection[0]==2) {KeyDirection[1]=1;KeyDirection[2]=3;}
			else return 0;
		}
		if(a>key&&c>key)
		{
			if(KeyDirection[0]==1) {KeyDirection[1]=3;KeyDirection[2]=2;}
			else if(KeyDirection[0]==3) {KeyDirection[1]=1;KeyDirection[2]=2;}
			else return 0;
		}
		if(b>key&&c>key)
		{
			if(KeyDirection[0]==2) {KeyDirection[1]=3;KeyDirection[2]=1;}
			else if(KeyDirection[0]==3) {KeyDirection[1]=2;KeyDirection[2]=1;}
			else return 0;
		}
	}
	//һ���䶯;
	if(a>key&&b<key&&c<key)
	{
		if(KeyDirection[0]!=1) return 0;
		else return 1;
	}
	if(a<key&&b>key&&c<key)
	{
		if(KeyDirection[0]!=2) return 0;
		else return 1;
	}
	if(a<key&&b<key&&c>key)
	{
		if(KeyDirection[0]!=3) return 0;
		else return 1;
	}
	//�����䶯;
	if(a>key&&b>key&&c<key)
	{
		if(KeyDirection[0]==1&&KeyDirection[1]==2) return 2;
		if(KeyDirection[0]==2&&KeyDirection[1]==1) return 2;
		else return 0;
	}
	if(a>key&&b<key&&c>key)
	{
		if(KeyDirection[0]==1&&KeyDirection[1]==3) return 2;
		if(KeyDirection[0]==3&&KeyDirection[1]==1) return 2;
		else return 0;
	}
	if(a<key&&b>key&&c>key)
	{
		if(KeyDirection[0]==2&&KeyDirection[1]==3) return 2;
		if(KeyDirection[0]==3&&KeyDirection[1]==2) return 2;
		else return 0;
	}
	//�����䶯;
	if(a>key&&b>key&&c>key) return 3;
	return 0;
}

int TriData::ProcessDirection(TriDNodeDefine *nodenew,TriDNodeDefine *nodeold)
{
	if(!nodenew||!nodeold) return -1;
	double a,b,c;
	double key=0.00000001;
	a=fabs(nodenew->x-nodeold->x);
	b=fabs(nodenew->y-nodeold->y);
	c=fabs(nodenew->z-nodeold->z);
	if(KeyDirection[0]==0)//ȷ��KeyDirection0���ƶ�����
	{
		if((a>key&&b>key)||(a>key&&c>key)||(c>key&&b>key)) return 0;
		if(a>key) KeyDirection[0]=1;
		else if(b>key) KeyDirection[0]=2;
		else KeyDirection[0]=3;
	}
	if(KeyDirection[1]==0)//ȷ��KeyDirection1��KeyDirection2���ƶ�����
	{
		if(a>key&&b>key&&c>key) return 0;
		if(a>key&&b>key)
		{
			if(KeyDirection[0]==1) {KeyDirection[1]=2;KeyDirection[2]=3;}
			else if(KeyDirection[0]==2) {KeyDirection[1]=1;KeyDirection[2]=3;}
			else return 0;
		}
		if(a>key&&c>key)
		{
			if(KeyDirection[0]==1) {KeyDirection[1]=3;KeyDirection[2]=2;}
			else if(KeyDirection[0]==3) {KeyDirection[1]=1;KeyDirection[2]=2;}
			else return 0;
		}
		if(b>key&&c>key)
		{
			if(KeyDirection[0]==2) {KeyDirection[1]=3;KeyDirection[2]=1;}
			else if(KeyDirection[0]==3) {KeyDirection[1]=2;KeyDirection[2]=1;}
			else return 0;
		}
	}
	//һ���䶯;
	if(a>key&&b<key&&c<key)
	{
		if(KeyDirection[0]!=1) return 0;
		else return 1;
	}
	if(a<key&&b>key&&c<key)
	{
		if(KeyDirection[0]!=2) return 0;
		else return 1;
	}
	if(a<key&&b<key&&c>key)
	{
		if(KeyDirection[0]!=3) return 0;
		else return 1;
	}
	//�����䶯;
	if(a>key&&b>key&&c<key)
	{
		if(KeyDirection[0]==1&&KeyDirection[1]==2) return 2;
		if(KeyDirection[0]==2&&KeyDirection[1]==1) return 2;
		else return 0;
	}
	if(a>key&&b<key&&c>key)
	{
		if(KeyDirection[0]==1&&KeyDirection[1]==3) return 2;
		if(KeyDirection[0]==3&&KeyDirection[1]==1) return 2;
		else return 0;
	}
	if(a<key&&b>key&&c>key)
	{
		if(KeyDirection[0]==2&&KeyDirection[1]==3) return 2;
		if(KeyDirection[0]==3&&KeyDirection[1]==2) return 2;
		else return 0;
	}
	//�����䶯;
	if(a>key&&b>key&&c>key) return 3;
	return 0;
}
//��node1��data�����ݼ����꿽����node2;
bool TriData::CopyDataNode1toNode2(TriDNodeDefine *node1,TriDNodeDefine *node2)
{
	if(!node1||!node2) return false;
	node2->x=node1->x;
	node2->y=node1->y;
	node2->z=node1->z;
	for(int n=0;n<int(node1->data.size());n++)
		node2->data.push_back(node1->data[n]);
	return true;
}
//���ļ�FullName��ȡ���ݣ����ļ���ʽͬtecplot��ȡ��ʽ;
TriDNodeHeadDefine * TriData::ReadData(std::string FullName)
{
    ifstream read(FullName);  //���ļ�;
    if(!read) {
        std::cout << "open sample file failed" << std::endl;
        return nullptr;
    }
    bool Success=true;
    int i;
    int NuLines=0;
    int NuData;
    char buf[LineChar_MaxLength];
    double datatemp[1000];
    vector <std::string> VARIABLESTemp;
    vector <std::string> VARIABLESDataTemp;
    TriDNodeHeadDefine * head=nullptr;
    std::string Name;
    std::string CTemp;
    int NuDataTotal=0;
    int keyDirection;
    double x,y,z;
    TriDNodeDefine *node1,*node2,*node3,*nodeold;
    //nodenew=new TriDNodeDefine;
    KeyDirection[0]=0;KeyDirection[1]=0;KeyDirection[2]=0;
    while(read) {
        for(i=0;i<LineChar_MaxLength;i++) buf[i]='\0';
        read.getline(buf,LineChar_MaxLength);//���ж�ȡ�ļ�;
        CTemp.clear(); CTemp=buf;
        NuLines++;
//        std::cout << NuLines << "::::::::" << CTemp << std::endl;

        if(NuLines==1)
        {
            Name=Process1stLine(buf);
        }
        else if(NuLines==2)
        {
            NuData=Process2ndLine(buf,VARIABLESTemp,VARIABLESDataTemp);
            if(NuData==0) {Success=false;break;}
        }
        else if(NuLines==3) {
            if(!Process3rdLine(buf)) {Success=false;break;}
            if(!TriDataUsingTemp) {Success=false;break;}
            TriDataUsingTemp->Title=Name;
            TriDataUsingTemp->NuDataUsed=NuData-NuIJK;//������������ܸ�����ȥ����IJK�ĸ���;

            for(i=NuIJK;i<(int)VARIABLESTemp.size();i++) {
                TriDataUsingTemp->VARIABLES.push_back(VARIABLESTemp[i]);//��������û�и���;
                TriDataUsingTemp->VARIABLESData.push_back(stof(VARIABLESDataTemp[i]));
            }

            if(IsExistI>=0) TriDataUsingTemp->TitleXYZ[0]=VARIABLESTemp[IsExistI];
            if(IsExistJ>=0) TriDataUsingTemp->TitleXYZ[1]=VARIABLESTemp[IsExistJ];
            if(IsExistK>=0) TriDataUsingTemp->TitleXYZ[2]=VARIABLESTemp[IsExistK];
            VARIABLESTemp.clear();
            VARIABLESDataTemp.clear();
            head=TriDataUsingTemp;
            node1=head->FirstNode;
            node2=node1;node3=node1;
        } else if(CTemp.find("END")<=10000||CTemp.find("end")<=10000||CTemp.find("End")<=10000) {
            break;
        } else {
//            cout << "reeee" << buf << endl;
            NuData=IdentifyData2(buf,datatemp);
            if(TriDataUsingTemp->NuDataUsed!=NuData-NuIJK) {Success=false;break;}
            NuDataTotal++;

            if(IsExistI<0) x=0;
            else x=datatemp[IsExistI];
            if(IsExistJ<0) y=0;
            else y=datatemp[IsExistJ];
            if(IsExistK<0) z=0;
            else z=datatemp[IsExistK];

            if(NuDataTotal>1)
            {
                keyDirection=ProcessDirection(x,y,z,nodeold);
                if(keyDirection<=0) {Success=false;break;}
                if(keyDirection==1) node1=MoveNode(node1,KeyDirection[0]);
                else if(keyDirection==2) {node2=MoveNode(node2,KeyDirection[1]);node1=node2;}
                else {node3=MoveNode(node3,KeyDirection[2]);node1=node3;node2=node3;}
            }
            node1->x=x;
            node1->y=y;
            node1->z=z;
            if(node1->data.size()>0) node1->data.clear();
            for(i=NuIJK;i<NuData;i++)
                node1->data.push_back(datatemp[i]);
            nodeold=node1;
        }
    }
    read.close();
    if(head&&NuDataTotal!=head->NuNodesTotal) Success=false;
    if(!Success)
		{
			DeleteNodes(head);
			return NULL;
		}
		return head;
}
//�������ļ������շ����Լ�IJK��������ȡ
TriDNodeHeadDefine * TriData::ReadData(string FullName,string direction)
{
		ifstream read(FullName);  //���ļ�;
		if(!read) {return NULL;}
		bool Success=true;
		int i;
		int NuLines=0;
		int NuData;
		char buf[LineChar_MaxLength];
		double datatemp[1000];
		vector <std::string> VARIABLESTemp;
		vector <std::string> VARIABLESDataTemp;
		TriDNodeHeadDefine * head=NULL;
    std::string Name;
    std::string CTemp;
		int NuDataTotal=0;
		double x,y,z;
		TriDNodeDefine *node1,*node2,*node3,*nodeold,*nodet;
		//nodenew=new TriDNodeDefine;
		KeyDirection[0]=0;KeyDirection[1]=0;KeyDirection[2]=0;
		while(read)
		{///
			for(i=0;i<LineChar_MaxLength;i++) buf[i]='\0';
			read.getline(buf,LineChar_MaxLength);//���ж�ȡ�ļ�;
			CTemp.clear();CTemp=buf;
			if(NuLines==0)
			{
				Name=Process1stLine(buf);
			}
			else if(NuLines==1)
			{
				NuData=Process2ndLine(buf,VARIABLESTemp,VARIABLESDataTemp);
				if(NuData==0) {Success=false;break;}
			}
			else if(NuLines==2)
			{
				if(!Process3rdLine(buf)) {Success=false;break;}
				if(!TriDataUsingTemp) {Success=false;break;}
				TriDataUsingTemp->Title=Name;
				TriDataUsingTemp->NuDataUsed=NuData-NuIJK;//������������ܸ�����ȥ����IJK�ĸ���;

				for(i=NuIJK;i<VARIABLESTemp.size();i++)
				{
					TriDataUsingTemp->VARIABLES.push_back(VARIABLESTemp[i]);//��������û�и���;
					TriDataUsingTemp->VARIABLESData.push_back(stof(VARIABLESDataTemp[i]));
				}

				if(IsExistI>=0) TriDataUsingTemp->TitleXYZ[0]=VARIABLESTemp[IsExistI];
				if(IsExistJ>=0) TriDataUsingTemp->TitleXYZ[1]=VARIABLESTemp[IsExistJ];
				if(IsExistK>=0) TriDataUsingTemp->TitleXYZ[2]=VARIABLESTemp[IsExistK];
				VARIABLESTemp.clear();
				VARIABLESDataTemp.clear();
				head=TriDataUsingTemp;
				node1=head->FirstNode;
				node2=node1;node3=node1;

				if(direction.find("xyz")>=0||direction.find("XYZ")>=0)
				{
					KeyDirection[0]=1;KeyDirection[1]=2;KeyDirection[2]=3;
				}
				else if(direction.find("yxz")>=0||direction.find("YXZ")>=0)
				{
					KeyDirection[0]=2;KeyDirection[1]=1;KeyDirection[2]=3;
				}
				else if(direction.find("yzx")>=0||direction.find("YZX")>=0)
				{
					KeyDirection[0]=2;KeyDirection[1]=3;KeyDirection[2]=1;
				}
				else if(direction.find("zxy")>=0||direction.find("ZXY")>=0)
				{
					KeyDirection[0]=3;KeyDirection[1]=1;KeyDirection[2]=2;
				}
				else if(direction.find("zyx")>=0||direction.find("ZYX")>=0)
				{
					KeyDirection[0]=3;KeyDirection[1]=2;KeyDirection[2]=1;
				}
			}
			else if(CTemp.find(("END"))>=0||CTemp.find(("end"))>=0||CTemp.find(("End"))>=0) break;
			else
			{

				NuData=IdentifyData2(buf,datatemp);
				if(TriDataUsingTemp->NuDataUsed!=NuData-NuIJK) {Success=false;break;}
				NuDataTotal++;
				if(IsExistI<0) x=0;
				else x=datatemp[IsExistI];
				if(IsExistJ<0) y=0;
				else y=datatemp[IsExistJ];
				if(IsExistK<0) z=0;
				else z=datatemp[IsExistK];
				node1->x=x;
				node1->y=y;
				node1->z=z;
				if(node1->data.size()>0) node1->data.clear();
				for(i=NuIJK;i<NuData;i++)
					node1->data.push_back(datatemp[i]);

				nodet=MoveNode(node1,KeyDirection[0]);
				if(nodet) node1=nodet;
				else
				{
					nodet=MoveNode(node2,KeyDirection[1]);
					if(nodet) {node2=nodet;node1=node2;}
					else
					{
						nodet=MoveNode(node3,KeyDirection[2]);
						if(nodet) {node3=nodet;node1=node3;node2=node3;}
						else break;
					}
				}
				nodeold=node1;
			}
			NuLines++;
		}///
		read.close();

		if(head&&NuDataTotal!=head->NuNodesTotal) Success=false;
		if(!Success)
		{
			DeleteNodes(head);
			return NULL;
		}
		return head;
}

//��VARIABLES�洢���ַ�������ֵ����;
void TriData::ATOF_VARIABLES(TriDNodeHeadDefine *nodehead)
{
	nodehead->VARIABLESData.resize(nodehead->VARIABLES.size());
	for(int i=0;i<(int)nodehead->VARIABLES.size();i++)
		nodehead->VARIABLESData[i]=stof(nodehead->VARIABLES[i]);
}


//������ֵ,��������x����������y��������������ֵx���飬��ֵ�洢�Ŀռ�y����ֵ����;
bool  TriData::SplineInterpolation(double xAxis[], double yAxis[], int NYT, double x[],double y[],int Nyt)
{
	int i,j,k;
	for(i=0;i<Nyt;i++) y[i]=0;
	int MaxNYT=300;
	if(NYT>MaxNYT) return false;
	if(NYT==0) return false;
	double aaa,sig,p,bbb,ccc;
	double y2[300];
	double u[300];

	if(NYT==1)//�������ֻ��һ�������е�ֵ������ͬ;

	for (i = 0; i<Nyt; i++)
		y[i]=yAxis[0];
	y2[0] = 0;
	u[0] = 0;
	for (i = 1; i<NYT-1; i++)
	{
		sig = (xAxis[i] - xAxis[i - 1]) / (xAxis[i + 1] - xAxis[i - 1]);
		p = sig * y2[i - 1] + 2.0;
		y2[i] = (sig - 1.0) / p;
		aaa = (yAxis[i + 1] - yAxis[i]) / (xAxis[i + 1] - xAxis[i]);
		bbb = (yAxis[i] - yAxis[i - 1]) / (xAxis[i] - xAxis[i - 1]);
		ccc = xAxis[i + 1] - xAxis[i - 1];
		u[i] = (6.0 * (aaa - bbb) / ccc - sig * u[i - 1]) / p;
	}
	double qn = 0.0;
	double un = 0.0;
	y2[NYT-1] = 0;
	for (k = NYT - 2;k>=1;k--)
		y2[k] = y2[k] * y2[k + 1] + u[k];

	int klo,khi;
	double h,a,b;
//�����ֵ��xС�ڱ�����С�����꣬ȡy��ֵ�����������;
//�����ֵ��x���ڱ����������꣬ȡy��ֵ�����������;
	for(j=0;j<Nyt;j++)
	{
// 		if(x[j]<=xAxis[0]) y[j]=yAxis[0];
// 		else if(x[j]>=xAxis[NYT-1]) y[j]=yAxis[NYT-1];
// 		else
		{
		klo=0;khi= NYT-1;
		while (khi - klo > 1 )
		{
			k = (khi + klo) / 2;
			if (xAxis[k] > x[j]) 	khi = k;
			else klo = k;
		}
		h = xAxis[khi] - xAxis[klo];
		if (fabs(h) <1e-10 ) return false;
		a = (xAxis[khi] - x[j]) / h;
		b = (x[j] - xAxis[klo]) / h;
		aaa = a * yAxis[klo] + b * yAxis[khi];
		bbb = (a*a*a - a) * y2[klo] + (b*b*b - b) * y2[khi];
		y[j]=aaa + bbb * h*h /6.0;
		}
	}
	return true;
}
//������ֵ,��������x����������y��������������ֵx���飬��ֵ�洢�Ŀռ�y����ֵ�������Ƿ�yֵָ���任;
bool  TriData::SplineInterpolation(double xAxis[], double yAxis[], int NYT, double x[],double y[],int Nyt,bool IsExponent)
{
	double MinData=pow(10.0,-100);
	int i;
	double yAxis2[300];
	bool key;
	if(IsExponent)
	{
		for(i=0;i<NYT;i++)
		{
			if(yAxis[i]<=0) yAxis2[i]=log(MinData);
			else yAxis2[i]=log(yAxis[i]);
		}
		key=SplineInterpolation(xAxis, yAxis2, NYT, x, y, Nyt);
		for(i=0;i<Nyt;i++)
			y[i]=exp(y[i]);
	}
	else key=SplineInterpolation(xAxis, yAxis, NYT, x, y, Nyt);
	return key;
}




//�������ղ�ֵ,��������x����������y��������������ֵx���飬��ֵ�洢�Ŀռ�y����ֵ����;
bool  TriData::LagrangeInterpolation(double xAxis[], double yAxis[], int NYT, double x[],double y[],int Nyt)
{
	int i,j,k;
	for(i=0;i<Nyt;i++) y[i]=0;
	int MaxNYT=100;
	if(NYT>MaxNYT) return false;
	if(NYT==0) return false;
	double data,FS;
	for(i=0;i<Nyt;i++)
	{
		data=0;
		for(j=0;j<NYT;j++)
		{
			FS=yAxis[j];
			for(k=0;k<NYT;k++)
				if(k!=j) FS*=((x[i]-xAxis[k])/(xAxis[j]-xAxis[k]));
			data+=FS;
		}
		y[i]=data;
	}
	return true;
}

//��һ���������У��ҵ�x���ڵ�λ�ã������i��i+1֮�䣬����i�����Ϲ涨����-2������߷���-1,���ұ߷���NYT-1;
int TriData::LocationInLineArray(double xAxis[],int NYT,double x)
{
	int MaxNYT=300;
	if(NYT>MaxNYT||NYT<=1) return -2;
	int i;
	i=int(NYT*0.5)-1;
	if(i<0) i=0;
	if(xAxis[i]<xAxis[i+1])//����;
	{
		while(i>=0&&i<NYT-1)
		{
			if(x>=xAxis[i]&&x<xAxis[i+1]) break;
			else if(x<xAxis[i]) i--;
			else if(x>=xAxis[i+1]) i++;
		}
	}
	else//�ݼ�;
	{
		while(i>=0&&i<NYT-1)
		{
			if(x<=xAxis[i]&&x>xAxis[i+1]) break;
			else if(x>xAxis[i]) i--;
			else if(x<=xAxis[i+1]) i++;
		}
	}
		return i;
}
//���Բ�ֵ,��������x����������y��������������ֵx���飬��ֵ�洢�Ŀռ�y����ֵ����;
bool  TriData::LineInterpolation(double xAxis[], double yAxis[], int NYT, double x[],double y[],int Nyt)
{
	int i,j;
	for(i=0;i<Nyt;i++) y[i]=0;
	if(NYT==0) return false;
	for(j=0;j<Nyt;j++)
	{
		i=LocationInLineArray(xAxis,NYT,x[j]);
		if(i<0) y[j]=yAxis[0];
		else if(i>=NYT-1) y[j]=yAxis[NYT-1];
		else y[j]=(yAxis[i+1]-yAxis[i])*(x[j]-xAxis[i])/(xAxis[i+1]-xAxis[i])+yAxis[i];
	}
	return true;
}


double  TriData::Interpolation(TriDNodeDefine *node0,int direction,int Start,int End,int noData,double xdata)
{
	double *sample=new double [End-Start+1];
	double *x=new double [End-Start+1];
	TriDNodeDefine *node1=node0;
	int i,j;
	for(i=0,j=0;i<=End&&node1;i++)
	{
		if(i>=Start)
		{
			if(direction==1||direction==-1) x[j]=node1->x;
			else if(direction==2||direction==-2) x[j]=node1->y;
			else if(direction==3||direction==-3) x[j]=node1->z;

			sample[j++]=node1->data[noData];
		}
		node1=MoveNode(node1,direction);//����1��2��3�ֱ�Ϊx��y��z���򣬸��ű�ʾ������;
	}
	//������ֵ,��������x����������y��������������ֵx���飬��ֵ�洢�Ŀռ�y����ֵ�������Ƿ�yֵָ���任;
	double x2[1]={xdata};
	double y2[1]={0};
	if(!SplineInterpolation(x, sample, j, x2,y2,1,true)) return 0;
	delete []x;
	delete []sample;
	return y2[0];
}
void TriData::Interpolation(TriDNodeDefine *node0,int direction,int Start,int End,int noData,double xdata[],double ydata[],int Nu)
{
	double *sample=new double [End-Start+1];
	double *x=new double [End-Start+1];
	TriDNodeDefine *node1=node0;
	int i,j;
	for(i=0,j=0;i<=End&&node1;i++)
	{
		if(i>=Start)
		{
			if(direction==1||direction==-1) x[j]=node1->x;
			else if(direction==2||direction==-2) x[j]=node1->y;
			else if(direction==3||direction==-3) x[j]=node1->z;
			sample[j++]=node1->data[noData];
		}
		node1=MoveNode(node1,direction);//����1��2��3�ֱ�Ϊx��y��z���򣬸��ű�ʾ������;
	}
	//������ֵ,��������x����������y��������������ֵx���飬��ֵ�洢�Ŀռ�y����ֵ�������Ƿ�yֵָ���任;
	if(!SplineInterpolation(x, sample, j, xdata,ydata, Nu,true))
	{
		for(i=0,j=0;i<Nu;i++)
			ydata[i]=0;
	}
	delete []x;
	delete []sample;
}


/////////////////////////////
//�����Ȼ��
bool TriData::MLEM_Iteration(TriDNodeHeadDefine *headA,string DirectionA,TriDNodeHeadDefine *headB,string DirectionB,TriDNodeHeadDefine *headX,string DirectionX,double  Resid[])
{
	if(!headA||!headB||!headX) return false;
	if(headA->NuDataUsed==0||headA->NuDataUsed>MaxNuDataInTriDNode) return false;
	if(headA->NuDataUsed!=headB->NuDataUsed||headA->NuDataUsed!=headX->NuDataUsed) return false;
	int directionAI=0,directionAJ=0,directionB=0,directionX=0;
    std::string CTemp;
	if(DirectionA.find(("XY"))>=0||DirectionA.find(("xy"))>=0||DirectionA.find(("Xy"))>=0||DirectionA.find(("xY"))>=0) {directionAI=1;directionAJ=2;}
	else if(DirectionA.find(("YX"))>=0||DirectionA.find(("yx"))>=0||DirectionA.find(("Yx"))>=0||DirectionA.find(("yX"))>=0) {directionAI=2;directionAJ=1;}
	else if(DirectionA.find(("XZ"))>=0||DirectionA.find(("xz"))>=0||DirectionA.find(("Xz"))>=0||DirectionA.find(("xZ"))>=0) {directionAI=1;directionAJ=3;}
	else if(DirectionA.find(("ZX"))>=0||DirectionA.find(("zx"))>=0||DirectionA.find(("Zx"))>=0||DirectionA.find(("zX"))>=0) {directionAI=3;directionAJ=1;}
	else if(DirectionA.find(("YZ"))>=0||DirectionA.find(("yz"))>=0||DirectionA.find(("Yz"))>=0||DirectionA.find(("yZ"))>=0) {directionAI=2;directionAJ=3;}
	else if(DirectionA.find(("ZY"))>=0||DirectionA.find(("zy"))>=0||DirectionA.find(("Zy"))>=0||DirectionA.find(("zY"))>=0)	{directionAI=3;directionAJ=2;}
	else return false;
	if(DirectionB.find(("X"))>=0||DirectionB.find(("x"))>=0) directionB=1;
	else if(DirectionB.find(("Y"))>=0||DirectionB.find(("y"))>=0) directionB=2;
	else if(DirectionB.find(("Z"))>=0||DirectionB.find(("z"))>=0) directionB=3;
	else return false;
	if(DirectionX.find("X")>=0||DirectionX.find(("x"))>=0) directionX=1;
	else if(DirectionX.find(("Y"))>=0||DirectionX.find(("y"))>=0) directionX=2;
	else if(DirectionX.find(("Z"))>=0||DirectionX.find(("z"))>=0) directionX=3;
	else return false;
//CTemp.Format(_T("%d %d"),directionAI,directionAJ);::MessageBox(NULL,CTemp,_T("Test"),MB_ICONWARNING | MB_OK);}

	TriDNodeDefine *nodeATemp,*nodeATemp2,*nodeALine;
	TriDNodeDefine *nodeBTemp;
	TriDNodeDefine *nodeXTemp;
	double Cancha[MaxNuDataInTriDNode],Neiji,Length[MaxNuDataInTriDNode];
	int noData;
	nodeATemp=headA->FirstNode;
	nodeXTemp=headX->FirstNode;
	while(nodeATemp&&nodeXTemp)
	{//
		nodeALine=headA->FirstNode;
		nodeATemp2=nodeATemp;
		nodeBTemp=headB->FirstNode;
		for(noData=0;noData<headA->NuDataUsed&&noData<MaxNuDataInTriDNode;noData++)
		{Cancha[noData]=0;Length[noData]=0;}
		while(nodeATemp2&&nodeALine&&nodeBTemp)
		{//
			for(noData=0;noData<headA->NuDataUsed&&noData<MaxNuDataInTriDNode;noData++)
			{
				Neiji=ABVectorsProduct(nodeALine,directionAJ,headX->FirstNode,directionX,noData);
				if(Neiji>0.0000000001) Cancha[noData]+=nodeATemp2->data[noData]*nodeBTemp->data[noData]/Neiji;
				Length[noData]+=nodeATemp2->data[noData];
			}
			nodeALine=MoveNode(nodeALine,directionAI);//��������;
			nodeATemp2=MoveNode(nodeATemp2,directionAI);//��������;
			nodeBTemp=MoveNode(nodeBTemp,directionB);//��������;
		}//
		for(noData=0;noData<headA->NuDataUsed&&noData<MaxNuDataInTriDNode;noData++)
		{
			if(fabs(Length[noData])>0.0000000001) nodeXTemp->data[noData]*=Cancha[noData]/Length[noData];
		}
		nodeXTemp=MoveNode(nodeXTemp,directionX);
		nodeATemp=MoveNode(nodeATemp,directionAJ);
	}//
	//��в�;
	ResidualOfABX(headA,directionAI,directionAJ,headB,directionB,headX,directionX,Resid);
	return true;
}
//����A��B���ڻ�;
double TriData::ABVectorsProduct(TriDNodeDefine *nodeA,int directionA,TriDNodeDefine *nodeB,int directionB,int NoData)
{
	TriDNodeDefine *nodeATemp=nodeA;
	TriDNodeDefine *nodeBTemp=nodeB;
	double neiji=0;
	if(NoData<0||NoData>=MaxNuDataInTriDNode) return -1e-10;
	while(nodeATemp&&nodeBTemp)
	{
		neiji+=nodeATemp->data[NoData]*nodeBTemp->data[NoData];
		nodeATemp=MoveNode(nodeATemp,directionA);
		nodeBTemp=MoveNode(nodeBTemp,directionB);
	}
	return neiji;
}

//����A��ƽ����ƽ��;
double TriData::VectorsMeanSquare(TriDNodeDefine *nodeA,int directionA,int NoData)
{
	TriDNodeDefine *nodeATemp=nodeA;
	double neiji=0;
	int n=0;
	if(NoData<0||NoData>=MaxNuDataInTriDNode) return -1e-10;
	while(nodeATemp)
	{
		neiji+=nodeATemp->data[NoData]*nodeATemp->data[NoData];
		nodeATemp=MoveNode(nodeATemp,directionA);
		n++;
	}
	if(n>0) return neiji/n;
	else return -1e-10;
}

//�󷽳̲в�,RMS(B-AX),ϵ������A��ͷ�ڵ㣬�����ұ�Bͷ�ڵ㣬δ֪��Xͷ�ڵ㣬����в�ĵ�NoData������;
bool TriData::ResidualOfABX(TriDNodeHeadDefine *headA,int directionAI,int directionAJ,TriDNodeHeadDefine *headB,int directionB,TriDNodeHeadDefine *headX,int directionX,double Resid[])
{
	if(!headA||!headB||!headX) return false;
	TriDNodeDefine *nodeATemp;
	TriDNodeDefine *nodeBTemp;
	double cancha,neiji;
	int NLine;
	int noData;
	for(noData=0;noData<headA->NuDataUsed&&noData<MaxNuDataInTriDNode;noData++)
	{
		nodeATemp=headA->FirstNode;
		nodeBTemp=headB->FirstNode;
		NLine=0;
		cancha=0;
	while(nodeATemp&&nodeBTemp)
	{
		neiji=ABVectorsProduct(nodeATemp,directionAJ,headX->FirstNode,directionX,noData);
		cancha+=pow(nodeBTemp->data[noData]-neiji,2.0);
		nodeATemp=MoveNode(nodeATemp,directionAI);
		nodeBTemp=MoveNode(nodeBTemp,directionB);
		NLine++;
	}
		if(NLine>0) Resid[noData]=sqrt(cancha/NLine);
		else Resid[noData]=-999999;
	}
	return true;
}

//X����ֵ,X=AT*B,ϵ������A����������ƽ�棨�硰XY��ָXΪ����YΪ���򣩣��������B������δ֪�����ݿռ�X������;
bool TriData::InitializationX(TriDNodeHeadDefine *headA,std::string DirectionA,TriDNodeHeadDefine *headB,std::string DirectionB,TriDNodeHeadDefine *headX,string DirectionX)
{
	if(!headA||!headB||!headX) return false;
	if(headA->NuDataUsed==0||headA->NuDataUsed>MaxNuDataInTriDNode) return false;
	if(headA->NuDataUsed!=headB->NuDataUsed||headA->NuDataUsed!=headX->NuDataUsed) return false;
	int directionAI,directionAJ,directionB,directionX;
	if(DirectionA.find("XY")>=0||DirectionA.find(("xy"))>=0||DirectionA.find(("Xy"))>=0||DirectionA.find(("xY"))>=0) {directionAI=1;directionAJ=2;}
	else if(DirectionA.find(("YX"))>=0||DirectionA.find(("yx"))>=0||DirectionA.find(("Yx"))>=0||DirectionA.find(("yX"))>=0) {directionAI=2;directionAJ=1;}
	else if(DirectionA.find(("XZ"))>=0||DirectionA.find(("xz"))>=0||DirectionA.find(("Xz"))>=0||DirectionA.find(("xZ"))>=0) {directionAI=1;directionAJ=3;}
	else if(DirectionA.find(("ZX"))>=0||DirectionA.find(("zx"))>=0||DirectionA.find(("Zx"))>=0||DirectionA.find(("zX"))>=0) {directionAI=3;directionAJ=1;}
	else if(DirectionA.find(("YZ"))>=0||DirectionA.find(("yz"))>=0||DirectionA.find(("Yz"))>=0||DirectionA.find(("yZ"))>=0) {directionAI=2;directionAJ=3;}
	else if(DirectionA.find(("ZY"))>=0||DirectionA.find(("zy"))>=0||DirectionA.find(("Zy"))>=0||DirectionA.find(("zY"))>=0) {directionAI=3;directionAJ=2;}
	else return false;
	if(DirectionB.find(("X"))>=0||DirectionB.find(("x"))>=0) directionB=1;
	else if(DirectionB.find(("Y"))>=0||DirectionB.find(("y"))>=0) directionB=2;
	else if(DirectionB.find(("Z"))>=0||DirectionB.find(("z"))>=0) directionB=3;
	else return false;
	if(DirectionX.find(("X"))>=0||DirectionX.find(("x"))>=0) directionX=1;
	else if(DirectionX.find(("Y"))>=0||DirectionX.find(("y"))>=0) directionX=2;
	else if(DirectionX.find(("Z"))>=0||DirectionX.find(("z"))>=0) directionX=3;
	else return false;

	TriDNodeDefine *nodeATemp,*nodeATemp2,*nodeALine;
	TriDNodeDefine *nodeBTemp;
	TriDNodeDefine *nodeXTemp;
	double Length,lineMS;
	int noData;
	for(noData=0;noData<headA->NuDataUsed&&noData<MaxNuDataInTriDNode;noData++)
	{
	nodeATemp=headA->FirstNode;
	nodeXTemp=headX->FirstNode;
	while(nodeATemp)
	{
	nodeBTemp=headB->FirstNode;
	nodeALine=headA->FirstNode;
	nodeATemp2=nodeATemp;
	Length=0;
	nodeXTemp->data[noData]=0;
	while(nodeATemp2)
	{
		lineMS=VectorsMeanSquare(nodeALine,directionAJ,noData);
		nodeXTemp->data[noData]+=pow(nodeATemp2->data[noData],2.0)*nodeBTemp->data[noData]/lineMS;
		Length+=nodeATemp2->data[noData];
		nodeALine=MoveNode(nodeALine,directionAI);//��������;
		nodeATemp2=MoveNode(nodeATemp2,directionAI);//��������;
		nodeBTemp=MoveNode(nodeBTemp,directionB);//��������;
	}
	if(fabs(Length)>0.000001) nodeXTemp->data[noData]/=Length;
	nodeXTemp=MoveNode(nodeXTemp,directionX);
	nodeATemp=MoveNode(nodeATemp,directionAJ);
	}
	}
	return true;
}