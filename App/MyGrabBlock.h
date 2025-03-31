 #ifndef __MyGrabBlock_H
 #define __MyGrabBlock_H
 //底层库
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
	float H1;//爪子移动高度
	float H2;//爪子抓方块高度
	float H3;//爪子抓方块上升高度
	float H4;//爪子放方块下降高度
	
	float X1;//爪子抓方块坐标
	float Y1;//爪子抓方块坐标
	
	float X2;//爪子放方块坐标
	float Y2;//爪子放方块坐标
	
	float T1;//爪子下降时转盘位置
	float T2;//爪子放方块时转盘位置
	char GrabBlockStatus;
	float dt;
	float tempt;
}GrabBlockItem_Typedef;


// 成员变量
typedef struct
{
	char GrabBlockStatus;
} MyGrabBlockDate_Typedef;
 
// 类
typedef struct
{
	MyGrabBlockDate_Typedef Date;
	// 方法
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
