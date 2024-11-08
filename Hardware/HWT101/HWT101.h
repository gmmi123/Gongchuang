#ifndef __HWT101_H
#define __HWT101_H

#include "main.h"

#define IMU_Uart huart1			//�����Ǵ���

typedef struct
{
	float YawAngle;
	float angular_velocity_y;
	float angular_velocity_z;
	float Full_YawAngle;				//��ȫƫ����
}
IMU_Typedef;

extern IMU_Typedef IMU;

uint8_t CalculateChecksum(uint8_t *data, uint16_t length);//��У��
void ParseAndPrintData(uint8_t *data, uint16_t length);//�������
void ResetHWT101(void);//�����ǽǶ�����
#endif
