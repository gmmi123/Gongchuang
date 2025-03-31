#include "Motor_can.h"
#include "MyCarControl.h"
#include "IMU.h"
motor_t motor[num];

void dm4310_motor_init(void)
{
	// ��ʼ��Motor1��Motor2�ĵ���ṹ
	memset(&motor[Motor1], 0, sizeof(motor[Motor1]));
	memset(&motor[Motor2], 0, sizeof(motor[Motor2]));
	memset(&motor[Motor3], 0, sizeof(motor[Motor3]));
	memset(&motor[Motor4], 0, sizeof(motor[Motor4]));
	memset(&motor[Motor5], 0, sizeof(motor[Motor5]));
	memset(&motor[Motor6], 0, sizeof(motor[Motor6]));

	// ����Motor1�ĵ����Ϣ
	motor[Motor1].id = 0x01;
	motor[Motor1].ctrl.mode = 2; // 0: MITģʽ   1: λ���ٶ�ģʽ   2: �ٶ�ģʽ
	motor[Motor1].ctrl.vel_set = 1.0f;
	//	motor[Motor1].ctrl.kp_set = 1.0f;
	motor[Motor1].ctrl.kd_set = 1.0f;

	motor[Motor2].id = 0x02;
	motor[Motor2].ctrl.mode = 2; // 0: MITģʽ   1: λ���ٶ�ģʽ   2: �ٶ�ģʽ
								 //	motor[Motor2].ctrl.vel_set = 1.0f;
	motor[Motor2].ctrl.kd_set = 1.0f;

	motor[Motor3].id = 0x03;
	motor[Motor3].ctrl.mode = 2; // 0: MITģʽ   1: λ���ٶ�ģʽ   2: �ٶ�ģʽ
	motor[Motor3].ctrl.vel_set = 1.0f;
	motor[Motor3].ctrl.kd_set = 1.0f;

	motor[Motor4].id = 0x04;
	motor[Motor4].ctrl.mode = 0; // 0: MITģʽ   1: λ���ٶ�ģʽ   2: �ٶ�ģʽ
	motor[Motor4].ctrl.vel_set = 1.0f;
	motor[Motor4].ctrl.kd_set = 1.0f;

	motor[Motor5].id = 0x05;
	motor[Motor5].ctrl.mode = 1; // 0: MITģʽ   1: λ���ٶ�ģʽ   2: �ٶ�ģʽ
	motor[Motor5].ctrl.vel_set = 1.0f;
	motor[Motor5].ctrl.kd_set = 1.0f;

	motor[Motor6].id = 0x06;
	motor[Motor6].ctrl.mode = 1; // 0: MITģʽ   1: λ���ٶ�ģʽ   2: �ٶ�ģʽ
	motor[Motor6].ctrl.vel_set = 1.0f;
	motor[Motor6].ctrl.kd_set = 1.0f;
}

/**
************************************************************************
* @brief:      	fdcan1_rx_callback: CAN1���ջص�����
* @param:      	void
* @retval:     	void
* @details:    	����CAN1�����жϻص������ݽ��յ���ID�����ݣ�ִ����Ӧ�Ĵ���
*               �����յ�IDΪ0ʱ������dm4310_fbdata��������Motor�ķ������ݡ�
************************************************************************
**/
void fdcan1_rx_callback(void)
{
	uint16_t rec_id;
	uint8_t rx_data[8] = {0};
	fdcanx_receive(&hfdcan1, &rec_id, rx_data);
	switch (rec_id)
	{
	case 0x11:
		dm4310_fbdata(&motor[Motor1], rx_data);
		break;
	case 0x12:
		dm4310_fbdata(&motor[Motor2], rx_data);
		break;
	case 0x13:
		dm4310_fbdata(&motor[Motor3], rx_data);
		break;
	case 0x14:
		dm4310_fbdata(&motor[Motor4], rx_data);
		break;
	case 0x15:
		dm4310_fbdata_1(&motor[Motor5], rx_data);
		break;
	case 0x16:
		dm4310_fbdata_1(&motor[Motor6], rx_data);
		break;
	}
}

