#ifndef MyMahonyAHRS_h
#define MyMahonyAHRS_h

#include "dsp/fast_math_functions.h"    
#include "arm_math.h"


typedef struct {
	
	    // 成员变量
    float twoKi;
    float q0, q1, q2, q3;
    float integralFBx, integralFBy, integralFBz;
    float invSampleFreq;
    float roll_mahony, pitch_mahony, yaw_mahony;
    char anglesComputed;

}MahonyAHRSDate_Typedef;


typedef struct {
	//成员变量
	MahonyAHRSDate_Typedef MahonyAHRS_Date;

    // 方法
	void (*init)(MahonyAHRSDate_Typedef*);
    void (*update)(MahonyAHRSDate_Typedef* MahonyAHRS, float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
    void (*updateIMU)(MahonyAHRSDate_Typedef* MahonyAHRS, float gx, float gy, float gz, float ax, float ay, float az);
    void (*computeAngles)(MahonyAHRSDate_Typedef*);

} MahonyAHRS_Typedef;

extern MahonyAHRS_Typedef MahonyAHRS;

// 静态函数声明
float MyMahony_invSqrt(float x);
//1.初始化对象
void MyMahony_Init(MahonyAHRS_Typedef*MahonyAHRS, float sampleFrequency);//频率参数要非常准确
//2.传入陀螺仪数值,初始更新数据
void MyMahonyAHRSinit(MahonyAHRSDate_Typedef* MahonyAHRSDate,float ax, float ay, float az, float mx, float my, float mz);
//3.更新四元数(带磁力计)
void MyMahony_update(MahonyAHRSDate_Typedef* MahonyAHRS, float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
//3.更新四元数(无磁力计)
void MyMahony_updateIMU(MahonyAHRSDate_Typedef* MahonyAHRS, float gx, float gy, float gz, float ax, float ay, float az);
//4.计算角度
void MyMahony_computeAngles(MahonyAHRSDate_Typedef* MahonyAHRS);



#endif