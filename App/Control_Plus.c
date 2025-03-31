#include "Control_Plus.h"
//底层驱动
#include "Sever_control.h"
#include "Buzzer.h"
#include "usart.h"
#include "string.h"
#include "PID.h"
#include "UsartDate.h"


#define SM_TargetX 324
#define SM_TargetY 230

//@目录:
//@内部变量声明
//@外部变量声明
//@机械爪
//@机械爪调试
//@机械爪底层接口
//@机械爪函数调用
//@机械臂
//@机械臂调试
//@机械臂底层接口
//@机械臂函数调用
//@方块抓取参数
//@小车
//@小车调试
//@小车底层接口
//@小车函数调用


//@机械爪
//可查看和修改的变量
//1.机械爪偏置				
//2.机械爪目标角度
//3.机械爪状态

//机械爪偏置:编号为1，2的两个机械爪的偏置（偏置是指机械爪在0度闭合时，爪的开合大小）
//机械爪当前抓取的角度变量控制机械爪张开的角度
//控制原理：循环线程中放入Claw_Task驱动函数，这样在其他线程调用接口函数时，只会改变变量,不会执行驱动函数，驱动函数只能在底层控制线程掌握MCU的时间时执行

//@机械爪调试
//调试需要更改变量
ClawTypeDef Claw;					//添加Claw到监视窗口进行在线调试
#define dfClaw_PZ1 			130		//将调试的结果更改到dfClaw类宏定义中
#define dfClaw_PZ2 			47
#define dfClaw_OnAngle 		40.0
#define dfClaw_OffAngle 	8.0
#define dfClaw_recAngle 	8.0

//@机械爪底层接口
//可修改：
//Claw_ControlAngle
void Claw_ControlAngle(float Angle,int Claw_PZ1,int Claw_PZ2)
{
	//此处放入底层函数
	Claw_Angle(Angle,Claw_PZ1,Claw_PZ2);
}

//接口转化函数
void Claw_Interface_Transfor(ClawTypeDef* pClaw,void (*Underfunction)(float,int,int))
{
	//上层控制逻辑：
	//状态机
	if(pClaw->Claw_Status ==1)//初始化状态
	{
		pClaw->Claw_PZ1=dfClaw_PZ1;
		pClaw->Claw_PZ2=dfClaw_PZ2;
		pClaw->Claw_Status=2;
	}
	else if(pClaw->Claw_Status==2)//运行状态
	{
		Underfunction(pClaw->Claw_AngleVal,pClaw->Claw_PZ1,pClaw->Claw_PZ2);
	}
}

//@机械爪函数调用
//该函数放进底层控制线程
void Claw_Task(void)
{
	//后续加入注册表，添加多个机械臂
	//目前是手动加入(加入方法,复制下面的函数,定义一个新的机械爪变量,并且提交底层驱动函数)
	Claw_Interface_Transfor(&Claw,Claw_ControlAngle);
	
}
// 控制机械爪开
void Claw_On(ClawTypeDef* Claw)
{
	Claw->Claw_AngleVal=40.0;

}
// 控制机械爪关
void Claw_Off(ClawTypeDef* Claw)
{
	Claw->Claw_AngleVal=8.0;

}
//视觉识别机械爪张开
void Claw_rec(ClawTypeDef* Claw)
{
	Claw->Claw_AngleVal=180;
}


//@机械臂
//可查看和修改的变量
//1.Z轴的目标坐标
//2.Z轴当前的坐标
//3.Z轴的运动速度
//4.Y轴的目标坐标
//5.Y轴当前的坐标
//6.Y轴的运动速度
//7.机械臂的状态

//@机械臂调试
//@方块抓取参数
#define NumBlock  15 //15次抓取
RobotArmTypeDef RobotArm;
//Y轴移动速度参数
#define Block_YSpeed	2
#define Block_ZSpeed	10

//抓方块坐标和放方块坐标
#define Get_sequence1 -5.81 + 0.01
#define Get_sequence2 -0.01 + 0.005 	// 减小往右
#define Get_sequence3 -0.53 + 0			//
#define Place_sequence1 -5.81 + 0.01
#define Place_sequence2 -6.35 + 0.01
#define Place_sequence3 -0.555 + 0.01

