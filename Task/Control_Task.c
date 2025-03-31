#include "Control_Task.h"
#include "Motor_can.h"
#include "math.h"
#include "Control.h"
#include "IMU.h"
#include "Buzzer.h"
#include "Control_Plus.h"
//��ά����Ϣ
//ץȡ����ɫ��Ϣ
extern char contentMV[20];
#define two_dimensional_1_grab  contentMV[1]  
#define two_dimensional_2_grab  contentMV[2]
#define two_dimensional_3_grab  contentMV[3]
#define two_dimensional_4_grab  contentMV[5]  
#define two_dimensional_5_grab  contentMV[6]
#define two_dimensional_6_grab  contentMV[7]
//mv����
extern char contentMV[20];

extern uint8_t Xianweikaiguan_Status;//��λ���ر�־��   2Ϊײ�����(��λ���)
extern float Motor_InfiniteZ;//���Z����ֵ
extern motor_t motor[num];// ���������ȡ
extern float yaw_mahony;// �����ǽǶ�
// ��������������
extern float World_X;
extern float World_Y;
extern float Target_X;
extern float Target_Y;

uint8_t Robotic_arminit_Status = 0; // ��е�۳�ʼ����־λ,0Ϊδ��ʼ��,1Ϊ��ʼ�����
//��е���������
float Arm_Zero=0;


// �Ƕ�ת����
float degrees_to_radians(float degrees)
{
	return degrees * (3.1415926 / 180.0);
}

// ����ת�Ƕ�
float radians_to_degrees(float radians)
{
	return radians * (180.0 / 3.1415926);
}

// ·�̼������������������ͬһ����ʱ���жϣ�
void Distance_calcu_Task(void)
{
	//	// �����ǽǶ�ת����
	float IMU_radians_Y = degrees_to_radians(IMU.YawAngle); // С������ϵ��Y(��С��ǰ���ķ���ΪY)
	float IMU_radians_X = degrees_to_radians(yaw_mahony);	// С������ϵ��X(��ǰ���ķ���,������X)
	// ��¼·���ھ��������µ�X��.Y��

	float Car_Vy = (motor[Motor1].para.vel + (-motor[Motor2].para.vel) + (-motor[Motor3].para.vel) + motor[Motor4].para.vel);
	float Car_Vx = -(motor[Motor1].para.vel - (-motor[Motor2].para.vel) + (-motor[Motor3].para.vel) - motor[Motor4].para.vel);
	World_Y += Car_Vy * sinf(IMU_radians_Y) + Car_Vx * sinf(IMU_radians_X);
	World_X += Car_Vx * cosf(IMU_radians_X) + Car_Vy * cosf(IMU_radians_Y);
}

#define CarSpeed 10   

uint8_t Arm_PlaceStatus=1;//0Ϊ��ͣ��1Ϊ������״̬��2Ϊ�������״̬
uint8_t Arm_PlaceCont =0;//ץȡ����
void Control_Task_2() //��̨����,���ƻ�е��
{
	if(Arm_PlaceStatus==1)//����״̬
	{	
		if(Arm_PlaceCont==1)//��1��ץȡ
		{
			if(Arm_Get_Block(Get_sequence1))
			{
				Arm_PlaceStatus=2;
			}
		}
		else if(Arm_PlaceCont==2)//��2��ץȡ
		{
			if(Arm_Get_Block(Get_sequence2))
			{
				Arm_PlaceStatus=2;
			}
		}
		else if(Arm_PlaceCont==3)//��3��ץȡ
		{
			if(Arm_Get_Block(Get_sequence3))
			{
				Arm_PlaceStatus=2;
			}		
		}

	}
	else if(Arm_PlaceStatus==2)//��λ
	{
		Arm_PlaceStatus=0;
	}
	
}

