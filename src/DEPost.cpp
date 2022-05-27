#include "DEPost.h"
#include <string>
#define NuDensity 16



DEPost::DEPost(void)
{
	EHead= nullptr;
}

DEPost::~DEPost(void)
{

}
////////////////////////////////*****此处需要设置
bool DEPost::Process()
{
/*
int key=0;//0 200L，1 400L
if(key==0)
{
	_PWaste.SetPoint(0,0,0);
	_WD1.Cylinder(_PWaste,'z',28,2000);//内径
	_WD2.Cylinder(_PWaste,'z',28.15,2000);//内径

}
else
{
	_PWaste.SetPoint(0,0,0);
	_WD1.Cylinder(_PWaste,'z',35,2000);//内径
	_WD2.Cylinder(_PWaste,'z',35.2,2000);//内径
}
EProcess2(key);
*/
return true;
}
//计算几何效率,秦山10月
//void  DEPost::EProcess2(int key)
//{
//
//	double WD_x0=_PGe.x;
//	double WD_y0=_PGe.y;
//	double WD_z0=_PGe.z;
//	_PB.SetPoint(_PGe);
//	int NoY;
//	if(WD_y0>=0) NoY=(int)(WD_y0/7+0.5);
//	else NoY=(int)(-WD_y0/7+0.5);
//
//	//读文件
//	std::string FileName;
//	if(key==0) FileName.Format("EWD200_Points_%d.dat",int(WD_y0/7+0.5));
//	else       FileName.Format(_T("EWD400_Points_%d.dat"),int(WD_y0/7+0.5));
//	TriDNodeHeadDefine *EHead=TriDataB.ReadData(m_Folder+FileName);
//	if(!EHead)
//	{
//		AfxMessageBox(_T("未能读取E_Points数据"));
//		return;
//	}
//	///////////////设置
//	double densitydata[NuDensity]={0,0.25,0.5,0.75,1.0,1.25,1.5,2.0,2.5,2.7,4.0,5.0,6.0,8.0,10.0,12.0};
//
//	std::string CTemp;
//	double R_WD;
//	double H_WD;
//	int NZ;
//
//	int NR;
//	double dH;
//	double dR;
//
//	int key200_400=key;//0 200L 1:400L
//	double R_Y[10];
//	if(key200_400==0)
//	{
//		R_WD=28;
//		H_WD=90;
//		NZ=6;
//		NR=4;
//	}
//	else
//	{
//		R_WD=35;
//		H_WD=100;
//		NZ=10;
//		NR=4;
//	}
//	dH=H_WD/NZ;
//	dR=R_WD/NR;
//	for(int i=0;i<=NR;i++)
//	    R_Y[i]=i*dR;
//
//	std::string Title;
//	int i,j,k,e,nd;
//	double u,uFe,L1,L2;
//	double Sum[200];
//	//圆周平均
//	TriDNodeHeadDefine *EHead1=TriDataB.CreatNodes(NuDensity,EHead->NuNodesInY,EHead->NuNodesInZ,EHead);
//	EHead1->TitleXYZ[0]=_T("Density");
//	EHead1->TitleXYZ[1]=_T("R");
//	EHead1->TitleXYZ[2]=_T("H");
//	double angle1,angle2,dangle,sumangle;
//	TriDNodeDefine *node12,*node13,*node11=EHead->FirstNode;
//	TriDNodeDefine *node22,*node23,*node21=EHead1->FirstNode;
//	for(i=0;i<EHead->NuNodesInZ;i++)
//	{
//		node12=node11;
//		node22=node21;
//		for(j=0;j<EHead->NuNodesInY;j++)
//		{//j
//			node23=node22;
//			for(nd=0;nd<NuDensity;nd++)//不同密度
//			{//nd
//				node13=node12;
//				sumangle=0;
//				for(k=0;k<EHead->NuNodesInX-1;k++)//圆周方向
//				{//k
//					_PA.SetPoint(   WD_x0+node13->y*cos(node13->x*PI/180),
//									WD_y0+node13->y*sin(node13->x*PI/180),
//									WD_z0+node13->z
//									);
//				 L1=CellB.L_Cell(_PA,_PB,_WD1);
//				 L2=CellB.L_Cell(_PA,_PB,_WD2);
//
//				   dangle=node13->NextX->x-node13->x;
//				   sumangle+=dangle;
//					for(e=0;e<EHead->NuDataUsed;e++)
//					{//e
//						//1 真空 2water 3Polyethylene 4Concrete 5Glass 6Al 7Ge 8Fe 9Cu 10Pb 其他水
//						if(nd==0)u=CellB.Attenuation(1,densitydata[nd],EHead->VARIABLESData[e]);
//						else if(nd<5)u=CellB.Attenuation(2,densitydata[nd],EHead->VARIABLESData[e]);
//						else if(nd<9)u=CellB.Attenuation(4,densitydata[nd],EHead->VARIABLESData[e]);
//						else if(nd<11)u=CellB.Attenuation(6,densitydata[nd],EHead->VARIABLESData[e]);
//						else if(nd<14)u=CellB.Attenuation(8,densitydata[nd],EHead->VARIABLESData[e]);
//						else if(nd<16)u=CellB.Attenuation(10,densitydata[nd],EHead->VARIABLESData[e]);
//
//						uFe=CellB.Attenuation(8,densitydata[nd],EHead->VARIABLESData[e]);
//
//						if(k==0)Sum[e]=0;
//						Sum[e]+=node13->data[e]*exp(-u*L1-uFe*(L2-L1))*dangle;
//
//					}//e
//					node13=node13->NextX;
//				}//k
//				for(e=0;e<EHead->NuDataUsed;e++)
//					node23->data[e]=Sum[e]/sumangle;
//
//				node23->x=densitydata[nd];
//				node23->y=node12->y;
//				node23->z=node12->z;
//				node23=node23->NextX;
//			}//nd
//			node12=node12->NextY;
//			node22=node22->NextY;
//		}//j
//		node11=node11->NextZ;
//		node21=node21->NextZ;
//	}//i
//	if(key200_400==0)
//	{
//		Title.Format("EWD200_R_H_%d.dat",NoY);
//		TriDataB.outputdata_3d(EHead1,m_SavingFolder+Title,_T("xyz"));
//	}
//	else
//	{
//		Title.Format("EWD400_R_H_%d.dat",NoY);
//		TriDataB.outputdata_3d(EHead1,m_SavingFolder+Title,_T("xyz"));
//	}
//
//	//插值
//	double ydata_R[500];
//	double xdata_R[500];
//	int NR_Chazhi;
//	if(key200_400==0) NR_Chazhi=56;
//	else NR_Chazhi=70;
//	for(i=0;i<=NR_Chazhi;i++)
//      xdata_R[i]=i*R_WD/NR_Chazhi;
//
//	//R方向插值
//	TriDNodeHeadDefine *EHead_CZ1=TriDataB.CreatNodes(NuDensity,NR_Chazhi+1,EHead1->NuNodesInZ,EHead1);
//	node11=EHead1->FirstNode;
//	node21=EHead_CZ1->FirstNode;
//	for(nd=0;nd<NuDensity;nd++)
//	{
//		node12=node11;
//		node22=node21;
//		for(i=0;i<EHead1->NuNodesInZ;i++)
//		{//
//			for(e=0;e<EHead1->NuDataUsed;e++)
//			{
//				  TriDataB.Interpolation(node12,2,0,999,e,xdata_R,ydata_R,NR_Chazhi+1);//插值
//				  node23=node22;
//				  for(j=0;j<EHead_CZ1->NuNodesInY;j++)
//				  {
//					 if(e==0) node23->x=node12->x;
//					 if(e==0) node23->y=xdata_R[j];
//					 if(e==0) node23->z=node12->z;
//					 node23->data[e]=ydata_R[j];
//					 node23=node23->NextY;
//				  }
//			}
//			node12=node12->NextZ;
//			node22=node22->NextZ;
//		}//
//		node11=node11->NextX;
//		node21=node21->NextX;
//	}
//	//H方向插值
//	double ydata_H[500];
//	double xdata_H[500];
//	int NH_Chazhi=100;
//	for(i=0;i<=NH_Chazhi;i++)
//      xdata_H[i]=i*50.0/NH_Chazhi;
//	double dh_Chazhi=50.0/NH_Chazhi;
//	TriDNodeHeadDefine *EHead_CZ2=TriDataB.CreatNodes(NuDensity,NR_Chazhi+1,NH_Chazhi+1,EHead1);
//	node11=EHead_CZ1->FirstNode;
//	node21=EHead_CZ2->FirstNode;
//	for(nd=0;nd<NuDensity;nd++)
//	{
//		node12=node11;
//		node22=node21;
//		for(i=0;i<EHead_CZ1->NuNodesInY;i++)
//		{//
//			for(e=0;e<EHead_CZ1->NuDataUsed;e++)
//			{
//				  TriDataB.Interpolation(node12,3,0,999,e,xdata_H,ydata_H,NH_Chazhi+1);//插值
//				  node23=node22;
//				  for(j=0;j<EHead_CZ2->NuNodesInZ;j++)
//				  {
//					 if(e==0) node23->x=node12->x;
//					 if(e==0) node23->y=node12->y;
//					 if(e==0) node23->z=xdata_H[j];
//					 node23->data[e]=ydata_H[j];
//					 node23=node23->NextZ;
//				  }
//			}
//			node12=node12->NextY;
//			node22=node22->NextY;
//		}//
//		node11=node11->NextX;
//		node21=node21->NextX;
//	}
//	if(key200_400==0)
//	{
//		Title.Format("EWD200_RH_CZ_%d.dat",NoY);
//		TriDataB.outputdata_3d(EHead_CZ2,m_SavingFolder+Title,_T("xyz"));
//	}
//	else
//	{
//		Title.Format("EWD400_RH_CZ_%d.dat",NoY);
//		TriDataB.outputdata_3d(EHead_CZ2,m_SavingFolder+Title,_T("xyz"));
//	}
//
//	//半径方向，分四个环
//	TriDNodeHeadDefine *EHead2=TriDataB.CreatNodes(NuDensity,NR,EHead_CZ2->NuNodesInZ,EHead_CZ2);
//	EHead2->TitleXYZ[0]="Density";
//	EHead2->TitleXYZ[1]="R";
//	EHead2->TitleXYZ[2]="H";
//	node11=EHead_CZ2->FirstNode;
//	node21=EHead2->FirstNode;
//	double sum_E_R[200];
//	double sumR;
//	double y_middle;
//	double E_middle;
//	for(nd=0;nd<NuDensity;nd++)
//	{////
//		node12=node11;
//		node22=node21;
//		for(i=0;i<EHead_CZ2->NuNodesInZ;i++)
//		{///
//			node23=node22;
//            for(e=0;e<EHead2->NuDataUsed;e++)
//                 sum_E_R[e]=0;
//			sumR=0;
//		    node13=node12;
//			j=0;
//			for(k=0;k<EHead_CZ2->NuNodesInY-1;k++)
//			{//
//				y_middle=(node13->y+node13->NextY->y)*(node13->NextY->y-node13->y);
//				//CTemp.Format("k=%d j=%d",k,j);
//				//AfxMessageBox(CTemp);
//				if(y_middle<R_Y[j+1]&&y_middle>=R_Y[j])//如果在圆环范围内
//				{
//					sumR+=y_middle;//权重求和
//					for(e=0;e<EHead2->NuDataUsed;e++)
//					{
//						E_middle=(node13->data[e]+node13->NextY->data[e])/2;
//						sum_E_R[e]+=E_middle*y_middle;//效率对半径加权求和
//					}
//				}
//				if(y_middle>=R_Y[j+1]||node13->NextY->NextY==NULL)
//				{
//					for(e=0;e<EHead2->NuDataUsed;e++)
//					{
//						node23->data[e]= sum_E_R[e]/sumR;
//						E_middle=(node13->data[e]+node13->NextY->data[e])/2;
//						sum_E_R[e]=E_middle*y_middle;
//					}
//				//CTemp.Format("j=%d E=%e",k,node23->data[9]);
//				//AfxMessageBox(CTemp);
//                    sumR=y_middle;
//					node23->x=node13->x;
//					node23->y=j;
//					node23->z=node13->z;
//					node23=node23->NextY;
//					j++;
//				}
//				node13=node13->NextY;
//			}//
//			node12=node12->NextZ;
//			node22=node22->NextZ;
//		}///
//		node11=node11->NextX;
//		node21=node21->NextX;
//	}////
//	if(key200_400==0)
//	{
//		Title.Format("EWD200_Ring_H_%d.dat",NoY);
//		TriDataB.outputdata_3d(EHead2,m_SavingFolder+Title,_T("xyz"));
//	}
//	else
//	{
//		Title.Format("EWD400_Ring_H_%d.dat",NoY);
//		TriDataB.outputdata_3d(EHead2,m_SavingFolder+Title,_T("xyz"));
//	}
//
//	//在竖直方向按照层高，分块
//	TriDNodeHeadDefine *EHead3=TriDataB.CreatNodes(NuDensity,NR,NZ,EHead2);
//	EHead3->TitleXYZ[0]="Density";
//	EHead3->TitleXYZ[1]="NoR";
//	EHead3->TitleXYZ[2]="NoZ";
//	node21=EHead3->FirstNode;
//	node11=EHead2->FirstNode;
//	double middle,data1,data2;
//	double dh;
//	for(nd=0;nd<NuDensity;nd++)
//	{
//		node22=node21;
//		node12=node11;
//
//		for(i=0;i<EHead2->NuNodesInY;i++)
//		{
//			node23=node22;
//			//第1，N层
//			for(e=0;e<EHead2->NuDataUsed;e++)
//			   Sum[e]=0;
//			sumR=0;
//			j=0;
//			node13=node12;
//			for(k=0;k<EHead2->NuNodesInZ-1;k++)
//			{//
//				dh=node13->NextZ->z-node13->z;
//				sumR+=dh;
//				  for(e=0;e<EHead2->NuDataUsed;e++)
//				 {
//					middle=0.5*(node13->data[e]+node13->NextZ->data[e]);
//					Sum[e]+=middle*dh;
//				  }
//				  if(node13->NextZ->NextZ==NULL||(node13->NextZ->z>=dH*(j+0.5)))
//				  {
//			  			for(e=0;e<EHead2->NuDataUsed;e++)
//						{
//							node23->data[e]=Sum[e]/sumR;
//							Sum[e]=0;
//						}
//						sumR=0;
//						node23->x=node12->x;
//						node23->y=node12->y;
//						node23->z=j;
//						node23=node23->NextZ;
//						j++;
//				  }
//				if(j==NZ) break;
//				node13=node13->NextZ;
//			}//
//		node12=node12->NextY;
//		node22=node22->NextY;
//		}
//		node11=node11->NextX;
//		node21=node21->NextX;
//	}
//	if(key200_400==0)
//	{
//		Title.Format("EWD200_Ring_DH_%d.dat",NoY);
//		TriDataB.outputdata_3d(EHead3,m_SavingFolder+Title,_T("xyz"));
//	}
//	else
//	{
//		Title.Format("EWD400_Ring_DH_%d.dat",NoY);
//		TriDataB.outputdata_3d(EHead3,m_SavingFolder+Title,_T("xyz"));
//	}
//
//	//STGS
//	TriDNodeHeadDefine *EHead4=TriDataB.CreatNodes(NuDensity,NR,NZ,EHead3);
//	EHead4->TitleXYZ[0]="Density";
//	EHead4->TitleXYZ[1]="NoR";
//	EHead4->TitleXYZ[2]="NoZ";
//
//	node21=EHead4->FirstNode;
//	node11=EHead3->FirstNode;
//	for(nd=0;nd<NuDensity;nd++)
//	{
//		node22=node21;
//		node12=node11;
//		for(k=0;k<EHead3->NuNodesInY;k++)
//		{
//			node23=node22;
//			//第1，N层
//			for(j=0;j<NZ;j++)
//			{
//				//上半层
//				for(e=0;e<EHead3->NuDataUsed;e++)
//				 Sum[e]=0;
//				node13=node12;
//				for(i=0;i<EHead3->NuNodesInZ&&i<NZ-j;i++)
//				{
//					for(e=0;e<EHead3->NuDataUsed;e++)
//						Sum[e]+=node13->data[e];
//					node13=node13->NextZ;
//				}
//				//下半层
//				node13=node12->NextZ;
//				for(i=1;i<EHead3->NuNodesInZ&&i<=j;i++)
//				{
//					for(e=0;e<EHead3->NuDataUsed;e++)
//						Sum[e]+=node13->data[e];
//					node13=node13->NextZ;
//				}
//				for(e=0;e<EHead3->NuDataUsed;e++)
//					node23->data[e]=(Sum[e]/NZ);
//				node23->x=node12->x;
//				node23->y=node12->y;
//				node23->z=j;
//				node23=node23->NextZ;
//			}
//		node12=node12->NextY;
//		node22=node22->NextY;
//		}
//		node11=node11->NextX;
//		node21=node21->NextX;
//	}
//	//输出
//	if(key200_400==0)
//	{
//		Title.Format("EWD200_STGS_%d.dat",NoY);
//		TriDataB.outputdata_3d(EHead4,m_SavingFolder+Title,_T("xyz"));
//	}
//	else
//	{
//		Title.Format("EWD400_STGS_%d.dat",NoY);
//		TriDataB.outputdata_3d(EHead4,m_SavingFolder+Title,_T("xyz"));
//	}
//
//   //把STGS变成SGS，各层加权平均
//	TriDNodeHeadDefine *EHead5=TriDataB.CreatNodes(NuDensity,1,NZ,EHead4);
//	EHead5->TitleXYZ[0]="Density";
//	EHead5->TitleXYZ[1]="None";
//	EHead5->TitleXYZ[2]="NoZ";
//	node11=EHead4->FirstNode;//注意是EHead2
//	node21=EHead5->FirstNode;
//	for(nd=0;nd<NuDensity;nd++)
//	{
//		node12=node11;
//		node22=node21;
//		for(i=0;i<EHead4->NuNodesInZ;i++)
//		{
//			node13=node12;
//			for(e=0;e<EHead4->NuDataUsed;e++)//初值
//				Sum[e]=0;
//			for(k=0;k<EHead4->NuNodesInY;k++)//半径方向
//			{
//				for(e=0;e<EHead4->NuDataUsed;e++)
//					Sum[e]+=node13->data[e]*(R_Y[k+1]*R_Y[k+1]-R_Y[k]*R_Y[k]);
//				node13=node13->NextY;
//			}
//			for(e=0;e<EHead4->NuDataUsed;e++)
//				node22->data[e]=Sum[e]/(R_WD*R_WD);
//			node22->x=node12->x;
//			node22->z=node12->z;
//			node22->y=0;
//		node12=node12->NextZ;
//		node22=node22->NextZ;
//		}
//		node11=node11->NextX;
//		node21=node21->NextX;
//	}
//	//输出
//	//输出
//	if(key200_400==0)
//	{
//		Title.Format("EWD200_SGS_%d.dat",NoY);
//		TriDataB.outputdata_3d(EHead5,m_SavingFolder+Title,_T("xyz"));
//	}
//	else
//	{
//		Title.Format("EWD400_SGS_%d.dat",NoY);
//		TriDataB.outputdata_3d(EHead5,m_SavingFolder+Title,_T("xyz"));
//	}
//
//   //HGS
//	TriDNodeHeadDefine *EHead6=TriDataB.CreatNodes(NuDensity,EHead_CZ2->NuNodesInY,1,EHead_CZ2);
//	EHead6->TitleXYZ[0]="Density";
//	EHead6->TitleXYZ[1]="r";
//	EHead6->TitleXYZ[2]="none";
//	node11=EHead_CZ2->FirstNode;
//	node21=EHead6->FirstNode;
//	double E_h[500];
//	int NuH=int(H_WD/dh_Chazhi+0.5);
//	double sumE;
//	for(nd=0;nd<NuDensity;nd++)
//	{
//		node12=node11;
//		node22=node21;
//		for(k=0;k<EHead6->NuNodesInY;k++)//半径方向
//		{
//			for(e=0;e<EHead6->NuDataUsed;e++)
//			{//e
//				node13=node12;
//				//按照插值的位数，把效率拷贝在数组里
//				//假定在每个插值高度做一个测量，其效率是相对效率求和除以总共的数量（从0到H高度）
//				//螺旋扫描是这些插值高度的效率求平均值
//				for(j=0;j<EHead_CZ2->NuNodesInZ;j++,node13=node13->NextZ)//高度方向
//					E_h[j]=node13->data[e];
//				for(j=EHead_CZ2->NuNodesInZ;j<500;j++)
//					E_h[j]=0;//把值补充完
//				sumE=0;
//				for(i=0;i<NuH;i++)
//				{
//					for(j=0;j<NuH-i;j++)
//                      sumE+=E_h[j];
//					for(j=1;j<=i;j++)
//                      sumE+=E_h[j];
//				}
//				node22->data[e]=sumE/(NuH*NuH);
//			}//e
//			node22->x=node12->x;
//			node22->y=node12->y;
//			node22->z=0;
//			node12=node12->NextY;
//			node22=node22->NextY;
//		}
//	node11=node11->NextX;
//	node21=node21->NextX;
//	}
//		if(key200_400==0)
//		{
//			Title.Format("EWD200_HGS_%d.dat",NoY);
//			TriDataB.outputdata_3d(EHead6,m_SavingFolder+Title,_T("xyz"));
//		}
//		else
//		{
//			Title.Format("EWD400_HGS_%d.dat",NoY);
//			TriDataB.outputdata_3d(EHead6,m_SavingFolder+Title,_T("xyz"));
//		}
//
//	TriDataB.DeleteNodes(EHead);
//	TriDataB.DeleteNodes(EHead1);
//	TriDataB.DeleteNodes(EHead_CZ1);
//	TriDataB.DeleteNodes(EHead_CZ2);
//	TriDataB.DeleteNodes(EHead2);
//	TriDataB.DeleteNodes(EHead3);
//	TriDataB.DeleteNodes(EHead4);
//	TriDataB.DeleteNodes(EHead5);
//	TriDataB.DeleteNodes(EHead6);
//	//TriDataB.DeleteNodes(EnergySample);
//	if(key200_400==0)  Title.Format("刻度结束：Waste Drum 200_%d",NoY);
//	else Title.Format("刻度结束：Waste Drum 400_%d",NoY);
//	AfxMessageBox(Title);
//}



