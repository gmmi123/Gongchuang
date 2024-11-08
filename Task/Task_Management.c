#include "Task_Management.h"
#include "Hardware_Task.h"

//低级任务管理中心(在main中执行)
void Low_Task(void)
{
	//低压检测任务
	PowerWarnig_Task();

}

//中级(20ms)
void Medium_Task(void)
{

	
	
}

//高级(15ms)
void Hight_Task(void)
{

	//获取陀螺仪数据任务
	GetIMUdate_Task();
}