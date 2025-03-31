#include "MyDebug.h"
#include "MyLib.h"
MyDebug_Typedef Debug={
.Date.setAngle = &Claw.Date.setAngle,
.Date.setPosition = &Lift.Date.setPosition,
.Date.setArmX = &Arm.Date.RCS.setX,
.Date.setArmY = &Arm.Date.RCS.setY,
};

void MyDebug_ControlDebug(MyDebugDate_Typedef* object)
{
//	if(object->Status==Debug_ArmX0Y200)
//	{
//		Arm.Date.RCS.setX =0;
//		Arm.Date.RCS.setY =200;
//		object->Status = Debug_Waitting;
//	}
	
	switch(object->Status)
	{
		case Debug_ArmX0Y200:
			Arm.Date.RCS.setX =0;
			Arm.Date.RCS.setY =200;
			object->Status = Debug_Waitting;
			break;
		case Debug_ClawClose:
			Claw.Date.setAngle = 157;
			object->Status = Debug_Waitting;
			break;
		case Debug_ClawOpen:
			Claw.Date.setAngle = 71;
			object->Status = Debug_Waitting;
			break;
		case Debug_RsapberryPiMod_0:
			RaspberryPi.Date.Mod = '0';
			object->Status = Debug_Waitting;
			break;
		case Debug_RsapberryPiMod_1:
			RaspberryPi.Date.Mod = '1';
			object->Status = Debug_Waitting;
			break;
		case Debug_RsapberryPiMod_2:
			RaspberryPi.Date.Mod = '2';
			object->Status = Debug_Waitting;
			break;
	}
}




void MyDebug_Task(MyDebug_Typedef* object)
{

	//¿ØÖÆ
	MyDebug_ControlDebug(&object->Date);
}