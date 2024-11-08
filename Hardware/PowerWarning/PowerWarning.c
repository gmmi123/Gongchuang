#include "PowerWarning.h"
#include "Buzzer.h"
//µÍÑ¹Ô¤¾¯
extern uint16_t adc_val[2];
extern float vbus;
float PowerWarning(void)
{
	vbus = (adc_val[0]*3.3f/65535)*11.0f;
	
	if(vbus>20&&vbus<22)
	{
		Buzzer_On();
		HAL_Delay(1000);
		Buzzer_Off();
		HAL_Delay(1000);
	}
	
	return vbus;
}
