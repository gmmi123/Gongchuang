 #ifndef __MyClaw_H
 #define __MyClaw_H
 //底层库
 #include "math.h"
#include "Motor_can.h"
#include "dm4310_drv.h"

 //测量L1和L2的长度(单位mm)
#define measureL1	140	
#define measureL2	140
//初始角度,初始距离,初始速度
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
	float Angle; //当前角度
	float setAngle;//目标角度
}SingleClaw_Typedef;
 


// 成员变量
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
 
// 类
typedef struct
{
	MyClawDate_Typedef Date;
	// 方法
	void (*setClaw)(MyClawDate_Typedef* object,float Angle,float Distance);
} MyClaw_Typedef;

extern MyClaw_Typedef Claw;
void MyClaw_Task(MyClaw_Typedef* object);
void MyClaw_Init(MyClaw_Typedef* object);
 #endif
 