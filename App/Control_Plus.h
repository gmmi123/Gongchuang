#ifndef __Control_Plus_H
#define __Control_Plus_H

//Control_Plus对控制系统进行优化，目的使控制更为简便，调试更方便
//对机械臂，机械爪，小车底盘，Openmv，树莓派，串口屏的变量进行封装，确避免耦合

//设备状态
typedef enum
{
	Device_Disable = 0,
	Device_Initing = 1,
	Device_Running = 2
}Device_status;
//任务完成情况
typedef enum
{
	Task_NonStart = 0,
	Task_Runnig = 1,
	Task_Complete = 2
}Task_status;
//初始化结构体
typedef enum
{
	Init_Non=0,//未初始化
	Init_Enable=1,//已初始化
}InitStatusTypeDef;

//机械爪结构体
typedef struct
{
		int 		Claw_PZ1;
		int 		Claw_PZ2;
		float 		Claw_AngleVal;
	Device_status 	Claw_Status;//0失能，1初始化状态，2运行状态
}ClawTypeDef;
//机械臂结构体
typedef struct
{
		float 		RobotArm_MeasureZ;			//Z轴机械臂的当前值
		float 		RobotArm_TargetZ;			//Z轴机械臂要移动到的目标位置
		float 		RobotArm_SpeedZ;
		float 		RobotArm_MeasureY;
		float 		RobotArm_TargetY;
		float 		RobotArm_SpeedY;
	Device_status 	RobotArm_Status;

}RobotArmTypeDef;
typedef enum
{
	Reg=1,
	Blue=2,
	Green=3
	
}BlockColorTypeDef;
typedef enum
{
	Block_NotStart=0,
	Block_Readygrab=1,//准备抓取
	Block_Identifier=2,//识别
	Block_Grasping=3,//抓取中
	Block_Reload=4,//重新复位
	Block_Placing=5,//放置中
	Block_Reloadagain=6,
	Block_End=7
}BlockStatusTypeDef;



//识别模式下的运动控制标志位
typedef enum
{
	Movement_Non,
	Movement_traceYLMasse_Movement,//追踪原料区上的物块
	Movement_traceMasse_Movement,//追踪地上的方块
	Movement_traceMasse_nonMovement,//不追踪原料区上的物块
	Movement_traceCircle_Movement,
	Movement_End,//结束识别

}Identifier_MovementFlagTypeDef;

typedef struct
{
	float Identifier_Z;
	float Y1;
	float Y1_Speed;
	float Z1;	
	float Z1_Speed;
	float Y2;
	float Y2_Speed;
	float Z2;
	float Z2_Speed;
	float Identifier_Y;
	Identifier_MovementFlagTypeDef Movement_flag;
	
}BlockCoordinatesTypeDef;






//小车信息
typedef struct
{
	float MotorA;//A轮速度
	float MotorB;
	float MotorC;
	float MotorD;
	float Ab_X;//绝对坐标系的X
	float Ab_Y;//绝对坐标系的Y
	float TargetAngle;
	float CurrentAngle;
	float TargetSpeed;
}CarDateTypeDef;




//坐标系
typedef enum
{
	Absolute_Coordinate_System=0,//绝对坐标系（以小车的初始化方向有关）
	Car_Coordinate_System=1,//小车坐标系
	
}Coordinate_SystemTypeDef;

typedef struct
{
	float Angle;  //小车移动方向
	float Speed;  //小车移动速度
	float Turn_Speed; //小车自转速度
	Coordinate_SystemTypeDef Coordinate_System;//不同参考系下的控制不一样
}CarControlTypeDef;
typedef struct
{
	CarDateTypeDef 		CarDate;
	CarControlTypeDef 	CarControl;
	Device_status 		Car_Status;
}CarTypeDef;

typedef enum
{
	Index_Car_Speed,
	Index_Car_Angle,
	Index_Car_Turn_Speed,
	Index_Car_TargetX,//触发下一个任务的目标X和Y
	Index_Car_TargetY,
	Index_Car_TargetAngle,//小车要保持的角度
	CarArr_DateIndex,
}CarArr_Date;

typedef enum
{
	Index_lineTask,
	CarArr_TaskIndex,
}CarArr_Task;


//存储小车运动一次的信息
typedef struct
{
	//从A移动到B
	BlockCoordinatesTypeDef Coordinates;//坐标和速度信息
	BlockColorTypeDef Block_Color;//物块颜色
	Task_status Completion_status;//任务完成情况
	BlockStatusTypeDef BlockStatus;//运行状态
	InitStatusTypeDef InitStatus;//信息初始化
	
}CarMessageTypeDef;

