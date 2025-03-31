#include "MyRaspberryPi.h"
#include "string.h"
#include <stdio.h>
#include "main.h"
#include "usart.h"
#include "MyCarControl.h"
#include "math.h"
#include "MyArm.h"
#include <stdlib.h>
#include <string.h>
#include "MyMainWhile.h"
MyRaspberryPi_Typedef RaspberryPi={
.Date.Mod = '0',
.Date.dt = 0.02,
.Date.t = 1,
.Date.PID.Kp =-0.2,//-1
.Date.PID.Kd =0,
.Date.PID.TargetX = 310,
.Date.PID.TargetY = 220,
	
.Date.ArmPID.Kp =0.1,//-1
.Date.ArmPID.Kd =0,
.Date.ArmPID.TargetX = 310,
.Date.ArmPID.TargetY = 220,
.Date.ArmPID.TempH = 10,
.Date.ArmPID.TempXYH = 2,
};

int TempTest;
int TempTest2;
int TempTest3;

int MyRaspberryPi_ParsesColor(void)
{
//	if(RaspberryPi.Date.ColorFlag ==0)
//	{
//		RaspberryPi.Date.LastColot = RaspberryPi.Date.TaskMassage.BlockColor[ RaspberryPi.Date.TaskMassage.num];
//	}
//	if(RaspberryPi.Date.Block.color!=0&&RaspberryPi.Date.ColorFlag ==0)
//	{
//		
//		if(RaspberryPi.Date.LastColot!= RaspberryPi.Date.Block.color&&RaspberryPi.Date.ColorFlag ==0)
//		{
//			RaspberryPi.Date.ColorFlag =1;
//		}
//		else
//		{
//			RaspberryPi.Date.LastColot = RaspberryPi.Date.Block.color;
//		}
//		
//	}&&RaspberryPi.Date.ColorFlag==1
	if(RaspberryPi.Date.Block.color == RaspberryPi.Date.TaskMassage.BlockColor[ RaspberryPi.Date.TaskMassage.num])
	{
		return 1;
	}
	else
	{
		return 0;
	}


}


//float TempNum1;
//float TempNum2;
//float TempNum3;
//float TempNum4;
//float TempNum5;
int MyRaspberryPi_ArmTraceBlock(MyRaspberryPiDate_Typedef *object)
{
	float retX;
	float retY;	
	float TempX;
	float TempY;
	
	static float LastX;
	static float LastY;
	static char  Block;
	//判断与上一次的误差
	if(fabsf(fabsf(LastX)-fabsf(object->Block.x))<1)
	{
		if(fabsf(fabsf(LastY)-fabsf(object->Block.y))<1)
		{
			Block=1;//认为方块静止
		}
	}
	if(Block==1)
	{
		TempX = -(object->Block.x-object->ArmPID.TargetX)*cosf((180-Arm.Date.SingleArm[ArmB].Angle)/180.0f*3.1415926f)+(object->Block.y-object->ArmPID.TargetY)*sinf((180-Arm.Date.SingleArm[ArmB].Angle)/180.0f*3.1415926f);
		TempY = (object->Block.x-object->ArmPID.TargetX)*sinf((180-Arm.Date.SingleArm[ArmB].Angle)/180.0f*3.1415926f)+(object->Block.y-object->ArmPID.TargetY)*cosf((180-Arm.Date.SingleArm[ArmB].Angle)/180.0f*3.1415926f);

		object->ArmPID.ErrX =-TempX;
		object->ArmPID.ErrY =-TempY;
		
		retX = object->ArmPID.ErrX*object->ArmPID.Kp + object->ArmPID.Kd*(object->ArmPID.ErrX-object->ArmPID.LastErrX);
		retY = object->ArmPID.ErrY*object->ArmPID.Kp + object->ArmPID.Kd*(object->ArmPID.ErrY-object->ArmPID.LastErrY);

		object->ArmPID.LastErrX = object->ArmPID.ErrX;
		object->ArmPID.LastErrY = object->ArmPID.ErrY;
		
		if(retX>object->ArmPID.TempH)
		{
			retX = object->ArmPID.TempH ;
		}
		if(retX<-object->ArmPID.TempH)
		{
			retX = -object->ArmPID.TempH ;
		}
		
		if(retY>object->ArmPID.TempH)
		{
			retY = object->ArmPID.TempH ;
		}
		if(retY<-object->ArmPID.TempH)
		{
			retY = -object->ArmPID.TempH ;
		}
		Arm.Date.RCS.Vx = retX;
		Arm.Date.RCS.Vy  = retY;

		if(fabsf(object->ArmPID.ErrX)<object->ArmPID.TempXYH&&fabsf(object->ArmPID.ErrY)<object->ArmPID.TempXYH)
		{
			//识别完成,复位变量
			RaspberryPi.Date.Block.x = 0;
			RaspberryPi.Date.Block.y = 0;
			Arm.Date.RCS.Vx =0;
			Arm.Date.RCS.Vy =0;
			Block=0;
			return 1;
		}
		else
		{
			return 0;
		}
	}
	
	
	//更新方块位置
	LastX = object->Block.x;
	LastY = object->Block.y;
	return 0;
}
int MyRaspberryPi_carTraceBlock(MyRaspberryPiDate_Typedef *object)
{
	float retX;
	float retY;	
	float TempX;
	float TempY;
	

	TempX = -(object->Block.x-object->PID.TargetX)*cosf((180-Arm.Date.SingleArm[ArmB].Angle)/180.0f*3.1415926f)+(object->Block.y-object->PID.TargetY)*sinf((180-Arm.Date.SingleArm[ArmB].Angle)/180.0f*3.1415926f);
	TempY = (object->Block.x-object->PID.TargetX)*sinf((180-Arm.Date.SingleArm[ArmB].Angle)/180.0f*3.1415926f)+(object->Block.y-object->PID.TargetY)*cosf((180-Arm.Date.SingleArm[ArmB].Angle)/180.0f*3.1415926f);

	object->PID.ErrX =-TempX;
	object->PID.ErrY =-TempY;
	
	retX = object->PID.ErrX*object->PID.Kp + object->PID.Kd*(object->PID.ErrX-object->PID.LastErrX);
	retY = object->PID.ErrY*object->PID.Kp + object->PID.Kd*(object->PID.ErrY-object->PID.LastErrY);

	object->PID.LastErrX = object->PID.ErrX;
	object->PID.LastErrY = object->PID.ErrY;
	
	if(retX>80)
	{
		retX = 80 ;
	}
	if(retX<-80)
	{
		retX = -80 ;
	}
	
	if(retY>20)
	{
		retY = 20 ;
	}
	if(retY<-20)
	{
		retY = -20 ;
	}
	Car.Date.SetVx = retX;
	Car.Date.SetVy = retY;
	if(fabsf( object->PID.ErrX)<2&&fabsf( object->PID.ErrY)<2)
	{

		return 1;
	}
	else
	{
		return 0;
	}
}


