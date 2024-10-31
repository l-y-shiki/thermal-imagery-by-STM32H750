#include "dac.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32H7������
//DAC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2019/4/29
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved 
////////////////////////////////////////////////////////////////////////////////// 	
 
//DACͨ��1�����ʼ��
void Dac1_Init(void)
{   	
	RCC->APB1LENR|=1<<29;	//ʹ��DACʱ��	   
	RCC->AHB4ENR|=1<<0;  	//ʹ��PORTAʱ��	  
	GPIO_Set(GPIOA,PIN4,GPIO_MODE_AIN,0,0,GPIO_PUPD_PU);//PA4,ģ������,����   
 
	DAC1->CCR=0;
	DAC1->MCR&=~(7<<0);		//MODE1[2:0]����
	DAC1->MCR|=0;			//MODE1[2:0]=0,DACͨ��1���ӵ��ⲿ����,��ʹ��BUFFER
	
	DAC1->CR=0;				//DR�Ĵ�������
	DAC1->CR|=0<<1;			//TEN1=0,��ʹ�ô������� 
	DAC1->CR|=0<<2;			//TSEL[3:0]=0,�������
	DAC1->CR|=0<<6;			//WAVE1[1:0]=0,��ʹ�ò��η���
 	DAC1->CR|=0<<12;		//DMAEN1=0,DAC1 DMA��ʹ��    
	DAC1->CR|=0<<14;		//CEN1=0,DAC1��������ͨģʽ 

	DAC1->CR|=1<<0;			//ʹ��DAC1 
	DAC1->DHR12R1=0;
}
//����ͨ��1�����ѹ
//vol:0~3300,����0~3.3V
void Dac1_Set_Vol(u16 vol)
{
	double temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
	DAC1->DHR12R1=temp;
}





















