void fdcan2_rx_callback(void)
{
	uint16_t rec_id;
	uint8_t rx_data[8] = {0};
	fdcanx_receive(&hfdcan2, &rec_id, rx_data);
	switch (rec_id)
	{
	case 0x11:
		dm4310_fbdata(&motor[Motor1], rx_data);
		break;
	case 0x12:
		dm4310_fbdata(&motor[Motor2], rx_data);
		break;
	case 0x13:
		dm4310_fbdata(&motor[Motor3], rx_data);
		break;
	case 0x14:
		dm4310_fbdata(&motor[Motor4], rx_data);
		break;
	case 0x15:
		dm4310_fbdata_1(&motor[Motor5], rx_data);
		break;
	case 0x16:
		dm4310_fbdata_1(&motor[Motor6], rx_data);
		break;
	}
}

void Delay(uint32_t time)
{
	while (time--)
		;
}


//// ����������滻���¼���
void MotorA_SetSpeed(float Val)
{

	speed_ctrl(&hfdcan1, motor[Motor1].id, Val);
	Delay(4800);
}

void MotorB_SetSpeed(float Val)
{
	speed_ctrl(&hfdcan1, motor[Motor2].id, Val);
	Delay(4800);
}

void MotorC_SetSpeed(float Val)
{
	speed_ctrl(&hfdcan2, motor[Motor3].id, Val);
	Delay(4800);
}

void MotorD_SetSpeed(float Val)
{
	speed_ctrl(&hfdcan2, motor[Motor4].id, Val);
	Delay(4800);
}

//// ��������С��ǰ��
void Car_RunSpeed(float Val)
{
	MotorA_SetSpeed(Val);
	MotorB_SetSpeed(-Val);
	MotorC_SetSpeed(-Val);
	MotorD_SetSpeed(Val);
}

// ��������С������
void Car_TurnSpeed(float Val)
{
	MotorA_SetSpeed(Val);
	MotorB_SetSpeed(-Val);
	MotorC_SetSpeed(Val);
	MotorD_SetSpeed(-Val);
}

// ��������С��ƽ��
void Car_TRASpeed(float Val)
{
	MotorA_SetSpeed(-Val);
	MotorB_SetSpeed(-Val);
	MotorC_SetSpeed(Val);
	MotorD_SetSpeed(Val);
}

//// �Զ������:С��ǰ���ٶ�Run_Speed,С��ƽ���ٶ�TRA_Speed,С�������ٶ�(ʹ�ýǶȻ���������̶��Ƕ�)Turn_Speed
void Car_SetSpeed(float Run_Speed, float TRA_Speed, float Turn_Speed)
{
	MotorA_SetSpeed(Run_Speed + Turn_Speed - TRA_Speed);
	MotorB_SetSpeed(-Run_Speed - Turn_Speed - TRA_Speed);
	MotorC_SetSpeed(-Run_Speed + Turn_Speed + TRA_Speed);
	MotorD_SetSpeed(Run_Speed - Turn_Speed + TRA_Speed);
}
extern IMU_Typedef IMU;
//// �ϳ��ٶȵĴ�СSpeed(��ת������ǰ�����ٶ�),����н�Angle(�����������ļн�,��С������ʱ��Ϊ��),��ת�ٶ�Turn_Speed(��ʱ��Ϊ��)
void Car_AllangleSpeed(float Speed, float Angle, float Turn_Speed)
{
	float Run_Speed, TRA_Speed;

	double angle_in_degrees = Car.Date.CarAngle - Angle; // С��λ�Ƶķ���Ƕ�(cos)

	double Angle_cos = angle_in_degrees * (3.1415926 / 180.0);			// ���Ƕ�ת��Ϊ����
	double Angle_sin = (90.0 - angle_in_degrees) * (3.1415926 / 180.0); // ���Ƕ�ת��Ϊ����
	double result_cos = cos(Angle_cos);
	double result_sin = cos(Angle_sin);

	Run_Speed = (Speed * result_cos);
	TRA_Speed = (Speed * result_sin);

	MotorA_SetSpeed(Run_Speed - Turn_Speed - TRA_Speed);
	MotorB_SetSpeed(-Run_Speed - Turn_Speed - TRA_Speed);
	MotorC_SetSpeed(-Run_Speed - Turn_Speed + TRA_Speed);
	MotorD_SetSpeed(Run_Speed - Turn_Speed + TRA_Speed);
}


 void Car_XYControl(float X_Out, float Y_Out,float Speed ,float Turn_Speed)
{

     static float degrees;
     static float result;

    result = atan2f(Y_Out, X_Out);
    // ������ת��Ϊ�Ƕ�
    degrees = result * (180.0 / 3.1415926);
	
    Car_AllangleSpeed(Speed, degrees, Turn_Speed);
}

