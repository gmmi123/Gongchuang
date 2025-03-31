#include "MyGrabBlock.h"
#include "MyRaspberryPi.h"
#include "MyMainWhile.h"
#define PlateH   28 //28
#define BlockDownBase 0  //0
MyGrabBlock_Typedef GrabBlock;
TaskColorBlockXY_Typedef TaskColorBlockXY[3] = { 
{.X =150,.Y =200},
{.X =-150,.Y =200},
{.X =0,.Y =200},
};
GrabBlockItem_Typedef TaskBlockItemYL[3]={
{//0
	//抓物料盘上
//	.X1 = 0,
//	.Y1 = 275,

	.T1 = 2,
	.T2 = 2,
//	
//	.H1 = 60,
	.H2 = 28,
	.H3 = 65,
	.H4 = 40,
	
	.X2 = 0,
	.Y2 = 83,

},{//1
//	.X1 = 0,
//	.Y1 = 279,

	.T1 = 60,
	.T2 = 120,
	
	.H1 = 60,
	.H2 = 28,
	.H3 = 65,
	.H4 = 40,
	
	.X2 = 0,
	.Y2 = 83,

},
{//2
//	.X1 = 0,
//	.Y1 = 279,

	.T1 = 60,
	.T2 = 240,
	
	.H1 = 60,
	.H2 = 28,
	.H3 = 65,
	.H4 = 40,
	
	.X2 = 0,
	.Y2 = 83,

}
};
GrabBlockItem_Typedef TaskBlockItem[9]={
{//0
	//抓物料盘上
	.X1 = 0,
	.Y1 = 200,

	.T1 = 2,
	.T2 = 2,
	
	.H1 = 40,
	.H2 = PlateH,
	.H3 = 65,
	.H4 = 40,
	
	.X2 = 0,
	.Y2 = 83,

},{//1
	.X1 = 0,
	.Y1 = 200,

	.T1 = 60,
	.T2 = 120,
	
	.H1 = 40,
	.H2 = PlateH,
	.H3 = 65,
	.H4 = 40,
	
	.X2 = 0,
	.Y2 = 83,

},
{//2
	.X1 = 0,
	.Y1 = 200,

	.T1 = 60,
	.T2 = 240,
	
	.H1 = 40,
	.H2 = PlateH,
	.H3 = 65,
	.H4 = 40,
	
	.X2 = 0,
	.Y2 = 83,

},//放地上3
{   .X1 = 0,
	.Y1 = 83,
	
	.T1 = 0,
	.T2 = 180,
	
	.H1 = 40,
	.H2 = 40,
	.H3 = 65,
	
	.X2 = 0,
	.Y2 = 200,

	.H4 = -50+BlockDownBase,
},
{//4
    .X1 = 0,
	.Y1 = 83,
	
	.T1 = 120,
	.T2 = 240,
	
	.H1 = 40,
	.H2 = 40,
	.H3 = 65,
	
	.X2 = 150,
	.Y2 = 200,

	.H4 = -50+BlockDownBase,

},
{//5
    .X1 = 0,
	.Y1 = 83,
	
	.T1 = 240,
	.T2 = 240,
	
	.H1 = 40,
	.H2 = 40,
	.H3 = 65,
	
	.X2 = -150,
	.Y2 = 200,

	.H4 = -50+BlockDownBase,
},
{//6
	.X1 = 0,
	.Y1 = 200,

	.T1 = 2,
	.T2 = 2,
	
	.H1 = 40,
	.H2 = -55,
	.H3 = 65,
	.H4 = 40+BlockDownBase,
	
	.X2 = 0,
	.Y2 = 83,

},{//7
	.X1 = 0,
	.Y1 = 200,

	.T1 = 60,
	.T2 = 120,
	
//	.H1 = 10,
//	.H2 = -35,
//	.H3 = 76.7,
//	.H4 = 56.7,
	.H1 = 40,
	.H2 = -55,
	.H3 = 65,
	.H4 = 40+BlockDownBase,
	
	.X2 = 0,
	.Y2 = 83,

},
{//8
	.X1 = 0,
	.Y1 = 200,

	.T1 = 60,
	.T2 = 240,
	
	.H1 = 40,
	.H2 = -55,
	.H3 = 65,
	.H4 = 40+BlockDownBase,
	
	.X2 = 0,
	.Y2 = 83,

},//放地上
};

