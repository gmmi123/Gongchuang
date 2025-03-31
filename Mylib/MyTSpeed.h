#ifndef __MyTSpeed_H
#define __MyTSpeed_H

#include "math.h"
#include "MyCarControl.h"
// 
typedef struct
{
	float AccMax;//为正值
	float V;//当前速度
	float Vm;//最大速度  //为正值
	float St;//剩余路程
	float Sc;//以当前最大减速度减速时的路程
	float AccOut;//输出加速度
	float VOut;//输出速度
} MyTSpeed_Typedef;

extern MyTSpeed_Typedef TSpeedX;
extern MyTSpeed_Typedef TSpeedY;
void MyTSpeed_Task(void);

#endif
