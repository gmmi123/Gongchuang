#include "MyTurnPlate.h"

#define TurnPlateServoDegree 270	//¶æ»ú¶ÈÊý·¶Î§

MyTurnPlate_Typedef TurnPlate ={
	.Date.dt = 0.02,
	.Date.Angle = 0,
	.Date.setAngle = 0,
	.Date.setVw =300,
};

int MyTurnPlate_Move(MyTurnPlateDate_Typedef* oject,float Angle)
{
	oject->setAngle = Angle;

	if(oject->Angle==Angle)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void MyTurnPlate_setControl(MyTurnPlateDate_Typedef* object,float Angle,float Speed)
{
	object->setVw = Speed;
	object->setAngle = Angle;
}

void MyTurnPlate_Control(MyTurnPlateDate_Typedef* object)
{
	
	if(object->setVw!=0)
	{
		if(fabsf(object->setAngle-object->Angle)<= fabsf(object->setVw*object->dt))
		{
			object->Angle =object->setAngle;
		}
		else if(object->setAngle-object->Angle>0)
		{
			
			object->Angle += object->setVw*object->dt;
		}
		else
		{
			object->Angle -= object->setVw*object->dt;
		}
	}
	else
	{
		object->Angle = object->setAngle;
	}
	
	TIM2->CCR1 = (2000/TurnPlateServoDegree*(int)object->Angle)+500;
}

void MyTurnPlate_Init(MyTurnPlate_Typedef* object)
{
	object->Date.dt =0.02;

}

void MyTurnPlate_Task(MyTurnPlate_Typedef* object)
{

	
	MyTurnPlate_Control(&object->Date);

	
}

