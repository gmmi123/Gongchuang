#ifndef __MyCarControl_H
#define __MyCarControl_H
// �ײ��
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

// ��Ա����
typedef struct
{
	MyCarControlWheel_Typedef Wheel[wheelnum]; // ����
	float x;
	float y;
	float CarAngle;
	float CarSpeed;
	float setCarSpeed;
	float setCarSpeedAngle;
	float setCarAngle;
	float controlfrequency;
	float controlperiod; // ��������(��λ��)
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
#endif
