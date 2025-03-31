#include "Car_App.h"
//@小车

//@小车调试
CarTypeDef Car;

//@小车底层接口

void Car_Control(float Speed, float Angle, float Turn_Speed)
{
	Car_AllangleSpeed(Speed,Angle,Turn_Speed);
}

void Car_DateTransfor(CarTypeDef* Car)
{
	Car->CarDate.Ab_X=World_X;
	Car->CarDate.Ab_Y=World_Y;
	Car->CarDate.TargetAngle=Car->CarControl.Angle;
	Car->CarDate.CurrentAngle=IMU.Full_YawAngle;
	Car->CarDate.TargetSpeed=Car->CarControl.Speed;
	Car->CarDate.MotorA=motor[Motor1].para.vel;
	Car->CarDate.MotorB=motor[Motor2].para.vel;
	Car->CarDate.MotorC=motor[Motor3].para.vel;
	Car->CarDate.MotorD=motor[Motor4].para.vel;
}

void Car_Interface_Transfor(CarTypeDef* Car,void (*Underfunction)(float,float,float),void (*UnderDatefunction)(CarTypeDef*))
{
	if(Car->Car_Status==1)//初始化
	{
		//初始化参数
		dm4310_clear_err(&hfdcan1, &motor[Motor1]);
		dm4310_clear_err(&hfdcan1, &motor[Motor2]);
		dm4310_clear_err(&hfdcan2, &motor[Motor3]);
		dm4310_clear_err(&hfdcan2, &motor[Motor4]);
		dm4310_enable(&hfdcan1, &motor[Motor1]);
		dm4310_enable(&hfdcan1, &motor[Motor2]);
		dm4310_enable(&hfdcan2, &motor[Motor3]);
		dm4310_enable(&hfdcan2, &motor[Motor4]);
		Car->Car_Status=2;
	}
	if(Car->Car_Status==2)
	{
		UnderDatefunction(Car);
		Underfunction(Car->CarControl.Speed,Car->CarControl.Angle,Car->CarControl.Turn_Speed);
	}
}


//@小车函数调用
//接口转化函数

void Car_Task(void)//对象，对象操作函数，对象信息函数
{
	Car_Interface_Transfor(&Car,Car_Control,Car_DateTransfor);
}