//从原料区抓物块到小车上高度参数
#define Grab_to_Cart_Z1	6
#define Grab_to_Cart_Z2	2.5
//将小车上的物块放到加工区(地上)高度参数
#define Place_to_Ground_Z1 3.5
#define Place_to_Ground_Z2 19
//将物块从加工区放到小车上
#define	Reload_to_Cart_Z1 20
#define	Reload_to_Cart_Z2 2.5
//将小车上的物块放到放置区的第一层
#define Stack_First_Layer_Z1 3.5
#define Stack_First_Layer_Z2 19
//将小车上的物块放到放置区的第二层
#define Stack_Second_LayerZ1 3.5
#define Stack_Second_LayerZ2 7

//Color;Y1;Y1_Speed;Z1;Z1_Speed;Y2;Y2_Speed;Z2;Z2_Speed;
float Block_Date[Block_Arr_Task][BlockArr_Date]={
1,	-3.14,Block_YSpeed,	Grab_to_Cart_Z1,Block_ZSpeed,		Get_sequence1,Block_YSpeed,	Grab_to_Cart_Z2,Block_ZSpeed,	0,Movement_traceYLMasse_Movement,-3.14,
2,	-3.14,Block_YSpeed,	Grab_to_Cart_Z1,Block_ZSpeed,		Get_sequence2,Block_YSpeed,	Grab_to_Cart_Z2,Block_ZSpeed,	0,Movement_traceMasse_nonMovement,-3.14,
3,	-3.14,Block_YSpeed,	Grab_to_Cart_Z1,Block_ZSpeed,		Get_sequence3,Block_YSpeed,	Grab_to_Cart_Z2,Block_ZSpeed,	0,Movement_traceMasse_nonMovement,-3.14,
1,	Place_sequence1,Block_YSpeed,		Place_to_Ground_Z1,Block_ZSpeed,		-3.14,Block_YSpeed,		Place_to_Ground_Z2,Block_ZSpeed,	0,Movement_traceCircle_Movement,-3.14,
2,	Place_sequence2,Block_YSpeed,		Place_to_Ground_Z1,Block_ZSpeed,		-3.14,Block_YSpeed,		Place_to_Ground_Z2,Block_ZSpeed,	0,Movement_traceCircle_Movement,-3.14,
3,	Place_sequence3,Block_YSpeed,		Place_to_Ground_Z1,Block_ZSpeed,		-3.14,Block_YSpeed,		Place_to_Ground_Z2,Block_ZSpeed,	0,Movement_traceCircle_Movement,-3.14,
1,	-3.14,Block_YSpeed,	Reload_to_Cart_Z1,Block_ZSpeed,		Get_sequence1,Block_YSpeed,	Reload_to_Cart_Z2,Block_ZSpeed,	6,Movement_traceMasse_Movement,-3.14,
2,	-3.14,Block_YSpeed,	Reload_to_Cart_Z1,Block_ZSpeed,		Get_sequence2,Block_YSpeed,	Reload_to_Cart_Z2,Block_ZSpeed,	6,Movement_traceMasse_Movement,-3.14,
3,	-3.14,Block_YSpeed,	Reload_to_Cart_Z1,Block_ZSpeed,		Get_sequence3,Block_YSpeed,	Reload_to_Cart_Z2,Block_ZSpeed,	6,Movement_traceMasse_Movement,-3.14,
1,	Place_sequence1,Block_YSpeed,		Stack_First_Layer_Z1,Block_ZSpeed,		-3.14,Block_YSpeed,		Stack_First_Layer_Z2,Block_ZSpeed,	0,Movement_traceCircle_Movement,-3.14,
2,	Place_sequence2,Block_YSpeed,		Stack_First_Layer_Z1,Block_ZSpeed,		-3.14,Block_YSpeed,		Stack_First_Layer_Z2,Block_ZSpeed,	0,Movement_traceCircle_Movement,-3.14,
3,	Place_sequence3,Block_YSpeed,		Stack_First_Layer_Z1,Block_ZSpeed,		-3.14,Block_YSpeed,		Stack_First_Layer_Z2,Block_ZSpeed,	0,Movement_traceCircle_Movement,-3.14,
1,	Place_sequence1,Block_YSpeed,		Stack_Second_LayerZ1,Block_ZSpeed,		-3.14,Block_YSpeed,		Stack_Second_LayerZ2,Block_ZSpeed,	0,Movement_traceMasse_Movement,-3.14,
2,	Place_sequence2,Block_YSpeed,		Stack_Second_LayerZ1,Block_ZSpeed,		-3.14,Block_YSpeed,		Stack_Second_LayerZ2,Block_ZSpeed,	0,Movement_traceMasse_Movement,-3.14,
3,	Place_sequence3,Block_YSpeed,		Stack_Second_LayerZ1,Block_ZSpeed,		-3.14,Block_YSpeed,		Stack_Second_LayerZ2,Block_ZSpeed,	0,Movement_traceMasse_Movement,-3.14,

};

