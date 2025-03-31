#include "MyCarControl.h"

//初始化对象
#define CatMove(x,y)  MyCarControl_Move(&Car.Date,x,y)
#define CatEnd() MyCarControl_MoveEnd(&Car.Date)

MyCarControl_Typedef Car ={
.Date.dt =0.02,
.Date.DistanceControl.SetDistanceSpeed =200,
.Date.DistanceControl.SetDistancethreshold = 0.5,
.Date.DistanceControl.KpX =5,
.Date.DistanceControl.KpY =5,
.Date.DistanceControl.KdX =0.01,
.Date.DistanceControl.KdY =0.01,
.Date.DistanceControl.Enable =1,
.Date.EnableMotor =1,
.Date.EnableControl =1,
.Date.AnglePID.Kp = 0.08,
.Date.AnglePID.Kd = 0.02,
.Date.CarL = 150.5,
.Date.RelativeFixedPointMove.CurrentNum =1,
.Date.RelativeFixedPointMove.setTargetNum =1,
.Date.RelativeFixedPointMove.Size[1].x = 0,
.Date.RelativeFixedPointMove.Size[1].y = 0,
.Date.RelativeFixedPointMove.Size[2].x = -90,
.Date.RelativeFixedPointMove.Size[2].y = 0,
.Date.RelativeFixedPointMove.Size[3].x = 90,
.Date.RelativeFixedPointMove.Size[3].y = 0,
.Date.RelativeFixedPointMove.setV = 200,
//.Date.RelativeFixedPointMove.Size[]
};
//170
int MyCarControl_MoveAngle(MyCarControlDate_Typedef *object,float Angle)
{
	object->setCarAngle = Angle;
	if(fabsf(object->CarAngle-Angle)<1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int MyCarControl_MoveEnd(MyCarControlDate_Typedef *object)
{
	if(fabsf(object->DistanceControl.ResidueDistanceX)<1&&fabsf(object->DistanceControl.ResidueDistanceY)<1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
	
void MyCarControl_Move(MyCarControlDate_Typedef *object,float x,float y)
{
//	if(object->WheelStopFlag==1&&object->DistanceControl.Movingflag==0)
//	{
		object->DistanceControl.ResidueDistanceX = x;
		object->DistanceControl.ResidueDistanceY = y;
//		object->DistanceControl.Movingflag =1;
//		return 0;
//	}
//	else if(object->WheelStopFlag==1&&object->DistanceControl.Movingflag==1)
//	{
//		object->DistanceControl.Movingflag=0;
//		return 1;
//	}
//	else
//	{	
//		return 0;
//	}
	
}

void MyCarControl_setCarDistance(MyCarControlDate_Typedef *object)
{
#define errX object->DistanceControl.ResidueDistanceX
#define errY object->DistanceControl.ResidueDistanceY
	
#define Last_errX object->DistanceControl.ResidueDistanceX
#define Last_errY object->DistanceControl.ResidueDistanceY
	
#define KpX object->DistanceControl.KpX
#define KdX object->DistanceControl.KdX	
	
#define KpY object->DistanceControl.KpY
#define KdY object->DistanceControl.KdY	

#define retY object->DistanceControl.retY
#define retX object->DistanceControl.retX	

	if(object->DistanceControl.Enable ==1)
	{
//		if(
//		fabsf(object->DistanceControl.ResidueDistanceX)
//		>object->DistanceControl.SetDistancethreshold
//		||fabsf(object->DistanceControl.ResidueDistanceY)
//		>object->DistanceControl.SetDistancethreshold
//		)
//		{
			object->DistanceControl.ResidueDistanceX -= object->Vx * object->dt;
			object->DistanceControl.ResidueDistanceY -= object->Vy * object->dt;
			
			retX = errX*KpX + KdX*(errX-Last_errX);
			retY = errY*KpY + KdY*(errY-Last_errY);
			
			//限幅
			
			if(retX>object->DistanceControl.SetDistanceSpeed)
			{
				retX =object->DistanceControl.SetDistanceSpeed;
			}
			else if(retX<-object->DistanceControl.SetDistanceSpeed)
			{
				retX =-object->DistanceControl.SetDistanceSpeed;
			}
		
			if(retY>object->DistanceControl.SetDistanceSpeed)
			{
				retY =object->DistanceControl.SetDistanceSpeed;
			}
			else if(retY<-object->DistanceControl.SetDistanceSpeed)
			{
				retY =-object->DistanceControl.SetDistanceSpeed;
			}
			
			object->SetVx = retX;
			object->SetVy = retY;
//		}
//		else
//		{
//			object->SetVx = 0;
//			object->SetVy = 0;
//		}
	}
#undef errX 
#undef errY 	 
#undef Last 
#undef Last 		 
#undef KpX  
#undef KdX  
#undef KpY  
#undef KdY  		
#undef retY 
#undef retX 
}

void MyCarControl_GetCarXY(MyCarControlDate_Typedef *object)
{
	object->x += object->Vx_global * object->dt;
	object->y += object->Vy_global * object->dt;
}

void MyCarControl_GetCarGlobalSpeed(MyCarControlDate_Typedef *object)
{
	object->Vx_global = object->Vx * cosf(IMU.YawAngle * 3.14f/180.0f) 
	- object->Vy * sinf(IMU.YawAngle * 3.14f/180.0f);
	
	object->Vy_global = object->Vx * sinf(IMU.YawAngle * 3.14f/180.0f) 
	+ object->Vy * cosf(IMU.YawAngle * 3.14f/180.0f);
}


void MyCarControl_setCarAngle_PID(MyCarControlDate_Typedef *object)
{
//	if(object->CarMoveStatus==0)//小车不移动状态
//	{
//		if(object->WheelStopFlag==1)//轮子停止
//		{
//			object->SetVw =0;//小车不旋转
//		}
//		if(object->WheelStopFlag==0||(fabsf(object->setCarAngle-object->CarAngle))>10)//轮子移动
//		{
//			//修正角度
//			  MyIMU.MyIMUDate.Full_YawAnglePY =  -Car.Date.CarAngle + object->AnglePID.recordAngle;
//			
//			object->CarMoveStatus =1;//切换状态
//		}
//	}
//	if(object->CarMoveStatus==1)//小车移动状态
//	{
//		if(object->WheelStopFlag==0||(fabsf(object->setCarAngle-object->CarAngle))>10)//轮子运动,启用角度环
//		{
			object->AnglePID.err = object->setCarAngle - object->CarAngle;
			object->AnglePID.ret = 
			object->AnglePID.Kp * object->AnglePID.err
			+object->AnglePID.Kd * (object->AnglePID.err - object->AnglePID.Last_err);
			object->AnglePID.Last_err = object->AnglePID.err;
			object->SetVw = object->AnglePID.ret;
//		}
//		if(object->WheelStopFlag==1) 
//		{
//			//记录角度
//   		    object->AnglePID.recordAngle = object->CarAngle;
//			
//			object->CarMoveStatus =0;//切换状态
//		}
//	}
	

	

	
}

void MyCarControl_Mecanumwheel_setSpeed(MyCarControlDate_Typedef *object)
{
	 object->Wheel[wheel2].setSpeed = object->SetVx / WheelR - object->SetVy / WheelR -object->SetVw/WheelR*WheelL;
	 object->Wheel[wheel3].setSpeed = object->SetVx / WheelR + object->SetVy / WheelR -object->SetVw/WheelR*WheelL;
	 object->Wheel[wheel1].setSpeed = object->SetVx / WheelR + object->SetVy / WheelR +object->SetVw/WheelR*WheelL;
	 object->Wheel[wheel4].setSpeed = object->SetVx / WheelR - object->SetVy / WheelR +object->SetVw/WheelR*WheelL;


}

void MyCarControl_setWheel(MyCarControlDate_Typedef *object)
{
	
	if(object->EnableMotor==1)
	{
	speed_ctrl(&hfdcan1, motor[Motor1].id, -object->Wheel[wheel1].setSpeed);
	speed_ctrl(&hfdcan1, motor[Motor2].id, object->Wheel[wheel2].setSpeed);
	speed_ctrl(&hfdcan2, motor[Motor3].id, object->Wheel[wheel3].setSpeed);
	speed_ctrl(&hfdcan2, motor[Motor4].id, -object->Wheel[wheel4].setSpeed);
	}
	else if(object->EnableMotor==0)
	{
	speed_ctrl(&hfdcan1, motor[Motor1].id, 0);
	speed_ctrl(&hfdcan1, motor[Motor2].id, 0);
	speed_ctrl(&hfdcan2, motor[Motor3].id, 0);
	speed_ctrl(&hfdcan2, motor[Motor4].id, 0);

	}

}


void MyCarControl_GetCarSpeed(MyCarControlDate_Typedef *object)
{
	object->Vx = (
	object->Wheel[wheel1].Speed + 
	object->Wheel[wheel2].Speed +
	object->Wheel[wheel3].Speed +
	object->Wheel[wheel4].Speed)*WheelR/4.0f;
	
	object->Vy = (
	-object->Wheel[wheel2].Speed + 
	object->Wheel[wheel3].Speed +
	object->Wheel[wheel1].Speed -
	object->Wheel[wheel4].Speed)*WheelR/4.0f;
	
	object->Vw = (
	object->Wheel[wheel2].Speed + 
	object->Wheel[wheel3].Speed -
	object->Wheel[wheel1].Speed -
	object->Wheel[wheel4].Speed)/4.0f*WheelR/object->CarL;//*WheelR/object->CarL
	
}


void MyCarControl_GetwheelDate(MyCarControlDate_Typedef *object)
{
    // 此处没有写数组越界检测
	//获取速度
    object->Wheel[wheel1].Speed = -motor[Motor1].para.vel;
    object->Wheel[wheel2].Speed = motor[Motor2].para.vel;
    object->Wheel[wheel3].Speed = motor[Motor3].para.vel;
    object->Wheel[wheel4].Speed = -motor[Motor4].para.vel;
	//获取轮子状态
	if((fabsf(object->Wheel[wheel1].Speed)<0.1
	&& fabsf(object->Wheel[wheel2].Speed)<0.1
	&& fabsf(object->Wheel[wheel3].Speed)<0.1
	&& fabsf(object->Wheel[wheel4].Speed)<0.1)
	&&(fabsf(object->DistanceControl.ResidueDistanceX)<0.1)
	&&(fabsf(object->DistanceControl.ResidueDistanceY)<0.1)
	)
	
	{
		object->WheelStopFlag=1;
	}
	else
	{
		object->WheelStopFlag=0;
	}	
	
}

void MyCarControl_GetCarAngle(MyCarControlDate_Typedef *object)
{
    object->CarAngle = MyIMU.MyIMUDate.Full_YawAngle;
//	if(fabsf(IMU.gz)>0.01)
//	{
//		object->CarAngleInWheel -= object->Vw;
//	}
//	object->CarAngle = object->CarAngleInWheel;
}


void MyCarControl_rotate(MyCarControlDate_Typedef *object, float Angle)
{
	object->setCarAngle =Angle;
}
//切换到num位置
int RelativeFixedPointMove(MyCarControlDate_Typedef *object,char num)
{

	object->RelativeFixedPointMove.setTargetNum = num;
	if(object->RelativeFixedPointMove.setTargetNum != object->RelativeFixedPointMove.CurrentNum)
	{
		object->RelativeFixedPointMove.Enable=1;
	}
	if(object->RelativeFixedPointMove.setTargetNum == object->RelativeFixedPointMove.CurrentNum)
	{
		object->RelativeFixedPointMove.Status=0;
		object->RelativeFixedPointMove.Enable=0;
		return 1;
	}
	return 0;
}
	
void MyCarControl_setRelativeFixedPointMove(MyCarControlDate_Typedef *object)
{
	if(object->RelativeFixedPointMove.Enable==1)
	{
		if(object->RelativeFixedPointMove.Status==0)//初始化
		{
			//记录之前速度
			object->DistanceControl.Enable=1;
			Car.Date.EnableMotor=1;
			object->RelativeFixedPointMove.Last_V= object->DistanceControl.SetDistanceSpeed;
			object->RelativeFixedPointMove.Status=1;
		}
		else if(object->RelativeFixedPointMove.Status==1)
		{
			if(object->RelativeFixedPointMove.setTargetNum != object->RelativeFixedPointMove.CurrentNum)
			{
				//赋值当前速度
				object->DistanceControl.SetDistanceSpeed = object->RelativeFixedPointMove.setV;
				//获取当前值和目标值
				object->RelativeFixedPointMove.TargetX = object->RelativeFixedPointMove.Size[ object->RelativeFixedPointMove.setTargetNum].x;
				object->RelativeFixedPointMove.TargetY = object->RelativeFixedPointMove.Size[ object->RelativeFixedPointMove.setTargetNum].y;
				object->RelativeFixedPointMove.CurrentX = object->RelativeFixedPointMove.Size[ object->RelativeFixedPointMove.CurrentNum].x;
				object->RelativeFixedPointMove.CurrentY = object->RelativeFixedPointMove.Size[ object->RelativeFixedPointMove.CurrentNum].y;
				//移动到
				CatMove(
				object->RelativeFixedPointMove.TargetX - object->RelativeFixedPointMove.CurrentX,
				object->RelativeFixedPointMove.TargetY - object->RelativeFixedPointMove.CurrentY);
				object->RelativeFixedPointMove.Status=2;
			}
		}
		else if(object->RelativeFixedPointMove.Status==2)
		{
			//开始运行
			if(CatEnd())
			{
				//到达指定位置
				object->RelativeFixedPointMove.CurrentX = object->RelativeFixedPointMove.TargetX;
				object->RelativeFixedPointMove.CurrentY = object->RelativeFixedPointMove.TargetY;
				object->RelativeFixedPointMove.CurrentNum = object->RelativeFixedPointMove.setTargetNum;
				object->RelativeFixedPointMove.Status=1;
			}
		}
	
	}
	else
	{
		if(object->RelativeFixedPointMove.Status!=0)
		{				//取回速度
			object->DistanceControl.SetDistanceSpeed= object->RelativeFixedPointMove.Last_V;
		}

	}

}


void MyCarControl_Task(MyCarControl_Typedef *object)
{
	MyCarControl_GetCarAngle(&object->Date);
	MyCarControl_GetwheelDate(&object->Date);
	MyCarControl_GetCarSpeed(&object->Date);
	MyCarControl_GetCarGlobalSpeed(&object->Date);
	MyCarControl_GetCarXY(&object->Date);
//	
	if(object->Date.EnableControl==1)
	{
	 MyCarControl_setCarDistance(&object->Date);
	 MyCarControl_setCarAngle_PID(&object->Date);
	 MyCarControl_setRelativeFixedPointMove(&object->Date);

	}
	
	MyCarControl_Mecanumwheel_setSpeed(&object->Date);
	MyCarControl_setWheel(&object->Date);
}

