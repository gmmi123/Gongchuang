 #ifndef __MyTest_H
 #define __MyTest_H
 //�ײ��



// ��Ա����
typedef struct
{

} MyTestDate_Typedef;
 
// ��
typedef struct
{
	MyTestDate_Typedef Date;
	// ����
} MyTest_Typedef;

extern MyTest_Typedef Test;
void MyTest_Task(MyTest_Typedef* object);
void MyTest_Init(MyTest_Typedef* object);
 #endif
 