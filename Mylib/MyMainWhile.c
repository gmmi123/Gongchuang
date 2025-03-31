#include "MyMainWhile.h"
#include "math.h"
#include "UsartDate.h"
#include "MySerialScreen.h"
#include "MyCarControl.h"
#include "MyLift.h"
#include "MyClaw.h"
#include "MyArm.h"
#include "MyRaspberryPi.h"
#include "MyGrabBlock.h"
#include "MyIMU.h"
#include "Buzzer.h"
//方块补偿距离
float BlockCompensateX;
float BlockCompensateY;
//补偿基准值
float BlockBaseX=0;
float BlockBaseY=200;
//第二次校准
float YawPY2=0;//负是逆时针
MyLine_Typedef oLine_1={.Start = Line_1};
MyLine_Typedef oLine_2={.Start = Line_2};
MyLine_Typedef oLine_3={.Start = Line_3};
MyLine_Typedef oLine_4={.Start = Line_4};
MyLine_Typedef oLine_5={.Start = Line_5};
MyLine_Typedef oLine_6={.Start = Line_6};

#define  BlueX	-155
#define  BlueY 197  
#define  RedX   152
#define  RedY   197
#define  GreenX  -2
#define  GreenY  197

#define RED 1
#define GERRN 2
#define BLUE 3
MyMainWhileDate_Typedef Main= {
.Status = 0,
.mod=1,
};
int Trace_FangHangKuai_1(void);
int Trace_FangHangKuai_2(void);
int ZhuaYuanLiao(unsigned char *color_order,char Current,char *temp,char *Armtemp);
void MyMainwhile(void)
{
#define Status Main.Status
	if(Status==0)
	{
		if(SerialScreen.Date.EnableCar_flag==1)//串口屏点击使能
		{
			Status=1;
		}
	}
	else if(Status==1)//移动到二维码
	{
		if(oLine_1.Start(&oLine_1.Date))
		{
			Status = 2;
		}
	}
	else if(Status==2)//识别二维码
	{
		if(SerialScreen.Date.SendTaskMassage_flag==1)
		{
			Status=3;
		}
		
	}
	else if(Status==3)//移动到转盘
	{

		if(oLine_2.Start(&oLine_2.Date))
		{
			Status = 4;
		}
	}
	else if(Status==4)//执行抓取任务
	{
		RaspberryPi.Date.ArmPID.TempH = 10;
		RaspberryPi.Date.ArmPID.Kp=0.2;
		RaspberryPi.Date.ArmPID.TempXYH=40;
		if(ZhuaYuanLiaoTask_1())
		{
			Status = 5;
		}
	}
	else if(Status==5)//移动到加工区
	{
		if(oLine_3.Start(&oLine_3.Date))
		{
			Status = 6;
		}
	}
	else if(Status==6)//进行加工
	{
		RaspberryPi.Date.ArmPID.Kp=0.2;
		RaspberryPi.Date.ArmPID.TempH = 5;
		RaspberryPi.Date.ArmPID.TempXYH=2;
		if(Main.mod==0)
		{
			if(Trace_FangHangKuai_1())
			{
				Status = 7;
			}
		}
		else
		{
			if(FangHangKuai_1())
			{
				Status = 7;
			}
		}

		
	}
	else if(Status==7)//移动到放置区
	{
		if(oLine_4.Start(&oLine_4.Date))
		{
			Status = 8;
		}
	}
	else if(Status==8)//放置
	{
		if(Main.mod==0)
		{
			if(Trace_FangHangKuai_2())
			{
				Status=9;	
			}
		}
		else
		{
			if(FangHangKuai_2())
			{
				Status = 9;
			}
		}

	}
	else if(Status==9)
	{
		if(oLine_5.Start(&oLine_5.Date))
		{
			oLine_1.Date.LineStatus=0;
			oLine_2.Date.LineStatus=0;
			oLine_3.Date.LineStatus=0;
			oLine_4.Date.LineStatus=0;
			oLine_5.Date.LineStatus=0;
			Status = 10;
		}
	}
	else if(Status==10)//执行抓取任务
	{
		RaspberryPi.Date.ArmPID.TempH = 10;
		RaspberryPi.Date.ArmPID.Kp=0.2;
		RaspberryPi.Date.ArmPID.TempXYH=40;
		if(ZhuaYuanLiaoTask_2())
		{
			Status = 11;
		}
		
	}
	else if(Status==11)//移动到加工区
	{
		if(oLine_3.Start(&oLine_3.Date))
		{
			Status = 12;
		}
	}
	else if(Status==12)//进行加工
	{
		RaspberryPi.Date.ArmPID.TempH = 5;
		RaspberryPi.Date.ArmPID.Kp=0.2;
		RaspberryPi.Date.ArmPID.TempXYH=2;
		if(Main.mod==0)
		{
			if(Trace_FangHangKuai_1())
			{
				Status = 13;
			}
		}
		else
		{
			if(FangHangKuai_3())
			{
				Status = 13;
			}
		}


	}
	else if(Status==13)//移动到放置区
	{
		if(oLine_4.Start(&oLine_4.Date))
		{
			Status = 14;
		}
	}
	else if(Status==14)//放置
	{
		if(FangHangKuai_4())
		{
			Status=15;
			ClawOpen();
		}
		
	}
	else if(Status==15)
	{
		if(oLine_6.Start(&oLine_6.Date))
		{
			Status = 16;
		}
	}
}
#undef Status
//

