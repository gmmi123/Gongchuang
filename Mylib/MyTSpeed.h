#ifndef __MyTSpeed_H
#define __MyTSpeed_H

#include "math.h"
#include "MyCarControl.h"
// 
typedef struct
{
	float AccMax;//Ϊ��ֵ
	float V;//��ǰ�ٶ�
	float Vm;//����ٶ�  //Ϊ��ֵ
	float St;//ʣ��·��
	float Sc;//�Ե�ǰ�����ٶȼ���ʱ��·��
	float AccOut;//������ٶ�
	float VOut;//����ٶ�
} MyTSpeed_Typedef;

extern MyTSpeed_Typedef TSpeedX;
extern MyTSpeed_Typedef TSpeedY;
void MyTSpeed_Task(void);

#endif
