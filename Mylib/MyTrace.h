#ifndef __MyTrace_H
#define __MyTrace_H


 //�ײ��



// ��Ա����
typedef struct
{

} MyTraceDate_Typedef;
 
// ��
typedef struct
{
	MyTraceDate_Typedef Date;
	// ����
} MyTrace_Typedef;

extern MyTrace_Typedef Test;
void MyTrace_Task(MyTrace_Typedef* object);
void MyTrace_Init(MyTrace_Typedef* object);

#endif
