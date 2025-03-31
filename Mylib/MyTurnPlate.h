#ifndef __MyTurnPlate_H
#define __MyTurnPlate_H

//�ײ��
#include "tim.h"
#include "math.h"

#define TurnPlate(x)  MyTurnPlate_Move(&TurnPlate.Date,x)

// ��Ա����
typedef struct
{
		
	float Angle;
	float dt;
	float setVw;
	float setAngle;
	
}MyTurnPlateDate_Typedef;

// ��
typedef struct
{
	MyTurnPlateDate_Typedef Date;

} MyTurnPlate_Typedef;

extern MyTurnPlate_Typedef TurnPlate;

void MyTurnPlate_Task(MyTurnPlate_Typedef* object);
void MyTurnPlate_Init(MyTurnPlate_Typedef* object);
int MyTurnPlate_Move(MyTurnPlateDate_Typedef* oject,float Angle);
#endif
