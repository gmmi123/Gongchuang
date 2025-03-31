#ifndef __MyLift_H
#define __MyLift_H
//底层库
#include "usart.h"
#include "math.h"

//调用函数
//x为Z轴的坐标
#define Z(x)	MyLift_Move(&Lift.Date,(x),500)

// 成员变量
typedef struct
{
	float Position;
	float dt;
	
	unsigned char  Sendarr[20];
	char Direction;		//1上  0下
	char Acceleration;
	char Mod;  //0相对，1绝对
	float setPosition;
	int setSpeed;//rpm  
	
	
}MyLiftDate_Typedef;

// 类
typedef struct
{
	
	MyLiftDate_Typedef Date;
	// 方法

} MyLift_Typedef;

extern MyLift_Typedef Lift;

void MyLift_Init(MyLift_Typedef *object);

void MyLift_Task(MyLift_Typedef *object);
int MyLift_Move(MyLiftDate_Typedef* oject,float Position,float Speed);


#endif