GrabBlockItem_Typedef Test2={
	.X1 = 0,
	.Y1 = 205,
	
	.T1 = 60,
	.T2 = 120,
	
	.H1 = 10,
	.H2 = -35,
	.X2 = 0,
	.Y2 = 80,
	.H3 = 76.7,
	.H4 = 56.7,


};


GrabBlockItem_Typedef Test3={
	
	.X1 = -150,
	.Y1 = 200,
	
	.T1 = 62,
	.T2 = 242,
	
	.H1 = 10,
	.H2 = -35,
	.X2 = 0,
	.Y2 = 80,
	.H3 = 76.7,
	.H4 = 56.7,


};


GrabBlockItem_Typedef Test4={
	


};


GrabBlockItem_Typedef Test5={
	
	.X1 = 0,
	.Y1 = 80,
	
	.T1 = 120,
	.T2 = 240,
	
	.H1 = 56.7,
	.H2 = 56.7,
	.H3 = 76.7,
	
	.X2 = 0,
	.Y2 = 200,

	.H4 = -35,


};


GrabBlockItem_Typedef Test6={
	
	.X1 = 0,
	.Y1 = 80,
	
	.T1 = 240,
	.T2 = 240,
	
	.H1 = 56.7,
	.H2 = 56.7,
	.H3 = 76.7,
	
	.X2 = -150,
	.Y2 = 200,

	.H4 = -35,


};



int MyGrabBlock_ControlGrabBlock(GrabBlockItem_Typedef* object)
{
#define	 Status object->GrabBlockStatus
#define	 H1 object-> H1
#define	 H2 object-> H2
#define	 H3 object-> H3
#define	 H4 object-> H4
#define	 X1 object-> X1
#define	 Y1 object-> Y1
#define	 X2 object-> X2
#define	 Y2 object-> Y2
#define	 T1 object-> T1
#define	 T2 object-> T2
	if(Status==0)
	{
		
		if(Z(H1))
		{
			if(ClawOpen())
			{
				Status=1;
			}
		}
	}
	else if(Status==1)
	{
		if(Claw.Date.Angle<100)
		{
			Arm(X1,Y1);
			Status=2;
		}

		

	}
	else if(Status==2)
	{
		if(Arm.Date.RCS.Y>100)
		{
			Claw(130);
		}
		TurnPlate(T1);
		if(Z(H2))
		{
			
			Status=3;
		}

	}
	else if(Status==3)
	{
		if(ClawClose())
		{
			Status=4;
		}
	}
	else if(Status==4)
	{
		
		if(Z(H3))
		{
			if(TurnPlate(T2))
			{
				if(Arm(X2,Y2))
				{
					Status = 5;
				}
			}
		}
		
		
	}
	else if(Status==5)
	{
		if(Z(H4))
		{
			if(Claw(110))
			{	
				Status =6;
			}
		}
	}

	
	
	if(Status==6)
	{
		return 1;
	}
	else
	{
		return 0 ;
	}
}
int MyGrabBlock_ControlGrabBlockYL(GrabBlockItem_Typedef* object)
{
#define	 Status object->GrabBlockStatus
#define	 H1 object-> H1
#define	 H2 object-> H2
#define	 H3 object-> H3
#define	 H4 object-> H4
#define	 X1 object-> X1
#define	 Y1 object-> Y1
#define	 X2 object-> X2
#define	 Y2 object-> Y2
#define	 T1 object-> T1
#define	 T2 object-> T2
	if(Status==0)
	{
		H1 = Lift.Date.Position;
		X1 = Arm.Date.RCS.X;
		Y1 = Arm.Date.RCS.Y;
//		if(Z(H1))
//		{
//			if(ClawClose())
//			{
				Status=1;
//			}
//		}
	}
	else if(Status==1)
	{
//		if(Claw.Date.Angle<100)
//		{
//			Arm(X1,Y1);
			Status=2;
//		}

		

	}
	else if(Status==2)
	{
		if(Arm.Date.RCS.Y>100)
		{
			Claw(130);
		}
		TurnPlate(T1);
		if(Z(H2))
		{
			
			Status=3;
		}

	}
	else if(Status==3)
	{
		if(ClawClose())
		{
			Status=4;
		}
	}
	else if(Status==4)
	{
		
		if(Z(H3))
		{
			if(TurnPlate(T2))
			{
				if(Arm(X2,Y2))
				{
					Status = 5;
				}
			}
		}
		
		
	}
	else if(Status==5)
	{
		if(Z(H4))
		{
			if(Claw(110))
			{	
				Status =6;
			}
		}
	}
	else if(Status==6)
	{
		if(Z(H1))
		{
			if(Arm(X1,Y1))
			{
				Status=7;
			}
		}
	}

	
	
	if(Status==7)
	{
		return 1;
	}
	else
	{
		return 0 ;
	}
}
int MyGrabBlock_ControlPlaceBlock(GrabBlockItem_Typedef* object)
{
	if(Status == 0)
	{

		Z(H1);
		
		if(Lift.Date.Position>0)
		{
			ClawOpen();
			TurnPlate(T1);
			Status =1;
		}
	}
	
	else if(Status==1)
	{
		

		if(Arm(X1,Y1))
		{
			if(Z(H2))
			{
				if( ClawClose())
				{
					Status =2;
				}
			}
		}
	}
	else if(Status==2)
	{
		
		if(Z(H3))
		{
			if(Arm(X2,Y2))
			{
				TurnPlate(T2);
//				if()
//				{
					Status =3;
//				}
			}
		}
	}
	if(Status == 3)
	{
		if(Z(H4))
		{
			if(Claw(120))
			{
				Status = 4;
			}
		}
	}
	
	if(Status==4)
	{
		return 1;
	}
	else
	{
		return 0 ;
	}

	
#undef	 Status 
#undef	 H1 
#undef	 H2 
#undef	 H3 
#undef	 H4
#undef	 X1 
#undef	 Y1
#undef	 X2
#undef	 Y2
#undef	 T1 
#undef	 T2
}

