#include "MyMahonyAHRS.h"

MahonyAHRS_Typedef MahonyAHRS;

#define twoKpDef	(2.0f * 0.5f)	// 2 * proportional gain
#define twoKiDef	(2.0f * 0.0f)	// 2 * integral gain

// 初始化函数
void MyMahony_Init(MahonyAHRS_Typedef *MahonyAHRS, float sampleFrequency)
{
    MahonyAHRS->MahonyAHRS_Date.twoKi = twoKiDef;
    MahonyAHRS->MahonyAHRS_Date.q0 = 1.0f;
    MahonyAHRS->MahonyAHRS_Date.q1 = 0.0f;
    MahonyAHRS->MahonyAHRS_Date.q2 = 0.0f;
    MahonyAHRS->MahonyAHRS_Date.q3 = 0.0f;
    MahonyAHRS->MahonyAHRS_Date.integralFBx = 0.0f;
    MahonyAHRS->MahonyAHRS_Date.integralFBy = 0.0f;
    MahonyAHRS->MahonyAHRS_Date.integralFBz = 0.0f;
    MahonyAHRS->MahonyAHRS_Date.anglesComputed = 0;
    MahonyAHRS->MahonyAHRS_Date.invSampleFreq = 1.0f / sampleFrequency;

    // 绑定方法

	MahonyAHRS->update = MyMahony_update;
	MahonyAHRS->updateIMU = MyMahony_updateIMU;
	MahonyAHRS->computeAngles = MyMahony_computeAngles;

}

float MyMahony_invSqrt(float x)
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	y = y * (1.5f - (halfx * y * y));
	return y;
}

static float MyinvSqrt(float x)  // if use other platform please use float Mahony_invSqrt(float x)
{
	volatile float tmp = 1.0f;
	tmp /= __sqrtf(x);
	return tmp;
}



void MyMahonyAHRSinit(MahonyAHRSDate_Typedef* MahonyAHRSDate,float ax, float ay, float az, float mx, float my, float mz)
{
    float recipNorm;
    float init_yaw, init_pitch, init_roll;
    float cr2, cp2, cy2, sr2, sp2, sy2;
    float sin_roll, cos_roll, sin_pitch, cos_pitch;
    float magX, magY;

    recipNorm = MyinvSqrt(ax * ax + ay * ay + az * az);
    ax *= recipNorm;
    ay *= recipNorm;
    az *= recipNorm;

    if((mx != 0.0f) && (my != 0.0f) && (mz != 0.0f)) 
    {
	    recipNorm = MyinvSqrt(mx * mx + my * my + mz * mz);
	    mx *= recipNorm;
	    my *= recipNorm;
	    mz *= recipNorm;
	}

    init_pitch = atan2f(-ax, az);
    init_roll = atan2f(ay, az);

    sin_roll  = sinf(init_roll);
    cos_roll  = cosf(init_roll);
    cos_pitch = cosf(init_pitch);
    sin_pitch = sinf(init_pitch);

    if((mx != 0.0f) && (my != 0.0f) && (mz != 0.0f))
    {
    	magX = mx * cos_pitch + my * sin_pitch * sin_roll + mz * sin_pitch * cos_roll;
    	magY = my * cos_roll - mz * sin_roll;
    	init_yaw  = atan2f(-magY, magX);
    }
    else
    {
    	init_yaw=0.0f;
    }

    cr2 = cosf(init_roll * 0.5f);
    cp2 = cosf(init_pitch * 0.5f);
    cy2 = cosf(init_yaw * 0.5f);
    sr2 = sinf(init_roll * 0.5f);
    sp2 = sinf(init_pitch * 0.5f);
    sy2 = sinf(init_yaw * 0.5f);

    MahonyAHRSDate->q0 = cr2 * cp2 * cy2 + sr2 * sp2 * sy2;
    MahonyAHRSDate->q1= sr2 * cp2 * cy2 - cr2 * sp2 * sy2;
    MahonyAHRSDate->q2 = cr2 * sp2 * cy2 + sr2 * cp2 * sy2;
    MahonyAHRSDate->q3= cr2 * cp2 * sy2 - sr2 * sp2 * cy2;

    // Normalise quaternion
    recipNorm = MyMahony_invSqrt(MahonyAHRSDate->q0 * MahonyAHRSDate->q0 + MahonyAHRSDate->q1 * MahonyAHRSDate->q1 + MahonyAHRSDate->q2 * MahonyAHRSDate->q2 + MahonyAHRSDate->q3 * MahonyAHRSDate->q3);
    MahonyAHRSDate->q0 *= recipNorm;
    MahonyAHRSDate->q1 *= recipNorm;
    MahonyAHRSDate->q2 *= recipNorm;
    MahonyAHRSDate->q3 *= recipNorm;
}

