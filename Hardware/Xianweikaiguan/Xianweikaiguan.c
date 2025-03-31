#include "Xianweikaiguan.h"
#include "dm4310_drv.h"
#include "Motor_can.h"

uint8_t Button_Status=0;//1Ϊ���£�0Ϊ�ɿ�
uint8_t ChangAnChiXu=0;//����������
uint8_t DuanAnChiXu=0;//�̰�������
uint8_t ChangAn = 0;//����
uint8_t DuanAn =0;//�̰�
uint8_t LianJi_DuanAn =0;//�̰�����
uint8_t Shuangji=0;//˫��
uint8_t LianJi_ChangAn =0;//��������(�ɿ�)
uint8_t ButtonVal=0;//���������Ĺ���
uint32_t time=0;//���µ�ʱ��
uint32_t time2=0;//�ɿ���ʱ��
uint32_t time3=0;

//��λ��������һ�α�־λ
uint8_t Xianweikaiguan_Status=0;

void Check_ButtonStatus()
{
	//��ⰴ��״̬
	if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_9)==0)
	{
		Button_Status=1;//
	}
	else
	{
		Button_Status=0;
	}
}


void Judement_ButtonStatus()
{
	//���ݰ���״̬��ʱ���жϳ������̰�
	
	if(Button_Status==1)
	{
		//�����ɿ���ʱ
		time2=0;
		if(time<1000)time++;//����5��//����ʱ������
		if(time>200)//����1��,��������
		{
			ChangAnChiXu=1;
		}
		else if(time>0&&time<200)//С��1s,�̰�����
		{
			DuanAnChiXu=1;
		}
		
	}
	else
	{
		//������������
		ChangAnChiXu=0;
		DuanAnChiXu=0;
		if(time>200)//����1��,����
		{
			ChangAn=1;
		}
		else if(time>0&&time<200)//С��1s,�̰�
		{
			DuanAn=1;
		}
		//�������¼�ʱ
		time=0;
		//�ɿ���ʱ
		if(time2<1000)time2++;//����5��
	}
	
		//�̰�����
	if(DuanAn==1)
	{
		
		LianJi_DuanAn++;
		DuanAn=0;
		if(time3<=50)//˫��
		{
			Shuangji=1;
		}
		time3=1;
	}
	else
	{
		if(time3<=1000)
		{
			time3++;
		}
		if(time3>=10)
		{
			Shuangji=0;
		}
		
	}
	
	if(time2>200)//�����̰�����
	{
		LianJi_DuanAn=0;
	}
	

	
	
	if(ChangAn==1)
	{
		LianJi_ChangAn++;
		ChangAn=0;
	}
	if(time2>600)//������������
	{
		LianJi_ChangAn=0;
	}
}


extern float Arm_Zero;
extern float Motor_InfiniteZ;
void Process_Button()//��λ���ش���
{
	//������λ�����������
	if(DuanAnChiXu==1)
	{
		//������λ���ص��ֹͣ
		pos_speed_ctrl(&hfdcan2,motor[Motor6].id,0,0);
		//����һ�����
		if(Xianweikaiguan_Status<1)
		{
			Xianweikaiguan_Status=1;//��һ�����������
		}
		
		if(Xianweikaiguan_Status==1)//�������
		{
			 
			//���ֹͣ
			pos_speed_ctrl(&hfdcan2,motor[Motor6].id,0,0);
			Xianweikaiguan_Status=2;//��������㣬��������������״̬���ڵ͵�ƽ����
//			Arm_Zero=Motor_InfiniteZ;// �������
			

			
		}
	}
	if(DuanAnChiXu==0)
	{
		//����������״̬
		Xianweikaiguan_Status=0;
		
	}
}

void Xianweikaiguan(void)
{
	Check_ButtonStatus();//��ⰴ��״̬
	Judement_ButtonStatus();//���ݰ���״̬��ʱ���жϳ������̰���û�а���
	Process_Button();//������
}