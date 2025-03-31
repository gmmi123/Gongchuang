#include "UsartDate.h"
#include "string.h"
#include <stdio.h>
char contentCKP[20];
char contentMV[20];

//����8���
//����9��ݮ��

SMTypeDef SM;

//������������Ϣ
int CKP_Parses(uint8_t *date)
{

	sscanf((const char*)date, "pm%[^mp]mp", contentCKP);
	if(contentCKP[0]=='s')
	{
		return 1;
	}
	
	return 0;
}	

//����MV��ά����Ϣ

int SM_QRParses(uint8_t *date)
{
	sscanf((const char*)date,"sm %[^ms]" ,contentMV);
	if(contentMV[4]=='+')
	{
		return 1;
	}
	return 0;
	
}

//������ݮ����Ϣ
int SM_XYParses(uint8_t *date)
{
	
	if(date[0]=='s'&&date[1]=='m')
	{
		sscanf((const char*)date,"sm%f,%f,%dms" ,&SM.SM_X,&SM.SM_Y,&SM.SM_coloar);
		if(SM.SM_coloar==0)
		{
			return -1;//û��ʶ��
		}
		return 1;//ʶ����
	}
	return 0;//����û�д���
	
}

