#ifndef __MyTrace_H
#define __MyTrace_H


 //底层库



// 成员变量
typedef struct
{

} MyTraceDate_Typedef;
 
// 类
typedef struct
{
	MyTraceDate_Typedef Date;
	// 方法
} MyTrace_Typedef;

extern MyTrace_Typedef Test;
void MyTrace_Task(MyTrace_Typedef* object);
void MyTrace_Init(MyTrace_Typedef* object);

#endif
