#ifndef __Hardware_Task_H
#define __Hardware_Task_H

#include "main.h"
//��λ���ؼ������
void Xianweikaiguan_Task(void);
//��ѹ�������
void PowerWarnig_Task(void);

//��ȡ��������������
void GetIMUdate_Task(void);
//����Z������
void InfiniteZ_Task(void);
//��⴮������һ��������������
void CKP_CheckStart_Task(void);
//������ά����������
void two_dimensional_Task(void);
//�������ݸ�������
void SendDateCKP_Task(void);
#endif