int FangHangKuai_1(void)//加工区
{
	static char Status =0;
	if(Status==0)
	{

		for(int i=0;i<3;i++)
		{
			if(RaspberryPi.Date.TaskMassage.BlockColor[0+i]==1)//第一个任务方框的位置
			{
				TaskBlockItem[3+i].X2 = RedX;
				TaskBlockItem[3+i].Y2 = RedY;
				
				TaskBlockItem[6+i].X1 = RedX;
				TaskBlockItem[6+i].Y1 = RedY;
			}
			if(RaspberryPi.Date.TaskMassage.BlockColor[0+i]==2)//第一个任务方框的位置
			{
				TaskBlockItem[3+i].X2 = GreenX;
				TaskBlockItem[3+i].Y2 = GreenY;
				
				TaskBlockItem[6+i].X1 = GreenX;
				TaskBlockItem[6+i].Y1 = GreenY;
			}
			if(RaspberryPi.Date.TaskMassage.BlockColor[0+i]==3)//第一个任务方框的位置
			{
				TaskBlockItem[3+i].X2 = BlueX;
				TaskBlockItem[3+i].Y2 = BlueY;
				
				TaskBlockItem[6+i].X1 = BlueX;
				TaskBlockItem[6+i].Y1 = BlueY;
			}
		}
		Status=1;
	}
	else if(Status==1)
	{
		static char TempStatus=0;
		if(TempStatus ==0)
		{
			if(MyGrabBlock_ControlPlaceBlock(&TaskBlockItem[3]))
			{
				TempStatus = 1;
			}
		}
		else if(TempStatus ==1)
		{
			if(MyGrabBlock_ControlPlaceBlock(&TaskBlockItem[4]))
			{
				TempStatus = 2;
			}
		}
		else if(TempStatus ==2)
		{
			if(MyGrabBlock_ControlPlaceBlock(&TaskBlockItem[5]))
			{
				TempStatus = 3;
			}
		}

		else if(TempStatus ==3)
		{
			if(MyGrabBlock_ControlGrabBlock(&TaskBlockItem[6]))
			{
				TempStatus = 4;
			}
		}
		else if(TempStatus ==4)
		{
			if(MyGrabBlock_ControlGrabBlock(&TaskBlockItem[7]))
			{
				TempStatus = 5;
			}
		}
		else if(TempStatus ==5)
		{
			if(MyGrabBlock_ControlGrabBlock(&TaskBlockItem[8]))
			{
				for(int i =0;i<6;i++)
				{
					TaskBlockItem[i+3].GrabBlockStatus=0;
				}
				TempStatus = 0;
				Status=0;
				return 1;
			}
		}
	}
	return 0;
}
int FangHangKuai_2(void)//放置区
{
	static char Status =0;
	if(Status==0)
	{
		for(int i=0;i<3;i++)
		{
			if(RaspberryPi.Date.TaskMassage.BlockColor[0+i]==1)//第一个任务方框的位置
			{
				TaskBlockItem[3+i].X2 = RedX;
				TaskBlockItem[3+i].Y2 = RedY;
				
				TaskBlockItem[6+i].X1 = RedX;
				TaskBlockItem[6+i].Y1 = RedY;
			}
			if(RaspberryPi.Date.TaskMassage.BlockColor[0+i]==2)//第一个任务方框的位置
			{
				TaskBlockItem[3+i].X2 = GreenX;
				TaskBlockItem[3+i].Y2 = GreenY;
				
				TaskBlockItem[6+i].X1 = GreenX;
				TaskBlockItem[6+i].Y1 = GreenY;
			}
			if(RaspberryPi.Date.TaskMassage.BlockColor[0+i]==3)//第一个任务方框的位置
			{
				TaskBlockItem[3+i].X2 = BlueX;
				TaskBlockItem[3+i].Y2 = BlueY;
				
				TaskBlockItem[6+i].X1 = BlueX;
				TaskBlockItem[6+i].Y1 = BlueY;
			}
		}
		Status=1;
	}
	else if(Status==1)
	{
		static char TempStatus=0;
		if(TempStatus ==0)
		{
			if(MyGrabBlock_ControlPlaceBlock(&TaskBlockItem[3]))
			{
				TempStatus = 1;
			}
		}
		else if(TempStatus ==1)
		{
			if(MyGrabBlock_ControlPlaceBlock(&TaskBlockItem[4]))
			{
				TempStatus = 2;
			}
		}
		else if(TempStatus ==2)
		{
			if(MyGrabBlock_ControlPlaceBlock(&TaskBlockItem[5]))
			{
				TempStatus = 3;
			}
		}
		else if(TempStatus ==3)
		{
			return 1;
		}
	}
	return 0;
}
int FangHangKuai_3(void)//加工区
{
	static char Status =0;
	if(Status==0)
	{
		TaskBlockItem[3].GrabBlockStatus=0;
		TaskBlockItem[4].GrabBlockStatus=0;
		TaskBlockItem[5].GrabBlockStatus=0;
		TaskBlockItem[6].GrabBlockStatus=0;
		TaskBlockItem[7].GrabBlockStatus=0;
		TaskBlockItem[8].GrabBlockStatus=0;
		for(int i=0;i<3;i++)
		{
			if(RaspberryPi.Date.TaskMassage.BlockColor[0+i]==1)//第一个任务方框的位置
			{
				TaskBlockItem[3+i].X2 = RedX;
				TaskBlockItem[3+i].Y2 = RedY;
				
				TaskBlockItem[6+i].X1 = RedX;
				TaskBlockItem[6+i].Y1 = RedY;
			}
			if(RaspberryPi.Date.TaskMassage.BlockColor[0+i]==2)//第一个任务方框的位置
			{
				TaskBlockItem[3+i].X2 = GreenX;
				TaskBlockItem[3+i].Y2 = GreenY;
				
				TaskBlockItem[6+i].X1 = GreenX;
				TaskBlockItem[6+i].Y1 = GreenY;
			}
			if(RaspberryPi.Date.TaskMassage.BlockColor[0+i]==3)//第一个任务方框的位置
			{
				TaskBlockItem[3+i].X2 = BlueX;
				TaskBlockItem[3+i].Y2 = BlueY;
				
				TaskBlockItem[6+i].X1 = BlueX;
				TaskBlockItem[6+i].Y1 = BlueY;
			}
		}
		Status=1;
	}
	else if(Status==1)
	{
		static char TempStatus=0;
		if(TempStatus ==0)
		{
			if(MyGrabBlock_ControlPlaceBlock(&TaskBlockItem[3]))
			{
				TempStatus = 1;
			}
		}
		else if(TempStatus ==1)
		{
			if(MyGrabBlock_ControlPlaceBlock(&TaskBlockItem[4]))
			{
				TempStatus = 2;
			}
		}
		else if(TempStatus ==2)
		{
			if(MyGrabBlock_ControlPlaceBlock(&TaskBlockItem[5]))
			{
				TempStatus = 3;
			}
		}

		else if(TempStatus ==3)
		{
			if(MyGrabBlock_ControlGrabBlock(&TaskBlockItem[6]))
			{
				TempStatus = 4;
			}
		}
		else if(TempStatus ==4)
		{
			if(MyGrabBlock_ControlGrabBlock(&TaskBlockItem[7]))
			{
				TempStatus = 5;
			}
		}
		else if(TempStatus ==5)
		{
			if(MyGrabBlock_ControlGrabBlock(&TaskBlockItem[8]))
			{
				for(int i =0;i<6;i++)
				{
					TaskBlockItem[i+3].GrabBlockStatus=0;
				}
				TempStatus = 0;
				Status=0;
				return 1;
			}
		}
	}
	return 0;
}
int FangHangKuai_4(void)//放置区
{
	static char Status =0;
	if(Status==0)
	{
		TaskBlockItem[3].GrabBlockStatus=0;
		TaskBlockItem[4].GrabBlockStatus=0;
		TaskBlockItem[5].GrabBlockStatus=0;
		for(int i=0;i<3;i++)
		{
			if(RaspberryPi.Date.TaskMassage.BlockColor[0+i]==1)//第一个任务方框的位置
			{
				TaskBlockItem[3+i].X2 = RedX;
				TaskBlockItem[3+i].Y2 = RedY;
				
				TaskBlockItem[3+i].H4 = 15;
				
			}
			if(RaspberryPi.Date.TaskMassage.BlockColor[0+i]==2)//第一个任务方框的位置
			{
				TaskBlockItem[3+i].X2 = GreenX;
				TaskBlockItem[3+i].Y2 = GreenY;
				
				TaskBlockItem[6+i].X1 = GreenX;
				TaskBlockItem[6+i].Y1 = GreenY;
				TaskBlockItem[3+i].H4 = 15;
			}
			if(RaspberryPi.Date.TaskMassage.BlockColor[0+i]==3)//第一个任务方框的位置
			{
				TaskBlockItem[3+i].X2 = BlueX;
				TaskBlockItem[3+i].Y2 = BlueY;
				
				TaskBlockItem[6+i].X1 = BlueX;
				TaskBlockItem[6+i].Y1 = BlueY;
				TaskBlockItem[3+i].H4 = 15;
			}
		}
		Status=1;
	}
	else if(Status==1)
	{
		static char TempStatus=0;
		if(TempStatus ==0)
		{
			if(MyGrabBlock_ControlPlaceBlock(&TaskBlockItem[3]))
			{
				TempStatus = 1;
			}
		}
		else if(TempStatus ==1)
		{
			if(MyGrabBlock_ControlPlaceBlock(&TaskBlockItem[4]))
			{
				TempStatus = 2;
			}
		}
		else if(TempStatus ==2)
		{
			if(MyGrabBlock_ControlPlaceBlock(&TaskBlockItem[5]))
			{
				TempStatus = 3;
			}
		}
		else if(TempStatus ==3)
		{
			return 1;
		}
	}
	return 0;
}
int ZhuaYuanLiaoTask_1(void)//原料区
{
	static float tempX;
	static float tempY;
	static char Status=0;
	if(Status==0)
	{
		RaspberryPi.Date.Mod = '1';//切换树莓派
		Status=1;
	}
	else if(Status==1)
	{
		if(Z(40))
		{
			if(ClawOpen())
			{
				if(Arm(0,200))
				{
					Status=2;
				}
			}
		}
	}
	else if(Status==2)
	{
		
		static char tempColorStatus=0;
		static char tempTraceStatus=0;
		static char tempStatus=0;
		if(tempStatus==0)
		{

			if(ParsesColor&&MyRaspberryPi_ArmTraceBlock(&RaspberryPi.Date))
			{
				TaskBlockItem[0].X1 = Arm.Date.RCS.X;
				TaskBlockItem[0].Y1 = Arm.Date.RCS.Y;
				TaskBlockItem[1].X1 = Arm.Date.RCS.X;
				TaskBlockItem[1].Y1 = Arm.Date.RCS.Y;
				TaskBlockItem[2].X1 = Arm.Date.RCS.X;
				TaskBlockItem[2].Y1 = Arm.Date.RCS.Y;
				tempX=Arm.Date.RCS.X;
				tempY=Arm.Date.RCS.Y;
				
				//进行方块补偿
				BlockCompensateX = Arm.Date.RCS.X - BlockBaseX; //大于0，认为往X正方向偏 
				BlockCompensateY = Arm.Date.RCS.Y - BlockBaseY; //大于0，认为往Y正方向偏
				tempStatus=1;
			}
			if(ParsesColor)
			{}
			else
			{
				Arm.Date.RCS.Vx=0;
				Arm.Date.RCS.Vy=0;
			}
	
		}
		else if(tempStatus==1)
		{
			RaspberryPi.Date.TaskMassage.num = 0;
			if(MyGrabBlock_ControlGrabBlock(&TaskBlockItem[RaspberryPi.Date.TaskMassage.num]))
			{
				RaspberryPi.Date.TaskMassage.num=1;//识别下一个
				tempStatus=0;
				Status =3;
			}
		}

	}
	else if(Status==3)
	{
		if(Z(40))
		{
			if(ClawOpen())
			{
				if(Arm(tempX,tempY))
				{
					Status=4;
				}
			}
		}
	}
	else if(Status==4)
	{
		static char tempStatus=0;
		if(tempStatus==0)
		{
//			RaspberryPi.Date.TaskMassage.num=1;
//			if(ParsesColor&&MyRaspberryPi_ArmTraceBlock(&RaspberryPi.Date))
//			{
////				TaskBlockItem[1].X1 = Arm.Date.RCS.X;
////				TaskBlockItem[1].Y1 = Arm.Date.RCS.Y;
////				tempX=Arm.Date.RCS.X;
////				tempY=Arm.Date.RCS.Y;
//				tempStatus=1;
//			}
//			if(ParsesColor)
//			{}
//			else
//			{
//				Arm.Date.RCS.Vx=0;
//				Arm.Date.RCS.Vy=0;
//			}
			if(ParsesColor)
			{
				tempStatus=1;
			}
		}
		if(tempStatus==1)
		{
			RaspberryPi.Date.TaskMassage.num=1;
			if(MyGrabBlock_ControlGrabBlock(&TaskBlockItem[RaspberryPi.Date.TaskMassage.num]))
			{
				RaspberryPi.Date.TaskMassage.num=2;//识别下一个
				tempStatus=0;
				Status =5;
			}
		}	
	}
	else if(Status==5)
	{
		if(Z(40))
		{
			if(ClawOpen())
			{
				if(Arm(tempX,tempY))
				{
					Status=6;
				}
			}
		}
	}
	else if(Status==6)
	{
		static char tempStatus=0;
		if(tempStatus==0)
		{
//			RaspberryPi.Date.TaskMassage.num=2;
//			if(ParsesColor&&MyRaspberryPi_ArmTraceBlock(&RaspberryPi.Date))
//			{
////				TaskBlockItem[2].X1 = Arm.Date.RCS.X;
////				TaskBlockItem[2].Y1 = Arm.Date.RCS.Y;
//				tempStatus=1;
//			}
//			if(ParsesColor)
//			{}
//			else
//			{
//				Arm.Date.RCS.Vx=0;
//				Arm.Date.RCS.Vy=0;
//			}
			if(ParsesColor)
			{
				tempStatus=1;
			}
		}
		if(tempStatus==1)
		{
			RaspberryPi.Date.TaskMassage.num=2;
			if(MyGrabBlock_ControlGrabBlock(&TaskBlockItem[RaspberryPi.Date.TaskMassage.num]))
			{
				RaspberryPi.Date.TaskMassage.num=3;//识别下一个
				tempStatus=0;
				Status =0;
				return 1;
			}
		}
	}
	
	return 0;
}