//@机械臂底层接口
//控制机械臂Z轴
void RobotArm_ControlZ(float TargetZ,float Speed)
{
	//底层接口
													//零点校准
	pos_speed_ctrl(&hfdcan2, motor[Motor6].id,TargetZ+Arm_Zero, Speed);
}
//控制机械臂Y轴
void RobotArm_ControlY(float TargetY,float Speed)
{
	//底层接口
	pos_speed_ctrl(&hfdcan1, motor[Motor5].id,TargetY, Speed);
}
//机械臂底层数据数据转化
void RobotArm_DateTransfor(RobotArmTypeDef* RobotArm)
{
	RobotArm->RobotArm_MeasureZ=Motor_InfiniteZ-Arm_Zero;
	RobotArm->RobotArm_MeasureY=motor[Motor5].para.pos;
}

//接口转化函数
void RobotArm_Interface_Transfor(RobotArmTypeDef* RobotArm,void (*Underfunction_Y)(float,float),void (*Underfunction_Z)(float,float),void (*Underfunction_Date)(RobotArmTypeDef*))
{
	//上层控制逻辑：
	//状态机
	if(RobotArm->RobotArm_Status ==1)//初始化状态
	{
		RobotArm->RobotArm_SpeedY=1;//速度为零
		RobotArm->RobotArm_SpeedZ=5;
		RobotArm->RobotArm_TargetY=RobotArm->RobotArm_MeasureY;//目标位置为上电位置
		RobotArm->RobotArm_TargetZ=RobotArm->RobotArm_MeasureZ;
		RobotArm->RobotArm_Status=2;
	}
	else if(RobotArm->RobotArm_Status==2)//运行状态
	{
		
		Underfunction_Y(RobotArm->RobotArm_TargetY,RobotArm->RobotArm_SpeedY);//控制Y
		if(RobotArm->RobotArm_TargetZ>0)//RobotArm_TargetZ<=0时复位
		{
			Underfunction_Z(RobotArm->RobotArm_TargetZ,RobotArm->RobotArm_SpeedZ);//控制Z
		}
		else
		{
			//借助限位开关复位
			if (Xianweikaiguan_Status == 2)
			{
				pos_speed_ctrl(&hfdcan2, motor[Motor6].id, Arm_Zero - 20, 0);
			}
			else
			{
				pos_speed_ctrl(&hfdcan2, motor[Motor6].id, Arm_Zero - 20, 20);
			}
		}
		Underfunction_Date(RobotArm);//获取数据
		
		
	}
	
}

//@机械臂函数调用
void RobotArm_Task(void)
{
	RobotArm_Interface_Transfor(&RobotArm,RobotArm_ControlY,RobotArm_ControlZ,RobotArm_DateTransfor);
}

//绝对值函数
float Fabs(float Val)
{
	if(Val>=0)
	{
		return Val;
	}
	else 
	{
		return -Val;
	}
}

