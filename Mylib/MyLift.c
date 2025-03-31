#include "MyLift.h"
MyLift_Typedef Lift = {
	.Date.Mod =1,
	.Date.setSpeed =400,
	.Date.Direction =1,
	.Date.dt = 0.02,
};

int MyLift_Move(MyLiftDate_Typedef* oject,float Position,float Speed)
{
	oject->setPosition = Position;
	oject->setSpeed = Speed;
	if(oject->Position==Position)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
	

void MyLift_GetPosition(MyLiftDate_Typedef* object)
{
	if(fabsf(object->Position-object->setPosition)<object->dt * object->setSpeed * 8 / 60)
	{
		object->Position = object->setPosition;
	}
	else if(object->setPosition - object->Position>0)
	{
		object->Position += object->dt * object->setSpeed * 8 / 60;
	}
	else
	{
		object->Position -= object->dt * object->setSpeed * 8 / 60;
	}
}

void MyLift_Control(MyLiftDate_Typedef* object)
{
	unsigned char *p;
	char num=0;
	int tempPosition;
	//输入帧头
	object->Sendarr[num] = 0x01;
	num++;
	object->Sendarr[num] = 0xFD;
	num++;
	//输入旋转方向
	
	if(object->setPosition>0)
	{
		object->Direction =1;
		object->Sendarr[num] = 1;
		num++;
	}
	else
	{
		object->Direction =0;
		object->Sendarr[num] = 0;
		num++;
	}

	//输入速度
	p = (unsigned char *)&object->setSpeed;
	object->Sendarr[num] = p[1];
	num++;
	object->Sendarr[num] = p[0];
	num++;
	//输入加速度
	object->Sendarr[num] =object->Acceleration;
	num++;
	//输入位置
	tempPosition = (int)(400* fabsf(object->setPosition));
	p = (unsigned char *)&tempPosition;
	object->Sendarr[num]=p[3];
	num++;
	object->Sendarr[num]=p[2];
	num++;
	object->Sendarr[num]=p[1];
	num++;
	object->Sendarr[num]=p[0];
	num++;
	object->Sendarr[num]=object->Mod;
	num++;
	object->Sendarr[num]=0x00;
	num++;
	object->Sendarr[num]=0x6B;
	HAL_UART_Transmit(&huart8,object->Sendarr,13, 0xffff);
	
}

void MyLift_Init(MyLift_Typedef *object)
{
	unsigned char temp[] ={0x01, 0xF3,0xAB ,0x01 ,0x00 ,0x6B};
	HAL_UART_Transmit(&huart8,temp,6, 0xffff);
}


void MyLift_Task(MyLift_Typedef *object)
{
	MyLift_GetPosition(&object->Date);
	MyLift_Control(&object->Date);

}