int ZhuaYuanLiaoTask_2(void)//原料区
{
	static float tempX;
	static float tempY;
	static char Status=0;
	if(Status==0)
	{
		RaspberryPi.Date.Mod = '1';//切换树莓派
		Status=1;
		RaspberryPi.Date.TaskMassage.BlockColor[0]=RaspberryPi.Date.TaskMassage.BlockColor[3];
		RaspberryPi.Date.TaskMassage.BlockColor[1]=RaspberryPi.Date.TaskMassage.BlockColor[4];
		RaspberryPi.Date.TaskMassage.BlockColor[2]=RaspberryPi.Date.TaskMassage.BlockColor[5];
		TaskBlockItem[0].GrabBlockStatus=0;
		TaskBlockItem[1].GrabBlockStatus=0;
		TaskBlockItem[2].GrabBlockStatus=0;
	}
	else if(Status==1)
	{
		if(Z(40))
		{
			if(ClawOpen())
			{
				if(Arm(0,200))
				{
					Status=2;
				}
			}
		}
	}
	else if(Status==2)
	{
		static char tempStatus=0;
		if(tempStatus==0)
		{
			if(ParsesColor&&MyRaspberryPi_ArmTraceBlock(&RaspberryPi.Date))
			{
				TaskBlockItem[0].X1 = Arm.Date.RCS.X;
				TaskBlockItem[0].Y1 = Arm.Date.RCS.Y;
				TaskBlockItem[1].X1 = Arm.Date.RCS.X;
				TaskBlockItem[1].Y1 = Arm.Date.RCS.Y;
				TaskBlockItem[2].X1 = Arm.Date.RCS.X;
				TaskBlockItem[2].Y1 = Arm.Date.RCS.Y;
				tempX=Arm.Date.RCS.X;
				tempY=Arm.Date.RCS.Y;
				//方块补偿
				BlockCompensateX = Arm.Date.RCS.X - BlockBaseX +30; //大于0，认为往X正方向偏 
				BlockCompensateY = Arm.Date.RCS.Y - BlockBaseY; //大于0，认为往Y正方向偏
				tempStatus=1;
			}
			if(ParsesColor)
			{}
			else
			{
				Arm.Date.RCS.Vx=0;
				Arm.Date.RCS.Vy=0;
			}
		}
		else if(tempStatus==1)
		{
			RaspberryPi.Date.TaskMassage.num = 0;
			if(MyGrabBlock_ControlGrabBlock(&TaskBlockItem[RaspberryPi.Date.TaskMassage.num]))
			{
				RaspberryPi.Date.TaskMassage.num=1;//识别下一个
				tempStatus=0;
				Status =3;
			}
		}

	}
	else if(Status==3)
	{
		if(Z(40))
		{
			if(ClawOpen())
			{
				if(Arm(tempX,tempY))
				{
					Status=4;
				}
			}
		}
	}
	else if(Status==4)
	{
		static char tempStatus=0;
		if(tempStatus==0)
		{
			if(ParsesColor)
			{
				tempStatus=1;
			}
		}
		if(tempStatus==1)
		{
			RaspberryPi.Date.TaskMassage.num=1;
			if(MyGrabBlock_ControlGrabBlock(&TaskBlockItem[RaspberryPi.Date.TaskMassage.num]))
			{
				RaspberryPi.Date.TaskMassage.num=2;//识别下一个
				tempStatus=0;
				Status =5;
			}
		}	
	}
	else if(Status==5)
	{
		if(Z(40))
		{
			if(ClawOpen())
			{
				if(Arm(tempX,tempY))
				{
					Status=6;
				}
			}
		}
	}
	else if(Status==6)
	{

		static char tempStatus=0;
		if(tempStatus==0)
		{
			if(ParsesColor)
			{
				tempStatus=1;
			}
		}

		if(tempStatus==1)
		{
			RaspberryPi.Date.TaskMassage.num=2;
			if(MyGrabBlock_ControlGrabBlock(&TaskBlockItem[RaspberryPi.Date.TaskMassage.num]))
			{
				RaspberryPi.Date.TaskMassage.num=3;//识别下一个
				tempStatus=0;
				Status =0;
				return 1;
			}
		}
	}
	
	return 0;
}

