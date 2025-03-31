#include "Task_Management.h"
#include "Hardware_Task.h"
#include "MyClaw.h"
#include "MyCarControl.h"
#include "MyIMU.h"
#include "MyLift.h"
#include "MyTurnPlate.h"
#include "MyArm.h"
#include "MyGrabBlock.h"
#include "MyMainWhile.h"
#include "MyRaspberryPi.h"
#include "MySerialScreen.h"
#include "MyDebug.h"
#include "MyTSpeed.h"
#include "HWT101.h"
#include "usart.h"
#include "Buzzer.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�ܿ�����ر�־λ
uint8_t Enable_control = 0; // һ��������־λ
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




// �ͼ������������(��main��ִ��)
void Low_Task(void)
{
	// ��ѹ�������
	PowerWarnig_Task();
	MyRaspberryPi_GetBlockMassage(&RaspberryPi.Date);
	// ��ʼ������
//	Init_Task();
	//ʶ���ά������
//	two_dimensional_Task();
//	//�������ݸ�����������
//	SendDateCKP_Task();
}

// �м������������(20ms)
void Medium_Task(void)
{

	MyCarControl_Task(&Car);
	MyArm_Task(&Arm);
	
	MyRaspberryPi_Task(&RaspberryPi);
	MySerialScreen_Task(&SerialScreen);
	
	MyLift_Task(&Lift);
	MyTurnPlate_Task(&TurnPlate);
	MyClaw_Task(&Claw);
	MyMainwhile();
	MyDebug_Task(&Debug);
	Buzzer_Task();
//	MyGrabBlock_Task();
}

// �߼������������(15ms)
void Hight_Task(void)
{
	
	// ��λ����,˳�㸴λ
//	Xianweikaiguan_Task();
	// ��ȡ��������������
	if(MyIMU.MyIMUDate.Enable==1)
	{
		ParseAndPrintData(UART10_RX_Second_BUF,UART10_RX_LEN);
//		GetIMUdate_Task();
		MyIMU_Task(&MyIMU);
		
	}

}