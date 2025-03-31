#include "Sever_control.h"
//@¶æ»ú

//



//µ×²ãÇý¶¯
int Serve_1_PZ = 146;
int Serve_2_PZ = 47;//45

void Claw_Angle(float Angle,int Claw_PZ1,int Claw_PZ2)
{
	float Serve_1 = 149.0f;
	float Serve_2 = 40.5f;
	if(Angle<-5)
	{
		Angle = -5;
	}
	if(Angle>180)
	{
		Angle = 180;
	}
	
	Serve_1 = Claw_PZ1-Angle/2;
	Serve_2 = Claw_PZ2+Angle/2;
	
	
	
	TIM2->CCR1 = (2000/180*Serve_1)+500;//1850;
	TIM2->CCR3 = (2000/180*Serve_2)+500;//1610;

}





