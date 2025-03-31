 #ifndef __MyDebug_H
 #define __MyDebug_H
 //底层库

typedef enum
{
	Debug_Waitting = 0,
	Debug_ArmX0Y200 = 1,
	Debug_ClawClose = 2,
	Debug_ClawOpen = 3,
	Debug_RsapberryPiMod_0 = 4,
	Debug_RsapberryPiMod_1 = 5,
	Debug_RsapberryPiMod_2 = 6,
} MyDebugStatus_Typedef;


// 成员变量
typedef struct
{
	int Status;
	float* setAngle;
	float* setPosition;
	float* setArmX;
	float* setArmY;
} MyDebugDate_Typedef;
 
// 类
typedef struct
{
	MyDebugDate_Typedef Date;
	// 方法
} MyDebug_Typedef;

extern MyDebug_Typedef Debug;
void MyDebug_Task(MyDebug_Typedef* object);
void MyDebug_Init(MyDebug_Typedef* object);
 #endif
 