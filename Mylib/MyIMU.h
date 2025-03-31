#ifndef __MyIMU_H
#define __MyIMU_H

//�ײ�������
#include "IMU.h"
//�����ǽǶ��㷨
#include "MyMahonyAHRS.h"
//�궨�忪��
//#define MyIMUOriginalDate		//������Ϊԭʼ����ʱ�����ÿ���
//#define MyIMUGyroDrift		//��Ҫ��ȡ�����ǵ���Ʈʱ�����ÿ���
typedef struct
{
	// ��Ա����
	float Full_YawAnglePY;
	char Enable;
	float RollAngle;
	float PitchAngle;
	float YawAngle;		 // ��Χ0-180
	float Full_YawAngle; // ��Χ-16777216��16777216
	float ax;
	float ay;
	float az;
	float gx;
	float gy;
	float gz;
	float GyroDrift[3];//���ٶ���Ʈֵ
	int	GyroDriftNum;//���ٶ���Ʈֵ����
	float PY;
#ifdef	MyMahonyAHRS_h
	char MahonyFirstflag;
#endif
#ifdef MyIMUOriginalDate
	float ACCrange;	   // ���ٶ�����
	float GYROrange;   // ���ٶ�����
	float Original_ax; // ԭʼ����
	float Original_ay;
	float Original_az;
	float Original_gx;
	float Original_gy;
	float Original_gz;
#endif
}MyIMUDate_Typedef;

typedef struct
{
	// ��Ա����
	MyIMUDate_Typedef MyIMUDate;
	// ��Ա����
	void (*GetGyroDate)(MyIMUDate_Typedef *MyIMUDate);
#ifdef MyIMUOriginalDate
	void (*transformDate)(MyIMUDate_Typedef *MyIMUDate);
#endif
	void (*GetAngle)(MyIMUDate_Typedef *MyIMUDate);

} MyIMU_Typedef;

//����
extern MyIMU_Typedef MyIMU;

//�����ʼ��
void MyIMU_Init(MyIMU_Typedef *MyIMU);

//IMU����
void MyIMU_Task(MyIMU_Typedef* MyIMU);
#endif