int ZhuaYuanLiaoTask_3(void)//zz
{
	static  char temp;
	static  char Armtemp;
	static char Status=0;
	
	ZhuaYuanLiao((unsigned char *)&RaspberryPi.Date.TaskMassage.BlockColor[0],RaspberryPi.Date.Block.color,&temp,&Armtemp);
	if(Status==0)
	{
		Car.Date.EnableMotor =1;
		Car.Date.DistanceControl.Enable=0;
		Car.Date.EnableControl=1;
		RaspberryPi.Date.Mod = '1';//切换树莓派
		Status=1;
	}
	else if(Status==1)
	{
		if(Z(60))
		{
			if(ClawOpen())
			{
				if(Arm(0,160))
				{
					Status=2;
				}
			}
		}
	}
	else if(Status==2)
	{
		static char tempColorStatus=0;
		static char tempTraceStatus=0;
		static char tempStatus=0;
		
		if(tempStatus==0)
		{
			if(MyRaspberryPi_carTraceBlock(&RaspberryPi.Date))
			{
				temp=1;
				Status=3;
			}
		}

	}
	else if(Status==3)
	{
		Car.Date.RelativeFixedPointMove.Enable=1;
		if(Armtemp==1)
		{
			if(MyGrabBlock_ControlGrabBlockYL(&TaskBlockItemYL[0]))
			{
				temp=1;
				Armtemp=0;
				Status=4;
			}
		}
	}
	else if(Status==4)
	{
		if(Armtemp==1)
		{
			if(MyGrabBlock_ControlGrabBlockYL(&TaskBlockItemYL[1]))
			{
				temp=1;
				Armtemp=0;
				Status=5;
			}
		}
	}
	else if(Status==5)
	{
		if(Armtemp==1)
		{
			if(MyGrabBlock_ControlGrabBlockYL(&TaskBlockItemYL[2]))
			{
				temp=1;
				Armtemp=0;
				Status=6;
			}
		}
	}
	else if(Status==6)
	{
		Arm(0,80);
		Z(50);
		Car.Date.RelativeFixedPointMove.setTargetNum=1;
		Status = 7;
	}
	else if(Status==7)
	{
		if(CatEnd())
		{
			Car.Date.RelativeFixedPointMove.Enable=0;
			Status = 8;
		}
	}
	else if(Status==8)
	{
			BlockCompensateX = 0; //大于0，认为往X正方向偏 
			BlockCompensateY = -40; //大于0，认为往Y正方向偏
		Status=9;
		return 1;
	}
	return 0;
}

