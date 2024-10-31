#include "mpu.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//MPU��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2019/6/1
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

//����ĳ�������MPU����
//baseaddr:MPU��������Ļ�ַ(�׵�ַ)
//size:MPU��������Ĵ�С(������32�ı���,��λΪ�ֽ�)
//rnum:MPU���������,��Χ:0~7,���֧��8����������
//de:��ָֹ�����;0,����ָ�����;1,��ָֹ�����
//ap:����Ȩ��,���ʹ�ϵ����:
//0,�޷��ʣ���Ȩ&�û������ɷ��ʣ�
//1,��֧����Ȩ��д����
//2,��ֹ�û�д���ʣ���Ȩ�ɶ�д���ʣ�
//3,ȫ���ʣ���Ȩ&�û����ɷ��ʣ�
//4,�޷�Ԥ��(��ֹ����Ϊ4!!!)
//5,��֧����Ȩ������
//6,ֻ������Ȩ&�û���������д��
//���:STM32H7����ֲ�.pdf,4.6.6��,Table 91.
//sen:�Ƿ�������;0,������;1,����
//cen:�Ƿ�����cache;0,������;1,����
//ben:�Ƿ�������;0,������;1,����
//����ֵ;0,�ɹ�.
//    ����,����.
u8 MPU_Set_Protection(u32 baseaddr,u32 size,u32 rnum,u8 de,u8 ap,u8 sen,u8 cen,u8 ben)
{
	MPU_Region_InitTypeDef MPU_Initure;
	HAL_MPU_Disable();								        //����MPU֮ǰ�ȹر�MPU,��������Ժ���ʹ��MPU

	MPU_Initure.Enable=MPU_REGION_ENABLE;			        //ʹ�ܸñ������� 
	MPU_Initure.Number=rnum;			                    //���ñ�������
	MPU_Initure.BaseAddress=baseaddr;	                    //���û�ַ
	MPU_Initure.Size=size;				                    //���ñ��������С
	MPU_Initure.SubRegionDisable=0X00;                      //��ֹ������
	MPU_Initure.TypeExtField=MPU_TEX_LEVEL0;                //����������չ��Ϊlevel0
	MPU_Initure.AccessPermission=(u8)ap;		            	//���÷���Ȩ��,
	MPU_Initure.DisableExec=de;								//�Ƿ�����ָ�����
	MPU_Initure.IsShareable=sen;                            //�Ƿ�������
	MPU_Initure.IsCacheable=cen;                            //�Ƿ�����cache
	MPU_Initure.IsBufferable=ben;                           //�Ƿ�������
	HAL_MPU_ConfigRegion(&MPU_Initure);                     //����MPU
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);			        //����MPU
    return 0;
}


//������Ҫ�����Ĵ洢��
//����Բ��ִ洢�������MPU����,������ܵ��³��������쳣
//����MCU������ʾ,����ͷ�ɼ����ݳ���ȵ�����...
void MPU_Memory_Protection(void)
{		
	MPU_Set_Protection(0x20000000,MPU_REGION_SIZE_64KB,1,0,MPU_REGION_FULL_ACCESS,0,1,1);	//��������DTCM,��64K�ֽ�,����ָ�����,��ֹ����,����cache,������
	MPU_Set_Protection(0x20010000,MPU_REGION_SIZE_256KB,2,0,MPU_REGION_FULL_ACCESS,0,1,1);	//��������SRAM1/SRAM2,��256K�ֽ�,����ָ�����,��ֹ����,����cache,������
	MPU_Set_Protection(0x00000000,MPU_REGION_SIZE_16KB,3,0,MPU_REGION_FULL_ACCESS,0,1,1);	//��������ITCM,��16K�ֽ�,����ָ�����,��ֹ����,����cache,������ 

	MPU_Set_Protection(0x60000000,MPU_REGION_SIZE_64MB,4,0,MPU_REGION_FULL_ACCESS,0,0,0);	//����MCU LCD�����ڵ�FMC����,,��64M�ֽ�,����ָ�����,��ֹ����,��ֹcache,��ֹ����
	MPU_Set_Protection(0XC0000000,MPU_REGION_SIZE_64MB,5,0,MPU_REGION_FULL_ACCESS,0,1,1);	//����SDRAM����,��64M�ֽ�,����ָ�����,��ֹ����,����cache,������
	MPU_Set_Protection(0X80000000,MPU_REGION_SIZE_256MB,6,1,MPU_REGION_FULL_ACCESS,0,0,0);	//��������NAND FLASH����,��256M�ֽ�,��ָֹ�����,��ֹ����,��ֹcache,��ֹ����
}


//MemManage�������ж�
//������ж��Ժ�,���޷��ָ���������!!
void MemManage_Handler(void)
{ 
	LED1(0);							//����DS1
	printf("Mem Access Error!!\r\n"); 	//���������Ϣ
	delay_ms(1000);	
	printf("Soft Reseting...\r\n");		//��ʾ�������
	delay_ms(1000);	
	NVIC_SystemReset();					//��λ
}
