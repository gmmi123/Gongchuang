#include "MyTSpeed.h"
MyTSpeed_Typedef TSpeedX={.AccMax=5,.Vm=20};
MyTSpeed_Typedef TSpeedY={.AccMax=5,.Vm=20};;

#define AccMax	object->AccMax
#define dt		object->dt
#define V		object->V//��ǰ�ٶ�
#define Vm		object->Vm//����ٶ�
#define St		object->St//ʣ��·��
#define Sc		object->Sc//�Ե�ǰ�����ٶȼ���ʱ��·��
#define AccOut	object->AccOut//������ٶ�
#define VOut    object->VOut//����ٶ�

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
	//��֤AccΪ��ֵ
	AccMax = fabsf(AccMax);
	//����Sc
	Sc = V*V/(2*AccMax);
	//��ʣ�����St�������ٶȷ���
	if(St>=0)//·��Ϊ��ʱ
	{
		if(Sc>=St)
		{
			AccOut = -V*V/(2*St);//���ٶ�_
		}
		else 
		{
			if(Vm>=V/sqrtf(2))//�����������ٶȵ�����ٶ�
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
			AccOut = -V*V/(2*St);//���ٶ�_
		}
		else 
		{
			if(Vm>=-V/sqrtf(2))//�����������ٶȵ�����ٶ�
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