char state_dir;
char dir = 2;//表示方块的旋转方向，其中2代表没识别到
int ZhuaYuanLiao(unsigned char *color_order,char Current,char *temp,char *Armtemp)//sx
{
	static char flag=0;
	static char Last;
	static char Num = 0;//表示等了几个周期
	static char Take_Num = 0;//表示拿到第几个方块

	/*判断方块顺序虚拟转向*/

	if(*temp==0){return 0;}//*temp不等于1就不运行后面的代码
//	if(*temp==3){return 1;}//完成三个方块的抓取就停止运行1
//	if(Current==0){return 0;}//没识别到颜色也跳出去
	
	if(color_order[1]!=BLUE&&color_order[1]>color_order[0])state_dir=0;
	else if(color_order[1]==BLUE&&color_order[1]>color_order[0])state_dir=1;
	else if(color_order[1]!=RED&&color_order[1]<color_order[0])state_dir=1;
	else if(color_order[1]==RED&&color_order[1]<color_order[0])state_dir=0;
	
	if(flag==0)
	{
		if(color_order[Take_Num]==Current&&Num==0&&Current!=0)
		 {
			if(Last!=Current)
			{
				if(Current!=BLUE&&Current>Last)dir=0;
				else if(Current==BLUE&&Current>Last)dir=1;
				else if(Current!=RED&&Current<Last)dir=1;
				else if(Current==RED&&Current<Last)dir=0;
				Num++;
			}
			//到这里可以直接抓物料，只要让*temp=0这个函数就不会继续运行直到做完抓放的动作；
			Buzze=50;
			*Armtemp =1;
			Take_Num=1;
			flag=4;
			Num++;
		}
		else if(Last!=Current)
		{
			if(Last!=0)
			{
				if(Current!=BLUE&&Current>Last)dir=0;
				else if(Current==BLUE&&Current>Last)dir=1;
				else if(Current!=RED&&Current<Last)dir=1;
				else if(Current==RED&&Current<Last)dir=0;
				flag=1;
			}

		}
		Last=Current;
	}
	else if(flag==1)
	{
		if(Take_Num==0)
		{
			if(dir==1)
			{
				//这里实现在下方并抓取;
				Arm.Date.RCS.setX=0;
				Arm.Date.RCS.setY=279;
				Z(60);
				Car.Date.RelativeFixedPointMove.setTargetNum=2;
				if(color_order[Take_Num]==Current)
				{

					*Armtemp=1;
					*temp=0;
					Take_Num++;
					if(state_dir==dir)
					{
						flag=2;
//						Buzze=50;
					}
					else if(state_dir!=dir)
					{
						Buzze=50;
						flag=3;
					}
				}
				
			}
			else if(dir==0)
			{
				//这里实现在上方并抓取;
				Arm.Date.RCS.setX=0;
				Arm.Date.RCS.setY=279;
				Z(60);
				Car.Date.RelativeFixedPointMove.setTargetNum=3;
				if(color_order[Take_Num]==Current)
				{
					*temp=0;
					*Armtemp=1;
					Buzze=50;
					Take_Num++;
					if(state_dir==dir)flag=2;
					else if(state_dir!=dir)flag=3;
				}
				
			}
		 }
	}
	else if(flag==2)
	{
		if(color_order[Take_Num]==Current)
		{
			//这里不用移动看到了直接抓取
			*Armtemp=1;
			*temp=0;
			Buzze=50;
			Take_Num++;
			if(Take_Num==3)flag=5;
		}
	}
	else if(flag==3)
	{
		if(dir==1)
		{
			if(Take_Num==1)
			{
				//这里移动至上方
				Arm.Date.RCS.setX=0;
				Arm.Date.RCS.setY=279;
				Z(60);
				Car.Date.RelativeFixedPointMove.setTargetNum=3;
				if(color_order[Take_Num]==Current)
				{
					Buzze=50;
					//这里看到了直接抓
					*Armtemp=1;
					*temp=0;
					Take_Num++;
				}
			}
			else if(Take_Num==2)
			{
				//这里移动至这里移动至中间
				Car.Date.RelativeFixedPointMove.setTargetNum=1;
				Arm.Date.RCS.setX=0;
				Arm.Date.RCS.setY=160;
				Z(60);
				if(color_order[Take_Num]==Current)
				{
					Buzze=50;
					//这里看到了直接抓
					*Armtemp=1;
					Take_Num++;
					*temp=0;
					flag=5;
				}
			}
		}
		else if(dir==0)
		{
			if(Take_Num==1)
			{
				//这里移动至下方
				Arm.Date.RCS.setX=0;
				Arm.Date.RCS.setY=279;
				Z(60);
				Car.Date.RelativeFixedPointMove.setTargetNum=2;
				if(color_order[Take_Num]==Current)
				{
					Buzze=50;
					//这里看到了直接抓
					Take_Num++;
					*Armtemp=1;
					*temp=0;
				}
			}
			if(Take_Num==2)
			{
				//这里移动至这里移动至中间
				Car.Date.RelativeFixedPointMove.setTargetNum=1;
				Arm.Date.RCS.setX=0;
				Arm.Date.RCS.setY=160;
				Z(60);
				if(color_order[Take_Num]==Current)
				{
					Buzze=50;
					//这里看到了直接抓
					Take_Num++;
					*Armtemp=1;
					*temp=0;
					flag=5;
				}
			}
		}
	}
	else if(flag==4)
	{
		if(state_dir==dir)
		{
//			Arm.Date.RCS.setX=0;
//			Arm.Date.RCS.setY=160;
//			Z(60);
			if(color_order[Take_Num]==Current)
			{
				Buzze=50;
				//这里不用移动看到了直接抓取
				*Armtemp=1;
				Take_Num++;
				*temp=0;
				if(Take_Num==3)flag=5;
			}
		}
		else if(state_dir!=dir)
		{
			if(dir==1)
			{
				
				if(Take_Num==1)
				{
					//移动到下方
					Arm.Date.RCS.setX=0;
					Arm.Date.RCS.setY=279;
					Z(60);
					Car.Date.RelativeFixedPointMove.setTargetNum=2;
					if(color_order[Take_Num]==Current)
					{
						Buzze=50;
						//这里看到了直接抓
						*Armtemp=1;
						*temp=0;
						Take_Num++;
					}
				}
				else if(Take_Num==2)
				{
					//移动到上方
					Arm.Date.RCS.setX=0;
					Arm.Date.RCS.setY=279;
					Z(60);
					Car.Date.RelativeFixedPointMove.setTargetNum=3;
					if(color_order[Take_Num]==Current)
					{
						Buzze=50;
						//这里看到了直接抓
						*Armtemp=1;
						Take_Num++;
						*temp=0;
					}
				}
			}
			else if(dir==0)
			{
				if(Take_Num==1)
				{
					//移动到上方
					Arm.Date.RCS.setX=0;
					Arm.Date.RCS.setY=279;
					Z(60);
					Car.Date.RelativeFixedPointMove.setTargetNum=3;
					if(color_order[Take_Num]==Current)
					{
						Buzze=50;
						//这里看到了直接抓
						Take_Num++;
						*Armtemp=1;
						*temp=0;
					}
				}
				else if(Take_Num==2)
				{
					//移动到下方
					Arm.Date.RCS.setX=0;
					Arm.Date.RCS.setY=279;
					Z(60);
					Car.Date.RelativeFixedPointMove.setTargetNum=2;
					if(color_order[Take_Num]==Current)
					{
						Buzze=50;
						//这里看到了直接抓
						Take_Num++;
						*temp=0;
						*Armtemp=1;
					}
				}
			}
		}
	}
	else if(flag==5)
	{
		char flag=0;
		char dir = 2;
		char Last;
		char Num = 0;
		char Take_Num = 0;
		*temp = 0;
	}
	return 0;
}
//放方块追踪
int Trace_FangHangKuai_1(void)//加工区
{
	static char time=0;
	static char Status;
	if(Status==0)
	{
		//第一次已经在路程校准，直接记录位置
		MyGrabBlock_getArmXY(TaskBlockItem);//获取中间是什么色环
		Status=1;
	}
	else if(Status==1)
	{
		//移动到左边,看色环

			if(Arm(GreenX,GreenY))
			{
				RaspberryPi.Date.Block.x = 0;
				RaspberryPi.Date.Block.y = 0;
				Status =2;
				
			}
		

	}
	else if(Status==2)
	{
		time++;
		if(time>10)
		{
		//追踪220
			if(MyRaspberryPi_ArmTraceBlock(&RaspberryPi.Date))
			{
				MyGrabBlock_getArmXY(TaskBlockItem);
				Status = 3;
				time=0;
			}
		}
	}
	else if(Status==3)
	{
		//移动到右边,看色环
		if(Arm(RedX,RedY))
		{
			RaspberryPi.Date.Block.x = 0;
			RaspberryPi.Date.Block.y = 0;
			Status =4;
		}
	}
	else if(Status==4)
	{
		time++;
		if(time>10)
		{
			//追踪				
			if(MyRaspberryPi_ArmTraceBlock(&RaspberryPi.Date))
			{
				MyGrabBlock_getArmXY(TaskBlockItem);
				Status = 5;
				time=0;
			}
		}
	}
	//开始放方块
	else if(Status==5)
	{
		static char TempStatus=0;
		if(TempStatus ==0)
		{
			if(MyGrabBlock_ControlPlaceBlock(&TaskBlockItem[3]))
			{
				TempStatus = 1;
			}
		}
		else if(TempStatus ==1)
		{
			if(MyGrabBlock_ControlPlaceBlock(&TaskBlockItem[4]))
			{
				TempStatus = 2;
			}
		}
		else if(TempStatus ==2)
		{
			if(MyGrabBlock_ControlPlaceBlock(&TaskBlockItem[5]))
			{
				TempStatus = 3;
			}
		}

		else if(TempStatus ==3)
		{
			if(MyGrabBlock_ControlGrabBlock(&TaskBlockItem[6]))
			{
				TempStatus = 4;
			}
		}
		else if(TempStatus ==4)
		{
			if(MyGrabBlock_ControlGrabBlock(&TaskBlockItem[7]))
			{
				TempStatus = 5;
			}
		}
		else if(TempStatus ==5)
		{
			if(MyGrabBlock_ControlGrabBlock(&TaskBlockItem[8]))
			{
				for(int i =0;i<6;i++)
				{
					TaskBlockItem[i+3].GrabBlockStatus=0;
				}
				TempStatus = 0;
				Status=0;
				return 1;
			}
		}
	}
	return 0;
}