// 检测当前值和目标值的差小于误差
int CheckErr(float CurrentVal,float TargetVal,float Err)
{
	if(Fabs(Fabs(TargetVal)-Fabs(CurrentVal))<Fabs(Err))
	{
		return 1;
	}
	else
	{
		return 0;
	}

}
//移动Y轴
int RobotArm_MovementY(float Y_Val,float Speed)
{
	RobotArm.RobotArm_SpeedY=Speed;
	RobotArm.RobotArm_TargetY=Y_Val;
	if(CheckErr(RobotArm.RobotArm_MeasureY,RobotArm.RobotArm_TargetY,0.01))
	{
		return 1;
	}
	return 0;
}
//移动Z轴
int RobotArm_MovementZ(float Z_Val,float Speed)
{
	RobotArm.RobotArm_SpeedZ=Speed;
	RobotArm.RobotArm_TargetZ=Z_Val;
	if(RobotArm.RobotArm_MeasureZ<=0&&Z_Val<=0)
	{
		return 1;
	}
	if(CheckErr(RobotArm.RobotArm_MeasureZ,RobotArm.RobotArm_TargetZ,0.01))
	{
		return 1;
	}
	return 0;
}




//动作
int RobotArm_Block_Movement(BlockMessageTypeDef* Block)
{
	if(Block->Completion_status==Task_NonStart)
	{
		if(Block->BlockStatus==Block_NotStart)
		{
			Block->BlockStatus=Block_Readygrab;
		}
		Block->Completion_status=Task_Runnig;
	}
	
	if(Block->Completion_status==Task_Runnig)
	{
		if(Block->BlockStatus==Block_Readygrab)
		{
			//机械臂先提升到最高复位
			if(RobotArm_MovementZ(0,20))
			{
				Block->BlockStatus=Block_Identifier;
			}
		}
		else if(Block->BlockStatus==Block_Identifier)
		{
			//先移动到识别Y
			if(RobotArm_MovementY(Block->Coordinates.Identifier_Y,Block->Coordinates.Y1_Speed))
			{
				Claw_On(&Claw);//打开机械抓
				//下降到识别高度
				if(RobotArm_MovementZ(Block->Coordinates.Identifier_Z,20))
				{
					if(Block->Coordinates.Movement_flag==Movement_traceYLMasse_Movement)//识别并追踪物块块
					{
						if(Raspberry_PI_camerDate(SM_Masse)==SM_DateUpdate&&SM.SM_coloar==Block->Block_Color)//小车开始调整   （Y轴数据取负号转为小车坐标系）
						{
							XY_ReturnDateTypeDef XY_ReturnDate;
							XY_ReturnDate=XYPD_position(SM.SM_X,-SM.SM_Y,SM_TargetX,-SM_TargetY);
							Car.CarControl.Angle=XY_ReturnDate.Angle+Car.CarDate.CurrentAngle-90;;
							
							if(XY_ReturnDate.Err>150)//速度限幅
							{
								Car.CarControl.Speed=XY_ReturnDate.Speed;
								if(Car.CarControl.Speed<0.1)
								{
									Car.CarControl.Speed=0.1;
								}
							}
							else
							{
								Car.CarControl.Speed=0;
								Block->Coordinates.Movement_flag=Movement_End;//坐标定位完成
							}
						}
					}
					else if(Block->Coordinates.Movement_flag==Movement_traceMasse_nonMovement)
					{
						if(Raspberry_PI_camerDate(SM_Masse)==SM_DateUpdate&&SM.SM_coloar==Block->Block_Color)//小车开始调整   （Y轴数据取负号转为小车坐标系）
						{
							Car.CarControl.Speed=0;
							Block->Coordinates.Movement_flag=Movement_End;//坐标定位完成
						}
					}
					else if(Block->Coordinates.Movement_flag==Movement_traceCircle_Movement)
					{
						if(Block->BlockTrace.BlockTraceStatus==Trace_Unstart)
						{
							if(Car_TraceCircle())
							{
								Block->BlockTrace.BlockTraceStatus=Trace_SeekTarget;
		
							}
							
							
						}
						else if(Block->BlockTrace.BlockTraceStatus==Trace_SeekTarget)
						{
				
							
							if(Car_MovetoColor(&Block->BlockTrace.TraceColor_and_Distance))
							{
								Block->BlockTrace.BlockTraceStatus=Trace_Location;
							}
							
						}
						else if(Block->BlockTrace.BlockTraceStatus==Trace_Location)
						{
							if(Raspberry_PI_camerDate(SM_Circle)==SM_DateUpdate)//小车开始调整   （Y轴数据取负号转为小车坐标系）
							{
								XY_ReturnDateTypeDef XY_ReturnDate;
								XY_ReturnDate=XYPD_position(SM.SM_X,-SM.SM_Y,SM_TargetX,-SM_TargetY);
								Car.CarControl.Angle=XY_ReturnDate.Angle+Car.CarDate.CurrentAngle-90;
								
								if(XY_ReturnDate.Err>100)//速度限幅
								{
									Car.CarControl.Speed=XY_ReturnDate.Speed;
									if(Car.CarControl.Speed<0.1)
									{
										Car.CarControl.Speed=0.1;
									}
								}
								else
								{
									Car.CarControl.Speed=0;
									Block->BlockTrace.BlockTraceStatus=Trace_End;
									//坐标定位完成
								}
							}
						}
						else if(Block->BlockTrace.BlockTraceStatus==Trace_End)
						{
							
							Block->Coordinates.Movement_flag=Movement_End;
						}

					}
					else if(Block->Coordinates.Movement_flag==Movement_traceMasse_Movement)
					{
						if(Block->BlockTrace.BlockTraceStatus==Trace_Unstart)
						{
							if(Car_TraceCircle())
							{
								Block->BlockTrace.BlockTraceStatus=Trace_SeekTarget;
		
							}
							
						}
						else if(Block->BlockTrace.BlockTraceStatus==Trace_SeekTarget)
						{
							
							//Pass
							
							if(Car_MovetoColor(&Block->BlockTrace.TraceColor_and_Distance))
							{
								Block->BlockTrace.BlockTraceStatus=Trace_Location;
							}
							
						}
						else if(Block->BlockTrace.BlockTraceStatus==Trace_Location)
						{
							if(Raspberry_PI_camerDate(SM_Masse)==SM_DateUpdate)//小车开始调整   （Y轴数据取负号转为小车坐标系）
							{
								XY_ReturnDateTypeDef XY_ReturnDate;
								XY_ReturnDate=XYPD_position(SM.SM_X,-SM.SM_Y,SM_TargetX,-SM_TargetY);
								Car.CarControl.Angle=XY_ReturnDate.Angle+Car.CarDate.CurrentAngle-90;
								
								if(XY_ReturnDate.Err>100)//速度限幅
								{
									Car.CarControl.Speed=XY_ReturnDate.Speed;
									if(Car.CarControl.Speed<0.1)
									{
										Car.CarControl.Speed=0.1;
									}
								}
								else
								{
									Car.CarControl.Speed=0;
									Block->BlockTrace.BlockTraceStatus=Trace_End;
									//坐标定位完成
								}
							}
						}
						else if(Block->BlockTrace.BlockTraceStatus==Trace_End)
						{
							
							Block->Coordinates.Movement_flag=Movement_End;
						}
					
					}
					
					else 
					{
						Block->Coordinates.Movement_flag=Movement_End;
					}
					if(Block->Coordinates.Movement_flag==Movement_End)
					{
						Block->BlockStatus=Block_Grasping;
						Claw_On(&Claw);
					}
				
				}
			}
		}
		else if(Block->BlockStatus==Block_Grasping)//抓取过程
		{
			
			//先移动到Y1
			if(RobotArm_MovementY(Block->Coordinates.Y1,Block->Coordinates.Y1_Speed))
			{
				//再移动到Z1
				if(RobotArm_MovementZ(Block->Coordinates.Z1,Block->Coordinates.Z1_Speed))
				{
					//抓
					Claw_Off(&Claw);
					Block->BlockStatus=Block_Reload;
				}
			}
		
		}
		else if(Block->BlockStatus==Block_Reload)//复位完成抓取
		{
			if(RobotArm_MovementZ(0,20))
			{
				Block->BlockStatus=Block_Placing;
			}
		
		}
		else if(Block->BlockStatus==Block_Placing)
		{
			//先移动到Y2
			if(RobotArm_MovementY(Block->Coordinates.Y2,Block->Coordinates.Y2_Speed))
			{
				//再移动到Z2
				if(RobotArm_MovementZ(Block->Coordinates.Z2,Block->Coordinates.Z2_Speed))
				{
					//放
					Claw_On(&Claw);
					Block->BlockStatus=Block_Reloadagain;
				}
			}
		}
		else if(Block->BlockStatus==Block_Reloadagain)//复位完成抓取
		{
			if(RobotArm_MovementZ(0,20))
			{
				Block->BlockStatus=Block_End;
				Block->Completion_status=Task_Complete;
			}
		}
	}
	
	if(Block->Completion_status==Task_Complete)
	{
		return 1;
	}
	else
	{
		return 0;
	}
	
	
}
//方块初始化
void Block_Init(BlockMessageTypeDef* Block,float* Date)
{
	Block->Block_Color=Date[0];
	Block->Coordinates.Y1=Date[1];
	Block->Coordinates.Y1_Speed=Date[2];
	Block->Coordinates.Z1=Date[3];
	Block->Coordinates.Z1_Speed=Date[4];
	Block->Coordinates.Y2=Date[5];
	Block->Coordinates.Y2_Speed=Date[6];
	Block->Coordinates.Z2=Date[7];
	Block->Coordinates.Z2_Speed=Date[8];
	Block->InitStatus=Init_Enable;
	Block->Coordinates.Identifier_Z=Date[Z_Identifier];
	Block->Coordinates.Movement_flag=Date[MovementFlag];
	Block->Coordinates.Identifier_Y=Date[Y_Identifier];
	Block->BlockTrace.TraceColor_and_Distance.TraceColor.TargetColor=Block->Block_Color;

}



