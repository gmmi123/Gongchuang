#include "Task_Management.h"
#include "Hardware_Task.h"

//�ͼ������������(��main��ִ��)
void Low_Task(void)
{
	//��ѹ�������
	PowerWarnig_Task();

}

//�м�(20ms)
void Medium_Task(void)
{

	
	
}

//�߼�(15ms)
void Hight_Task(void)
{

	//��ȡ��������������
	GetIMUdate_Task();
}