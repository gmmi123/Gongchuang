#ifndef __MyCarControl_H
#define __MyCarControl_H
// �ײ��
#include "Motor_can.h"
#include "MyIMU.h"

#define WheelL 170.0f //���Ӿ��복�����ĵľ��룬��λmm
#define WheelR 40.0f //���Ӱ뾶����λmm


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
	char recordAngleOneFlag;//1Ϊ�Ѽ�¼
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
// ��Ա����
typedef struct
{
	MyCarControlRelativeFixedPointMove_Typedef RelativeFixedPointMove;
	MyCarControlAnglePID_Typedef AnglePID;
	MyCarControlWheel_Typedef Wheel[wheelnum]; // ����
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
	float Vx_global;//��Ե���
	float Vy_global;
	float SetVx;
	float SetVy;
	float SetVw;
	float setCarAngle;
	float EnableMotor;
	float EnableControl;
	char WheelStopFlag;
	char CarMoveStatus;//0Ϊ���ƶ� 1Ϊ�ƶ���
					   //	float temp_Targetdistance;
} MyCarControlDate_Typedef;

// ��
typedef struct
{
	MyCarControlDate_Typedef Date;
	// ����
	// С����ָ���ٶ���ָ�������ƶ�ָ������
	int (*move)(MyCarControlDate_Typedef *object, float direction, float distance, float speed);
	// �����ƶ�
	void (*endmove)(MyCarControlDate_Typedef *object);
	// С���ĳ��峯������
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
