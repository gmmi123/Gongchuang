#include "Xianweikaiguan.h"
#include "dm4310_drv.h"
#include "Motor_can.h"

uint8_t Button_Status=0;//1为按下，0为松开
uint8_t ChangAnChiXu=0;//长按持续中
uint8_t DuanAnChiXu=0;//短按持续中
uint8_t ChangAn = 0;//长按
uint8_t DuanAn =0;//短按
uint8_t LianJi_DuanAn =0;//短按连击
uint8_t Shuangji=0;//双击
uint8_t LianJi_ChangAn =0;//长按连击(松开)
uint8_t ButtonVal=0;//按键反馈的功能
uint32_t time=0;//按下的时间
uint32_t time2=0;//松开的时间
uint32_t time3=0;

//限位开关设置一次标志位
uint8_t Xianweikaiguan_Status=0;

void Check_ButtonStatus()
{
	//检测按键状态
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
	//根据按键状态和时间判断长按，短按
	
	if(Button_Status==1)
	{
		//结束松开计时
		time2=0;
		if(time<1000)time++;//上限5秒//按下时间增加
		if(time>200)//大于1秒,长按持续
		{
			ChangAnChiXu=1;
		}
		else if(time>0&&time<200)//小于1s,短按持续
		{
			DuanAnChiXu=1;
		}
		
	}
	else
	{
		//结束按键持续
		ChangAnChiXu=0;
		DuanAnChiXu=0;
		if(time>200)//大于1秒,长按
		{
			ChangAn=1;
		}
		else if(time>0&&time<200)//小于1s,短按
		{
			DuanAn=1;
		}
		//结束按下计时
		time=0;
		//松开计时
		if(time2<1000)time2++;//上限5秒
	}
	
		//短按连击
	if(DuanAn==1)
	{
		
		LianJi_DuanAn++;
		DuanAn=0;
		if(time3<=50)//双击
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
	
	if(time2>200)//结束短按连击
	{
		LianJi_DuanAn=0;
	}
	

	
	
	if(ChangAn==1)
	{
		LianJi_ChangAn++;
		ChangAn=0;
	}
	if(time2>600)//结束长按连击
	{
		LianJi_ChangAn=0;
	}
}


extern float Arm_Zero;
extern float Motor_InfiniteZ;
void Process_Button()//限位开关处理
{
	//碰到限位开关设置零点
	if(DuanAnChiXu==1)
	{
		//碰到限位开关电机停止
		pos_speed_ctrl(&hfdcan2,motor[Motor6].id,0,0);
		//设置一次零点
		if(Xianweikaiguan_Status<1)
		{
			Xianweikaiguan_Status=1;//置一，设置零点中
		}
		
		if(Xianweikaiguan_Status==1)//设置零点
		{
			 
			//电机停止
			pos_speed_ctrl(&hfdcan2,motor[Motor6].id,0,0);
			Xianweikaiguan_Status=2;//设置完零点，保持设置完成零点状态，在低电平清理
//			Arm_Zero=Motor_InfiniteZ;// 设置零点
			

			
		}
	}
	if(DuanAnChiXu==0)
	{
		//清除设置零点状态
		Xianweikaiguan_Status=0;
		
	}
}

void Xianweikaiguan(void)
{
	Check_ButtonStatus();//检测按键状态
	Judement_ButtonStatus();//根据按键状态和时间判断长按，短按和没有按下
	Process_Button();//处理按键
}