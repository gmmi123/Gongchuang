#ifndef __PID_H
#define __PID_H

#include "main.h"

typedef struct {
  float Kp;
  float Ki;
  float Kd;
	float Target;
	float Last_Error;				
	float Error;		
	float I_OUT;
	float I_OUT_Limit;	  
	float PID_Out;  
	float PID_OUT_Limit;
	float Current_Value;
	uint8_t Clear_flag;
}PID_TypeDef;


float Position_PID(PID_TypeDef *PID,float Target);
float Angle_PID(PID_TypeDef *PID,float Target,float groy);
void Set_PID(PID_TypeDef *PID,float Kp,float Ki,float Kd);
void PID_Init(PID_TypeDef *PID);
//float Angle_error(float Target, float Now);
#endif
