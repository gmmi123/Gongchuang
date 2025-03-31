#include "UsartDate.h"
#include "string.h"
#include <stdio.h>
char contentCKP[20];
char contentMV[20];

//串口8电机
//串口9树莓派

SMTypeDef SM;

//解析串口屏信息
int CKP_Parses(uint8_t *date)
{

	sscanf((const char*)date, "pm%[^mp]mp", contentCKP);
	if(contentCKP[0]=='s')
	{
		return 1;
	}
	
	return 0;
}	

//解析MV二维码信息

int SM_QRParses(uint8_t *date)
{
	sscanf((const char*)date,"sm %[^ms]" ,contentMV);
	if(contentMV[4]=='+')
	{
		return 1;
	}
	return 0;
	
}

//解析树莓派信息
int SM_XYParses(uint8_t *date)
{
	
	if(date[0]=='s'&&date[1]=='m')
	{
		sscanf((const char*)date,"sm%f,%f,%dms" ,&SM.SM_X,&SM.SM_Y,&SM.SM_coloar);
		if(SM.SM_coloar==0)
		{
			return -1;//没有识别到
		}
		return 1;//识别到了
	}
	return 0;//数据没有传到
	
}

