#ifndef __HWT101_H
#define __HWT101_H

#include "main.h"

#define IMU_Uart huart10			//陀螺仪串口

typedef struct
{
	float YawAngle;
	float angular_velocity_y;
	float angular_velocity_z;
	float Full_YawAngle;				//完全偏航角
	float ax;
	float ay;
	float az;
	float gx;
	float gy;
	float gz;
}
IMUHWT101_Typedef;

extern IMUHWT101_Typedef IMUHWT101;

uint8_t CalculateChecksum(uint8_t *data, uint16_t length);//和校验
void ParseAndPrintData(uint8_t *data, uint16_t length);//解包函数
void ResetHWT101(void);//陀螺仪角度置零
#endif