int Trace_FangHangKuai_2(void)//放置区   //放置区第二次不能用
{	
	static char time=0;
	static char Status;
	if(Status==0)
	{
		//第一次已经在路程校准，直接记录位置
		MyGrabBlock_getArmXY(TaskBlockItem);//获取中间是什么色环
		Status=1;
	}
	else if(Status==1)
	{
		//移动到左边,看色环
		if(Arm(GreenX,GreenY))
		{
			RaspberryPi.Date.Block.x = 0;
			RaspberryPi.Date.Block.y = 0;
			Status =2;
		}
	}
	else if(Status==2)
	{
		time++;
		if(time>10)
		{
			//追踪				
			if(MyRaspberryPi_ArmTraceBlock(&RaspberryPi.Date))
			{
				MyGrabBlock_getArmXY(TaskBlockItem);
				Status = 3;
				time=0;
			}
		}

	}
	else if(Status==3)
	{
		//移动到右边,看色环
		if(Arm(RedX,RedY))
		{
			RaspberryPi.Date.Block.x = 0;
			RaspberryPi.Date.Block.y = 0;
			Status =4;
		}
	}
	else if(Status==4)
	{
		time++;
		if(time>10)
		{
			//追踪				
			if(MyRaspberryPi_ArmTraceBlock(&RaspberryPi.Date))
			{
				MyGrabBlock_getArmXY(TaskBlockItem);
				Status = 5;
				time=0;
			}
		}
	}
	//开始放方块
	else if(Status==5)
	{
		static char TempStatus=0;
		if(TempStatus ==0)
		{
			if(MyGrabBlock_ControlPlaceBlock(&TaskBlockItem[3]))
			{
				TempStatus = 1;
			}
		}
		else if(TempStatus ==1)
		{
			if(MyGrabBlock_ControlPlaceBlock(&TaskBlockItem[4]))
			{
				TempStatus = 2;
			}
		}
		else if(TempStatus ==2)
		{
			if(MyGrabBlock_ControlPlaceBlock(&TaskBlockItem[5]))
			{
				TempStatus = 3;
			}
		}
		else if(TempStatus ==3)
		{
			for(int i =0;i<6;i++)
			{
				TaskBlockItem[i+3].GrabBlockStatus=0;
			}
			TempStatus = 0;
			Status=0;
			return 1;
		}
	}
	return 0;
}
#define Status Date->LineStatus
int Line_1(MyLineDate_Typedef* Date)//移动到二维码
{
	
	if(Status==0)
	{
		Car.Date.DistanceControl.SetDistanceSpeed=200;
		Car.Date.EnableMotor =1;
		CatMove(550,200);
		Status = 1;
	}
	else if(Status==1)
	{
		if(CatEnd())
		{
			Car.Date.DistanceControl.SetDistanceSpeed=200;
			return 1;
		}
	}
	
	return 0;
}
//
int Line_2(MyLineDate_Typedef* Date)//移动到转盘
{
	
	if(Status==0)
	{
		Car.Date.DistanceControl.SetDistanceSpeed=400;
		Car.Date.EnableMotor =1;
		CatMove(919,0);

		Status = 1;
	}
	else if(Status==1)
	{
//		if(Car.Date.DistanceControl.ResidueDistanceY>-10)
//		{
//			Car.Date.DistanceControl.SetDistanceSpeed=200;
//			Status=2;

//		}
		Car.Date.DistanceControl.SetDistanceSpeed+=10;
		if(Car.Date.DistanceControl.SetDistanceSpeed>800)
		{
			Car.Date.DistanceControl.SetDistanceSpeed=800;
		}
		
		if(CatEnd())
		{
			Car.Date.DistanceControl.SetDistanceSpeed=200;
			CatMove(0,-130);

			Status = 2;
		}
	}
	
	else if(Status==2)
	{
		if(CatEnd())
		{
			Car.Date.DistanceControl.ResidueDistanceX=0;
			Car.Date.DistanceControl.ResidueDistanceY=0;
			Car.Date.SetVw = 0;
			Car.Date.SetVx =0 ;
			Car.Date.SetVy =0 ;
			Car.Date.DistanceControl.SetDistanceSpeed=200;
			Main.tempAngle = Car.Date.CarAngle;
			Car.Date.EnableMotor =0;
			return 1;
		}
		
	}
	
	return 0;
}
//
int Line_3(MyLineDate_Typedef* Date)//移动到加工区
{
	if(Status==0)
	{
		//重新定位陀螺仪
//		MyIMU.MyIMUDate.Full_YawAnglePY =YawPY2+MyIMU.MyIMUDate.Full_YawAnglePY -Car.Date.CarAngle + Main.tempAngle;
		
//		Car.Date.EnableMotor =-1;//使用底层库操作
//		Car.Date.EnableControl=0;
		Car.Date.EnableMotor =1;
		Status = 1;
	}
	else if(Status==1)
	{
		//纠正小车 
		CatMove(-BlockCompensateX-30,-BlockCompensateY);
		Status = 2;
	}
	else if(Status==2)
	{
		if(CatEnd())
		{
			Car.Date.EnableMotor =-1;//使用底层库操作
			Car.Date.EnableControl=0;
			Status=3;
		}
	}
	else if(Status==3)
	{
		Car_AllangleSpeed(5.3,0,4);
		if(Car.Date.CarAngle>90)
		{
			Car_AllangleSpeed(0,0,0);
			Car.Date.setCarAngle = 90;
			Car.Date.EnableMotor =1;//使能
			Car.Date.EnableControl=1;
			Status=4;
		}
	}
	else if(Status==4)
	{
		Car.Date.DistanceControl.SetDistanceSpeed=100;
//		CatMove(1500-(BlockCompensateX*10),0-(BlockCompensateY*10));
		CatMove(1530,0);
		Status=5;
	}
	else if(Status==5)
	{
		Car.Date.DistanceControl.SetDistanceSpeed+=10;
		if(Car.Date.DistanceControl.SetDistanceSpeed>800)
		{
			Car.Date.DistanceControl.SetDistanceSpeed=800;
		}
		if(Car.Date.DistanceControl.ResidueDistanceX<80)
		{
			Car.Date.DistanceControl.ResidueDistanceX=0;
			Car.Date.DistanceControl.ResidueDistanceY=0;
			Car.Date.SetVw = 0;
			Car.Date.SetVx =0 ;
			Car.Date.SetVy =0 ;
			Car.Date.DistanceControl.SetDistanceSpeed=200;
			Car.Date.EnableMotor =-1;//使用底层库操作
			Car.Date.EnableControl=0;
			Status=6;
		}
	}
	else if(Status==6)
	{
		Car_AllangleSpeed(5.5,-100,4);
		if(Car.Date.CarAngle>176)
		{
			Car.Date.DistanceControl.ResidueDistanceX = 0;
			Car.Date.DistanceControl.ResidueDistanceY = 0;
			Car.Date.SetVx = 0;
			Car.Date.SetVy = 0;
			Car_AllangleSpeed(0,0,0);
			Status=7;
		}
	}
	else if(Status==7)
	{
		if(ClawOpen())
		{
			if(Arm(0,200))
			{
				Status=9;
			}
		}
		
	}

	else if(Status==9)
	{
		Car.Date.setCarAngle = 180;
		Car.Date.EnableMotor =1;
		Car.Date.EnableControl=1;
		Car.Date.DistanceControl.Enable =0;
		RaspberryPi.Date.Mod = '2';//切换树莓派模式
		if(Z(0))
		{
			TurnPlate(TaskBlockItem[0].T1);
			if(MyRaspberryPi_carTraceBlock(&RaspberryPi.Date))//对准圆心
			{
				Car.Date.SetVx=0;
				Car.Date.SetVy=0;
				Car.Date.EnableMotor =0;
				Status = 10;
				return 1;
			}
		}

	}
	return 0;
}

