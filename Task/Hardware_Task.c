#include "Hardware_Task.h"
#include "PowerWarning.h"
#include "HWT101.h"
#include "usart.h"

//��ѹ�������
void PowerWarnig_Task(void)
{
	float temp;//�鿴��ѹ
	temp=PowerWarning();
}

//��ȡ��������������
void GetIMUdate_Task(void)
{
	ParseAndPrintData(UART1_RX_Second_BUF,UART1_RX_LEN);//�����ǽ������ȡ����������
}
	