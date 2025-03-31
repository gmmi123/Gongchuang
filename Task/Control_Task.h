#ifndef __Control_Task_H
#define __Control_Task_H
#include "main.h"
typedef enum
{
	Init_Error = -1,				   // ��е�۳�ʼ������
	Init_Disable = 0,				   // ��е��δ��ʼ��
	Init_Robotic_arminit_Enabling = 1, // ��е�۳�ʼ����
	Init_Robotic_arminit_Enable = 2,   // ��е�۳�ʼ�����
	Init_ALLEnable = 3				   // ���г�ʼ�����(��е�ۣ���̨�����̵��)
} Control_InitTypeDef;

void Control_Task_1();
void Control_Task_2();
void Distance_calcu_Task(void);
//��е�۳�ʼ��
uint8_t Robotic_arminit_Task(void);
#endif