//小车朝指定方向移动指定距离
int Car_XYMovement(float Speed,float Angle,Distance_TypeDef*Distance)
{

	if(Distance->InitStatus==Init_Non)
	{
		Distance->X_Measure=Car.CarDate.Ab_X;//初始化当前路程
		Distance->Y_Measure=Car.CarDate.Ab_Y;
		Distance->InitStatus=Init_Enable;
	}
	else if(Distance->InitStatus==Init_Enable)
	{
		if(Fabs(Distance->DistanceVal)>sqrtf((Car.CarDate.Ab_X-Distance->X_Measure)*(Car.CarDate.Ab_X-Distance->X_Measure)+(Car.CarDate.Ab_Y-Distance->Y_Measure)*(Car.CarDate.Ab_Y-Distance->Y_Measure)))
		{
			Car.CarControl.Angle=Angle;
			Car.CarControl.Speed=Speed;
		}
		else
		{
			Car.CarControl.Angle=0;
			Car.CarControl.Speed=0;
			return 1;
		}
	}
	return 0;
}



//小车追圆

int Car_TraceCircle(void)
{
	
	if(Raspberry_PI_camerDate(SM_Circle)==SM_DateUpdate)//小车开始调整   （Y轴数据取负号转为小车坐标系）
	{
		XY_ReturnDateTypeDef XY_ReturnDate;
		XY_ReturnDate=XYPD_position(SM.SM_X,-SM.SM_Y,SM_TargetX,-SM_TargetY);
		Car.CarControl.Angle=XY_ReturnDate.Angle+Car.CarDate.CurrentAngle-90;
		
		if(XY_ReturnDate.Err>150)//速度限幅
		{
			Car.CarControl.Speed=XY_ReturnDate.Speed;
			if(Car.CarControl.Speed<0.1)
			{
				Car.CarControl.Speed=0.1;
			}
		}
		else
		{
			Car.CarControl.Speed=0;
			return 1;
		}
	}
	
	
	return 0;
}
//小车识别圆圈颜色
BlockColorTypeDef Car_Circle_color(void)
{
	if(Raspberry_PI_camerDate(SM_Circle)==SM_DateUpdate)
	{
		return SM.SM_coloar;
	}
	return 0;
}
float assign_color_weight(BlockColorTypeDef Color)
{
	if(Color==Reg)
	{
		return 920;//权值
	}
	else if(Color==Green)
	{
		return 460;
	}
	else if(Color==Blue)
	{
		return 0;
	}
	return -1;
}


