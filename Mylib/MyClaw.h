 #ifndef __MyClaw_H
 #define __MyClaw_H
 //�ײ��
#include "tim.h"
#include "math.h"

#define ClawServoDegree 270

#define ClawOpen() Claw(90)
#define ClawClose() Claw(165)
#define Claw(x)  MyClaw_Move(&Claw.Date,x)


// ��Ա����
typedef struct 
{
	float Angle;
	float dt;
	float setVw;
	float setAngle;
} MyClawDate_Typedef;
 
// ��
typedef struct
{
	MyClawDate_Typedef Date;
	// ����
} MyClaw_Typedef;

extern MyClaw_Typedef Claw;

void MyClaw_Task(MyClaw_Typedef* object);
void MyClaw_Init(MyClaw_Typedef* object);
int MyClaw_Move(MyClawDate_Typedef* oject,float Angle);
 #endif
 