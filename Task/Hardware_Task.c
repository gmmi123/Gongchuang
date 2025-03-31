#include "Hardware_Task.h"
#include "PowerWarning.h"
#include "usart.h"
#include "PID.h"
#include "IMU.h"
#include "Xianweikaiguan.h"
#include "Motor_can.h"
#include "UsartDate.h"


extern uint8_t Enable_control; // 一键启动标志位
float Motor_InfiniteZ=0;
extern char contentMV[20];
//低压检测任务
void PowerWarnig_Task(void)
{
	float temp;//查看电压

	temp=PowerWarning();
}

//获取陀螺仪数据任务
void GetIMUdate_Task(void)
{
	GetIMUdate();
}

//PID初始化任务
void PIDInit_Task(void)
{
	
}

//解析二维码数据任务
void two_dimensional_Task(void)
{
	static uint8_t status=0;
	if(status==0)
	{
//		if(MV_Parses(UART10_RX_Second_BUF))
//		{
//			status=1;//关闭任务
//		}
	}
	
	
}

//发送数据给串口屏
void SendDateCKP_Task(void)
{
	static uint8_t status=0;
	if(status==0)
	{
		if(contentMV[4]=='+')
		{
			  uint8_t temp[] = {'p', 'a', 'g', 'e', '0', '.', 't', '1', '.', 't', 'x', 't', '=', '\"', contentMV[1],contentMV[2],contentMV[3], '+', contentMV[5], contentMV[6],contentMV[7], '\"', 0xff, 0xff, 0xff};
              HAL_UART_Transmit(&huart7, temp, 25, 0xffff);
			  status=1;
			  //转化字符串数据
			  for(int i=0;i<8;i++)
			  {
				if(contentMV[i]=='1')
				{
					contentMV[i]=1;
				}
				else if(contentMV[i]=='2')
				{
					contentMV[i]=2;
				}
				else if(contentMV[i]=='3')
				{
					contentMV[i]=3;
				}
			  }
		}
	}

}

//限位开关检测任务
void Xianweikaiguan_Task(void)
{
	Xianweikaiguan();
}

//串口一键开始检测
void CKP_CheckStart_Task()
{
	if(CKP_Parses(UART7_RX_Second_BUF))
	{
		Enable_control=1;
	}
}

//无限Z轴任务
//Z轴移动
float InfiniteZ(float Now_Z)
{
	static int flag=0;//记录偏航角突变次数
	static float Last_Z=0;
	if(Now_Z-Last_Z<-12.5)//如果存在角度突变
	{
		flag++;
	}
	else if(Now_Z-Last_Z>12.5)
	{
		flag--;
	}
 
    Last_Z = Now_Z;
    return Now_Z+ flag*25;
}

void InfiniteZ_Task(void)
{
	Motor_InfiniteZ=InfiniteZ(motor[Motor6].para.pos);

}