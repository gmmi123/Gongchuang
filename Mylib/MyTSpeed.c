#include "MyTSpeed.h"
MyTSpeed_Typedef TSpeedX={.AccMax=5,.Vm=20};
MyTSpeed_Typedef TSpeedY={.AccMax=5,.Vm=20};;

#define AccMax	object->AccMax
#define dt		object->dt
#define V		object->V//当前速度
#define Vm		object->Vm//最大速度
#define St		object->St//剩余路程
#define Sc		object->Sc//以当前最大减速度减速时的路程
#define AccOut	object->AccOut//输出加速度
#define VOut    object->VOut//输出速度

void MyTSpeed_GetDateX(MyTSpeed_Typedef* object)
{
	V = Car.Date.Vx;
	St = Car.Date.DistanceControl.ResidueDistanceX;
}

void MyTSpeed_GetDateY(MyTSpeed_Typedef* object)
{
	V = Car.Date.Vy;
	St = Car.Date.DistanceControl.ResidueDistanceY;
}

void MyTSpeed_OutPutAcc(MyTSpeed_Typedef* object)
{
	//保证Acc为正值
	AccMax = fabsf(AccMax);
	//计算Sc
	Sc = V*V/(2*AccMax);
	//由剩余距离St决定加速度方向
	if(St>=0)//路程为正时
	{
		if(Sc>=St)
		{
			AccOut = -V*V/(2*St);//减速度_
		}
		else 
		{
			if(Vm>=V/sqrtf(2))//可以以最大加速度到最大速度
			{
				AccOut = AccMax;//+
			}
			else
			{
				AccOut = 0;
			}
		}
	}
	else
	{
		
		if(Sc>=fabsf(St))
		{
			AccOut = -V*V/(2*St);//减速度_
		}
		else 
		{
			if(Vm>=-V/sqrtf(2))//可以以最大加速度到最大速度
			{
				AccOut = -AccMax;//
			}
			else
			{
				AccOut = 0;
			}
		}
	}
}

void MyTSpeed_Task(void)
{
	MyTSpeed_GetDateX(&TSpeedX);
	MyTSpeed_GetDateY(&TSpeedY);
	
	MyTSpeed_OutPutAcc(&TSpeedX);
	MyTSpeed_OutPutAcc(&TSpeedY);
	
}