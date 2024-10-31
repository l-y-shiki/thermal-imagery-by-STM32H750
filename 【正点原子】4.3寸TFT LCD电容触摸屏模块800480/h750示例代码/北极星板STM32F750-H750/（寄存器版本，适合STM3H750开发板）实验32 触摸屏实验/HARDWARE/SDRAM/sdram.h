#ifndef _SDRAM_H
#define _SDRAM_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32H7������
//SDRAM ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2019/4/21
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
//********************************************************************************
//�޸�˵��
//20190524 V1.1
//�޸�CASΪ2,���SDRAM��������
//////////////////////////////////////////////////////////////////////////////////  


#define Bank5_SDRAM_ADDR    ((u32)(0XC0000000)) 			//SDRAM��ʼ��ַ
 
u8 SDRAM_Send_Cmd(u8 bankx,u8 cmd,u8 refresh,u16 regval); 
void SDRAM_Init(void);
void FMC_SDRAM_WriteBuffer(u8 *pBuffer,u32 WriteAddr,u32 n);
void FMC_SDRAM_ReadBuffer(u8 *pBuffer,u32 ReadAddr,u32 n);
#endif



































