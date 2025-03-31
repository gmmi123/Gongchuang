#include "Motor_can.h"
#include "MyCarControl.h"
#include "IMU.h"
motor_t motor[num];

void dm4310_motor_init(void)
{
	// 初始化Motor1和Motor2的电机结构
	memset(&motor[Motor1], 0, sizeof(motor[Motor1]));
	memset(&motor[Motor2], 0, sizeof(motor[Motor2]));
	memset(&motor[Motor3], 0, sizeof(motor[Motor3]));
	memset(&motor[Motor4], 0, sizeof(motor[Motor4]));
	memset(&motor[Motor5], 0, sizeof(motor[Motor5]));
	memset(&motor[Motor6], 0, sizeof(motor[Motor6]));

	// 设置Motor1的电机信息
	motor[Motor1].id = 0x01;
	motor[Motor1].ctrl.mode = 2; // 0: MIT模式   1: 位置速度模式   2: 速度模式
	motor[Motor1].ctrl.vel_set = 1.0f;
	//	motor[Motor1].ctrl.kp_set = 1.0f;
	motor[Motor1].ctrl.kd_set = 1.0f;

	motor[Motor2].id = 0x02;
	motor[Motor2].ctrl.mode = 2; // 0: MIT模式   1: 位置速度模式   2: 速度模式
								 //	motor[Motor2].ctrl.vel_set = 1.0f;
	motor[Motor2].ctrl.kd_set = 1.0f;

	motor[Motor3].id = 0x03;
	motor[Motor3].ctrl.mode = 2; // 0: MIT模式   1: 位置速度模式   2: 速度模式
	motor[Motor3].ctrl.vel_set = 1.0f;
	motor[Motor3].ctrl.kd_set = 1.0f;

	motor[Motor4].id = 0x04;
	motor[Motor4].ctrl.mode = 0; // 0: MIT模式   1: 位置速度模式   2: 速度模式
	motor[Motor4].ctrl.vel_set = 1.0f;
	motor[Motor4].ctrl.kd_set = 1.0f;

	motor[Motor5].id = 0x05;
	motor[Motor5].ctrl.mode = 1; // 0: MIT模式   1: 位置速度模式   2: 速度模式
	motor[Motor5].ctrl.vel_set = 1.0f;
	motor[Motor5].ctrl.kd_set = 1.0f;

	motor[Motor6].id = 0x06;
	motor[Motor6].ctrl.mode = 1; // 0: MIT模式   1: 位置速度模式   2: 速度模式
	motor[Motor6].ctrl.vel_set = 1.0f;
	motor[Motor6].ctrl.kd_set = 1.0f;
}

/**
************************************************************************
* @brief:      	fdcan1_rx_callback: CAN1接收回调函数
* @param:      	void
* @retval:     	void
* @details:    	处理CAN1接收中断回调，根据接收到的ID和数据，执行相应的处理。
*               当接收到ID为0时，调用dm4310_fbdata函数更新Motor的反馈数据。
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


//// 电机驱动，替换以下即可
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

//// 单独控制小车前进
void Car_RunSpeed(float Val)
{
	MotorA_SetSpeed(Val);
	MotorB_SetSpeed(-Val);
	MotorC_SetSpeed(-Val);
	MotorD_SetSpeed(Val);
}

// 单独控制小车自旋
void Car_TurnSpeed(float Val)
{
	MotorA_SetSpeed(Val);
	MotorB_SetSpeed(-Val);
	MotorC_SetSpeed(Val);
	MotorD_SetSpeed(-Val);
}

// 单独控制小车平移
void Car_TRASpeed(float Val)
{
	MotorA_SetSpeed(-Val);
	MotorB_SetSpeed(-Val);
	MotorC_SetSpeed(Val);
	MotorD_SetSpeed(Val);
}

//// 自定义控制:小车前进速度Run_Speed,小车平移速度TRA_Speed,小车自旋速度(使用角度环的输出来固定角度)Turn_Speed
void Car_SetSpeed(float Run_Speed, float TRA_Speed, float Turn_Speed)
{
	MotorA_SetSpeed(Run_Speed + Turn_Speed - TRA_Speed);
	MotorB_SetSpeed(-Run_Speed - Turn_Speed - TRA_Speed);
	MotorC_SetSpeed(-Run_Speed + Turn_Speed + TRA_Speed);
	MotorD_SetSpeed(Run_Speed - Turn_Speed + TRA_Speed);
}
extern IMU_Typedef IMU;
//// 合成速度的大小Speed(旋转过程中前进的速度),方向夹角Angle(世界绝对坐标的夹角,以小车的逆时针为正),旋转速度Turn_Speed(逆时针为正)
void Car_AllangleSpeed(float Speed, float Angle, float Turn_Speed)
{
	float Run_Speed, TRA_Speed;

	double angle_in_degrees = Car.Date.CarAngle - Angle; // 小车位移的方向角度(cos)

	double Angle_cos = angle_in_degrees * (3.1415926 / 180.0);			// 将角度转换为弧度
	double Angle_sin = (90.0 - angle_in_degrees) * (3.1415926 / 180.0); // 将角度转换为弧度
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
    // 将弧度转换为角度
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
//    // 将弧度转换为角度
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


//圆型坐标轴数值修正为直线坐标轴数值
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

//达妙电机位置模式扩展无限圈
//当电机返回1时，说明已经达到目标位置
//电机理论位置
float Theoretical_position=0;
//电机实际位置
float Actual_position=0;
//电机上一次实际位置
float Last_Actual_position=0;
//电机实际位置转理论位置数量记录标准位
int Actual_to_Theoretical_flag=0;
//实际目标值
float Actual_Tagetpos=0;

void InfiniteZ_motor_Control(float pos, float vel)//pos为理论值的目标速度
{
	
	Actual_position=motor[Motor6].para.pos;//获取实际值
	Theoretical_position=InfiniteVal(Actual_position,&Last_Actual_position,&Actual_to_Theoretical_flag,-12.5,12.5);//转换为理论值
	
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


