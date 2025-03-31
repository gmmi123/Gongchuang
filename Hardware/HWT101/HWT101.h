#ifndef __HWT101_H
#define __HWT101_H

#include "main.h"

#define IMU_Uart huart10			//�����Ǵ���

typedef struct
{
	float YawAngle;
	float angular_velocity_y;
	float angular_velocity_z;
	float Full_YawAngle;				//��ȫƫ����
	float ax;
	float ay;
	float az;
	float gx;
	float gy;
	float gz;
}
IMUHWT101_Typedef;

extern IMUHWT101_Typedef IMUHWT101;

uint8_t CalculateChecksum(uint8_t *data, uint16_t length);//��У��
void ParseAndPrintData(uint8_t *data, uint16_t length);//�������
void ResetHWT101(void);//�����ǽǶ�����
#endif
