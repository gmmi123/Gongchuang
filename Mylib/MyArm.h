 #ifndef __MyArm_H
 #define __MyArm_H
 //底层库
 #include "math.h"
#include "Motor_can.h"
#include "dm4310_drv.h"


#define Arm(x,y)  MyArm_Move(&Arm.Date,x,y)

 //测量L1和L2的长度(单位mm)
#define measureL1	140	
#define measureL2	140
//初始角度,初始距离,初始速度
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
	RCS,//直角坐标系
	PCS,//极坐标系
} CS_Mod;


typedef struct
{
	float Speed;
	float setSpeed;
	float Angle; //当前角度
	float setAngle;//目标角度
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



// 成员变量
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
 
// 类
typedef struct
{
	MyArmDate_Typedef Date;
	// 方法
	void (*setArm)(MyArmDate_Typedef* object,float Angle,float Distance);
} MyArm_Typedef;

extern MyArm_Typedef Arm;
void MyArm_Task(MyArm_Typedef* object);
void MyArm_Init(MyArm_Typedef* object);
int MyArm_Move(MyArmDate_Typedef* object,float X,float Y);
 #endif
 