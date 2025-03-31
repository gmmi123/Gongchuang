 #ifndef __MyArm_H
 #define __MyArm_H
 //�ײ��
 #include "math.h"
#include "Motor_can.h"
#include "dm4310_drv.h"


#define Arm(x,y)  MyArm_Move(&Arm.Date,x,y)

 //����L1��L2�ĳ���(��λmm)
#define measureL1	140	
#define measureL2	140
//��ʼ�Ƕ�,��ʼ����,��ʼ�ٶ�
#define initAngle    90
#define initDistance 200
#define initArmSpeed 2

typedef enum
{
	ArmA,
	ArmB,
	Armnum
} Arm_Num;

typedef enum
{
	RCS,//ֱ������ϵ
	PCS,//������ϵ
} CS_Mod;


typedef struct
{
	float Speed;
	float setSpeed;
	float Angle; //��ǰ�Ƕ�
	float setAngle;//Ŀ��Ƕ�
}SingleArm_Typedef;
 

typedef struct
{
	float dt;
	float Vx;
	float Vy;
	float X;
	float Y;
	float setX;
	float setY;
}Rectangular_Coordinate_System_Typedef;

typedef struct
{
	float R;
	float Angle;
	float setR;
	float setAngle;
}Polar_Coordinate_System_Typedef;



// ��Ա����
typedef struct
{
	CS_Mod Mod; 
	float setArmSpeed;
	float L1;
	float L2;
	SingleArm_Typedef SingleArm[Armnum];
	Rectangular_Coordinate_System_Typedef RCS;
	Polar_Coordinate_System_Typedef PCS;
} MyArmDate_Typedef;
 
// ��
typedef struct
{
	MyArmDate_Typedef Date;
	// ����
	void (*setArm)(MyArmDate_Typedef* object,float Angle,float Distance);
} MyArm_Typedef;

extern MyArm_Typedef Arm;
void MyArm_Task(MyArm_Typedef* object);
void MyArm_Init(MyArm_Typedef* object);
int MyArm_Move(MyArmDate_Typedef* object,float X,float Y);
 #endif
 