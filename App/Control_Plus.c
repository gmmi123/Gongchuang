#include "Control_Plus.h"
//�ײ�����
#include "Sever_control.h"
#include "Buzzer.h"
#include "usart.h"
#include "string.h"
#include "PID.h"
#include "UsartDate.h"


#define SM_TargetX 324
#define SM_TargetY 230

//@Ŀ¼:
//@�ڲ���������
//@�ⲿ��������
//@��еצ
//@��еצ����
//@��еצ�ײ�ӿ�
//@��еצ��������
//@��е��
//@��е�۵���
//@��е�۵ײ�ӿ�
//@��е�ۺ�������
//@����ץȡ����
//@С��
//@С������
//@С���ײ�ӿ�
//@С����������


//@��еצ
//�ɲ鿴���޸ĵı���
//1.��еצƫ��				
//2.��еצĿ��Ƕ�
//3.��еצ״̬

//��еצƫ��:���Ϊ1��2��������еצ��ƫ�ã�ƫ����ָ��еצ��0�ȱպ�ʱ��צ�Ŀ��ϴ�С��
//��еצ��ǰץȡ�ĽǶȱ������ƻ�еצ�ſ��ĽǶ�
//����ԭ��ѭ���߳��з���Claw_Task���������������������̵߳��ýӿں���ʱ��ֻ��ı����,����ִ��������������������ֻ���ڵײ�����߳�����MCU��ʱ��ʱִ��

//@��еצ����
//������Ҫ���ı���
ClawTypeDef Claw;					//���Claw�����Ӵ��ڽ������ߵ���
#define dfClaw_PZ1 			130		//�����ԵĽ�����ĵ�dfClaw��궨����
#define dfClaw_PZ2 			47
#define dfClaw_OnAngle 		40.0
#define dfClaw_OffAngle 	8.0
#define dfClaw_recAngle 	8.0

//@��еצ�ײ�ӿ�
//���޸ģ�
//Claw_ControlAngle
void Claw_ControlAngle(float Angle,int Claw_PZ1,int Claw_PZ2)
{
	//�˴�����ײ㺯��
	Claw_Angle(Angle,Claw_PZ1,Claw_PZ2);
}

//�ӿ�ת������
void Claw_Interface_Transfor(ClawTypeDef* pClaw,void (*Underfunction)(float,int,int))
{
	//�ϲ�����߼���
	//״̬��
	if(pClaw->Claw_Status ==1)//��ʼ��״̬
	{
		pClaw->Claw_PZ1=dfClaw_PZ1;
		pClaw->Claw_PZ2=dfClaw_PZ2;
		pClaw->Claw_Status=2;
	}
	else if(pClaw->Claw_Status==2)//����״̬
	{
		Underfunction(pClaw->Claw_AngleVal,pClaw->Claw_PZ1,pClaw->Claw_PZ2);
	}
}

//@��еצ��������
//�ú����Ž��ײ�����߳�
void Claw_Task(void)
{
	//��������ע�����Ӷ����е��
	//Ŀǰ���ֶ�����(���뷽��,��������ĺ���,����һ���µĻ�еצ����,�����ύ�ײ���������)
	Claw_Interface_Transfor(&Claw,Claw_ControlAngle);
	
}
// ���ƻ�еצ��
void Claw_On(ClawTypeDef* Claw)
{
	Claw->Claw_AngleVal=40.0;

}
// ���ƻ�еצ��
void Claw_Off(ClawTypeDef* Claw)
{
	Claw->Claw_AngleVal=8.0;

}
//�Ӿ�ʶ���еצ�ſ�
void Claw_rec(ClawTypeDef* Claw)
{
	Claw->Claw_AngleVal=180;
}


//@��е��
//�ɲ鿴���޸ĵı���
//1.Z���Ŀ������
//2.Z�ᵱǰ������
//3.Z����˶��ٶ�
//4.Y���Ŀ������
//5.Y�ᵱǰ������
//6.Y����˶��ٶ�
//7.��е�۵�״̬

//@��е�۵���
//@����ץȡ����
#define NumBlock  15 //15��ץȡ
RobotArmTypeDef RobotArm;
//Y���ƶ��ٶȲ���
#define Block_YSpeed	2
#define Block_ZSpeed	10

