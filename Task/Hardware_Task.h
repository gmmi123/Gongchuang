#ifndef __Hardware_Task_H
#define __Hardware_Task_H

#include "main.h"
//限位开关检测任务
void Xianweikaiguan_Task(void);
//低压检测任务
void PowerWarnig_Task(void);

//获取陀螺仪数据任务
void GetIMUdate_Task(void);
//无限Z轴任务
void InfiniteZ_Task(void);
//检测串口屏的一键启动开关任务
void CKP_CheckStart_Task(void);
//解析二维码数据任务
void two_dimensional_Task(void);
//发送数据给串口屏
void SendDateCKP_Task(void);
#endif
