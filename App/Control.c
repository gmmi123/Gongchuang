#include "Control.h"
#include "Sever_control.h"
#include "Buzzer.h"
#include "usart.h"
#include "string.h"
#include "PID.h"
#include "UsartDate.h"
#include "Control_Plus.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 树莓派变量
//X325,Y230  1:红,2:蓝,3:绿
#define SM_TargetX 324
#define SM_TargetY 230
#define SM_Red 1
#define SM_Green 1
#define SM_Blue 1
extern SMTypeDef SM;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//机械臂相关变量
extern float Arm_Zero;				  // 机械臂零点
extern float Motor_InfiniteZ;		  // 机械臂Z轴
extern uint8_t Xianweikaiguan_Status; // 限位开关标志位
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 小车控制相关变量
//  世界绝对坐标变量
float World_X = 0;
float World_Y = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 机械臂控制相关变量
float tempY = -3.15; // 顺时针点3 -5.75/-5.8  顺时针点2 -6.26/-6.35  逆时针点2 -0.05  逆时钟点1 -0.555
float tempZ = 0;	 // 放在车上Arm_Zero+2.8/2.5,放到地下的原料+20   放原料堆叠+8    夹取车上物体+3.2    夹原料+5
float tempj2 = 0;
int tempk = 1;
int staute_test = 0;

// 工作区间的范围
#define WorkRange_L -3.65
#define WorkRange_R -2.65
// 机械臂水平转动位置（云台）:用Y来表示
#define Arm_PositionY motor[Motor5].para.pos
#define Arm_PositionZ Motor_InfiniteZ // 使用转化后的数据
// 点位
#define WorkPosition -3.15
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 以下为机械臂的控制动作
// 将机械臂云台的旋转方向分为4个方向:工作方向,存储方向1,2,3
// 控制机械臂到工作方向








//$机械臂
//机械臂移动到工作区
int Arm_WorkDirection(void)
{


	pos_speed_ctrl(&hfdcan1, motor[Motor5].id, -3.15, 1);
	if (motor[Motor5].para.pos > -3.15 - 0.1 && motor[Motor5].para.pos < -3.15 + 0.1)
	{
		return 1; // 到达指定位置返回1
	}

	return 0;
}
// 控制机械臂上升复位
int Arm_Up()
{
	if (Xianweikaiguan_Status == 2)
	{
		pos_speed_ctrl(&hfdcan2, motor[Motor6].id, Arm_Zero - 20, 0);
		return 1;
	}
	else
	{
		pos_speed_ctrl(&hfdcan2, motor[Motor6].id, Arm_Zero - 20, 20);
		return 0;
	}
}
// 控制机械臂水平转动
void Arm_MovementY(float Val) 
{
	pos_speed_ctrl(&hfdcan1, motor[Motor5].id, Val, 2); // 2
}
 // 控制机械臂上下移动
void Arm_MovementZ(float Val)
{
	pos_speed_ctrl(&hfdcan2, motor[Motor6].id, Arm_Zero + Val, 20);
}
// 检测机械臂Z轴是否到指定位置
int Arm_CheckPosition_Z(float Val)
{
	if (Arm_PositionZ > Arm_Zero + Val - 0.1 && Arm_PositionZ < Arm_Zero + Val + 0.1)
	{
		return 1;
	}
	return 0;
}
// 检测机械臂Y轴是否到指定位置
int Arm_CheckPosition_Y(float Val)
{
	if (Arm_PositionY > Val - 0.1 && Arm_PositionY < Val + 0.1)
	{
		return 1;
	}
	return 0;
}