//ץ��������ͷŷ�������
#define Get_sequence1 -5.81 + 0.01
#define Get_sequence2 -0.01 + 0.005 	// ��С����
#define Get_sequence3 -0.53 + 0			//
#define Place_sequence1 -5.81 + 0.01
#define Place_sequence2 -6.35 + 0.01
#define Place_sequence3 -0.555 + 0.01

//��ԭ����ץ��鵽С���ϸ߶Ȳ���
#define Grab_to_Cart_Z1	6
#define Grab_to_Cart_Z2	2.5
//��С���ϵ����ŵ��ӹ���(����)�߶Ȳ���
#define Place_to_Ground_Z1 3.5
#define Place_to_Ground_Z2 19
//�����Ӽӹ����ŵ�С����
#define	Reload_to_Cart_Z1 20
#define	Reload_to_Cart_Z2 2.5
//��С���ϵ����ŵ��������ĵ�һ��
#define Stack_First_Layer_Z1 3.5
#define Stack_First_Layer_Z2 19
//��С���ϵ����ŵ��������ĵڶ���
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

//@��е�۵ײ�ӿ�
//���ƻ�е��Z��
void RobotArm_ControlZ(float TargetZ,float Speed)
{
	//�ײ�ӿ�
													//���У׼
	pos_speed_ctrl(&hfdcan2, motor[Motor6].id,TargetZ+Arm_Zero, Speed);
}
//���ƻ�е��Y��
void RobotArm_ControlY(float TargetY,float Speed)
{
	//�ײ�ӿ�
	pos_speed_ctrl(&hfdcan1, motor[Motor5].id,TargetY, Speed);
}
//��е�۵ײ���������ת��
void RobotArm_DateTransfor(RobotArmTypeDef* RobotArm)
{
	RobotArm->RobotArm_MeasureZ=Motor_InfiniteZ-Arm_Zero;
	RobotArm->RobotArm_MeasureY=motor[Motor5].para.pos;
}

//�ӿ�ת������
void RobotArm_Interface_Transfor(RobotArmTypeDef* RobotArm,void (*Underfunction_Y)(float,float),void (*Underfunction_Z)(float,float),void (*Underfunction_Date)(RobotArmTypeDef*))
{
	//�ϲ�����߼���
	//״̬��
	if(RobotArm->RobotArm_Status ==1)//��ʼ��״̬
	{
		RobotArm->RobotArm_SpeedY=1;//�ٶ�Ϊ��
		RobotArm->RobotArm_SpeedZ=5;
		RobotArm->RobotArm_TargetY=RobotArm->RobotArm_MeasureY;//Ŀ��λ��Ϊ�ϵ�λ��
		RobotArm->RobotArm_TargetZ=RobotArm->RobotArm_MeasureZ;
		RobotArm->RobotArm_Status=2;
	}
	else if(RobotArm->RobotArm_Status==2)//����״̬
	{
		
		Underfunction_Y(RobotArm->RobotArm_TargetY,RobotArm->RobotArm_SpeedY);//����Y
		if(RobotArm->RobotArm_TargetZ>0)//RobotArm_TargetZ<=0ʱ��λ
		{
			Underfunction_Z(RobotArm->RobotArm_TargetZ,RobotArm->RobotArm_SpeedZ);//����Z
		}
		else
		{
			//������λ���ظ�λ
			if (Xianweikaiguan_Status == 2)
			{
				pos_speed_ctrl(&hfdcan2, motor[Motor6].id, Arm_Zero - 20, 0);
			}
			else
			{
				pos_speed_ctrl(&hfdcan2, motor[Motor6].id, Arm_Zero - 20, 20);
			}
		}
		Underfunction_Date(RobotArm);//��ȡ����
		
		
	}
	
}

//@��е�ۺ�������
void RobotArm_Task(void)
{
	RobotArm_Interface_Transfor(&RobotArm,RobotArm_ControlY,RobotArm_ControlZ,RobotArm_DateTransfor);
}

//����ֵ����
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

// ��⵱ǰֵ��Ŀ��ֵ�Ĳ�С�����
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
//�ƶ�Y��
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
//�ƶ�Z��
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




