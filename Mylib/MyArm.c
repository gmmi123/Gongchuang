#include "MyArm.h"

#include "math.h"


MyArm_Typedef Arm = {
	.Date.Mod = RCS,
	.Date.RCS.setX =25,
	.Date.RCS.setY = 80,
	.Date.L1 = measureL1,
	.Date.L2 = measureL2,
	.Date.setArmSpeed = initArmSpeed,
	.Date.RCS.dt = 0.02,
	
};

void MyArm_setSpeedMove(MyArmDate_Typedef* object)
{
	object->RCS.setX += object->RCS.Vx*object->RCS.dt;
	object->RCS.setY += object->RCS.Vy*object->RCS.dt;
}
int MyArm_Move(MyArmDate_Typedef* object,float X,float Y)
{
	object->RCS.setX =X;
	object->RCS.setY =Y;
	if(fabsf(object->RCS.X-X)<1&&fabsf(object->RCS.Y-Y)<1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


//获取极坐标系的R,Angle
void MyArm_GetPCSDate(MyArmDate_Typedef* object)
{
//T1=(b-a)/2+a,T2=b-a,a>=0,b>a+N,180>=b,L=L1*cos(T2/2)+√(L2^2-(L1^2)*sin^2(T2/2))
	object->PCS.Angle = (object->SingleArm[ArmB].Angle
	- object->SingleArm[ArmA].Angle)
	/ 2
	+ object->SingleArm[ArmA].Angle;
	
	object->PCS.R = object->L1 * cosf(((object->SingleArm[ArmB].Angle
	- object->SingleArm[ArmA].Angle)/2) /180.0f*3.14f)
	
	+
	sqrtf(
	object->L2
	*object->L2
	-(
	object->L1
	*object->L1
	)
	*sinf(((object->SingleArm[ArmB].Angle
	- object->SingleArm[ArmA].Angle)/2) /180.0f*3.14f)	
	*sinf(((object->SingleArm[ArmB].Angle
	- object->SingleArm[ArmA].Angle)/2) /180.0f*3.14f));
	
}

void MyArm_setPCS(MyArmDate_Typedef* object)
{
	//逆解角度和路程
	float temp = 0;
	temp =acosf((object->L1*object->L1+object->PCS.setR*object->PCS.setR-object->L2*object->L2)/(2*object->L1*object->PCS.setR));
//	if (temp < -1.0f || temp > 1.0f) {
//    // 处理错误情况，例如设置一个默认值或抛出异常
//    temp = 1.0f*180.0f/3.14f; // 或者其他合理的默认值
//	}
//	else
//	{
//	temp=temp *180.0f/3.14f;
//	}
	temp=temp *180.0f/3.14f;
	object->SingleArm[ArmA].setAngle = object->PCS.setAngle - temp;
	object->SingleArm[ArmB].setAngle = object->PCS.setAngle + temp;
	

	//加入限幅
	if(object->SingleArm[ArmA].setAngle<0)
	{
		object->SingleArm[ArmA].setAngle = 0;
	}
	if(object->SingleArm[ArmB].setAngle>180)
	{
		object->SingleArm[ArmB].setAngle = 180;
	}
	
	if(object->SingleArm[ArmA].setAngle>object->SingleArm[ArmB].setAngle)
	{
		object->SingleArm[ArmA].setAngle = object->SingleArm[ArmB].setAngle-3;
	}
	
	
}



//获取直角坐标系的XY
void MyArm_GetRCSDate(MyArmDate_Typedef* object)
{
	object->RCS.X = object->PCS.R * cosf((object->PCS.setAngle) /180.0f*3.14f);
	object->RCS.Y = object->PCS.R * sinf((object->PCS.setAngle)/180.0f*3.14f);
}


void MyArm_setRCS(MyArmDate_Typedef* object)
{
	float theta;
	
	object->PCS.setR = sqrtf(object->RCS.setX *object->RCS.setX +object->RCS.setY *object->RCS.setY);
	
	if (object->RCS.setX  == 0) {
	if (object->RCS.setY > 0) {
		theta = 3.14 / 2; // 90度
	} else if (object->RCS.setY < 0) {
		theta = -3.14 / 2; // -90度
	} else {
		theta = 0; // 原点
	}
    } else {
        theta = atan2(object->RCS.setY, object->RCS.setX ); // 使用 atan2 函数计算角度
    }
	
	object->PCS.setAngle = theta *180/3.14;

}


//获取爪子的数据
void MyArm_GetArmDate(MyArmDate_Typedef* object)
{
	object->SingleArm[ArmA].Angle = motor[Motor5].para.pos*180.0f/3.14f;
	object->SingleArm[ArmB].Angle = 180.0f-motor[Motor6].para.pos*180.0f/3.14f;
	object->SingleArm[ArmA].Speed = motor[Motor5].para.vel;
	object->SingleArm[ArmB].Speed = motor[Motor6].para.vel;
}

//控制爪子的速度和角度
void MyArm_ControlArm(MyArmDate_Typedef* object)
{

	if(object->Mod == RCS)
	{
		MyArm_setRCS(object);
	}
	MyArm_setPCS(object);
	//控制速度
	object->SingleArm[ArmA].setSpeed = object->setArmSpeed;
	object->SingleArm[ArmB].setSpeed = object->setArmSpeed;
	//控制电机
	
	if(isnan(object->SingleArm[ArmA].setAngle)||isnan(object->SingleArm[ArmB].setAngle))
	{
			//故障
	}
	else
	{
		pos_speed_ctrl(&hfdcan1,motor[Motor5].id,object->SingleArm[ArmA].setAngle*3.14f/180.0f,object->SingleArm[ArmA].setSpeed);
		pos_speed_ctrl(&hfdcan2,motor[Motor6].id,(180.0f-object->SingleArm[ArmB].setAngle)*3.14f/180.0f,object->SingleArm[ArmB].setSpeed);

	}
}
void MyArm_setArm(MyArmDate_Typedef* object,float Angle,float Distance)
{
	float temp = 0;
	temp =acosf((object->L1*object->L1+Distance*Distance-object->L2*object->L2)/2*object->L1*Distance)*90.0f/3.14f;
	object->SingleArm[ArmA].Angle = Angle - temp;
	object->SingleArm[ArmB].Angle = Angle + temp;
}


void MyArm_Init(MyArm_Typedef* object)
{
	//初始化成员变量
	object->Date.L1 = measureL1;
	object->Date.L2 = measureL2;
	object->Date.setArmSpeed = initArmSpeed;
	object->Date.SingleArm[ArmA].setSpeed =initArmSpeed;
	object->Date.SingleArm[ArmB].setSpeed =initArmSpeed;
	object->Date.PCS.setR = initDistance;
	object->Date.PCS.setAngle =initAngle;
	//绑定方法
	object->setArm = MyArm_setArm;
	
}


void MyArm_Task(MyArm_Typedef* object)
{

	
	//获取目前单个爪子的角度
	MyArm_GetArmDate(&object->Date);
	//转化为坐标系
	MyArm_GetPCSDate(&object->Date);
	MyArm_GetRCSDate(&object->Date);
	//控制
	MyArm_setSpeedMove(&object->Date);
	MyArm_ControlArm(&object->Date);
}