typedef struct
{
	float Angle;
	float Speed;
	float Err;
}XY_ReturnDateTypeDef;

typedef struct
{
	
	float X_Measure;
	float Y_Measure;
	float DistanceVal;
	InitStatusTypeDef InitStatus;
}Distance_TypeDef;

typedef struct
{
	BlockColorTypeDef MeasureColor;
	BlockColorTypeDef TargetColor;
	InitStatusTypeDef InitStatus;
}TraceColor_TypeDef;

typedef struct
{
	Distance_TypeDef	Distance;
	TraceColor_TypeDef	TraceColor;
}TraceColor_and_DistanceTypeDef;

typedef enum
{
	Trace_Unstart,
	Trace_SeekTarget,
	Trace_Location,
	Trace_End,

}BlockTraceStatusTypeDef;

typedef struct
{
	BlockTraceStatusTypeDef BlockTraceStatus;
	TraceColor_and_DistanceTypeDef 	TraceColor_and_Distance;
}BlockTraceTypeDef;

typedef enum
{
	Color,		//前面几个为索引
	Y1,
	Y1_Speed,
	Z1,
	Z1_Speed,
	Y2,
	Y2_Speed,
	Z2,
	Z2_Speed,
	Z_Identifier,
	MovementFlag,
	Y_Identifier,
	BlockArr_Date,//最后一行占位
}BlockDateArr_Indexs;

typedef enum
{
	Grab_to_CartTask1,
	Grab_to_CartTask2,
	Grab_to_CartTask3,
	Place_to_GroundTask1,
	Place_to_GroundTask2,
	Place_to_GroundTask3,
	Reload_to_CartTask1,
	Reload_to_CartTask2,
	Reload_to_CartTask3,
	Stack_First_LayerTask1,
	Stack_First_LayerTask2,
	Stack_First_LayerTask3,
	Stack_Second_LayerTask1,
	Stack_Second_LayerTask2,
	Stack_Second_LayerTask3,
	Block_Arr_Task,
}BlockTaskDate_Indexs;

//存储物块的坐标，颜色信息结构体
typedef struct
{
	//从A移动到B
	BlockTraceTypeDef			BlockTrace;
	BlockCoordinatesTypeDef 	Coordinates;//坐标和速度信息
	BlockColorTypeDef 			Block_Color;//物块颜色
	Task_status 				Completion_status;//完成情况
	BlockStatusTypeDef 			BlockStatus;//抓取状态
	InitStatusTypeDef 			InitStatus;//信息初始化
}BlockMessageTypeDef;

#include "main.h"
#include "Motor_can.h"
#include "dm4310_drv.h"
#include "IMU.h"
#include "Motor_can.h"
#include "Control.h"
#include "UsartDate.h"
#include "PID.h"
//@机械爪调用接口
void Claw_Task(void);		//该函数放进底层控制线程

void Claw_On(ClawTypeDef* Claw);			// 控制机械爪开

void Claw_Off(ClawTypeDef* Claw);		// 控制机械爪关

void Claw_rec(ClawTypeDef* Claw);		//视觉识别机械爪张开
//@机械臂调用接口
void RobotArm_Task(void);
void RobotArm_Test(void);
int RobotArm_MovementY(float Y_Val,float Speed);//移动Y轴
int RobotArm_MovementZ(float Z_Val,float Speed);//移动Z轴
int RobotArm_Block_Movement(BlockMessageTypeDef* Block);
//@小车调用接口
void Car_Task(void);
XY_ReturnDateTypeDef XYPD_position(float Measure_X, float Measure_Y,float TargetX,float TargetY);//输入转换后的坐标测量值,输出角度
void Block_Init(BlockMessageTypeDef* Block,float* Date);
int Car_TraceCircle(void);
int Car_XYMovement(float Speed,float Angle,Distance_TypeDef* Distance);
BlockColorTypeDef Car_Circle_color(void);//@变量声明
int Car_MovetoColor(TraceColor_and_DistanceTypeDef* Val);

extern ClawTypeDef Claw;
extern RobotArmTypeDef RobotArm;
extern CarTypeDef Car;

extern	BlockMessageTypeDef Block[15];
extern float Block_Date[Block_Arr_Task][BlockArr_Date];
//#define TempY	RobotArm.RobotArm_TargetY
//#define TempZ	RobotArm.RobotArm_TargetZ
//@外部变量声明
extern float Arm_Zero;
extern float Motor_InfiniteZ;
extern uint8_t Xianweikaiguan_Status;
extern float World_X; 
extern float World_Y;
extern IMU_Typedef IMU;
extern motor_t motor[num];
extern SMTypeDef SM;
extern	PID_TypeDef PD_X, PD_Y;
#endif
