#ifndef __MySerialScreen_H
#define __MySerialScreen_H

#include "MyRaspberryPi.h"
#include "UsartDate.h"
#include "usart.h"
// ��Ա����
typedef struct
{
	//������ʹ��С����־λ
	 char *TaskMassage;
	char SendTaskMassage_flag;
	char EnableCar_flag;

} MySerialScreenDate_Typedef;
 
// ��
typedef struct
{
	MySerialScreenDate_Typedef Date;
	
}MySerialScreen_Typedef;
extern MySerialScreen_Typedef SerialScreen;
void MySerialScreen_Task(MySerialScreen_Typedef* object);
#endif
