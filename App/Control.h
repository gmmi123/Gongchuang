#ifndef __Control_H
#define __Control_H

#include "main.h"
#include "Motor_can.h"
#include "dm4310_drv.h"
// ץȡ˳���,���ν���
#define Get_sequence1 -5.81 + 0.01
#define Get_sequence2 -0.01 + 0.005 // ��С����
#define Get_sequence3 -0.53 + 0		//
#define Place_sequence1 -5.81 + 0.01
#define Place_sequence2 -6.35 + 0.01
#define Place_sequence3 -0.555 + 0.01

typedef enum
{
    SM_DateUpdate = 1,
    SM_unidentified = -1,
    SM_DateReceive_error = 0,
    SM_Circle = '2',
	SM_Masse = '1'

} SM_DateTypeDef;

void Motor_ControlInit(void);
void Control_Test(void);
;
// ��ȡ��ݮ������ͷ����(Բ��)
int Raspberry_PI_camerDate(char DateType);//��Ҫ����������

void Arm_MovementY(float Val); // ���ƻ�е��ˮƽת��

void Arm_MovementZ(float Val); // ���ƻ�е�������ƶ�
//// ���ƻ�еצ��
//void Control_Sever_On(void);

//// ���ƻ�еצ��
//void Control_Sever_Off(void);


////�Ӿ�ʶ���еצ�ſ�
//void Control_Sever_rec(void);
// ���Z���Ƿ�ָ��λ��
int Arm_CheckPosition_Z(float Val);

// ���Y���Ƿ�ָ��λ��
int Arm_CheckPosition_Y(float Val);
//��е���ƶ���������
int Arm_WorkDirection(void);
//ʶ����鶨λ
int Identifier_Masse(int color);
//ʶ��ԲȦ
int Identifier_circle(void);

int Arm_Get_Block(float Target_Position);//���ƻ�е�۷ŷ���

int Arm_Place_Block(float Target_Position);//���ƻ�е�۷��÷���


#endif
