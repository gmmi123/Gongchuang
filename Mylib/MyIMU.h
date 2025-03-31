#ifndef __MyIMU_H
#define __MyIMU_H

//底层陀螺仪
#include "IMU.h"
//陀螺仪角度算法
#include "MyMahonyAHRS.h"
//宏定义开关
//#define MyIMUOriginalDate		//陀螺仪为原始数据时，启用开关
//#define MyIMUGyroDrift		//需要获取陀螺仪的零飘时，启用开关
typedef struct
{
	// 成员变量
	float Full_YawAnglePY;
	char Enable;
	float RollAngle;
	float PitchAngle;
	float YawAngle;		 // 范围0-180
	float Full_YawAngle; // 范围-16777216到16777216
	float ax;
	float ay;
	float az;
	float gx;
	float gy;
	float gz;
	float GyroDrift[3];//角速度零飘值
	int	GyroDriftNum;//角速度零飘值次数
	float PY;
#ifdef	MyMahonyAHRS_h
	char MahonyFirstflag;
#endif
#ifdef MyIMUOriginalDate
	float ACCrange;	   // 加速度量程
	float GYROrange;   // 角速度量程
	float Original_ax; // 原始数据
	float Original_ay;
	float Original_az;
	float Original_gx;
	float Original_gy;
	float Original_gz;
#endif
}MyIMUDate_Typedef;

typedef struct
{
	// 成员变量
	MyIMUDate_Typedef MyIMUDate;
	// 成员函数
	void (*GetGyroDate)(MyIMUDate_Typedef *MyIMUDate);
#ifdef MyIMUOriginalDate
	void (*transformDate)(MyIMUDate_Typedef *MyIMUDate);
#endif
	void (*GetAngle)(MyIMUDate_Typedef *MyIMUDate);

} MyIMU_Typedef;

//对象
extern MyIMU_Typedef MyIMU;

//对象初始化
void MyIMU_Init(MyIMU_Typedef *MyIMU);

//IMU任务
void MyIMU_Task(MyIMU_Typedef* MyIMU);
#endif
