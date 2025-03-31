#include "Control_Task.h"
#include "Motor_can.h"
#include "math.h"
#include "Control.h"
#include "IMU.h"
#include "Buzzer.h"
#include "Control_Plus.h"
//二维码信息
//抓取的颜色信息
extern char contentMV[20];
#define two_dimensional_1_grab  contentMV[1]  
#define two_dimensional_2_grab  contentMV[2]
#define two_dimensional_3_grab  contentMV[3]
#define two_dimensional_4_grab  contentMV[5]  
#define two_dimensional_5_grab  contentMV[6]
#define two_dimensional_6_grab  contentMV[7]
//mv数据
extern char contentMV[20];

extern uint8_t Xianweikaiguan_Status;//限位开关标志外   2为撞击完成(复位完成)
extern float Motor_InfiniteZ;//电机Z轴数值
extern motor_t motor[num];// 电机参数读取
extern float yaw_mahony;// 陀螺仪角度
// 世界绝对坐标变量
extern float World_X;
extern float World_Y;
extern float Target_X;
extern float Target_Y;

uint8_t Robotic_arminit_Status = 0; // 机械臂初始化标志位,0为未初始化,1为初始化完成
//机械臂上升零点
float Arm_Zero=0;


// 角度转弧度
float degrees_to_radians(float degrees)
{
	return degrees * (3.1415926 / 180.0);
}

// 弧度转角度
float radians_to_degrees(float radians)
{
	return radians * (180.0 / 3.1415926);
}

// 路程计算任务（与控制任务在同一个定时器中断）
void Distance_calcu_Task(void)
{
	//	// 陀螺仪角度转弧度
	float IMU_radians_Y = degrees_to_radians(IMU.YawAngle); // 小车坐标系的Y(以小车前进的方向为Y)
	float IMU_radians_X = degrees_to_radians(yaw_mahony);	// 小车坐标系的X(从前进的方向看,右手是X)
	// 记录路程在绝对坐标下的X轴.Y轴

	float Car_Vy = (motor[Motor1].para.vel + (-motor[Motor2].para.vel) + (-motor[Motor3].para.vel) + motor[Motor4].para.vel);
	float Car_Vx = -(motor[Motor1].para.vel - (-motor[Motor2].para.vel) + (-motor[Motor3].para.vel) - motor[Motor4].para.vel);
	World_Y += Car_Vy * sinf(IMU_radians_Y) + Car_Vx * sinf(IMU_radians_X);
	World_X += Car_Vx * cosf(IMU_radians_X) + Car_Vy * cosf(IMU_radians_Y);
}

#define CarSpeed 10   

uint8_t Arm_PlaceStatus=1;//0为暂停，1为工作中状态，2为工作完成状态
uint8_t Arm_PlaceCont =0;//抓取次数
void Control_Task_2() //云台任务,控制机械臂
{
	if(Arm_PlaceStatus==1)//工作状态
	{	
		if(Arm_PlaceCont==1)//第1次抓取
		{
			if(Arm_Get_Block(Get_sequence1))
			{
				Arm_PlaceStatus=2;
			}
		}
		else if(Arm_PlaceCont==2)//第2次抓取
		{
			if(Arm_Get_Block(Get_sequence2))
			{
				Arm_PlaceStatus=2;
			}
		}
		else if(Arm_PlaceCont==3)//第3次抓取
		{
			if(Arm_Get_Block(Get_sequence3))
			{
				Arm_PlaceStatus=2;
			}		
		}

	}
	else if(Arm_PlaceStatus==2)//复位
	{
		Arm_PlaceStatus=0;
	}
	
}

int PlaceTemp(int Val)//输入第n个方块
{
	if(Arm_PlaceStatus==0)//确定机械臂没有在工作
	{
		Arm_PlaceStatus=1;//开始抓方块
	}
	Arm_PlaceCont=Val;//抓第n个方块
	if(Arm_PlaceStatus==2)//抓完方块
	{
		return 1;
	}
	return 0;
}

uint8_t Status1=0;