//机械臂一键抓取动作
extern uint8_t Status1;
int Arm_Get_Block(float Target_Position)//Target_Position是抓取方块后的方块放入的位置
{
	static float TempY = -3.15;
	static float TempZ = 0;
	static uint8_t status = 0;
	static int Cont=0;//记录抓取次数
	if (status == 0) // 抓取前的准备状态
	{
		// 先打开机械爪
//		Control_Sever_On();
		// 移动到工作区
//		TempY = WorkPosition;
//		TempZ = -1; // 让出控制权
//		// 机械臂上升
//		int temp = Arm_Up();
		// 切换状态
		if (Arm_CheckPosition_Y(WorkPosition))
		{
			status = 1;
		}
	}
	else if (status == 1) // 下降机械爪
	{
		// 下降
		TempZ = 6;
//		if (Arm_CheckPosition_Z(6)) // 检测到达位置切换状态
//		{
			status = 2;
//		}
	}
	else if (status == 2) // 抓取物块
	{
		static uint8_t time=25;
//		Control_Sever_Off();
		if(!(time--))
		{
			time=25;
			status = 3;
		}

		
	}
	else if (status == 3) // 上升
	{

		//加速
		if(Cont==2&&Arm_PositionZ<Arm_Zero+5)
		{
			Status1 = 5;
		}
		TempZ = -1;	  // 让出控制权
		
		if (Arm_Up()) // 上升机械臂
		{
			status = 4;
		}
	}
	else if (status == 4) // Y移动到指定位置
	{
		TempY = Target_Position;
		if (Arm_CheckPosition_Y(TempY)) // 到达位置切换状态
		{
			status = 5;
		}
	}
	else if (status == 5) // Z移动到指定的车上位置
	{
		TempZ = 3;
		if (Arm_CheckPosition_Z(TempZ)) // 到达位置切换状态
		{
			status = 6;
		}
	}
	else if (status == 6) // 放置物块
	{
//		Control_Sever_On();
		status = 7;
	}
	else if (status == 7)
	{
		// 上升
		TempZ = -1; // 让出控制权
		if (Arm_Up())
		{
			status = 8;
		}
	}
	else if (status == 8)
	{
		TempY = WorkPosition; // 归位
		if (Arm_CheckPosition_Y(TempY))
		{
			status = 9;
		}
	}
	else if (status == 9)
	{
		// 处理后事
		TempY = -1;
		TempZ = -1;
		status = 0;
		Cont++;
		if(Cont==3)
		{
			Cont=0;
		}
		return 1;
	}
	if (TempY != -1) // 失能控制,让出控制权
	{
		Arm_MovementY(TempY);
	}

	if (TempZ != -1) // 失能控制,让出控制权（有Arm_Up（）函数都要让出控制权）
	{
		Arm_MovementZ(TempZ);
	}

	return 0;
}

//机械臂一键放置方块动作
int Arm_Place_Block(float Target_Position)//Target_Position是要抓取的方块的位置
{
	static float TempY = -3.15;
	static float TempZ = 0;
	static uint8_t status = 0;

	if (status == 0) // 抓取前的准备状态
	{
		// 先打开机械爪
//		Control_Sever_On();
		// 移动到工作区
		TempY = WorkPosition;
		TempZ = -1; // 让出控制权
		// 机械臂上升
		int temp = Arm_Up();
		// 切换状态
		if (Arm_CheckPosition_Y(WorkPosition) && (temp))
		{
			status = 1;
		}
	}
	else if (status == 1) // 移动到车上指定的位置
	{
		TempY = Target_Position;
		if (Arm_CheckPosition_Y(TempY)) // 到达指定位置
		{
			status = 2;
		}
	}
	else if (status == 2) // 下降到指定高度
	{
		TempZ = 4;
		if (Arm_CheckPosition_Z(TempZ)) // 到达指定位置
		{
			status = 3;
		}
	}
	else if (status == 3) // 闭合机械爪，夹取物块
	{
//		Control_Sever_Off();
		status = 4;
	}
	else if (status == 4) // 上升机械臂
	{
		TempZ = -1; // 使能控制
		if (Arm_Up())
		{
			// 到顶切换状态
			status = 5;
		}
	}
	else if (status == 5) // 回到工作区
	{
		TempY = WorkPosition;
		if (Arm_CheckPosition_Y(TempY))
		{
			// 回到工作区后切换状态
			status = 6;
		}
	}
	else if (status == 6)
	{
		// 下降到指定高度
		TempZ = 20;
		if (Arm_CheckPosition_Z(TempZ))
		{
			status = 7;
		}
	}
	else if (status == 7) // 放置方块
	{
//		Control_Sever_On();
		status = 8;
	}
	else if (status == 8) // 上升
	{
		TempZ = -1; // 让出控制权
		if (Arm_Up())
		{
			status = 9;
		}
	}
	else if (status == 9) // 处理后事
	{
		TempY = -1;
		TempZ = -1;
		status = 0;
		return 1;
	}
	if (TempY != -1) // 失能控制,让出控制权
	{
		Arm_MovementY(TempY);
	}

	if (TempZ != -1) // 失能控制,让出控制权（有Arm_Up（）函数都要让出控制权）
	{
		Arm_MovementZ(TempZ);
	}

	return 0;
}

