#include "Car_App.h"
//@С��

//@С������
CarTypeDef Car;

//@С���ײ�ӿ�

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
	if(Car->Car_Status==1)//��ʼ��
	{
		//��ʼ������
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


//@С����������
//�ӿ�ת������

void Car_Task(void)//���󣬶������������������Ϣ����
{
	Car_Interface_Transfor(&Car,Car_Control,Car_DateTransfor);
}
