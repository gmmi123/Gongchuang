#ifndef __MySerialScreen_H
#define __MySerialScreen_H

#include "MyRaspberryPi.h"
#include "UsartDate.h"
#include "usart.h"
// 成员变量
typedef struct
{
	//串口屏使能小车标志位
	 char *TaskMassage;
	char SendTaskMassage_flag;
	char EnableCar_flag;

} MySerialScreenDate_Typedef;
 
// 类
typedef struct
{
	MySerialScreenDate_Typedef Date;
	
}MySerialScreen_Typedef;
extern MySerialScreen_Typedef SerialScreen;
void MySerialScreen_Task(MySerialScreen_Typedef* object);
#endif
