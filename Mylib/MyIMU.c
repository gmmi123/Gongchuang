#include "MyIMU.h"
#include "HWT101.h"
MyIMU_Typedef MyIMU ={
.MyIMUDate.Enable =1,
.MyIMUDate.PY =-3,
};


// 获取底层陀螺仪的数据
void MyIMU_GetDate(MyIMUDate_Typedef *MyIMUDate)
{
//	MyIMUDate->Full_YawAngle = IMU.Full_YawAngle + MyIMUDate->Full_YawAnglePY;
//	MyIMUDate->YawAngle = IMU.YawAngle;
	MyIMUDate->Full_YawAngle = IMUHWT101.Full_YawAngle + MyIMUDate->Full_YawAnglePY;
	MyIMUDate->YawAngle = IMUHWT101.YawAngle;
}

//陀螺仪解算函数
void MyIMUGetAngle(MyIMUDate_Typedef *MyIMUDate)
{
#ifdef	MyMahonyAHRS_h
	if(MyIMUDate->MahonyFirstflag==0)
	{
		MyMahonyAHRSinit(
		&MahonyAHRS.MahonyAHRS_Date,
		MyIMUDate->ax,
		MyIMUDate->ay,
		MyIMUDate->az,
		0,0,0);
		MyIMUDate->MahonyFirstflag=1;
	}
	if(MyIMUDate->MahonyFirstflag==1)
	{
		//更新数据
		MyMahony_updateIMU(
		&MahonyAHRS.MahonyAHRS_Date,
		MyIMUDate->gx,
		MyIMUDate->gy,
		MyIMUDate->gz,
		MyIMUDate->ax,
		MyIMUDate->ay,
		MyIMUDate->az);
		//计算角度
		MyMahony_computeAngles(&MahonyAHRS.MahonyAHRS_Date);
		MyIMUDate->PitchAngle=MahonyAHRS.MahonyAHRS_Date.pitch_mahony;
		MyIMUDate->RollAngle = MahonyAHRS.MahonyAHRS_Date.roll_mahony;
		MyIMUDate->YawAngle = MahonyAHRS.MahonyAHRS_Date.yaw_mahony;
	}
#endif
}

//陀螺仪零飘处理
//GyroDriftTargetNumber大于0时：每次上电都会进行零飘处理，小于等于0时：会读取初始化里面的零飘值直接减去零飘
int MyIMU_removeGyroDrift(MyIMU_Typedef *MyIMU,int GyroDriftTargetNumber)
{

	if(GyroDriftTargetNumber>0)
	{
		if(MyIMU->MyIMUDate.GyroDriftNum<GyroDriftTargetNumber)
		{
			MyIMU->MyIMUDate.GyroDriftNum++;
			MyIMU->MyIMUDate.GyroDrift[0]+=MyIMU->MyIMUDate.gx;
			MyIMU->MyIMUDate.GyroDrift[1]+=MyIMU->MyIMUDate.gy;
			MyIMU->MyIMUDate.GyroDrift[2]+=MyIMU->MyIMUDate.gz;
			if(MyIMU->MyIMUDate.GyroDriftNum==GyroDriftTargetNumber)
			{
				MyIMU->MyIMUDate.GyroDrift[0]/=(float)GyroDriftTargetNumber;
				MyIMU->MyIMUDate.GyroDrift[1]/=(float)GyroDriftTargetNumber;
				MyIMU->MyIMUDate.GyroDrift[2]/=(float)GyroDriftTargetNumber;
			}
			return 0;
		}
		else
		{
			MyIMU->MyIMUDate.gx-=MyIMU->MyIMUDate.GyroDrift[0];
			MyIMU->MyIMUDate.gy-=MyIMU->MyIMUDate.GyroDrift[1];
			MyIMU->MyIMUDate.gz-=MyIMU->MyIMUDate.GyroDrift[2];
			return 1;
		}
		
	}
	else 
	{
		MyIMU->MyIMUDate.gx-=MyIMU->MyIMUDate.GyroDrift[0];
		MyIMU->MyIMUDate.gy-=MyIMU->MyIMUDate.GyroDrift[1];
		MyIMU->MyIMUDate.gz-=MyIMU->MyIMUDate.GyroDrift[2];
		return 1;
	}
}

