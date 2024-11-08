#include "Interrupt.h"
#include "tim.h"
#include "Task_Management.h"
#include "Hardware_Task.h"
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim6)//5ms
	{
		Hight_Task();
	}
	if(htim == &htim7)//10ms
	{
		Medium_Task();
	}
}