void MyGrabBlock_Init(MyGrabBlock_Typedef* object)
{

}

float TempStatus = 0;

//H40  Y200
void MyGrabBlock_Task(void)
{
	//控制
	if(TempStatus==0)
	{
//		if(MyGrabBlock_ControlGrabBlock(&Test1))
//		{
//			TempStatus=1;
//		}

	}
	if(TempStatus==1)
	{
		if(MyGrabBlock_ControlGrabBlock(&Test2))
		{
			TempStatus=2;
		}
	
	}
	if(TempStatus==2)
	{
		if(MyGrabBlock_ControlGrabBlock(&Test3))
		{
			TempStatus=3;
		}
	
	}
	if(TempStatus==3)
	{
		if(MyGrabBlock_ControlPlaceBlock(&Test4))
		{
			TempStatus=4;
		}
	}
	if(TempStatus==4)
	{
		if(MyGrabBlock_ControlPlaceBlock(&Test5))
		{
			TempStatus=5;
		}
	}
	if(TempStatus==5)
	{
		if(MyGrabBlock_ControlPlaceBlock(&Test6))
		{
			TempStatus=6;
		}
	}
	
}

//更改Item信息的颜色XY
void MyGrabBlock_setItem(GrabBlockItem_Typedef* object)
{
//	for(int i =0;i<3;i++)
//	{
//		if(RaspberryPi.Date.TaskMassage.BlockColor[RaspberryPi.Date.TaskMassage.num-3]==1)//第一个方块如果为红色
//		{
//			object[RaspberryPi.Date.TaskMassage.num].X2 = Arm.Date.RCS.setX;
//			object[RaspberryPi.Date.TaskMassage.num].Y2 = Arm.Date.RCS.setY;
//		}
//		if(RaspberryPi.Date.TaskMassage.BlockColor[RaspberryPi.Date.TaskMassage.num-3]==2)//第一个方块如果为蓝色
//		{
//			object[RaspberryPi.Date.TaskMassage.num].X2 = Arm.Date.RCS.setX;
//			object[RaspberryPi.Date.TaskMassage.num].Y2 = Arm.Date.RCS.setY;

//		}
//		if(RaspberryPi.Date.TaskMassage.BlockColor[RaspberryPi.Date.TaskMassage.num-3]==3)//第一个方块如果为绿色
//		{
//			object[RaspberryPi.Date.TaskMassage.num].X2 = Arm.Date.RCS.setX;
//			object[RaspberryPi.Date.TaskMassage.num].Y2 = Arm.Date.RCS.setX;
//		}
//	}

}

