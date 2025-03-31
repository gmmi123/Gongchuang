 #ifndef __MyTest_H
 #define __MyTest_H
 //底层库



// 成员变量
typedef struct
{

} MyTestDate_Typedef;
 
// 类
typedef struct
{
	MyTestDate_Typedef Date;
	// 方法
} MyTest_Typedef;

extern MyTest_Typedef Test;
void MyTest_Task(MyTest_Typedef* object);
void MyTest_Init(MyTest_Typedef* object);
 #endif
 