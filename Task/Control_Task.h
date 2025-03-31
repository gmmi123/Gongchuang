#ifndef __Control_Task_H
#define __Control_Task_H
#include "main.h"
typedef enum
{
	Init_Error = -1,				   // 机械臂初始化错误
	Init_Disable = 0,				   // 机械臂未初始化
	Init_Robotic_arminit_Enabling = 1, // 机械臂初始化中
	Init_Robotic_arminit_Enable = 2,   // 机械臂初始化完成
	Init_ALLEnable = 3				   // 所有初始化完成(机械臂，云台，底盘电机)
} Control_InitTypeDef;

void Control_Task_1();
void Control_Task_2();
void Distance_calcu_Task(void);
//机械臂初始化
uint8_t Robotic_arminit_Task(void);
#endif
