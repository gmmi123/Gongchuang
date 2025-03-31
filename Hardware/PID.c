#include "PID.h"
float k=0.0f;//停车给积分
float k_2=0.0f;

	
void PID_Init(PID_TypeDef *PID)
{
	PID->Kp = 0;
	PID->Ki = 0;
	PID->Kd = 0;
	PID->Target = 0;
	PID->Last_Error = 0;				
	PID->Error = 0;		    
	PID->I_OUT_Limit = 100000;	  
	PID->PID_Out = 0; 
	PID->PID_OUT_Limit = 9500; 
	PID->Current_Value = 0;
	PID->Clear_flag = 0;
}

void Set_PID(PID_TypeDef *PID,float Kp,float Ki,float Kd)
{
	PID->Kp = Kp;
	PID->Ki = Ki;
	PID->Kd = Kd;
}


float Position_PID(PID_TypeDef *PID,float Target)
{
	float k=0.75;
	PID->Target = Target;
	
	//得到误差
	PID->Error = PID->Current_Value - PID->Target;
	/*低通滤波*/
//	PID->Error = PID->Error*(1-k)+k * PID->Last_Error;
	
	//I积分
	PID->I_OUT += PID->Error;
	//积分限幅
	if(PID->I_OUT > PID->I_OUT_Limit){PID->I_OUT = PID->I_OUT_Limit;}
	else if(PID->I_OUT < (0 - PID->I_OUT_Limit)){PID->I_OUT = 0 - PID->I_OUT_Limit;}
	
	if(PID->Clear_flag == 1){PID->I_OUT = k*PID->I_OUT;PID->Clear_flag = 0;}
	if(PID->Clear_flag == 2){PID->I_OUT = k_2*PID->I_OUT;PID->Clear_flag = 0;}
	
	//记录当前的误差
	PID->Last_Error = PID->Error;
	//输出PID结果
	PID->PID_Out = PID->Error * PID->Kp + PID->I_OUT * PID->Ki + (PID->Error - PID->Last_Error)*PID->Kd;
	//输出限幅
	if(PID->PID_Out > PID->PID_OUT_Limit){PID->PID_Out = PID->PID_OUT_Limit;}
	else if(PID->PID_Out < (0 - PID->PID_OUT_Limit)){PID->PID_Out = 0 - PID->PID_OUT_Limit;}
	return PID->PID_Out;
}

float Angle_PID(PID_TypeDef *PID,float Target,float groy)
{
	PID->Target = Target;
	PID->Error = PID->Current_Value - PID->Target;

	PID->I_OUT+=PID->Error;
	
	if(PID->Clear_flag == 1){PID->I_OUT = 0;}
	else if(PID->I_OUT > PID->I_OUT_Limit){PID->I_OUT = PID->I_OUT_Limit;}
	else if(PID->I_OUT < (0 - PID->I_OUT_Limit)){PID->I_OUT = 0 - PID->I_OUT_Limit;}
	
	PID->PID_Out = PID->Error * PID->Kp + PID->I_OUT * PID->Ki + groy*PID->Kd;
	if(PID->PID_Out > PID->PID_OUT_Limit){PID->PID_Out = PID->PID_OUT_Limit;}
	else if(PID->PID_Out < (0 - PID->PID_OUT_Limit)){PID->PID_Out = 0 - PID->PID_OUT_Limit;}
	return PID->PID_Out;
}


