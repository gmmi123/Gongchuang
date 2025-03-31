#include "MyClaw.h"
MyClaw_Typedef Claw = {
	.Date.dt = 0.02,
	.Date.Angle = 100,
	.Date.setAngle =100,
	.Date.setVw = 400,
};


int MyClaw_Move(MyClawDate_Typedef* oject,float Angle)
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

//����צ�ӵ��ٶȺͽǶ�
void MyClaw_ControlClaw(MyClawDate_Typedef* object)
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
	
	//�ײ���ƶ��
	TIM2->CCR3 = (2000/ClawServoDegree*(int)object->Angle)+500;
	
}



void MyClaw_Init(MyClaw_Typedef* object)
{
	object->Date.dt = 0.02;
}


void MyClaw_Task(MyClaw_Typedef* object)
{

	//����
	MyClaw_ControlClaw(&object->Date);
}