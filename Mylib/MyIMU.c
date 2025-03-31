#include "MyIMU.h"
#include "HWT101.h"
MyIMU_Typedef MyIMU ={
.MyIMUDate.Enable =1,
.MyIMUDate.PY =-3,
};


// ��ȡ�ײ������ǵ�����
void MyIMU_GetDate(MyIMUDate_Typedef *MyIMUDate)
{
//	MyIMUDate->Full_YawAngle = IMU.Full_YawAngle + MyIMUDate->Full_YawAnglePY;
//	MyIMUDate->YawAngle = IMU.YawAngle;
	MyIMUDate->Full_YawAngle = IMUHWT101.Full_YawAngle + MyIMUDate->Full_YawAnglePY;
	MyIMUDate->YawAngle = IMUHWT101.YawAngle;
}

//�����ǽ��㺯��
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
		//��������
		MyMahony_updateIMU(
		&MahonyAHRS.MahonyAHRS_Date,
		MyIMUDate->gx,
		MyIMUDate->gy,
		MyIMUDate->gz,
		MyIMUDate->ax,
		MyIMUDate->ay,
		MyIMUDate->az);
		//����Ƕ�
		MyMahony_computeAngles(&MahonyAHRS.MahonyAHRS_Date);
		MyIMUDate->PitchAngle=MahonyAHRS.MahonyAHRS_Date.pitch_mahony;
		MyIMUDate->RollAngle = MahonyAHRS.MahonyAHRS_Date.roll_mahony;
		MyIMUDate->YawAngle = MahonyAHRS.MahonyAHRS_Date.yaw_mahony;
	}
#endif
}

//��������Ʈ����
//GyroDriftTargetNumber����0ʱ��ÿ���ϵ綼�������Ʈ����С�ڵ���0ʱ�����ȡ��ʼ���������Ʈֱֵ�Ӽ�ȥ��Ʈ
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
// ���������ǽǶ�,ʹ��MahonyAHRS����Ƕ�
void MyIMU_CalculatingAngle(MyIMU_Typedef *MyIMU, MahonyAHRS_Typedef *MahonyAHRS)
{
	// ��������
	MahonyAHRS->updateIMU(
		&MahonyAHRS->MahonyAHRS_Date,
		MyIMU->MyIMUDate.gx,
		MyIMU->MyIMUDate.gy,
		MyIMU->MyIMUDate.gz,
		MyIMU->MyIMUDate.ax,
		MyIMU->MyIMUDate.ay,
		MyIMU->MyIMUDate.az);
	// ��Ԫ������Ƕ�
	MahonyAHRS->computeAngles(&MahonyAHRS->MahonyAHRS_Date);
	// ���ؽǶ�
	MyIMU->MyIMUDate.PitchAngle = MahonyAHRS->MahonyAHRS_Date.pitch_mahony;
	MyIMU->MyIMUDate.RollAngle = MahonyAHRS->MahonyAHRS_Date.roll_mahony;
	MyIMU->MyIMUDate.YawAngle = MahonyAHRS->MahonyAHRS_Date.yaw_mahony;
}
#endif

#ifdef MyIMUOriginalDate
// ������ԭʼ����תfloat��������
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

// ��ʼ��
void MyIMU_Init(MyIMU_Typedef *MyIMU)
{
	//��ʼ����Ʈֵ
	MyIMU->MyIMUDate.GyroDrift[0]=0;
	MyIMU->MyIMUDate.GyroDrift[1]=0;
	MyIMU->MyIMUDate.GyroDrift[2]=0;
	//�󶨻�ȡ���������ݺ���
	MyIMU->GetGyroDate=MyIMU_GetDate;
#ifdef MyIMUOriginalDate
	//������������ת������
	MyIMU->transformDate(&MyIMU->MyIMUDate);
	//���������ǵ����̲���
	MyIMU->MyIMUDate.ACCrange=3;
	MyIMU->MyIMUDate.GYROrange=2000;
#endif
	//�������������ǽ��㺯��
	MyIMU->GetAngle = MyIMUGetAngle;
}

//IMU����
void MyIMU_Task(MyIMU_Typedef* MyIMU)
{
	//��ȡ����������
	MyIMU_GetDate(&MyIMU->MyIMUDate);
#ifdef MyIMUOriginalDate
	//ת������������
	MyIMU->transformDate(&MyIMU->MyIMUDate);
#endif
#ifdef MyIMUGyroDrift
	//��ȡ��������Ʈ
	if(MyIMU_removeGyroDrift(MyIMU,5000))
	{
#endif
		
	//���������ݽ���
//	MyIMU->GetAngle(&MyIMU->MyIMUDate);
		
#ifdef MyIMUGyroDrift
	}
#endif	

}