// void Car_Control(int Run_Speed, int Turn_Speed, int TRA_Speed)
//{
//     int Zero = 987;
//     static int Speed = 0;
//     static double degrees;
//     static double result;
//     static int li;
//     static int TRA_Speed2;

//    result = atan2((double)(Run_Speed - Zero), (double)(TRA_Speed - Zero));
//    // ������ת��Ϊ�Ƕ�
//    degrees = result * (180.0 / 3.1415926);

//    Speed = (int)((double)sqrt((double)((Run_Speed - Zero) * (Run_Speed - Zero)) + (double)((TRA_Speed - Zero) * (TRA_Speed - Zero))) / 2561.0 * 300.0);

//    li = (int)((((double)Turn_Speed - 992.0) / 815.0) * 60);
//    if (li < 10 && li > -10)
//    {
//        li = 0;
//    }
//    Car_AllangleSpeed(Speed, -degrees, li);
//}

void Motor_Test()
{
	//	fdcanx_send_data(&hfdcan1, 0x520, tx_data, 8);
}


//Բ����������ֵ����Ϊֱ����������ֵ
float InfiniteVal(float Val,float* Last_Val,int* NumberFlag,float min_Endpoint,float Max_Endpoint)
{
	
	if (Val - *Last_Val < min_Endpoint)
	{
		(*NumberFlag)++;
	}
	else if (Val - *Last_Val > Max_Endpoint)
	{
		(*NumberFlag)--;
	}

	*Last_Val = Val;

	if (Max_Endpoint - min_Endpoint > 0)
	{
		return Val + *NumberFlag * (Max_Endpoint - min_Endpoint);
	}
	else
	{
		return Val + *NumberFlag * -(Max_Endpoint - min_Endpoint);
	}
	return -1;
	
}

//������λ��ģʽ��չ����Ȧ
//���������1ʱ��˵���Ѿ��ﵽĿ��λ��
//�������λ��
float Theoretical_position=0;
//���ʵ��λ��
float Actual_position=0;
//�����һ��ʵ��λ��
float Last_Actual_position=0;
//���ʵ��λ��ת����λ��������¼��׼λ
int Actual_to_Theoretical_flag=0;
//ʵ��Ŀ��ֵ
float Actual_Tagetpos=0;

void InfiniteZ_motor_Control(float pos, float vel)//posΪ����ֵ��Ŀ���ٶ�
{
	
	Actual_position=motor[Motor6].para.pos;//��ȡʵ��ֵ
	Theoretical_position=InfiniteVal(Actual_position,&Last_Actual_position,&Actual_to_Theoretical_flag,-12.5,12.5);//ת��Ϊ����ֵ
	
}


float 	Kp = 0.01;
float	Kd = 0.1;
float   ret = 0;
void Car_Control(float Speed, float SpeedAngle,float CarBodyAngle)
{

	float err = 0;

	static float Last_err;
	err = CarBodyAngle - IMU.Full_YawAngle;
	ret = Kp*err+Kd*(err-Last_err);
	Last_err = err;
	
	Car_AllangleSpeed(Speed,SpeedAngle,ret);
}


