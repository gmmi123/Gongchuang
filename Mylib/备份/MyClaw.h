 #ifndef __MyClaw_H
 #define __MyClaw_H
 //�ײ��
 #include "math.h"
#include "Motor_can.h"
#include "dm4310_drv.h"

 //����L1��L2�ĳ���(��λmm)
#define measureL1	140	
#define measureL2	140
//��ʼ�Ƕ�,��ʼ����,��ʼ�ٶ�
#define initAngle   90
#define initDistance 200
#define initClawSpeed 0.1

typedef enum
{
	ClawA,
	ClawB,
	Clawnum
} Claw_Num;

typedef struct
{
	float Speed;
	float setSpeed;
	float Angle; //��ǰ�Ƕ�
	float setAngle;//Ŀ��Ƕ�
}SingleClaw_Typedef;
 


// ��Ա����
typedef struct
{
	float ClawSpeed;
	float L1;
	float L2;
	float Distance;
	float Angle;
	float setDistance;
	float setAngle;
	SingleClaw_Typedef SingleClaw[Clawnum];
	
} MyClawDate_Typedef;
 
// ��
typedef struct
{
	MyClawDate_Typedef Date;
	// ����
	void (*setClaw)(MyClawDate_Typedef* object,float Angle,float Distance);
} MyClaw_Typedef;

extern MyClaw_Typedef Claw;
void MyClaw_Task(MyClaw_Typedef* object);
void MyClaw_Init(MyClaw_Typedef* object);
 #endif
 