#ifndef __FDCAN_H
#define __FDCAN_H	 
#include "sys.h"	    
#include "stm32h7xx_hal_fdcan.h"	 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32H7������
//FDCAN���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2019/5/4
//�汾��V1.0 
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
 
//CAN1����RX0�ж�ʹ��
#define FDCAN1_RX0_INT_ENABLE	0			//0,��ʹ��;1,ʹ��.	

	
//��ֹ����HAL�� FDCAN������,���뱨��,����ʵ������3����
#define HAL_FDCAN_MODULE_ENABLED			//ʹ��FDCANģ��
#define assert_param(expr) 		((void)0)	//assert_param����
#define HAL_GetTick()			10			//����SysTick������ֵ,����û�õ�SysTick

										 							 				    
u8 FDCAN1_Mode_Init(u8 tnsjw,u16 tseg1,u8 tseg2,u16 tnbrp,u8 mode);	//��ʼ��FDCAN
u8 FDCAN1_Send_Msg(u8* msg,u32 len);		//�������� 
u8 FDCAN1_Receive_Msg(u8 *buf);				//��������  
#endif

















