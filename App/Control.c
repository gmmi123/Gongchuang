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
// ��ݮ�ɱ���
//X325,Y230  1:��,2:��,3:��
#define SM_TargetX 324
#define SM_TargetY 230
#define SM_Red 1
#define SM_Green 1
#define SM_Blue 1
extern SMTypeDef SM;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��е����ر���
extern float Arm_Zero;				  // ��е�����
extern float Motor_InfiniteZ;		  // ��е��Z��
extern uint8_t Xianweikaiguan_Status; // ��λ���ر�־λ
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// С��������ر���
//  ��������������
float World_X = 0;
float World_Y = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��е�ۿ�����ر���
float tempY = -3.15; // ˳ʱ���3 -5.75/-5.8  ˳ʱ���2 -6.26/-6.35  ��ʱ���2 -0.05  ��ʱ�ӵ�1 -0.555
float tempZ = 0;	 // ���ڳ���Arm_Zero+2.8/2.5,�ŵ����µ�ԭ��+20   ��ԭ�϶ѵ�+8    ��ȡ��������+3.2    ��ԭ��+5
float tempj2 = 0;
int tempk = 1;
int staute_test = 0;

// ��������ķ�Χ
#define WorkRange_L -3.65
#define WorkRange_R -2.65
// ��е��ˮƽת��λ�ã���̨��:��Y����ʾ
#define Arm_PositionY motor[Motor5].para.pos
#define Arm_PositionZ Motor_InfiniteZ // ʹ��ת���������
// ��λ
#define WorkPosition -3.15
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ����Ϊ��е�۵Ŀ��ƶ���
// ����е����̨����ת�����Ϊ4������:��������,�洢����1,2,3
// ���ƻ�е�۵���������








//$��е��
//��е���ƶ���������
int Arm_WorkDirection(void)
{


	pos_speed_ctrl(&hfdcan1, motor[Motor5].id, -3.15, 1);
	if (motor[Motor5].para.pos > -3.15 - 0.1 && motor[Motor5].para.pos < -3.15 + 0.1)
	{
		return 1; // ����ָ��λ�÷���1
	}

	return 0;
}
// ���ƻ�е��������λ
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
// ���ƻ�е��ˮƽת��
void Arm_MovementY(float Val) 
{
	pos_speed_ctrl(&hfdcan1, motor[Motor5].id, Val, 2); // 2
}
 // ���ƻ�е�������ƶ�
void Arm_MovementZ(float Val)
{
	pos_speed_ctrl(&hfdcan2, motor[Motor6].id, Arm_Zero + Val, 20);
}
// ����е��Z���Ƿ�ָ��λ��
int Arm_CheckPosition_Z(float Val)
{
	if (Arm_PositionZ > Arm_Zero + Val - 0.1 && Arm_PositionZ < Arm_Zero + Val + 0.1)
	{
		return 1;
	}
	return 0;
}
// ����е��Y���Ƿ�ָ��λ��
int Arm_CheckPosition_Y(float Val)
{
	if (Arm_PositionY > Val - 0.1 && Arm_PositionY < Val + 0.1)
	{
		return 1;
	}
	return 0;
}