//$底盘
//底盘电机超时错误清除
// 开始跑图前先初始化电机，清除电机的错误
void Motor_ControlInit(void)
{
	dm4310_clear_err(&hfdcan1, &motor[Motor1]);
	HAL_Delay(1);
	dm4310_clear_err(&hfdcan1, &motor[Motor2]);
	HAL_Delay(1);
	dm4310_clear_err(&hfdcan2, &motor[Motor3]);
	HAL_Delay(1);
	dm4310_clear_err(&hfdcan2, &motor[Motor4]);
	HAL_Delay(1);

	dm4310_enable(&hfdcan1, &motor[Motor1]);
	HAL_Delay(1);
	dm4310_enable(&hfdcan1, &motor[Motor2]);
	HAL_Delay(1);
	dm4310_enable(&hfdcan2, &motor[Motor3]);
	HAL_Delay(1);
	dm4310_enable(&hfdcan2, &motor[Motor4]);
	HAL_Delay(100);
}

//$追踪
//全局变量方便调试
//使用摄像头返回的坐标追踪圆圈或者方块
PID_TypeDef PD_X, PD_Y;//使用师兄PID库函数的变量
float  range_error= 1000;//距离误差
float XYPD_position_control(float Measure_X, float Measure_Y,float TargetX,float TargetY)//输入转换后的坐标测量值,输出角度
{
	// 初始化参数

	// PID参数
	static uint8_t XYPD_position_control_status = 0;
	static float KP_X = 20;
	static float KD_X = 0;
	static float Out_X_max = 100000;
	//
	static float KP_Y = -20;
	static float KD_Y = 0;
	static float Out_Y_max = 100000;
	//
	
	if (XYPD_position_control_status == 0) // PD初始化
	{
		PID_Init(&PD_X);
		PID_Init(&PD_Y);
		Set_PID(&PD_X, KP_X, 0, KD_X);
		Set_PID(&PD_Y, KP_Y, 0, KD_Y);
		// 输出限幅
		PD_X.PID_OUT_Limit = Out_X_max;
		PD_Y.PID_OUT_Limit = Out_Y_max;
		// 切换状态
		XYPD_position_control_status = 1;
	}
	if (XYPD_position_control_status == 1)
	{
		// 获取测量值（即获取世界坐标）
		PD_X.Current_Value = (float)Measure_X;
		PD_Y.Current_Value = (float)Measure_Y;
		// PID计算
		Position_PID(&PD_X, TargetX);
		Position_PID(&PD_Y, TargetY);
		
		range_error = sqrtf((PD_X.PID_Out * PD_X.PID_Out) + (PD_Y.PID_Out * PD_Y.PID_Out));
	}

	return   -atan2f(PD_Y.PID_Out, PD_X.PID_Out) * (180.0 / 3.1415926);

   
}


