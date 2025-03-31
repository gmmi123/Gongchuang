#include "MyClaw.h"
MyClaw_Typedef Claw;
//��ȡצ�ӵ�����
void MyClaw_GetClawDate(MyClawDate_Typedef* object)
{
	object->SingleClaw[ClawA].Angle = motor[Motor5].para.pos*180.0f/3.14f;
	object->SingleClaw[ClawB].Angle = 180.0f-motor[Motor6].para.pos*180.0f/3.14f;
	object->SingleClaw[ClawA].Speed = motor[Motor5].para.vel;
	object->SingleClaw[ClawB].Speed = motor[Motor6].para.vel;
}

//����צ�ӵ��ٶȺͽǶ�
void MyClaw_ControlClaw(MyClawDate_Typedef* object)
{
	float temp = 0;
	temp =acosf((object->L1*object->L1+object->setDistance*object->setDistance-object->L2*object->L2)/(2*object->L1*object->setDistance))*90.0f/3.14f;
	object->SingleClaw[ClawA].setAngle = object->setAngle - 2*temp;
	object->SingleClaw[ClawB].setAngle = object->setAngle + 2*temp;
	pos_speed_ctrl(&hfdcan1,motor[Motor5].id,object->SingleClaw[ClawA].setAngle*3.14f/180.0f,object->SingleClaw[ClawA].setSpeed);
	pos_speed_ctrl(&hfdcan2,motor[Motor6].id,(180.0f-object->SingleClaw[ClawB].setAngle)*3.14f/180.0f,object->SingleClaw[ClawB].setSpeed);
}
void MyClaw_setClaw(MyClawDate_Typedef* object,float Angle,float Distance)
{
	float temp = 0;
	temp =acosf((object->L1*object->L1+Distance*Distance-object->L2*object->L2)/2*object->L1*Distance)*90.0f/3.14f;
	object->SingleClaw[ClawA].Angle = Angle - temp;
	object->SingleClaw[ClawB].Angle = Angle + temp;
}


void MyClaw_Init(MyClaw_Typedef* object)
{
	//��ʼ����Ա����
	object->Date.L1 = measureL1;
	object->Date.L2 = measureL2;
	object->Date.ClawSpeed = initClawSpeed;
	object->Date.SingleClaw[ClawA].setSpeed =initClawSpeed;
	object->Date.SingleClaw[ClawB].setSpeed =initClawSpeed;
	object->Date.setDistance = initDistance;
	object->Date.setAngle =initAngle;
	//�󶨷���
	object->setClaw = MyClaw_setClaw;
	
}

void MyClaw_Task(MyClaw_Typedef* object)
{
	//��ȡĿǰ����צ�ӵĽǶ�
	MyClaw_GetClawDate(&object->Date);
	//����
	MyClaw_ControlClaw(&object->Date);
}