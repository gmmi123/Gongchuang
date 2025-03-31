#ifndef __Control_Plus_H
#define __Control_Plus_H

//Control_Plus�Կ���ϵͳ�����Ż���Ŀ��ʹ���Ƹ�Ϊ��㣬���Ը�����
//�Ի�е�ۣ���еצ��С�����̣�Openmv����ݮ�ɣ��������ı������з�װ��ȷ�������

//�豸״̬
typedef enum
{
	Device_Disable = 0,
	Device_Initing = 1,
	Device_Running = 2
}Device_status;
//����������
typedef enum
{
	Task_NonStart = 0,
	Task_Runnig = 1,
	Task_Complete = 2
}Task_status;
//��ʼ���ṹ��
typedef enum
{
	Init_Non=0,//δ��ʼ��
	Init_Enable=1,//�ѳ�ʼ��
}InitStatusTypeDef;

//��еצ�ṹ��
typedef struct
{
		int 		Claw_PZ1;
		int 		Claw_PZ2;
		float 		Claw_AngleVal;
	Device_status 	Claw_Status;//0ʧ�ܣ�1��ʼ��״̬��2����״̬
}ClawTypeDef;
//��е�۽ṹ��
typedef struct
{
		float 		RobotArm_MeasureZ;			//Z���е�۵ĵ�ǰֵ
		float 		RobotArm_TargetZ;			//Z���е��Ҫ�ƶ�����Ŀ��λ��
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
	Block_Readygrab=1,//׼��ץȡ
	Block_Identifier=2,//ʶ��
	Block_Grasping=3,//ץȡ��
	Block_Reload=4,//���¸�λ
	Block_Placing=5,//������
	Block_Reloadagain=6,
	Block_End=7
}BlockStatusTypeDef;



//ʶ��ģʽ�µ��˶����Ʊ�־λ
typedef enum
{
	Movement_Non,
	Movement_traceYLMasse_Movement,//׷��ԭ�����ϵ����
	Movement_traceMasse_Movement,//׷�ٵ��ϵķ���
	Movement_traceMasse_nonMovement,//��׷��ԭ�����ϵ����
	Movement_traceCircle_Movement,
	Movement_End,//����ʶ��

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






//С����Ϣ
typedef struct
{
	float MotorA;//A���ٶ�
	float MotorB;
	float MotorC;
	float MotorD;
	float Ab_X;//��������ϵ��X
	float Ab_Y;//��������ϵ��Y
	float TargetAngle;
	float CurrentAngle;
	float TargetSpeed;
}CarDateTypeDef;




//����ϵ
typedef enum
{
	Absolute_Coordinate_System=0,//��������ϵ����С���ĳ�ʼ�������йأ�
	Car_Coordinate_System=1,//С������ϵ
	
}Coordinate_SystemTypeDef;

typedef struct
{
	float Angle;  //С���ƶ�����
	float Speed;  //С���ƶ��ٶ�
	float Turn_Speed; //С����ת�ٶ�
	Coordinate_SystemTypeDef Coordinate_System;//��ͬ�ο�ϵ�µĿ��Ʋ�һ��
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
	Index_Car_TargetX,//������һ�������Ŀ��X��Y
	Index_Car_TargetY,
	Index_Car_TargetAngle,//С��Ҫ���ֵĽǶ�
	CarArr_DateIndex,
}CarArr_Date;

typedef enum
{
	Index_lineTask,
	CarArr_TaskIndex,
}CarArr_Task;


//�洢С���˶�һ�ε���Ϣ
typedef struct
{
	//��A�ƶ���B
	BlockCoordinatesTypeDef Coordinates;//������ٶ���Ϣ
	BlockColorTypeDef Block_Color;//�����ɫ
	Task_status Completion_status;//����������
	BlockStatusTypeDef BlockStatus;//����״̬
	InitStatusTypeDef InitStatus;//��Ϣ��ʼ��
	
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
	Color,		//ǰ�漸��Ϊ����
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
	BlockArr_Date,//���һ��ռλ
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

//�洢�������꣬��ɫ��Ϣ�ṹ��
typedef struct
{
	//��A�ƶ���B
	BlockTraceTypeDef			BlockTrace;
	BlockCoordinatesTypeDef 	Coordinates;//������ٶ���Ϣ
	BlockColorTypeDef 			Block_Color;//�����ɫ
	Task_status 				Completion_status;//������
	BlockStatusTypeDef 			BlockStatus;//ץȡ״̬
	InitStatusTypeDef 			InitStatus;//��Ϣ��ʼ��
}BlockMessageTypeDef;

#include "main.h"
#include "Motor_can.h"
#include "dm4310_drv.h"
#include "IMU.h"
#include "Motor_can.h"
#include "Control.h"
#include "UsartDate.h"
#include "PID.h"
//@��еצ���ýӿ�
void Claw_Task(void);		//�ú����Ž��ײ�����߳�

void Claw_On(ClawTypeDef* Claw);			// ���ƻ�еצ��

void Claw_Off(ClawTypeDef* Claw);		// ���ƻ�еצ��

void Claw_rec(ClawTypeDef* Claw);		//�Ӿ�ʶ���еצ�ſ�
//@��е�۵��ýӿ�
void RobotArm_Task(void);
void RobotArm_Test(void);
int RobotArm_MovementY(float Y_Val,float Speed);//�ƶ�Y��
int RobotArm_MovementZ(float Z_Val,float Speed);//�ƶ�Z��
int RobotArm_Block_Movement(BlockMessageTypeDef* Block);
//@С�����ýӿ�
void Car_Task(void);
XY_ReturnDateTypeDef XYPD_position(float Measure_X, float Measure_Y,float TargetX,float TargetY);//����ת������������ֵ,����Ƕ�
void Block_Init(BlockMessageTypeDef* Block,float* Date);
int Car_TraceCircle(void);
int Car_XYMovement(float Speed,float Angle,Distance_TypeDef* Distance);
BlockColorTypeDef Car_Circle_color(void);//@��������
int Car_MovetoColor(TraceColor_and_DistanceTypeDef* Val);

extern ClawTypeDef Claw;
extern RobotArmTypeDef RobotArm;
extern CarTypeDef Car;

extern	BlockMessageTypeDef Block[15];
extern float Block_Date[Block_Arr_Task][BlockArr_Date];
//#define TempY	RobotArm.RobotArm_TargetY
//#define TempZ	RobotArm.RobotArm_TargetZ
//@�ⲿ��������
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
