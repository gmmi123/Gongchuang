#ifndef __Control_H
#define __Control_H

#include "main.h"
#include "Motor_can.h"
#include "dm4310_drv.h"
// 抓取顺序点,依次进行
#define Get_sequence1 -5.81 + 0.01
#define Get_sequence2 -0.01 + 0.005 // 减小往右
#define Get_sequence3 -0.53 + 0		//
#define Place_sequence1 -5.81 + 0.01
#define Place_sequence2 -6.35 + 0.01
#define Place_sequence3 -0.555 + 0.01

typedef enum
{
    SM_DateUpdate = 1,
    SM_unidentified = -1,
    SM_DateReceive_error = 0,
    SM_Circle = '2',
	SM_Masse = '1'

} SM_DateTypeDef;

void Motor_ControlInit(void);
void Control_Test(void);
;
// 获取树莓派摄像头数据(圆环)
int Raspberry_PI_camerDate(char DateType);//需要的数据类型

void Arm_MovementY(float Val); // 控制机械臂水平转动

void Arm_MovementZ(float Val); // 控制机械臂上下移动
//// 控制机械爪开
//void Control_Sever_On(void);

//// 控制机械爪关
//void Control_Sever_Off(void);


////视觉识别机械爪张开
//void Control_Sever_rec(void);
// 检测Z轴是否到指定位置
int Arm_CheckPosition_Z(float Val);

// 检测Y轴是否到指定位置
int Arm_CheckPosition_Y(float Val);
//机械臂移动到工作区
int Arm_WorkDirection(void);
//识别物块定位
int Identifier_Masse(int color);
//识别圆圈
int Identifier_circle(void);

int Arm_Get_Block(float Target_Position);//控制机械臂放方块

int Arm_Place_Block(float Target_Position);//控制机械臂放置方块


#endif
