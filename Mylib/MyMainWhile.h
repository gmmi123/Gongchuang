#ifndef __MyMainWhile_H
#define __MyMainWhile_H
#include "MyCarControl.h"
#include "MyTurnPlate.h"
// 成员变量
typedef struct
{
	int num;
	float TempX[100];
	int	  Status;
	float tempAngle;
	char  roundStatus;
	char mod;
} MyMainWhileDate_Typedef;

// 类
typedef struct
{
	MyMainWhileDate_Typedef Date;

	// 方法
} MyMainWhile_Typedef;




typedef struct
{
	char LineStatus;
} MyLineDate_Typedef;

typedef struct
{
	MyLineDate_Typedef Date;
	int (*Start)(MyLineDate_Typedef* Date);
} MyLine_Typedef;


extern MyMainWhileDate_Typedef Main;
void MyMainwhile(void);
int Line_1(MyLineDate_Typedef* Date);
int Line_2(MyLineDate_Typedef* Date);
int Line_3(MyLineDate_Typedef* Date);
int Line_4(MyLineDate_Typedef* Date);
int Line_5(MyLineDate_Typedef* Date);
int Line_6(MyLineDate_Typedef* Date);
int ZhuaYuanLiaoTask_1(void);
int FangHangKuai_1(void);
int FangHangKuai_2(void);
int ZhuaYuanLiaoTask_2(void);
int FangHangKuai_3(void);
int FangHangKuai_4(void);
int ZhuaYuanLiaoTask_3(void);
#endif
