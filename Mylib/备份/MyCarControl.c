#include "MyCarControl.h"
MyCarControl_Typedef Car;
int MyCarControl_Move(MyCarControlDate_Typedef *object, float direction, float distance, float speed)
{
    if (object->temp_distanceflag == 0)
    {
        // 初始化参数
        object->temp_Measuredistance = 0;
        object->setCarSpeed = speed;
        object->setCarSpeedAngle = direction;
        object->temp_distanceflag = 1;
    }
    else if (object->temp_distanceflag == 1)
    {
        MyCarControl_GetDistanceDate(object); // 获取路程
        if (object->temp_Measuredistance >= distance)
        {
            object->temp_distanceflag = 2;
        }
    }
    else if (object->temp_distanceflag == 2)
    {
        object->setCarSpeed = 0; // 停止
        return 1;
                                                                                                      }
    else
    {
        return 0;
    }
	return 0;
}

void MyCarControl_Endmove(MyCarControlDate_Typedef *object)
{
    object->temp_distanceflag = 0;
}

void MyCarControl_rotate(MyCarControlDate_Typedef *object, float Angle)
{
	object->setCarAngle =Angle;
}

void MyCarControl_GetwheelDate(MyCarControlDate_Typedef *object)
{
    // 此处没有写数组越界检测
    object->Wheel[wheel1].Speed = motor[Motor1].para.vel;
    object->Wheel[wheel2].Speed = -motor[Motor2].para.vel;
    object->Wheel[wheel3].Speed = -motor[Motor3].para.vel;
    object->Wheel[wheel4].Speed = motor[Motor4].para.vel;
}

void MyCarControl_GetCarAngle(MyCarControlDate_Typedef *object)
{
    object->CarAngle = MyIMU.MyIMUDate.Full_YawAngle;
}

void MyCarControl_GetDistanceDate(MyCarControlDate_Typedef *object)
{
    object->temp_Measuredistance += (object->Wheel[wheel1].Speed + object->Wheel[wheel2].Speed + object->Wheel[wheel3].Speed + object->Wheel[wheel4].Speed) / 4.0* object->controlperiod*0.85;
}

void MyCarControl_GetCarSpeedDate(MyCarControlDate_Typedef *object)
{
    object->CarSpeed = (object->Wheel[wheel1].Speed + object->Wheel[wheel2].Speed + object->Wheel[wheel3].Speed + object->Wheel[wheel4].Speed) / 4.0 ;
}

void MyCarControl_Init(MyCarControl_Typedef *object,float Controlperiod)
{
	//初始化可修改变量
	object->Date.CarAngle =0 ;
	object->Date.CarSpeed =0;
	object->Date.temp_Measuredistance = 0;
	
	object->Date.controlperiod = Controlperiod;//20ms
	object->Date.controlfrequency =1.0/object->Date.controlperiod;
	
    object->Date.setCarSpeed = 0;
    object->Date.setCarAngle = 90;
	object->Date.setCarSpeedAngle = 0;
	object->Date.temp_distanceflag =0;
	
	//绑定方法
	object->move = MyCarControl_Move;
	object->endmove = MyCarControl_Endmove;
	object->rotate = MyCarControl_rotate;
}

void MyCarControl_Task(MyCarControl_Typedef *object)
{
    MyCarControl_GetwheelDate(&object->Date);
    MyCarControl_GetCarAngle(&object->Date);
    MyCarControl_GetCarSpeedDate(&object->Date);
    Car_Control(object->Date.setCarSpeed, object->Date.setCarSpeedAngle, object->Date.setCarAngle);
}