//
int Line_4(MyLineDate_Typedef* Date)//移动到放置区
{

	if(Status==0)
	{
		Arm(0,80);
		Car.Date.EnableMotor =1;
		Car.Date.DistanceControl.Enable =1;
		Car.Date.EnableControl=1;
		CatMove(0,70);//70
		RaspberryPi.Date.Mod = '4';//切换树莓派模式
		Status = 1;
	}
	else if(Status==1)
	{
		if(CatEnd())
		{
			Status = 2;
		}
	}
	else if(Status==2)
	{
//		if(Z(0))
//		{
//			if(RaspberryPi.Date.Block.color==4&&RaspberryPi.Date.LineAngle!=0)
//			{
//				MyIMU.MyIMUDate.Full_YawAnglePY =MyIMU.MyIMUDate.Full_YawAnglePY+ (RaspberryPi.Date.LineAngle - (180-Arm.Date.SingleArm[ArmB].Angle));//-MyIMU.MyIMUDate.PY
				Status =3;
//			}
//		}
	}
	else if(Status==3)
	{
		Car.Date.DistanceControl.SetDistanceSpeed=200;
		CatMove(-530,0);
		Status = 4;
	}
	else if(Status==4)
	{
		Car.Date.DistanceControl.SetDistanceSpeed+=10;
		if(Car.Date.DistanceControl.SetDistanceSpeed>800)
		{
			Car.Date.DistanceControl.SetDistanceSpeed=800;
		}
		
		if(Car.Date.DistanceControl.ResidueDistanceX>-80)
		{
			Car.Date.DistanceControl.ResidueDistanceX=0;
			Car.Date.DistanceControl.ResidueDistanceY=0;
			Car.Date.SetVw = 0;
			Car.Date.SetVx =0 ;
			Car.Date.SetVy =0 ;
			Car.Date.DistanceControl.SetDistanceSpeed=200;
			Car.Date.EnableMotor =-1;//使用底层库操作
			Car.Date.EnableControl=0;
			Status=5;
		}

	}
	else if(Status==5)
	{
		Car_AllangleSpeed(5.5,-180,-4);
		if(Car.Date.CarAngle<93)
		{
			Car.Date.DistanceControl.ResidueDistanceX = 0;
			Car.Date.DistanceControl.ResidueDistanceY = 0;
			Car.Date.SetVx = 0;
			Car.Date.SetVy = 0;
			Car_AllangleSpeed(0,0,0);
			Status=6;
		}
	}
	else if(Status==6)
	{
		Car.Date.EnableMotor =1;
		Car.Date.EnableControl=1;
		Car.Date.DistanceControl.Enable =1;
		Car.Date.DistanceControl.SetDistanceSpeed=200;
		Car.Date.setCarAngle=91;
		CatMove(-840,-70);
		Status = 7;
	}
	else if(Status==7)
	{
		Car.Date.DistanceControl.SetDistanceSpeed+=10;
		if(Car.Date.DistanceControl.SetDistanceSpeed>800)
		{
			Car.Date.DistanceControl.SetDistanceSpeed=800;
		}
		if(CatEnd())
		{
			Car.Date.SetVx=0;
			Car.Date.SetVy=0;
			Status=8;
			Arm(0,200);
		}
	}
	else if(Status==8)
	{
			Car.Date.setCarAngle=90;
			Car.Date.DistanceControl.Enable =0;
			if(Main.roundStatus==0)
			{
				RaspberryPi.Date.Mod = '2';//切换树莓派模式
			}
			else
			{
				RaspberryPi.Date.Mod = '1';
			}
			
			if(Z(0))
			{
				if(MyRaspberryPi_carTraceBlock(&RaspberryPi.Date))//对准圆心
				{
					Car.Date.EnableMotor =0;
					
					Status = 9;
					return 1;
				}
			}
	}
	return 0;
}