//小车智能识别颜色，并移动到指定位置
int Car_MovetoColor(TraceColor_and_DistanceTypeDef* Val)
{

	float TargetColor_To_Distance;
	float MeasureColor_To_Distance;
	float MoveDistance;
	BlockColorTypeDef TempColor;
	//测量当前颜色
	TempColor=Car_Circle_color();
	if(Val->TraceColor.InitStatus==Init_Non&&TempColor!=0)//初始化一次颜色
	{
		Val->TraceColor.InitStatus=Init_Enable;
		Val->TraceColor.MeasureColor=TempColor;
		TargetColor_To_Distance	 =	assign_color_weight(Val->TraceColor.TargetColor);
		MeasureColor_To_Distance =	assign_color_weight(Val->TraceColor.MeasureColor);
		//移动距离
		MoveDistance = TargetColor_To_Distance-MeasureColor_To_Distance;
		Val->Distance.DistanceVal=MoveDistance;
	}
	

	if(Val->TraceColor.InitStatus==Init_Enable)
	{
		//开始移动
		if(Val->Distance.DistanceVal>=0)
		{
			if(Car_XYMovement(10,90-270+Car.CarDate.CurrentAngle,&Val->Distance))
			{
				return 1;
			}
		}
		else
		{
			if(Car_XYMovement(10,-(90+270-Car.CarDate.CurrentAngle),&Val->Distance))
			{
				return 1;
			}
		}
		
	}
	return 0;
	

}