int PlaceTemp(int Val)//�����n������
{
	if(Arm_PlaceStatus==0)//ȷ����е��û���ڹ���
	{
		Arm_PlaceStatus=1;//��ʼץ����
	}
	Arm_PlaceCont=Val;//ץ��n������
	if(Arm_PlaceStatus==2)//ץ�귽��
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
void Control_Task_1(void) // ��������·��
{	

	
	static int time = 0;
	if (Status1 == 0) 
	{
		SpeedCar= CarSpeed;// ����
		AngleCar= 135.0;
		RobotArm.RobotArm_SpeedY = 1;//չ����е�ۿ����ά��
		RobotArm.RobotArm_TargetY = -3.14;
		if (World_X <= -450)
		{
			Status1 = 1;
		}
	} 
	if (Status1 == 1) // ǰ��ɨ����
	{
		SpeedCar= CarSpeed;
		AngleCar= 90.0;
		if (World_Y >= 2000)
		{

				Status1 = 2;
		}
	}
	if (Status1 == 2) // ����ɨ��
	{
		Car.CarControl.Speed= 0;
		if(contentMV[4]=='+')
		{
			SetBlockColor();
			Status1 = 3;//ʶ���룬ǰ��ץ��
		}
		
	}
	if (Status1 == 3) // ǰ��ץ��
	{
		SpeedCar= CarSpeed;
		AngleCar= 90.0;
		if (World_Y >= 4300)
		{
			Status1 = 4;
			SpeedCar=0;
		}
	}
	if (Status1 == 4) // ����ץ��
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
	if (Status1 == 5) // ��ͷ
	{
		Car.CarControl.Speed= CarSpeed;
		Car.CarControl.Angle= -90.0;
		if (World_Y <= 3400)
		{
			Status1 = 6;
			Car.CarControl.Speed= 0;
		}
	}
	if (Status1 == 6) // ת��
	{
		Car.CarControl.Speed= 0;
		Car.CarControl.Turn_Speed= 5;
		if (IMU.Full_YawAngle>= 175)
		{
			Car.CarControl.Turn_Speed= 0;
			Status1 = 7;
		}
	}
	if (Status1 == 7) // ǰ���ӹ���
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
	if (Status1 == 8) // ��ת׼���ӹ�
	{
		Car.CarControl.Turn_Speed= 5;
		if (IMU.Full_YawAngle>= 265)
		{
			Car.CarControl.Turn_Speed= 0;
			Status1 = 9;
		}
	}
	if (Status1 == 9) // �ӹ�
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
	if (Status1 == 10) // ��ֱ��
	{
		Car.CarControl.Speed= CarSpeed;
		Car.CarControl.Angle= 90;
		if (World_Y >= 5600)
		{
			Car.CarControl.Speed= 0;
			Status1 = 11;
		}
	}
	if (Status1 == 11) // ת��
	{
		Car.CarControl.Turn_Speed= -5;
		if (IMU.Full_YawAngle<= 180)
		{
			Car.CarControl.Turn_Speed= 0;
			Status1 = 12;
		}
	}
	if (Status1 == 12) // �ߵ�������
	{
		Car.CarControl.Speed= CarSpeed;
		Car.CarControl.Angle= 0;
		if (World_X >= -3000)
		{
			Car.CarControl.Speed= 0;
			Status1 = 13;
		}
	}
	if (Status1 == 13) // ����
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
	if (Status1 == 14) // ֱ��
	{
		Car.CarControl.Speed= CarSpeed;
		Car.CarControl.Angle= 0;
		if (World_X >= -300)
		{
			Car.CarControl.Speed= 0;
			Status1 = 15;
		}
	}
	if (Status1 == 15) // ת��
	{
		Car.CarControl.Turn_Speed= -5;
		if (IMU.Full_YawAngle< 90)
		{
			Car.CarControl.Turn_Speed= 0;
			Status1 = 16;
		}
	}
	if (Status1 == 16) // ֱ��
	{
		Car.CarControl.Speed= CarSpeed;
		Car.CarControl.Angle= -90;
		if (World_Y <= 4300)
		{

			Car.CarControl.Speed= 0;
		}
	}


		

}

// ��е�۳�ʼ��
uint8_t Robotic_arminit_Task(void)
{

	if (Robotic_arminit_Status == 0) // ��е�ۿ�ʼ��ʼ��
	{
		// �жϻ�е���Ƿ������,����ʱ�ȸ�λ
		if (Xianweikaiguan_Status == 0)
		{
//			// �ϵ��е������
			pos_speed_ctrl(&hfdcan2, motor[Motor6].id, -34, 6);
//			speed_ctrl(&hfdcan2, motor[Motor6].id,0.5);
		}
		if (Xianweikaiguan_Status != 0) // ��е�۸�λ���
		{
			

			Arm_Zero=Motor_InfiniteZ;// �������
			Robotic_arminit_Status = 1;
		}
		return 1;
	}
	if (Robotic_arminit_Status == 1) // ��̨�׵����λ
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

	return -1; // ����
}

