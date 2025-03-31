#ifndef __IMU_H
#define __IMU_H

#include "main.h"

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
IMU_Typedef;

extern IMU_Typedef IMU;
void GetIMUdate(void);

#endif