XY_ReturnDateTypeDef XYPD_position(float Measure_X, float Measure_Y,float TargetX,float TargetY)//输入转换后的坐标测量值,输出角度
{
	// 初始化参数

	// PID参数
	XY_ReturnDateTypeDef XY_ReturnDate;
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
		
		
		XY_ReturnDate.Speed = sqrtf((PD_X.PID_Out * PD_X.PID_Out) + (PD_Y.PID_Out * PD_Y.PID_Out))/1500.0;
		XY_ReturnDate.Angle = -atan2f(PD_Y.PID_Out, PD_X.PID_Out) * (180.0 / 3.1415926);
		XY_ReturnDate.Err = sqrtf((PD_X.PID_Out * PD_X.PID_Out) + (PD_Y.PID_Out * PD_Y.PID_Out));
	}

	return   XY_ReturnDate;

   
}




//测试函数
float Test_Y=-3.14;
float Test_YSpeed=0;
float Test_Val=0;
float Test_Z=1;
float Test_ZSpeed=0;
char Test_Status=0;
BlockMessageTypeDef Block[15];
void RobotArm_Test(void)
{
//	static TraceColor_and_DistanceTypeDef temp;
//	if(Test_Status==0)
//	{
//		Claw.Claw_AngleVal=80;
//		if(RobotArm_MovementY(-3.14,1))
//		{
//			Test_Status=1;
//			temp.TraceColor.TargetColor=Reg;
//		}
//	}
//	else
//	{
//		if(Car_MovetoColor(&temp))
//		{
//		
//		}
//	}
	Car_TraceCircle();

//	if(Test_Status<=NumBlock-1)
//	{
//		Block_Init(&Block[Test_Status],Block_Date[Test_Status]);
//		if(RobotArm_Block_Movement(&Block[Test_Status]))
//		{
//			Test_Status++;
//		}
//		
//	}
//	static	Distance_TypeDef Distance1;
//	Distance1.DistanceVal=400;
//	Car_XYMovement(1,90,&Distance1);


//	if(RobotArm_MovementY(Test_Y,Test_YSpeed))
//	{
////		Test_Val=1;
//	}
//	if(RobotArm_MovementZ(Test_Z,Test_ZSpeed))
//	{
//		Test_Val=1;
//	}
	
}







