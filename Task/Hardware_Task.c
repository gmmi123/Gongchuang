#include "Hardware_Task.h"
#include "PowerWarning.h"
#include "HWT101.h"
#include "usart.h"

//低压检测任务
void PowerWarnig_Task(void)
{
	float temp;//查看电压
	temp=PowerWarning();
}

//获取陀螺仪数据任务
void GetIMUdate_Task(void)
{
	ParseAndPrintData(UART1_RX_Second_BUF,UART1_RX_LEN);//陀螺仪解包，获取陀螺仪数据
}
	