#ifndef __MyLift_H
#define __MyLift_H
//�ײ��
#include "usart.h"
#include "math.h"

//���ú���
//xΪZ�������
#define Z(x)	MyLift_Move(&Lift.Date,(x),500)

// ��Ա����
typedef struct
{
	float Position;
	float dt;
	
	unsigned char  Sendarr[20];
	char Direction;		//1��  0��
	char Acceleration;
	char Mod;  //0��ԣ�1����
	float setPosition;
	int setSpeed;//rpm  
	
	
}MyLiftDate_Typedef;

// ��
typedef struct
{
	
	MyLiftDate_Typedef Date;
	// ����

} MyLift_Typedef;

extern MyLift_Typedef Lift;

void MyLift_Init(MyLift_Typedef *object);

void MyLift_Task(MyLift_Typedef *object);
int MyLift_Move(MyLiftDate_Typedef* oject,float Position,float Speed);


#endif
