#ifndef __MyTurnPlate_H
#define __MyTurnPlate_H

//底层库
#include "tim.h"
#include "math.h"

#define TurnPlate(x)  MyTurnPlate_Move(&TurnPlate.Date,x)

// 成员变量
typedef struct
{
		
	float Angle;
	float dt;
	float setVw;
	float setAngle;
	
}MyTurnPlateDate_Typedef;

// 类
typedef struct
{
	MyTurnPlateDate_Typedef Date;

} MyTurnPlate_Typedef;

extern MyTurnPlate_Typedef TurnPlate;

void MyTurnPlate_Task(MyTurnPlate_Typedef* object);
void MyTurnPlate_Init(MyTurnPlate_Typedef* object);
int MyTurnPlate_Move(MyTurnPlateDate_Typedef* oject,float Angle);
#endif
