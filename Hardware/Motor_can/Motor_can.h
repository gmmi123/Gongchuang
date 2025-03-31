#ifndef __Motor_can_H
#define __Motor_can_H

#include "main.h"
#include "bsp_fdcan.h"
#include "dm4310_drv.h"
#include "string.h"
#include "math.h"

extern motor_t motor[num];
void dm4310_motor_init(void);
void fdcan1_rx_callback(void);
void fdcan2_rx_callback(void);

void MotorA_SetSpeed(float Val);
void MotorB_SetSpeed(float Val);
void MotorC_SetSpeed(float Val);
void MotorD_SetSpeed(float Val);

//// 单独控制小车前进
void Car_RunSpeed(float Val);
// 单独控制小车自旋
void Car_TurnSpeed(float Val);
// 单独控制小车平移
void Car_TRASpeed(float Val);
// 角度全向控制
void Car_AllangleSpeed(float Speed, float Angle, float Turn_Speed);
// 位置全向控制
void Car_SetSpeed(float Run_Speed, float TRA_Speed, float Turn_Speed);
//xy轴控制
 void Car_XYControl(float X_Out, float Y_Out,float Speed ,float Turn_Speed);
 
  void Car_Control(float Speed, float SpeedAngle,float CarBodyAngle);

#endif