void MyGrabBlock_getArmXY(GrabBlockItem_Typedef* object)
{	//获取当前机械臂的坐标记录到Item中
	//找颜色填坐标
	//任务 = 当前
	if(Main.roundStatus==0)
	{
		if(RaspberryPi.Date.TaskMassage.BlockColor[0]==RaspberryPi.Date.Block.color)
		{
			object[3].X2 = Arm.Date.RCS.setX;
			object[3].Y2 = Arm.Date.RCS.setY;
			object[6].X1 = Arm.Date.RCS.setX;
			object[6].Y1 = Arm.Date.RCS.setY;
		}
		if(RaspberryPi.Date.TaskMassage.BlockColor[1]==RaspberryPi.Date.Block.color)
		{
			object[4].X2 = Arm.Date.RCS.setX;
			object[4].Y2 = Arm.Date.RCS.setY;
			object[7].X1 = Arm.Date.RCS.setX;
			object[7].Y1 = Arm.Date.RCS.setY;
		}
		if(RaspberryPi.Date.TaskMassage.BlockColor[2]==RaspberryPi.Date.Block.color)
		{
			object[5].X2 = Arm.Date.RCS.setX;
			object[5].Y2 = Arm.Date.RCS.setY;
			object[8].X1 = Arm.Date.RCS.setX;
			object[8].Y1 = Arm.Date.RCS.setY;
		}
	}
	else
	{
		if(RaspberryPi.Date.TaskMassage.BlockColor[3]==RaspberryPi.Date.Block.color)
		{
			object[3].X2 = Arm.Date.RCS.setX;
			object[3].Y2 = Arm.Date.RCS.setY;
			object[6].X1 = Arm.Date.RCS.setX;
			object[6].Y1 = Arm.Date.RCS.setY;
		}
		if(RaspberryPi.Date.TaskMassage.BlockColor[4]==RaspberryPi.Date.Block.color)
		{
			object[4].X2 = Arm.Date.RCS.setX;
			object[4].Y2 = Arm.Date.RCS.setY;
			object[7].X1 = Arm.Date.RCS.setX;
			object[7].Y1 = Arm.Date.RCS.setY;
		}
		if(RaspberryPi.Date.TaskMassage.BlockColor[5]==RaspberryPi.Date.Block.color)
		{
			object[5].X2 = Arm.Date.RCS.setX;
			object[5].Y2 = Arm.Date.RCS.setY;
			object[8].X1 = Arm.Date.RCS.setX;
			object[8].Y1 = Arm.Date.RCS.setY;
		}
	}


}

//	for(int i =0;i<3;i++)
//	{
//		if(RaspberryPi.Date.TaskMassage.BlockColor[RaspberryPi.Date.TaskMassage.num-3]==1)//第一个方块如果为红色
//		{
//			object[RaspberryPi.Date.TaskMassage.num].X2 = TaskColorBlockXY[0].X;
//			object[RaspberryPi.Date.TaskMassage.num].Y2 = TaskColorBlockXY[0].Y;
//		}
//		if(RaspberryPi.Date.TaskMassage.BlockColor[RaspberryPi.Date.TaskMassage.num-3]==2)//第一个方块如果为蓝色
//		{
//			object[RaspberryPi.Date.TaskMassage.num].X2 = TaskColorBlockXY[1].X;
//			object[RaspberryPi.Date.TaskMassage.num].Y2 = TaskColorBlockXY[1].Y;

//		}
//		if(RaspberryPi.Date.TaskMassage.BlockColor[RaspberryPi.Date.TaskMassage.num-3]==3)//第一个方块如果为绿色
//		{
//			object[RaspberryPi.Date.TaskMassage.num].X2 = TaskColorBlockXY[2].X;
//			object[RaspberryPi.Date.TaskMassage.num].Y2 = TaskColorBlockXY[2].Y;
//		}
//	}

