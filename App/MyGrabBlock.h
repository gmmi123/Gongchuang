 #ifndef __MyGrabBlock_H
 #define __MyGrabBlock_H
 //�ײ��
#include "MyArm.h"
#include "MyLift.h"
#include "MyTurnPlate.h"
#include "MyClaw.h"
typedef struct
{
	float X;
	float Y;
}TaskColorBlockXY_Typedef;

typedef struct
{
	float H1;//צ���ƶ��߶�
	float H2;//צ��ץ����߶�
	float H3;//צ��ץ���������߶�
	float H4;//צ�ӷŷ����½��߶�
	
	float X1;//צ��ץ��������
	float Y1;//צ��ץ��������
	
	float X2;//צ�ӷŷ�������
	float Y2;//צ�ӷŷ�������
	
	float T1;//צ���½�ʱת��λ��
	float T2;//צ�ӷŷ���ʱת��λ��
	char GrabBlockStatus;
	float dt;
	float tempt;
}GrabBlockItem_Typedef;


// ��Ա����
typedef struct
{
	char GrabBlockStatus;
} MyGrabBlockDate_Typedef;
 
// ��
typedef struct
{
	MyGrabBlockDate_Typedef Date;
	// ����
} MyGrabBlock_Typedef;

extern MyGrabBlock_Typedef GrabBlock;
extern GrabBlockItem_Typedef TaskBlockItemYL[3];
extern GrabBlockItem_Typedef TaskBlockItem[9];
void MyGrabBlock_Task(void);
void MyGrabBlock_Init(MyGrabBlock_Typedef* object);
int MyGrabBlock_ControlGrabBlock(GrabBlockItem_Typedef* object);
void MyGrabBlock_setItem(GrabBlockItem_Typedef* object); 
void MyGrabBlock_getArmXY(GrabBlockItem_Typedef* object);
int MyGrabBlock_ControlPlaceBlock(GrabBlockItem_Typedef* object);
int MyGrabBlock_ControlGrabBlockYL(GrabBlockItem_Typedef* object);
#endif