//$树莓派
// 获取树莓派摄像头数据(圆环和方块):圆环和方块返回的数据类型一样，用一个函数即可
int Raspberry_PI_camerDate(char DateType)//需要的数据类型
{

	int SM_Status = 0; // 树莓派信息状态

	HAL_UART_Transmit_IT(&huart9, (const uint8_t *)&DateType, 1); // 发送信息请求圆圈数据
	SM_Status = SM_Parses(UART9_RX_Second_BUF);
	if (SM_Status == SM_DateUpdate)
	{
		return SM_DateUpdate; // 表示数据已经更新
	}
	else if (SM_Status == SM_unidentified)
	{
		return SM_unidentified; // 树莓派没有识别到
	}
	return SM_DateReceive_error; // 数据没有接收
}
//$机械臂与小车底盘联合控制
//机械臂与小车联合控制动作
extern uint8_t Status1;
//识别物块原料台上的物块,结果是小车移动到物块的位置（会追踪物块跑）
int Identifier_Masse(int color)//
{
	static int Cont=0;//记录抓取次数
	static float Speed = 0;//小车移动速度
	static float Angle = 0;//小车移动角度  //+90左 -90右 （以抓取视角来看）
	static float Turn_Speed = 0;//小车自转速度
	static float TempY = -3.15;
	static float TempZ = 0;
	static uint8_t Status =0;//状态机标志位
	if(Status==0)//识别前准备工作
	{
		//移动机械臂到工作区

		if(RobotArm_MovementZ(0,20))//Z轴复位完毕
		{
			//打开机械爪
//			Control_Sever_rec();
			//移动到工作区
			if(Arm_WorkDirection())
			{
				Status=1;
			}
		}
	}
	if(Status==1)
	{
		//下降识别物块
		TempZ=6;
		if (Arm_CheckPosition_Z(TempZ)) // 到达指定位置
		{
			if(Raspberry_PI_camerDate(SM_Masse)==SM_DateUpdate)//小车开始调整   （Y轴数据取负号转为小车坐标系）
			{
				Angle=XYPD_position_control(SM.SM_X,-SM.SM_Y,SM_TargetX,-SM_TargetY);//输入测量角度（给Y轴加负号转换坐标系），得到角度
				
				if(range_error>150)//速度限幅
				{
					if(Cont==0||Cont==3)
					{
						Speed=range_error/1500.0*1;
						if(Speed<0.1)
						{
							Speed=0.1;
						}
					}

				}
				else
				{
					Speed=0;
					Status=2;//坐标定位完成
					
				}

			}
		}

	}
	if(Status==2)
	{
		if(Raspberry_PI_camerDate(SM_Masse)==SM_DateUpdate)//识别颜色
		{
			if(SM.SM_coloar==color)
			{
//				Control_Sever_On();
				//机械爪夹物块，释放控制权
				TempY=-1;
				TempZ=-1;
			
				Status=3;
				
				
			}
		}

	}
	else if(Status==3)//处理尾事
	{
		Cont++;
		TempY=-1;
		TempZ=-1;
		Status=0;
		return 1;
	}
	
	
	
	if (TempY != -1) // 失能控制,让出控制权
	{
		Arm_MovementY(TempY);
	}

	if (TempZ != -1) // 失能控制,让出控制权（有Arm_Up（）函数都要让出控制权）
	{
		Arm_MovementZ(TempZ);
	}
	
	Car_AllangleSpeed(Speed, Angle, Turn_Speed);
	return 0;
}


