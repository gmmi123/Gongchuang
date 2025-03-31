#ifndef __MyCarControl_H
#define __MyCarControl_H
// 底层库
#include "Motor_can.h"
#include "MyIMU.h"

typedef enum
{
	wheel1,
	wheel2,
	wheel3,
	wheel4,
	wheelnum
} wheel_Num;

typedef struct
{
	float Speed;
	float setSpeed;
} MyCarControlWheel_Typedef;

// 成员变量
typedef struct
{
	MyCarControlWheel_Typedef Wheel[wheelnum]; // 轮子
	float x;
	float y;
	float CarAngle;
	float CarSpeed;
	float setCarSpeed;
	float setCarSpeedAngle;
	float setCarAngle;
	float controlfrequency;
	float controlperiod; // 控制周期(单位秒)
					   //	float temp_Targetdistance;
} MyCarControlDate_Typedef;

// 类
typedef struct
{
	MyCarControlDate_Typedef Date;
	// 方法
	// 小车以指定速度向指定方向移动指定距离
	int (*move)(MyCarControlDate_Typedef *object, float direction, float distance, float speed);
	// 结束移动
	void (*endmove)(MyCarControlDate_Typedef *object);
	// 小车的车体朝向设置
	void (*rotate)(MyCarControlDate_Typedef *object, float Angle);
} MyCarControl_Typedef;

extern MyCarControl_Typedef Car;

void MyCarControl_GetDistanceDate(MyCarControlDate_Typedef *object);

void MyCarControl_Task(MyCarControl_Typedef *object);
void MyCarControl_Init(MyCarControl_Typedef *object,float Controlperiod);
#endif