int Line_5(MyLineDate_Typedef* Date)//移动到转盘
{
	if(Status==0)
	{
		Main.roundStatus=1;
		Car.Date.EnableMotor =1;
		Car.Date.DistanceControl.Enable =1;
		Car.Date.EnableControl=1;
		if(Z(0))
		{
			CatMove(0,70);//70
			RaspberryPi.Date.Mod = '4';//切换树莓派模式
			Status = 1;
		}


	}
	else if(Status==1)
	{
		if(CatEnd())
		{
			Status = 2;
			RaspberryPi.Date.LineAngle=0;
			Car.Date.setCarAngle = 90;
		}
	}
	else if(Status==2)
	{
		Arm(0,200);


		MyIMU.MyIMUDate.Full_YawAnglePY =MyIMU.MyIMUDate.Full_YawAnglePY-1;//-MyIMU.MyIMUDate.PY
		Status =3;

		
	}
	else if(Status==3)
	{
		Arm(0,80);
		Car.Date.DistanceControl.SetDistanceSpeed=200;
		CatMove(-640,0);
		Status = 4;	
	}
	
	else if(Status==4)
	{
		Car.Date.DistanceControl.SetDistanceSpeed+=10;
		if(Car.Date.DistanceControl.SetDistanceSpeed>800)
		{
			Car.Date.DistanceControl.SetDistanceSpeed=800;
		}
		if(Car.Date.DistanceControl.ResidueDistanceX>-80)
		{
			Car.Date.DistanceControl.ResidueDistanceX=0;
			Car.Date.DistanceControl.ResidueDistanceY=0;
			Car.Date.SetVw = 0;
			Car.Date.SetVx =0 ;
			Car.Date.SetVy =0 ;
			Car.Date.DistanceControl.SetDistanceSpeed=200;
			Car.Date.EnableMotor =-1;//使用底层库操作
			Car.Date.EnableControl=0;
			Status=5;
		}	
	}
	else if(Status==5)
	{
		Car_AllangleSpeed(5.5,90,-4);
		if(Car.Date.CarAngle<0)
		{
			Car.Date.DistanceControl.ResidueDistanceX = 0;
			Car.Date.DistanceControl.ResidueDistanceY = 0;
			Car.Date.SetVx = 0;
			Car.Date.SetVy = 0;
			Car_AllangleSpeed(0,0,0);
			Status=6;
		}
	}
	else if(Status==6)
	{
						
		Car.Date.EnableMotor =1;
		Car.Date.EnableControl=1;
		Car.Date.DistanceControl.Enable =1;
		Car.Date.DistanceControl.SetDistanceSpeed=200;
		Car.Date.setCarAngle=0;
		CatMove(-410,0);
		Status = 7;
	}
	else if(Status == 7)
	{
		if(CatEnd())
		{
			Status=8;
//			Main.tempAngle = Car.Date.CarAngle;
			Car.Date.EnableMotor =0;
//			YawPY2=1;
			return 1;
		}
	}
   return 0;
}


int Line_6(MyLineDate_Typedef* Date)//移动到家
{
	if(Status==0)
	{
		Car.Date.EnableMotor =1;
		Car.Date.DistanceControl.Enable =1;
		Car.Date.EnableControl=1;
		if(Z(0))
		{
			CatMove(0,70);//70
			RaspberryPi.Date.Mod = '4';//切换树莓派模式
			Status = 1;
		}


	}
	else if(Status==1)
	{
		if(CatEnd())
		{
			Status = 2;
		}
	}
	else if(Status==2)
	{
//		if(Arm(0,200))
//		{
//			if(RaspberryPi.Date.Block.color==4&&RaspberryPi.Date.LineAngle!=0)
//			{
//				MyIMU.MyIMUDate.Full_YawAnglePY =MyIMU.MyIMUDate.Full_YawAnglePY+ (RaspberryPi.Date.LineAngle - (180-Arm.Date.SingleArm[ArmB].Angle));//-MyIMU.MyIMUDate.PY
				Status =3;
//			}
//		}
	}
	else if(Status==3)
	{
		Arm(0,80);
		Car.Date.DistanceControl.SetDistanceSpeed=200;
		CatMove(-610,0);
		Status = 4;	
	}
	
	else if(Status==4)
	{
		Car.Date.DistanceControl.SetDistanceSpeed+=10;
		if(Car.Date.DistanceControl.SetDistanceSpeed>800)
		{
			Car.Date.DistanceControl.SetDistanceSpeed=800;
		}
		if(Car.Date.DistanceControl.ResidueDistanceX>-80)
		{
			Car.Date.DistanceControl.ResidueDistanceX=0;
			Car.Date.DistanceControl.ResidueDistanceY=0;
			Car.Date.SetVw = 0;
			Car.Date.SetVx =0 ;
			Car.Date.SetVy =0 ;
			Car.Date.DistanceControl.SetDistanceSpeed=200;
			Car.Date.EnableMotor =-1;//使用底层库操作
			Car.Date.EnableControl=0;
			Status=5;
		}	
	}
	else if(Status==5)
	{
		Car_AllangleSpeed(5.5,90,-4);
		if(Car.Date.CarAngle<0)
		{
			Car.Date.DistanceControl.ResidueDistanceX = 0;
			Car.Date.DistanceControl.ResidueDistanceY = 0;
			Car.Date.SetVx = 0;
			Car.Date.SetVy = 0;
			Car_AllangleSpeed(0,0,0);
			Status=6;
		}
	}
	else if(Status==6)
	{
						
		Car.Date.EnableMotor =1;
		Car.Date.EnableControl=1;
		Car.Date.DistanceControl.Enable =1;
		Car.Date.DistanceControl.SetDistanceSpeed=200;
		Car.Date.setCarAngle=1;
		CatMove(-1890,0);
		Status = 7;
	}
	else if(Status == 7)
	{
		Car.Date.DistanceControl.SetDistanceSpeed+=10;
		if(Car.Date.DistanceControl.SetDistanceSpeed>800)
		{
			Car.Date.DistanceControl.SetDistanceSpeed=800;
		}
		if(CatEnd())
		{
			Car.Date.SetVw = 0;
			Car.Date.SetVx =0 ;
			Car.Date.SetVy =0 ;
			Car.Date.DistanceControl.ResidueDistanceX=0;
			Car.Date.DistanceControl.ResidueDistanceY=0;
			Car.Date.DistanceControl.SetDistanceSpeed=200;
			CatMove(0,-120);//140
			Status=8;
		}
	}
   return 0;
}



#undef Status