void MyMahony_update(MahonyAHRSDate_Typedef* MahonyAHRSDate, float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz)
{
	float recipNorm;
	float q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;
	float hx, hy, bx, bz;
	float halfvx, halfvy, halfvz, halfwx, halfwy, halfwz;
	float halfex, halfey, halfez;
	float qa, qb, qc;
	// Convert gyroscope degrees/sec to radians/sec
//	gx *= 0.0174533f;
//	gy *= 0.0174533f;
//	gz *= 0.0174533f;

    // Use IMU algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalisation)
    if((mx == 0.0f) && (my == 0.0f) && (mz == 0.0f)) {
        MyMahony_updateIMU(MahonyAHRSDate,gx, gy, gz, ax, ay, az);
        return;
    }

	// Compute feedback only if accelerometer measurement valid
	// (avoids NaN in accelerometer normalisation)
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

		// Normalise accelerometer measurement
		recipNorm = MyinvSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;

		// Normalise magnetometer measurement
		recipNorm = MyinvSqrt(mx * mx + my * my + mz * mz);
		mx *= recipNorm;
		my *= recipNorm;
		mz *= recipNorm;

		// Auxiliary variables to avoid repeated arithmetic
		q0q0 = MahonyAHRSDate->q0 * MahonyAHRSDate->q0;
		q0q1 = MahonyAHRSDate->q0 * MahonyAHRSDate->q1;
		q0q2 = MahonyAHRSDate->q0 * MahonyAHRSDate->q2;
		q0q3 = MahonyAHRSDate->q0 * MahonyAHRSDate->q3;
		q1q1 = MahonyAHRSDate->q1 * MahonyAHRSDate->q1;
		q1q2 = MahonyAHRSDate->q1 * MahonyAHRSDate->q2;
		q1q3 = MahonyAHRSDate->q1 * MahonyAHRSDate->q3;
		q2q2 = MahonyAHRSDate->q2 * MahonyAHRSDate->q2;
		q2q3 = MahonyAHRSDate->q2 * MahonyAHRSDate->q3;
		q3q3 = MahonyAHRSDate->q3 * MahonyAHRSDate->q3;

		// Reference direction of Earth's magnetic field
		hx = 2.0f * (mx * (0.5f - q2q2 - q3q3) + my * (q1q2 - q0q3) + mz * (q1q3 + q0q2));
		hy = 2.0f * (mx * (q1q2 + q0q3) + my * (0.5f - q1q1 - q3q3) + mz * (q2q3 - q0q1));
		bx = sqrtf(hx * hx + hy * hy);
		bz = 2.0f * (mx * (q1q3 - q0q2) + my * (q2q3 + q0q1) + mz * (0.5f - q1q1 - q2q2));

		// Estimated direction of gravity and magnetic field
		halfvx = q1q3 - q0q2;
		halfvy = q0q1 + q2q3;
		halfvz = q0q0 - 0.5f + q3q3;
		halfwx = bx * (0.5f - q2q2 - q3q3) + bz * (q1q3 - q0q2);
		halfwy = bx * (q1q2 - q0q3) + bz * (q0q1 + q2q3);
		halfwz = bx * (q0q2 + q1q3) + bz * (0.5f - q1q1 - q2q2);

		// Error is sum of cross product between estimated direction
		// and measured direction of field vectors
		halfex = (ay * halfvz - az * halfvy) + (my * halfwz - mz * halfwy);
		halfey = (az * halfvx - ax * halfvz) + (mz * halfwx - mx * halfwz);
		halfez = (ax * halfvy - ay * halfvx) + (mx * halfwy - my * halfwx);

		// Compute and apply integral feedback if enabled
		if(MahonyAHRSDate->twoKi > 0.0f) {
			// integral error scaled by Ki
			MahonyAHRSDate->integralFBx += MahonyAHRSDate->twoKi * halfex * MahonyAHRSDate->invSampleFreq;
			MahonyAHRSDate->integralFBy += MahonyAHRSDate->twoKi * halfey * MahonyAHRSDate->invSampleFreq;
			MahonyAHRSDate->integralFBz += MahonyAHRSDate->twoKi * halfez * MahonyAHRSDate->invSampleFreq;
			gx += MahonyAHRSDate->integralFBx;	// apply integral feedback
			gy += MahonyAHRSDate->integralFBy;
			gz += MahonyAHRSDate->integralFBz;
		} else {
			MahonyAHRSDate->integralFBx = 0.0f;	// prevent integral windup
			MahonyAHRSDate->integralFBy = 0.0f;
			MahonyAHRSDate->integralFBz = 0.0f;
		}

		// Apply proportional feedback
		gx += twoKpDef * halfex;
		gy += twoKpDef * halfey;
		gz += twoKpDef * halfez;
	}

	// Integrate rate of change of quaternion
	gx *= (0.5f * MahonyAHRSDate->invSampleFreq);		// pre-multiply common factors
	gy *= (0.5f * MahonyAHRSDate->invSampleFreq);
	gz *= (0.5f * MahonyAHRSDate->invSampleFreq);
	qa = MahonyAHRSDate->q0;
	qb = MahonyAHRSDate->q1;
	qc = MahonyAHRSDate->q2;
	MahonyAHRSDate->q0 += (-qb * gx - qc * gy - MahonyAHRSDate->q3 * gz);
	MahonyAHRSDate->q1 += (qa * gx + qc * gz - MahonyAHRSDate->q3 * gy);
	MahonyAHRSDate->q2 += (qa * gy - qb * gz +MahonyAHRSDate->q3 * gx);
	MahonyAHRSDate->q3 += (qa * gz + qb * gy - qc * gx);

	// Normalise quaternion
	recipNorm = MyinvSqrt(MahonyAHRSDate->q0 * MahonyAHRSDate->q0 + MahonyAHRSDate->q1 * MahonyAHRSDate->q1 + MahonyAHRSDate->q2 * MahonyAHRSDate->q2 + MahonyAHRSDate->q3 * MahonyAHRSDate->q3);
	MahonyAHRSDate->q0 *= recipNorm;
	MahonyAHRSDate->q1 *= recipNorm;
	MahonyAHRSDate->q2 *= recipNorm;
	MahonyAHRSDate->q3 *= recipNorm;
	MahonyAHRSDate->anglesComputed = 0;
}