void MyRaspberryPi_GetTaskMassage(MyRaspberryPiDate_Typedef *object)
{
//	static int flag;
//	if(flag==0)
//	{
	if(object->Mod!='4')
	{
		sscanf((const char*)UART9_RX_Second_BUF,"sm %[^ms]" ,object->TaskMassage.TempDate);
	}
		
//		if(object->TaskMassage.TempDate[3]=='+')
//		{
//		
//		  flag =1;
//		}

//	}
}


void extractData(const char *input, float *f1, float *f2, int *i) {
    // 查找 "sm" 的位置
	TempTest2++;
	if(TempTest2>50)
	{
		TempTest3 = TempTest;
		TempTest2=0;
		TempTest=0;
	}
    const char *sm_pos = strstr(input, "sm");
    if (sm_pos == NULL) {
        return;
    }

    // 定位到 "sm" 之后的数据部分
    const char *data_start = sm_pos + 2; // 跳过 "sm"

    // 去掉结尾的 "ms"
    char buffer[30];
    strncpy(buffer, data_start, strlen(data_start) - 2); // 去掉 "ms"
    buffer[strlen(data_start) - 2] = '\0'; // 确保字符串以 '\0' 结尾

    // 使用 strtok 分割字符串
    char *token = strtok(buffer, ",");
    *f1 = atof(token);

    token = strtok(NULL, ",");
    *f2 = atof(token);

    token = strtok(NULL, ",");
    *i = atoi(token);
	TempTest++;

}



void MyRaspberryPi_GetBlockMassage(MyRaspberryPiDate_Typedef *object)
{
	UART9_flag=1;
	extractData((const char*)UART9_RX_Second_BUF ,&object->Block.x,&object->Block.y,&object->Block.color);
	UART9_flag=0;
	if(object->Block.color==4)
	{
		object->LineAngle = object->Block.x;
	}
}

void MyRaspberryPi_SendMod(MyRaspberryPiDate_Typedef *object)
{
	if(object->t>0)
	{
		object->t-=object->dt;
	}
	if(object->t<0)
	{
	 HAL_UART_Transmit(&huart9, (unsigned char*)&object->Mod, 1, 0xffff);
	 object->t = 0.5;
	}
}



void MyRaspberryPi_Init(MyRaspberryPi_Typedef* object)
{
	
}


void MyRaspberryPi_Task(MyRaspberryPi_Typedef* object)
{
	MyRaspberryPi_GetTaskMassage(&object->Date);
	MyRaspberryPi_SendMod(&object->Date);
}