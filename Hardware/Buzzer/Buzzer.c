#include "Buzzer.h"
#include "tim.h"

int Buzze;

void Buzzer_On()
{
	TIM12->CCR2 = 1000;
}

void Buzzer_Off()
{
	TIM12->CCR2 = 0;
}

void Buzzer_Task()
{
	if(Buzze>0)
	{
		Buzzer_On();
		Buzze--;
	}
	else
	{
		Buzzer_Off();
	}
}