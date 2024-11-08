#include "Buzzer.h"
#include "tim.h"

void Buzzer_On()
{
	TIM12->CCR2 = 1000;
}

void Buzzer_Off()
{
	TIM12->CCR2 = 0;
}

