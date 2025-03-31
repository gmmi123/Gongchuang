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
//总控制相关标志位
uint8_t Enable_control = 0; // 一键启动标志位
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




// 低级任务管理中心(在main中执行)
void Low_Task(void)
{
	// 低压检测任务
	PowerWarnig_Task();
	MyRaspberryPi_GetBlockMassage(&RaspberryPi.Date);
	// 初始化任务
//	Init_Task();
	//识别二维码任务
//	two_dimensional_Task();
//	//发送数据给串口屏任务
//	SendDateCKP_Task();
}

// 中级任务管理中心(20ms)
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

// 高级任务管理中心(15ms)
void Hight_Task(void)
{
	
	// 限位开关,顺便复位
//	Xianweikaiguan_Task();
	// 获取陀螺仪数据任务
	if(MyIMU.MyIMUDate.Enable==1)
	{
		ParseAndPrintData(UART10_RX_Second_BUF,UART10_RX_LEN);
//		GetIMUdate_Task();
		MyIMU_Task(&MyIMU);
		
	}

}