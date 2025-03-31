#include "IMU.h"
#include "BMI088driver.h"
#include "BMI088reg.h"
#include "BMI088Middleware.h"
#include "MahonyAHRS.h"
extern float gyro[3], accel[3], temp;
IMU_Typedef IMU;

float InfiniteYaw(float Now_Yaw)
{
	static int flag=0;//记录偏航角突变次数
	static float Last_Yaw=0;
	if(Now_Yaw-Last_Yaw<-180)//如果存在角度突变
	{
		flag++;
	}
	else if(Now_Yaw-Last_Yaw>180)
	{
		flag--;
	}
 
    Last_Yaw = Now_Yaw;
    return Now_Yaw+ flag*360;
}



void GetIMUdate(void)
{
//	ParseAndPrintData(UART10_RX_Second_BUF,UART10_RX_LEN);//陀螺仪解包，获取陀螺仪数据
	BMI088_read(gyro, accel, &temp);
	Get_MahonyAngle(accel[0],accel[1],accel[2],gyro[0],gyro[1],gyro[2]);
	IMU.ax=accel[0];
	IMU.ay=accel[1];
	IMU.az=accel[2];
	IMU.gx=gyro[0];
	IMU.gy=gyro[1];
	IMU.gz=gyro[2];
	IMU.YawAngle = yaw_mahony;
//	if(TempStatus==0)
//	{
//		if(yaw_mahony<=90.0+TempNum&&yaw_mahony>=-180.0+TempNum)
//		{
//		IMU.YawAngle=yaw_mahony+90.0+TempNum;
//		}
//		else //(yaw_mahony>90&&yaw_mahony<=180)
//		{
//			IMU.YawAngle=-270+yaw_mahony+TempNum;
//		}
//	}
//	


	IMU.Full_YawAngle=InfiniteYaw(IMU.YawAngle);
}