//识别地上的圆圈，最终结果是对准圆圈
int Identifier_circle(void)
{
	static float Speed = 0;//小车移动速度
	static float Angle = 0;//小车移动角度  //+90左 -90右 （以抓取视角来看）
	static float Turn_Speed = 0;//小车自转速度
	static float TempY = -1;//机械臂Y轴
	static float TempZ = -1;//机械臂Z轴
	static uint8_t Status =0;//状态机标志位
	if(Status==0)//识别前准备工作
	{
		//移动机械臂到工作区
		TempZ=-1;
		Arm_Up();//移动机械臂上升
		if(Xianweikaiguan_Status==2)//Z轴复位完毕
		{
			//打开机械爪
//			Control_Sever_rec();
			//移动到工作区
			if(Arm_WorkDirection())
			{
				Status=1;
			}
		}
	}
	if(Status==1)
	{
		//下降识别圆
		TempZ=15;
		if (Arm_CheckPosition_Z(TempZ)) // 到达指定位置
		{
			if(Raspberry_PI_camerDate(SM_Circle)==SM_DateUpdate)//小车开始调整   （Y轴数据取负号转为小车坐标系）
			{
				Angle=XYPD_position_control(SM.SM_X,-SM.SM_Y,SM_TargetX,-SM_TargetY);//输入测量角度（给Y轴加负号转换坐标系），得到角度
				
				if(range_error>5)//速度限幅
				{
					Speed=range_error/1500.0*1;
					if(Speed<0.1)
					{
						Speed=0.1;
					}
				}
				else
				{
					Speed=0;
					Status=2;
				}

			}
		}

	}
	if(Status==2)
	{
		//机械爪复位
		TempZ=-1;
//		Control_Sever_On();
		Arm_Up();//机械臂上升
		if(Xianweikaiguan_Status==2)
		{
			Status=3;
		}
	}
	if(Status==3)//处理尾事
	{
		TempY=-1;
		TempZ=-1;
		Status=0;
		return 1;
	}
	
	
	
	if (TempY != -1) // 失能控制,让出控制权
	{
		Arm_MovementY(TempY);
	}

	if (TempZ != -1) // 失能控制,让出控制权（有Arm_Up（）函数都要让出控制权）
	{
		Arm_MovementZ(TempZ);
	}
	
	Car_AllangleSpeed(Speed, Angle, Turn_Speed);
	return 0;
}






//$测试区
//以下为测试区
uint8_t arr[] = "object";
void Control_Test(void)
{
	static uint8_t staute = 0;
	if (staute == 0)
	{

		if (Arm_WorkDirection())
		{
			staute = 1;
		}
	}

	//		XYPD_position_control(Target_X,Target_Y);
//	if (staute == 1)
//	{
//		if(Identifier_Masse(3))
//		{
//			staute = 2;

//		}

//	}
//	if(staute==2)
//	{
//		if(Arm_Get_Block(Get_sequence1))
//		{
//			staute=3;
//		}
//	}
//	if(staute==3)
//	{
//		if(Identifier_Masse(2))
//		{ 
//			staute = 2;

//		}
//	}
//	

	if (staute_test == 2)
	{
		Arm_Up();
	}
	if (staute_test == 1)
	{
		Arm_MovementY(tempY);
		Arm_MovementZ(tempZ);
	}

	if (tempk == 0)
	{
//		Control_Sever_Off();
		Claw_Off(&Claw);
	}
	if (tempk == 1)
	{
//		Control_Sever_On();
		Claw_On(&Claw);
	}
	if(tempk == 2)
	{
//		Control_Sever_rec();
		Claw_rec(&Claw);
	}
}

//


//	static uint8_t staute = 0;
//	if (staute == 0)
//	{

//		if (Arm_WorkDirection())
//		{
//			staute = 1;
//		}
//	}
//	if (staute == 1)
//	{
//		if (Arm_Get_Block(Get_sequence1))
//		{
//			staute = 2;
//		}
//	}
//	if (staute == 2)
//	{

//		if (Arm_Get_Block(Get_sequence2))
//		{
//			staute = 3;
//		}
//	}
//	if (staute == 3)
//	{

//		if (Arm_Get_Block(Get_sequence3))
//		{
//			staute = 4;
//		}
//	}
//	if (staute == 4)
//	{
//		if (Arm_Place_Block(Place_sequence1))
//		{
//			staute = 5;
//		}
//	}
//	if (staute == 5)
//	{

//		if (Arm_Place_Block(Place_sequence2))
//		{
//			staute = 6;
//		}
//	}
//	if (staute == 6)
//	{

//		if (Arm_Place_Block(Place_sequence3))
//		{
//			staute = 7;
//		}
//	}
