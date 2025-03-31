#ifndef __MyCarControl_H
#define __MyCarControl_H
// 底层库
#include "Motor_can.h"
#include "MyIMU.h"

#define WheelL 170.0f //轮子距离车体中心的距离，单位mm
#define WheelR 40.0f //轮子半径，单位mm


#define CatMove(x,y)  MyCarControl_Move(&Car.Date,x,y)
#define CatEnd() MyCarControl_MoveEnd(&Car.Date)
#define CatAngle(x) MyCarControl_MoveAngle(&Car.Date,x)
#define CatMoveSize(x)  RelativeFixedPointMove(&Car.Date,x)

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


typedef struct
{
	float Kp;
	float Kd;
	float ret;
	float err;
	float Last_err;
	
	float recordAngle;
	char recordAngleOneFlag;//1为已记录
	char correctionAngleOneFlag;

} MyCarControlAnglePID_Typedef;

typedef struct
{
	float KpX;
	float KdX;
	
	float KpY;
	float KdY;
	
	float retY;
	float retX;

	float ResidueDistanceX;
	float ResidueDistanceY;
	
	float Last_ResidueDistanceX;
	float Last_ResidueDistanceY;
	
	float SetDistanceSpeed;
	float SetDistancethreshold;
	char XStatus;
	char YStatus;
	char Enable;
	char Movingflag;
} MyCarControlDistance_Typedef;

typedef struct
{
	float x;
	float y;
}MyCarControlSize_Typedef;
typedef struct
{
	MyCarControlSize_Typedef Size[5];
	float CurrentX;
	float CurrentY;
	float TargetX;
	float TargetY;
	char CurrentNum;
	char setTargetNum;
	float setV;
	float Last_V;
	char Status;
	char Enable;
}MyCarControlRelativeFixedPointMove_Typedef;
// 成员变量
typedef struct
{
	MyCarControlRelativeFixedPointMove_Typedef RelativeFixedPointMove;
	MyCarControlAnglePID_Typedef AnglePID;
	MyCarControlWheel_Typedef Wheel[wheelnum]; // 轮子
	MyCarControlDistance_Typedef DistanceControl;
	float CarL;
	float dt;
	float x;
	float y;
	float Vx;
	float Vy;
	float Vw;
	float CarAngle;
	float CarAngleInWheel;
	float Vx_global;//相对地面
	float Vy_global;
	float SetVx;
	float SetVy;
	float SetVw;
	float setCarAngle;
	float EnableMotor;
	float EnableControl;
	char WheelStopFlag;
	char CarMoveStatus;//0为不移动 1为移动中
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
void MyCarControl_Move(MyCarControlDate_Typedef *object,float x,float y);
int MyCarControl_MoveEnd(MyCarControlDate_Typedef *object);
int MyCarControl_MoveAngle(MyCarControlDate_Typedef *object,float Angle);
int RelativeFixedPointMove(MyCarControlDate_Typedef *object,char num);
#endif
