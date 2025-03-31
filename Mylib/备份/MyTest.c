#include "MyTest.h"
MyTest_Typedef Test;

void MyTest_ControlTest(MyTestDate_Typedef* object)
{
}



void MyTest_Init(MyTest_Typedef* object)
{

}

void MyTest_Task(MyTest_Typedef* object)
{

	//¿ØÖÆ
	MyTest_ControlTest(&object->Date);
}