//����
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
			//��е������������߸�λ
			if(RobotArm_MovementZ(0,20))
			{
				Block->BlockStatus=Block_Identifier;
			}
		}
		else if(Block->BlockStatus==Block_Identifier)
		{
			//���ƶ���ʶ��Y
			if(RobotArm_MovementY(Block->Coordinates.Identifier_Y,Block->Coordinates.Y1_Speed))
			{
				Claw_On(&Claw);//�򿪻�еץ
				//�½���ʶ��߶�
				if(RobotArm_MovementZ(Block->Coordinates.Identifier_Z,20))
				{
					if(Block->Coordinates.Movement_flag==Movement_traceYLMasse_Movement)//ʶ��׷������
					{
						if(Raspberry_PI_camerDate(SM_Masse)==SM_DateUpdate&&SM.SM_coloar==Block->Block_Color)//С����ʼ����   ��Y������ȡ����תΪС������ϵ��
						{
							XY_ReturnDateTypeDef XY_ReturnDate;
							XY_ReturnDate=XYPD_position(SM.SM_X,-SM.SM_Y,SM_TargetX,-SM_TargetY);
							Car.CarControl.Angle=XY_ReturnDate.Angle+Car.CarDate.CurrentAngle-90;;
							
							if(XY_ReturnDate.Err>150)//�ٶ��޷�
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
								Block->Coordinates.Movement_flag=Movement_End;//���궨λ���
							}
						}
					}
					else if(Block->Coordinates.Movement_flag==Movement_traceMasse_nonMovement)
					{
						if(Raspberry_PI_camerDate(SM_Masse)==SM_DateUpdate&&SM.SM_coloar==Block->Block_Color)//С����ʼ����   ��Y������ȡ����תΪС������ϵ��
						{
							Car.CarControl.Speed=0;
							Block->Coordinates.Movement_flag=Movement_End;//���궨λ���
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
							if(Raspberry_PI_camerDate(SM_Circle)==SM_DateUpdate)//С����ʼ����   ��Y������ȡ����תΪС������ϵ��
							{
								XY_ReturnDateTypeDef XY_ReturnDate;
								XY_ReturnDate=XYPD_position(SM.SM_X,-SM.SM_Y,SM_TargetX,-SM_TargetY);
								Car.CarControl.Angle=XY_ReturnDate.Angle+Car.CarDate.CurrentAngle-90;
								
								if(XY_ReturnDate.Err>100)//�ٶ��޷�
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
									//���궨λ���
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
							if(Raspberry_PI_camerDate(SM_Masse)==SM_DateUpdate)//С����ʼ����   ��Y������ȡ����תΪС������ϵ��
							{
								XY_ReturnDateTypeDef XY_ReturnDate;
								XY_ReturnDate=XYPD_position(SM.SM_X,-SM.SM_Y,SM_TargetX,-SM_TargetY);
								Car.CarControl.Angle=XY_ReturnDate.Angle+Car.CarDate.CurrentAngle-90;
								
								if(XY_ReturnDate.Err>100)//�ٶ��޷�
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
									//���궨λ���
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
		else if(Block->BlockStatus==Block_Grasping)//ץȡ����
		{
			
			//���ƶ���Y1
			if(RobotArm_MovementY(Block->Coordinates.Y1,Block->Coordinates.Y1_Speed))
			{
				//���ƶ���Z1
				if(RobotArm_MovementZ(Block->Coordinates.Z1,Block->Coordinates.Z1_Speed))
				{
					//ץ
					Claw_Off(&Claw);
					Block->BlockStatus=Block_Reload;
				}
			}
		
		}
		else if(Block->BlockStatus==Block_Reload)//��λ���ץȡ
		{
			if(RobotArm_MovementZ(0,20))
			{
				Block->BlockStatus=Block_Placing;
			}
		
		}
		else if(Block->BlockStatus==Block_Placing)
		{
			//���ƶ���Y2
			if(RobotArm_MovementY(Block->Coordinates.Y2,Block->Coordinates.Y2_Speed))
			{
				//���ƶ���Z2
				if(RobotArm_MovementZ(Block->Coordinates.Z2,Block->Coordinates.Z2_Speed))
				{
					//��
					Claw_On(&Claw);
					Block->BlockStatus=Block_Reloadagain;
				}
			}
		}
		else if(Block->BlockStatus==Block_Reloadagain)//��λ���ץȡ
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
//�����ʼ��
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



//С����ָ�������ƶ�ָ������
int Car_XYMovement(float Speed,float Angle,Distance_TypeDef*Distance)
{

	if(Distance->InitStatus==Init_Non)
	{
		Distance->X_Measure=Car.CarDate.Ab_X;//��ʼ����ǰ·��
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



//С��׷Բ

int Car_TraceCircle(void)
{
	
	if(Raspberry_PI_camerDate(SM_Circle)==SM_DateUpdate)//С����ʼ����   ��Y������ȡ����תΪС������ϵ��
	{
		XY_ReturnDateTypeDef XY_ReturnDate;
		XY_ReturnDate=XYPD_position(SM.SM_X,-SM.SM_Y,SM_TargetX,-SM_TargetY);
		Car.CarControl.Angle=XY_ReturnDate.Angle+Car.CarDate.CurrentAngle-90;
		
		if(XY_ReturnDate.Err>150)//�ٶ��޷�
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
//С��ʶ��ԲȦ��ɫ
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
		return 920;//Ȩֵ
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


//С������ʶ����ɫ�����ƶ���ָ��λ��
int Car_MovetoColor(TraceColor_and_DistanceTypeDef* Val)
{

	float TargetColor_To_Distance;
	float MeasureColor_To_Distance;
	float MoveDistance;
	BlockColorTypeDef TempColor;
	//������ǰ��ɫ
	TempColor=Car_Circle_color();
	if(Val->TraceColor.InitStatus==Init_Non&&TempColor!=0)//��ʼ��һ����ɫ
	{
		Val->TraceColor.InitStatus=Init_Enable;
		Val->TraceColor.MeasureColor=TempColor;
		TargetColor_To_Distance	 =	assign_color_weight(Val->TraceColor.TargetColor);
		MeasureColor_To_Distance =	assign_color_weight(Val->TraceColor.MeasureColor);
		//�ƶ�����
		MoveDistance = TargetColor_To_Distance-MeasureColor_To_Distance;
		Val->Distance.DistanceVal=MoveDistance;
	}
	

	if(Val->TraceColor.InitStatus==Init_Enable)
	{
		//��ʼ�ƶ�
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



XY_ReturnDateTypeDef XYPD_position(float Measure_X, float Measure_Y,float TargetX,float TargetY)//����ת������������ֵ,����Ƕ�
{
	// ��ʼ������

	// PID����
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
	
	if (XYPD_position_control_status == 0) // PD��ʼ��
	{
		PID_Init(&PD_X);
		PID_Init(&PD_Y);
		Set_PID(&PD_X, KP_X, 0, KD_X);
		Set_PID(&PD_Y, KP_Y, 0, KD_Y);
		// ����޷�
		PD_X.PID_OUT_Limit = Out_X_max;
		PD_Y.PID_OUT_Limit = Out_Y_max;
		// �л�״̬
		XYPD_position_control_status = 1;
	}
	if (XYPD_position_control_status == 1)
	{
		// ��ȡ����ֵ������ȡ�������꣩
		PD_X.Current_Value = (float)Measure_X;
		PD_Y.Current_Value = (float)Measure_Y;
		// PID����
		Position_PID(&PD_X, TargetX);
		Position_PID(&PD_Y, TargetY);
		
		
		XY_ReturnDate.Speed = sqrtf((PD_X.PID_Out * PD_X.PID_Out) + (PD_Y.PID_Out * PD_Y.PID_Out))/1500.0;
		XY_ReturnDate.Angle = -atan2f(PD_Y.PID_Out, PD_X.PID_Out) * (180.0 / 3.1415926);
		XY_ReturnDate.Err = sqrtf((PD_X.PID_Out * PD_X.PID_Out) + (PD_Y.PID_Out * PD_Y.PID_Out));
	}

	return   XY_ReturnDate;

   
}




//���Ժ���
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