//��е��һ��ץȡ����
extern uint8_t Status1;
int Arm_Get_Block(float Target_Position)//Target_Position��ץȡ�����ķ�������λ��
{
	static float TempY = -3.15;
	static float TempZ = 0;
	static uint8_t status = 0;
	static int Cont=0;//��¼ץȡ����
	if (status == 0) // ץȡǰ��׼��״̬
	{
		// �ȴ򿪻�еצ
//		Control_Sever_On();
		// �ƶ���������
//		TempY = WorkPosition;
//		TempZ = -1; // �ó�����Ȩ
//		// ��е������
//		int temp = Arm_Up();
		// �л�״̬
		if (Arm_CheckPosition_Y(WorkPosition))
		{
			status = 1;
		}
	}
	else if (status == 1) // �½���еצ
	{
		// �½�
		TempZ = 6;
//		if (Arm_CheckPosition_Z(6)) // ��⵽��λ���л�״̬
//		{
			status = 2;
//		}
	}
	else if (status == 2) // ץȡ���
	{
		static uint8_t time=25;
//		Control_Sever_Off();
		if(!(time--))
		{
			time=25;
			status = 3;
		}

		
	}
	else if (status == 3) // ����
	{

		//����
		if(Cont==2&&Arm_PositionZ<Arm_Zero+5)
		{
			Status1 = 5;
		}
		TempZ = -1;	  // �ó�����Ȩ
		
		if (Arm_Up()) // ������е��
		{
			status = 4;
		}
	}
	else if (status == 4) // Y�ƶ���ָ��λ��
	{
		TempY = Target_Position;
		if (Arm_CheckPosition_Y(TempY)) // ����λ���л�״̬
		{
			status = 5;
		}
	}
	else if (status == 5) // Z�ƶ���ָ���ĳ���λ��
	{
		TempZ = 3;
		if (Arm_CheckPosition_Z(TempZ)) // ����λ���л�״̬
		{
			status = 6;
		}
	}
	else if (status == 6) // �������
	{
//		Control_Sever_On();
		status = 7;
	}
	else if (status == 7)
	{
		// ����
		TempZ = -1; // �ó�����Ȩ
		if (Arm_Up())
		{
			status = 8;
		}
	}
	else if (status == 8)
	{
		TempY = WorkPosition; // ��λ
		if (Arm_CheckPosition_Y(TempY))
		{
			status = 9;
		}
	}
	else if (status == 9)
	{
		// �������
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
	if (TempY != -1) // ʧ�ܿ���,�ó�����Ȩ
	{
		Arm_MovementY(TempY);
	}

	if (TempZ != -1) // ʧ�ܿ���,�ó�����Ȩ����Arm_Up����������Ҫ�ó�����Ȩ��
	{
		Arm_MovementZ(TempZ);
	}

	return 0;
}

//��е��һ�����÷��鶯��
int Arm_Place_Block(float Target_Position)//Target_Position��Ҫץȡ�ķ����λ��
{
	static float TempY = -3.15;
	static float TempZ = 0;
	static uint8_t status = 0;

	if (status == 0) // ץȡǰ��׼��״̬
	{
		// �ȴ򿪻�еצ
//		Control_Sever_On();
		// �ƶ���������
		TempY = WorkPosition;
		TempZ = -1; // �ó�����Ȩ
		// ��е������
		int temp = Arm_Up();
		// �л�״̬
		if (Arm_CheckPosition_Y(WorkPosition) && (temp))
		{
			status = 1;
		}
	}
	else if (status == 1) // �ƶ�������ָ����λ��
	{
		TempY = Target_Position;
		if (Arm_CheckPosition_Y(TempY)) // ����ָ��λ��
		{
			status = 2;
		}
	}
	else if (status == 2) // �½���ָ���߶�
	{
		TempZ = 4;
		if (Arm_CheckPosition_Z(TempZ)) // ����ָ��λ��
		{
			status = 3;
		}
	}
	else if (status == 3) // �պϻ�еצ����ȡ���
	{
//		Control_Sever_Off();
		status = 4;
	}
	else if (status == 4) // ������е��
	{
		TempZ = -1; // ʹ�ܿ���
		if (Arm_Up())
		{
			// �����л�״̬
			status = 5;
		}
	}
	else if (status == 5) // �ص�������
	{
		TempY = WorkPosition;
		if (Arm_CheckPosition_Y(TempY))
		{
			// �ص����������л�״̬
			status = 6;
		}
	}
	else if (status == 6)
	{
		// �½���ָ���߶�
		TempZ = 20;
		if (Arm_CheckPosition_Z(TempZ))
		{
			status = 7;
		}
	}
	else if (status == 7) // ���÷���
	{
//		Control_Sever_On();
		status = 8;
	}
	else if (status == 8) // ����
	{
		TempZ = -1; // �ó�����Ȩ
		if (Arm_Up())
		{
			status = 9;
		}
	}
	else if (status == 9) // �������
	{
		TempY = -1;
		TempZ = -1;
		status = 0;
		return 1;
	}
	if (TempY != -1) // ʧ�ܿ���,�ó�����Ȩ
	{
		Arm_MovementY(TempY);
	}

	if (TempZ != -1) // ʧ�ܿ���,�ó�����Ȩ����Arm_Up����������Ҫ�ó�����Ȩ��
	{
		Arm_MovementZ(TempZ);
	}

	return 0;
}

//$����
//���̵����ʱ�������
// ��ʼ��ͼǰ�ȳ�ʼ��������������Ĵ���
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

//$׷��
//ȫ�ֱ����������
//ʹ������ͷ���ص�����׷��ԲȦ���߷���
PID_TypeDef PD_X, PD_Y;//ʹ��ʦ��PID�⺯���ı���
float  range_error= 1000;//�������
float XYPD_position_control(float Measure_X, float Measure_Y,float TargetX,float TargetY)//����ת������������ֵ,����Ƕ�
{
	// ��ʼ������

	// PID����
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
		
		range_error = sqrtf((PD_X.PID_Out * PD_X.PID_Out) + (PD_Y.PID_Out * PD_Y.PID_Out));
	}

	return   -atan2f(PD_Y.PID_Out, PD_X.PID_Out) * (180.0 / 3.1415926);

   
}


