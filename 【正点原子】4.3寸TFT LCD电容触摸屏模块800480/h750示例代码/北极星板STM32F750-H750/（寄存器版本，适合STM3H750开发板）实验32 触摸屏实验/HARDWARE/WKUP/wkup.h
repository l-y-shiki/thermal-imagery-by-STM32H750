#ifndef __WKUP_H
#define __WKUP_H	 
#include "sys.h"																   
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32H7������
//�������� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2019/4/28
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
	
	
#define WKUP_KD GPIO_Pin_Get(GPIOA,PIN0)		//PA0 ����Ƿ��ⲿWK_UP��������
	 
u8 Check_WKUP(void);  			//���WKUP�ŵ��ź�
void WKUP_Init(void); 			//PA0 WKUP���ѳ�ʼ��
void Sys_Enter_Standby(void);	//ϵͳ�������ģʽ
#endif


