#include "MySerialScreen.h"
#include "MyGrabBlock.h"
MySerialScreen_Typedef SerialScreen;
void MySerialScreen_GetTaskMassage(MySerialScreenDate_Typedef* object)
{
	
}

void MySerialScreen_GetEnableFlag(MySerialScreenDate_Typedef* object)
{
	if(CKP_Parses(UART7_RX_Second_BUF))
	{
		object->EnableCar_flag=1;
	}
}

void MySerialScreen_SendTaskMassage(MySerialScreenDate_Typedef* object)
{
	object->TaskMassage = RaspberryPi.Date.TaskMassage.TempDate;
	if(object->SendTaskMassage_flag==0&&RaspberryPi.Date.TaskMassage.TempDate[3]=='+')
	{
		  uint8_t temp[] = {'p', 'a', 'g', 'e', '0', '.',
			  't', '1', '.', 't', 'x', 't', '=', '\"',
		  object->TaskMassage[0],object->TaskMassage[1],object->TaskMassage[2],
		  '+', object->TaskMassage[4], object->TaskMassage[5],object->TaskMassage[6],
		  '\"', 0xff, 0xff, 0xff};
		  
		  HAL_UART_Transmit(&huart7, temp, 25, 0xffff);
		  int j;
		  for(int i=0;i<8;i++)
		  {
			if(object->TaskMassage[i]=='1')
			{
				RaspberryPi.Date.TaskMassage.BlockColor[j]= 1;
				j++;
			}
			else if(object->TaskMassage[i]=='2')
			{
				RaspberryPi.Date.TaskMassage.BlockColor[j]= 2;
				j++;
			}
			else if(object->TaskMassage[i]=='3')
			{
				RaspberryPi.Date.TaskMassage.BlockColor[j]= 3;
				j++;
			}
		  }
		  
		  MyGrabBlock_setItem(TaskBlockItem);
		  object->SendTaskMassage_flag=1;

	}
}

void MySerialScreen_Init(MySerialScreen_Typedef* object)
{

}


void MySerialScreen_Task(MySerialScreen_Typedef* object)
{
	MySerialScreen_GetEnableFlag(&object->Date);
	MySerialScreen_SendTaskMassage(&object->Date);
}