void MyMahony_updateIMU(MahonyAHRSDate_Typedef* MahonyAHRS,float gx, float gy, float gz, float ax, float ay, float az) {
    float recipNorm;
    float halfvx, halfvy, halfvz;
    float halfex, halfey, halfez;
    float qa, qb, qc;

    // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
    if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

        // Normalise accelerometer measurement
        recipNorm = MyinvSqrt(ax * ax + ay * ay + az * az);
        ax *= recipNorm;
        ay *= recipNorm;
        az *= recipNorm;

        // Estimated direction of gravity and vector perpendicular to magnetic flux
        halfvx = MahonyAHRS->q1 * MahonyAHRS->q3 - MahonyAHRS->q0 * MahonyAHRS->q2;
        halfvy = MahonyAHRS->q0 * MahonyAHRS->q1 + MahonyAHRS->q2 *MahonyAHRS->q3;
        halfvz = MahonyAHRS->q0 * MahonyAHRS->q0 - 0.5f + MahonyAHRS->q3 * MahonyAHRS->q3;

        // Error is sum of cross product between estimated and measured direction of gravity
        halfex = (ay * halfvz - az * halfvy);
        halfey = (az * halfvx - ax * halfvz);
        halfez = (ax * halfvy - ay * halfvx);

        // Compute and apply integral feedback if enabled
        if(MahonyAHRS->twoKi > 0.0f) {
            MahonyAHRS->integralFBx += MahonyAHRS->twoKi * halfex  * MahonyAHRS->invSampleFreq;	// integral error scaled by Ki
            MahonyAHRS->integralFBy += MahonyAHRS->twoKi * halfey  * MahonyAHRS->invSampleFreq;
            MahonyAHRS->integralFBz += MahonyAHRS->twoKi * halfez  * MahonyAHRS->invSampleFreq;
            gx += MahonyAHRS->integralFBx;	// apply integral feedback
            gy += MahonyAHRS->integralFBy;
            gz += MahonyAHRS->integralFBz;
        }
        else {
            MahonyAHRS->integralFBx = 0.0f;	// prevent integral windup
            MahonyAHRS->integralFBy = 0.0f;
            MahonyAHRS->integralFBz = 0.0f;
        }

        // Apply proportional feedback
        gx += twoKpDef * halfex;
        gy += twoKpDef * halfey;
        gz += twoKpDef * halfez;
    }

    // Integrate rate of change of quaternion
    gx *= (0.5f *   MahonyAHRS->invSampleFreq);		// pre-multiply common factors
    gy *= (0.5f  * MahonyAHRS->invSampleFreq);
    gz *= (0.5f  * MahonyAHRS->invSampleFreq);
    qa = MahonyAHRS->q0;
    qb = MahonyAHRS->q1;
    qc = MahonyAHRS->q2;
    MahonyAHRS->q0 += (-qb * gx - qc * gy - MahonyAHRS->q3 * gz);
    MahonyAHRS->q1 += (qa * gx + qc * gz - MahonyAHRS->q3 * gy);
    MahonyAHRS->q2 += (qa * gy - qb * gz + MahonyAHRS->q3 * gx);
    MahonyAHRS->q3 += (qa * gz + qb * gy - qc * gx);

    // Normalise quaternion
    recipNorm = MyinvSqrt(MahonyAHRS->q0 * MahonyAHRS->q0 + MahonyAHRS->q1 * MahonyAHRS->q1 + MahonyAHRS->q2 * MahonyAHRS->q2 + MahonyAHRS->q3 * MahonyAHRS->q3);
    MahonyAHRS->q0 *= recipNorm;
    MahonyAHRS->q1 *= recipNorm;
    MahonyAHRS->q2 *= recipNorm;
    MahonyAHRS->q3 *= recipNorm;
}


void MyMahony_computeAngles(MahonyAHRSDate_Typedef* MahonyAHRS)
{
	arm_atan2_f32(MahonyAHRS->q0*MahonyAHRS->q1 + MahonyAHRS->q2*MahonyAHRS->q3, 0.5f - MahonyAHRS->q1*MahonyAHRS->q1 - MahonyAHRS->q2*MahonyAHRS->q2,&MahonyAHRS->roll_mahony);
	MahonyAHRS->roll_mahony *= 57.29578f;  
	MahonyAHRS->pitch_mahony =57.29578f * asinf(-2.0f * (MahonyAHRS->q1*MahonyAHRS->q3 - MahonyAHRS->q0*MahonyAHRS->q2));
	arm_atan2_f32(MahonyAHRS->q1*MahonyAHRS->q2 + MahonyAHRS->q0*MahonyAHRS->q3, 0.5f - MahonyAHRS->q2*MahonyAHRS->q2 - MahonyAHRS->q3*MahonyAHRS->q3,&MahonyAHRS->yaw_mahony); 
	MahonyAHRS->yaw_mahony *=57.29578f;
	MahonyAHRS->anglesComputed = 1;
}