#ifdef MyMahonyAHRS_h
// 计算陀螺仪角度,使用MahonyAHRS计算角度
void MyIMU_CalculatingAngle(MyIMU_Typedef *MyIMU, MahonyAHRS_Typedef *MahonyAHRS)
{
	// 更新数据
	MahonyAHRS->updateIMU(
		&MahonyAHRS->MahonyAHRS_Date,
		MyIMU->MyIMUDate.gx,
		MyIMU->MyIMUDate.gy,
		MyIMU->MyIMUDate.gz,
		MyIMU->MyIMUDate.ax,
		MyIMU->MyIMUDate.ay,
		MyIMU->MyIMUDate.az);
	// 四元数计算角度
	MahonyAHRS->computeAngles(&MahonyAHRS->MahonyAHRS_Date);
	// 反回角度
	MyIMU->MyIMUDate.PitchAngle = MahonyAHRS->MahonyAHRS_Date.pitch_mahony;
	MyIMU->MyIMUDate.RollAngle = MahonyAHRS->MahonyAHRS_Date.roll_mahony;
	MyIMU->MyIMUDate.YawAngle = MahonyAHRS->MahonyAHRS_Date.yaw_mahony;
}
#endif

#ifdef MyIMUOriginalDate
// 陀螺仪原始数据转float类型数据
void MyIMU_transformDate(MyIMUDate_Typedef *MyIMUDate)
{
	MyIMUDate->ax = MyIMUDate->Original_ax * MyIMUDate->ACCrange * 9.8 / 32768.0;
	MyIMUDate->ay = MyIMUDate->Original_ay * MyIMUDate->ACCrange * 9.8 / 32768.0;
	MyIMUDate->az = MyIMUDate->Original_az * MyIMUDate->ACCrange * 9.8 / 32768.0;
	MyIMUDate->gx = MyIMUDate->Original_gx * MyIMUDate->GYROrange / 32768.0 * 3.1415926 / 180.0;
	MyIMUDate->gy = MyIMUDate->Original_gy * MyIMUDate->GYROrange / 32768.0 * 3.1415926 / 180.0;
	MyIMUDate->gz = MyIMUDate->Original_gz * MyIMUDate->GYROrange / 32768.0 * 3.1415926 / 180.0;
}
#endif

// 初始化
void MyIMU_Init(MyIMU_Typedef *MyIMU)
{
	//初始化零飘值
	MyIMU->MyIMUDate.GyroDrift[0]=0;
	MyIMU->MyIMUDate.GyroDrift[1]=0;
	MyIMU->MyIMUDate.GyroDrift[2]=0;
	//绑定获取陀螺仪数据函数
	MyIMU->GetGyroDate=MyIMU_GetDate;
#ifdef MyIMUOriginalDate
	//绑定陀螺仪数据转化函数
	MyIMU->transformDate(&MyIMU->MyIMUDate);
	//设置陀螺仪的量程参数
	MyIMU->MyIMUDate.ACCrange=3;
	MyIMU->MyIMUDate.GYROrange=2000;
#endif
	//绑定陀螺仪陀螺仪解算函数
	MyIMU->GetAngle = MyIMUGetAngle;
}

//IMU任务
void MyIMU_Task(MyIMU_Typedef* MyIMU)
{
	//获取陀螺仪数据
	MyIMU_GetDate(&MyIMU->MyIMUDate);
#ifdef MyIMUOriginalDate
	//转化陀螺仪数据
	MyIMU->transformDate(&MyIMU->MyIMUDate);
#endif
#ifdef MyIMUGyroDrift
	//获取陀螺仪零飘
	if(MyIMU_removeGyroDrift(MyIMU,5000))
	{
#endif
		
	//陀螺仪数据解算
//	MyIMU->GetAngle(&MyIMU->MyIMUDate);
		
#ifdef MyIMUGyroDrift
	}
#endif	

}
