#ifndef MyMahonyAHRS_h
#define MyMahonyAHRS_h

#include "dsp/fast_math_functions.h"    
#include "arm_math.h"


typedef struct {
	
	    // ��Ա����
    float twoKi;
    float q0, q1, q2, q3;
    float integralFBx, integralFBy, integralFBz;
    float invSampleFreq;
    float roll_mahony, pitch_mahony, yaw_mahony;
    char anglesComputed;

}MahonyAHRSDate_Typedef;


typedef struct {
	//��Ա����
	MahonyAHRSDate_Typedef MahonyAHRS_Date;

    // ����
	void (*init)(MahonyAHRSDate_Typedef*);
    void (*update)(MahonyAHRSDate_Typedef* MahonyAHRS, float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
    void (*updateIMU)(MahonyAHRSDate_Typedef* MahonyAHRS, float gx, float gy, float gz, float ax, float ay, float az);
    void (*computeAngles)(MahonyAHRSDate_Typedef*);

} MahonyAHRS_Typedef;

extern MahonyAHRS_Typedef MahonyAHRS;

// ��̬��������
float MyMahony_invSqrt(float x);
//1.��ʼ������
void MyMahony_Init(MahonyAHRS_Typedef*MahonyAHRS, float sampleFrequency);//Ƶ�ʲ���Ҫ�ǳ�׼ȷ
//2.������������ֵ,��ʼ��������
void MyMahonyAHRSinit(MahonyAHRSDate_Typedef* MahonyAHRSDate,float ax, float ay, float az, float mx, float my, float mz);
//3.������Ԫ��(��������)
void MyMahony_update(MahonyAHRSDate_Typedef* MahonyAHRS, float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
//3.������Ԫ��(�޴�����)
void MyMahony_updateIMU(MahonyAHRSDate_Typedef* MahonyAHRS, float gx, float gy, float gz, float ax, float ay, float az);
//4.����Ƕ�
void MyMahony_computeAngles(MahonyAHRSDate_Typedef* MahonyAHRS);



#endif