void SetBlockColor(void)
{
	for(int j=0;j<15;j++)
	{
		if(j%3==0)
		{
			Block_Date[j][Color]= contentMV[1];
		}
		if(j%3==1)
		{
			Block_Date[j][Color]= contentMV[2];
		}
		if(j%3==2)
		{
			Block_Date[j][Color]= contentMV[3];
		}
	}


	

}
#define SpeedCar Car.CarControl.Speed
#define AngleCar Car.CarControl.Angle
int BlockTask=0;
void Control_Task_1(void) // 主任务，跑路线
{	

	
	static int time = 0;
	if (Status1 == 0) 
	{
		SpeedCar= CarSpeed;// 出库
		AngleCar= 135.0;
		RobotArm.RobotArm_SpeedY = 1;//展开机械臂看向二维码
		RobotArm.RobotArm_TargetY = -3.14;
		if (World_X <= -450)
		{
			Status1 = 1;
		}
	} 
	if (Status1 == 1) // 前往扫码码
	{
		SpeedCar= CarSpeed;
		AngleCar= 90.0;
		if (World_Y >= 2000)
		{

				Status1 = 2;
		}
	}
	if (Status1 == 2) // 到达扫码
	{
		Car.CarControl.Speed= 0;
		if(contentMV[4]=='+')
		{
			SetBlockColor();
			Status1 = 3;//识别到码，前往抓物
		}
		
	}
	if (Status1 == 3) // 前往抓物
	{
		SpeedCar= CarSpeed;
		AngleCar= 90.0;
		if (World_Y >= 4300)
		{
			Status1 = 4;
			SpeedCar=0;
		}
	}
	if (Status1 == 4) // 到达抓物
	{
			
		if(Block[BlockTask].InitStatus==Init_Non)
		{
			Block_Init(&Block[BlockTask],Block_Date[BlockTask]);
		}
		if(RobotArm_Block_Movement(&Block[BlockTask]))
		{
			BlockTask++;
			if(BlockTask==3)
			{
				Status1=5;
			}
		}

		
	}
	if (Status1 == 5) // 回头
	{
		Car.CarControl.Speed= CarSpeed;
		Car.CarControl.Angle= -90.0;
		if (World_Y <= 3400)
		{
			Status1 = 6;
			Car.CarControl.Speed= 0;
		}
	}
	if (Status1 == 6) // 转弯
	{
		Car.CarControl.Speed= 0;
		Car.CarControl.Turn_Speed= 5;
		if (IMU.Full_YawAngle>= 175)
		{
			Car.CarControl.Turn_Speed= 0;
			Status1 = 7;
		}
	}
	if (Status1 == 7) // 前往加工区
	{
		Car.CarControl.Angle= -180;
		Car.CarControl.Speed= CarSpeed;
		Car.CarControl.Turn_Speed= 0;
		RobotArm.RobotArm_TargetY=-3.14;
		if (World_X <= -5700)
		{
			Status1 = 8;
			Car.CarControl.Speed= 0;
		}
	}
	if (Status1 == 8) // 旋转准备加工
	{
		Car.CarControl.Turn_Speed= 5;
		if (IMU.Full_YawAngle>= 265)
		{
			Car.CarControl.Turn_Speed= 0;
			Status1 = 9;
		}
	}
	if (Status1 == 9) // 加工
	{
//		Car_TraceCircle();
		if(Block[BlockTask].InitStatus==Init_Non)
		{
			Block_Init(&Block[BlockTask],Block_Date[BlockTask]);
		}
		
		if(RobotArm_Block_Movement(&Block[BlockTask]))
		{
			BlockTask++;
			if(BlockTask==Stack_First_LayerTask1)
			{
				Status1=10;
			}
		}
//		Status1 = 10;
	}
	if (Status1 == 10) // 走直线
	{
		Car.CarControl.Speed= CarSpeed;
		Car.CarControl.Angle= 90;
		if (World_Y >= 5600)
		{
			Car.CarControl.Speed= 0;
			Status1 = 11;
		}
	}
	if (Status1 == 11) // 转弯
	{
		Car.CarControl.Turn_Speed= -5;
		if (IMU.Full_YawAngle<= 180)
		{
			Car.CarControl.Turn_Speed= 0;
			Status1 = 12;
		}
	}
	if (Status1 == 12) // 走到放置区
	{
		Car.CarControl.Speed= CarSpeed;
		Car.CarControl.Angle= 0;
		if (World_X >= -3000)
		{
			Car.CarControl.Speed= 0;
			Status1 = 13;
		}
	}
	if (Status1 == 13) // 放置
	{
//		RobotArm.RobotArm_TargetY=-3.14;
//		Car_TraceCircle();
		if(Block[BlockTask].InitStatus==Init_Non)
		{
			Block_Init(&Block[BlockTask],Block_Date[BlockTask]);
		}
		
		if(RobotArm_Block_Movement(&Block[BlockTask]))
		{
			BlockTask++;
			if(BlockTask==Stack_Second_LayerTask1)
			{
				Status1=14;
			}
		}

	}
	if (Status1 == 14) // 直线
	{
		Car.CarControl.Speed= CarSpeed;
		Car.CarControl.Angle= 0;
		if (World_X >= -300)
		{
			Car.CarControl.Speed= 0;
			Status1 = 15;
		}
	}
	if (Status1 == 15) // 转弯
	{
		Car.CarControl.Turn_Speed= -5;
		if (IMU.Full_YawAngle< 90)
		{
			Car.CarControl.Turn_Speed= 0;
			Status1 = 16;
		}
	}
	if (Status1 == 16) // 直线
	{
		Car.CarControl.Speed= CarSpeed;
		Car.CarControl.Angle= -90;
		if (World_Y <= 4300)
		{

			Car.CarControl.Speed= 0;
		}
	}


		

}

// 机械臂初始化
uint8_t Robotic_arminit_Task(void)
{

	if (Robotic_arminit_Status == 0) // 机械臂开始初始化
	{
		// 判断机械臂是否在零点,不在时先复位
		if (Xianweikaiguan_Status == 0)
		{
//			// 上电机械臂上升
			pos_speed_ctrl(&hfdcan2, motor[Motor6].id, -34, 6);
//			speed_ctrl(&hfdcan2, motor[Motor6].id,0.5);
		}
		if (Xianweikaiguan_Status != 0) // 机械臂复位完成
		{
			

			Arm_Zero=Motor_InfiniteZ;// 设置零点
			Robotic_arminit_Status = 1;
		}
		return 1;
	}
	if (Robotic_arminit_Status == 1) // 云台底电机复位
	{
		if (motor[Motor5].para.pos < 0)
		{
			pos_speed_ctrl(&hfdcan1, motor[Motor5].id, -0.1, 1);
		}
		if (motor[Motor5].para.pos > 0)
		{
			Buzzer_On();
		}

		//		save_pos_zero(&hfdcan1,motor[Motor5].id,motor[Motor5].cmd.mode);
		if (motor[Motor5].para.pos > -0.1-0.1 && motor[Motor5].para.pos < -0.1+0.1)
		{
			return 2;
		}
		return 1;
	}

	return -1; // 错误
}