//$��ݮ��
// ��ȡ��ݮ������ͷ����(Բ���ͷ���):Բ���ͷ��鷵�ص���������һ������һ����������
int Raspberry_PI_camerDate(char DateType)//��Ҫ����������
{

	int SM_Status = 0; // ��ݮ����Ϣ״̬

	HAL_UART_Transmit_IT(&huart9, (const uint8_t *)&DateType, 1); // ������Ϣ����ԲȦ����
	SM_Status = SM_Parses(UART9_RX_Second_BUF);
	if (SM_Status == SM_DateUpdate)
	{
		return SM_DateUpdate; // ��ʾ�����Ѿ�����
	}
	else if (SM_Status == SM_unidentified)
	{
		return SM_unidentified; // ��ݮ��û��ʶ��
	}
	return SM_DateReceive_error; // ����û�н���
}
//$��е����С���������Ͽ���
//��е����С�����Ͽ��ƶ���
extern uint8_t Status1;
//ʶ�����ԭ��̨�ϵ����,�����С���ƶ�������λ�ã���׷������ܣ�
int Identifier_Masse(int color)//
{
	static int Cont=0;//��¼ץȡ����
	static float Speed = 0;//С���ƶ��ٶ�
	static float Angle = 0;//С���ƶ��Ƕ�  //+90�� -90�� ����ץȡ�ӽ�������
	static float Turn_Speed = 0;//С����ת�ٶ�
	static float TempY = -3.15;
	static float TempZ = 0;
	static uint8_t Status =0;//״̬����־λ
	if(Status==0)//ʶ��ǰ׼������
	{
		//�ƶ���е�۵�������

		if(RobotArm_MovementZ(0,20))//Z�Ḵλ���
		{
			//�򿪻�еצ
//			Control_Sever_rec();
			//�ƶ���������
			if(Arm_WorkDirection())
			{
				Status=1;
			}
		}
	}
	if(Status==1)
	{
		//�½�ʶ�����
		TempZ=6;
		if (Arm_CheckPosition_Z(TempZ)) // ����ָ��λ��
		{
			if(Raspberry_PI_camerDate(SM_Masse)==SM_DateUpdate)//С����ʼ����   ��Y������ȡ����תΪС������ϵ��
			{
				Angle=XYPD_position_control(SM.SM_X,-SM.SM_Y,SM_TargetX,-SM_TargetY);//��������Ƕȣ���Y��Ӹ���ת������ϵ�����õ��Ƕ�
				
				if(range_error>150)//�ٶ��޷�
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
					Status=2;//���궨λ���
					
				}

			}
		}

	}
	if(Status==2)
	{
		if(Raspberry_PI_camerDate(SM_Masse)==SM_DateUpdate)//ʶ����ɫ
		{
			if(SM.SM_coloar==color)
			{
//				Control_Sever_On();
				//��еצ����飬�ͷſ���Ȩ
				TempY=-1;
				TempZ=-1;
			
				Status=3;
				
				
			}
		}

	}
	else if(Status==3)//����β��
	{
		Cont++;
		TempY=-1;
		TempZ=-1;
		Status=0;
		return 1;
	}
	
	
	
	if (TempY != -1) // ʧ�ܿ���,�ó�����Ȩ
	{
		Arm_MovementY(TempY);
	}

	if (TempZ != -1) // ʧ�ܿ���,�ó�����Ȩ����Arm_Up����������Ҫ�ó�����Ȩ��
	{
		Arm_MovementZ(TempZ);
	}
	
	Car_AllangleSpeed(Speed, Angle, Turn_Speed);
	return 0;
}


//ʶ����ϵ�ԲȦ�����ս���Ƕ�׼ԲȦ
int Identifier_circle(void)
{
	static float Speed = 0;//С���ƶ��ٶ�
	static float Angle = 0;//С���ƶ��Ƕ�  //+90�� -90�� ����ץȡ�ӽ�������
	static float Turn_Speed = 0;//С����ת�ٶ�
	static float TempY = -1;//��е��Y��
	static float TempZ = -1;//��е��Z��
	static uint8_t Status =0;//״̬����־λ
	if(Status==0)//ʶ��ǰ׼������
	{
		//�ƶ���е�۵�������
		TempZ=-1;
		Arm_Up();//�ƶ���е������
		if(Xianweikaiguan_Status==2)//Z�Ḵλ���
		{
			//�򿪻�еצ
//			Control_Sever_rec();
			//�ƶ���������
			if(Arm_WorkDirection())
			{
				Status=1;
			}
		}
	}
	if(Status==1)
	{
		//�½�ʶ��Բ
		TempZ=15;
		if (Arm_CheckPosition_Z(TempZ)) // ����ָ��λ��
		{
			if(Raspberry_PI_camerDate(SM_Circle)==SM_DateUpdate)//С����ʼ����   ��Y������ȡ����תΪС������ϵ��
			{
				Angle=XYPD_position_control(SM.SM_X,-SM.SM_Y,SM_TargetX,-SM_TargetY);//��������Ƕȣ���Y��Ӹ���ת������ϵ�����õ��Ƕ�
				
				if(range_error>5)//�ٶ��޷�
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
		//��еצ��λ
		TempZ=-1;
//		Control_Sever_On();
		Arm_Up();//��е������
		if(Xianweikaiguan_Status==2)
		{
			Status=3;
		}
	}
	if(Status==3)//����β��
	{
		TempY=-1;
		TempZ=-1;
		Status=0;
		return 1;
	}
	
	
	
	if (TempY != -1) // ʧ�ܿ���,�ó�����Ȩ
	{
		Arm_MovementY(TempY);
	}

	if (TempZ != -1) // ʧ�ܿ���,�ó�����Ȩ����Arm_Up����������Ҫ�ó�����Ȩ��
	{
		Arm_MovementZ(TempZ);
	}
	
	Car_AllangleSpeed(Speed, Angle, Turn_Speed);
	return 0;
}






//$������
//����Ϊ������
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
