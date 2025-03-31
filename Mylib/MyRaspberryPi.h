#ifndef __MyRaspberryPi_H
#define __MyRaspberryPi_H

//�ײ��
#define ParsesColor MyRaspberryPi_ParsesColor()

typedef struct
{
	float Kp;
	float Kd;
	float LastErrX;
	float LastErrY;
	float ErrX;
	float ErrY;
	float TargetX;
	float TargetY;
}MyRaspberryPi_CatTracePID_Typedef;

typedef struct
{
	float Kp;
	float Kd;
	float LastErrX;
	float LastErrY;
	float ErrX;
	float ErrY;
	float TargetX;
	float TargetY;
	float TempH;
	float TempXYH;
}MyRaspberryPi_ArmTracePID_Typedef;

typedef struct
{
	int color;
	float x;
	float y;
}Block_Typedef;

typedef struct
{
	char num;//Item�嵥�ϵĵ�num�����������ץȡ�ͷ��ú���
	char BlockColor[6];
	char TempDate[20];
}TaskMassage_Typedef;


// ��Ա����
typedef struct
{
	MyRaspberryPi_ArmTracePID_Typedef ArmPID;
	MyRaspberryPi_CatTracePID_Typedef PID;
	float t;
	float dt;
	float LineAngle;
	char ColorFlag;
	char LastColot;
	char Mod;
	TaskMassage_Typedef TaskMassage;
	Block_Typedef Block;
	unsigned char Receive_Date[20];
}MyRaspberryPiDate_Typedef;

// ��
typedef struct
{
	MyRaspberryPiDate_Typedef Date;

} MyRaspberryPi_Typedef;
extern MyRaspberryPi_Typedef RaspberryPi;
void MyRaspberryPi_Task(MyRaspberryPi_Typedef* object);

int MyRaspberryPi_ParsesColor(void);
int MyRaspberryPi_carTraceBlock(MyRaspberryPiDate_Typedef *object);
int MyRaspberryPi_ArmTraceBlock(MyRaspberryPiDate_Typedef *object);
void MyRaspberryPi_GetBlockMassage(MyRaspberryPiDate_Typedef *object);

#endif
