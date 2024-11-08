#ifndef __Hardware_Task_H
#define __Hardware_Task_H

#include "main.h"

//低压检测任务
void PowerWarnig_Task(void);

//获取陀螺仪数据任务
void GetIMUdate_Task(void);

#endif
