#ifndef __UsartDate_H
#define __UsartDate_H

#include "main.h"

typedef struct SMTypeDef
{
	float SM_X;
	float SM_Y;
	int SM_coloar;
} SMTypeDef;

int CKP_Parses(uint8_t *date);
int SM_Parses(uint8_t *date);

int SM_QRParses(uint8_t *date);
int SM_XYParses(uint8_t *date);
//½âÎöÊ÷İ®ÅÉĞÅÏ¢
//int SM_Parses(uint8_t *date);
#endif
