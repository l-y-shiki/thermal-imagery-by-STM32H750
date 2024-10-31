#include "wkup.h"
#include "led.h"
#include "delay.h"	
#include "rtc.h"
#include "usart.h"																	   
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
			 
//ϵͳ�������ģʽ
void Sys_Enter_Standby(void)
{		
	u32 tempreg;			//��ʱ�洢�Ĵ���ֵ�� 
 	while(WKUP_KD);			//�ȴ�WK_UP�����ɿ�(����RTC�ж�ʱ,�����WK_UP�ɿ��ٽ������)
	delay_ms(100);
	EXTI_D1->PR1=1<<0;  	//���LINE0�ϵ��жϱ�־λ    
	//STM32F4/F7/H7,��������RTC����жϺ�,�����ȹر�RTC�ж�,�����жϱ�־λ,Ȼ����������
	//RTC�ж�,�ٽ������ģʽ�ſ�����������,�����������. 
	PWR->CR1|=1<<8;			//������дʹ��
	//�ر�RTC�Ĵ���д����
	RTC->WPR=0xCA;
	RTC->WPR=0x53; 
	tempreg=RTC->CR&(0X0F<<12);//��¼ԭ����RTC�ж�����
	RTC->CR&=~(0XF<<12);	//�ر�RTC�����ж�
	RTC->ISR&=~(0X3F<<8);	//�������RTC�жϱ�־. 
	RTC->CR|=tempreg;		//��������RTC�ж� 
	RTC->WPR=0xFF;			//ʹ��RTC�Ĵ���д����  
	Sys_Standby();			//�������ģʽ
}
//���WKUP�ŵ��ź�
//����ֵ1:��������3s����
//      0:����Ĵ���	
u8 Check_WKUP(void) 
{
	u8 t=0;
	u8 tx=0;	//��¼�ɿ��Ĵ���
	LED0(0); 	//����DS0 
	while(1)
	{
		if(WKUP_KD)//�Ѿ�������
		{
			t++;
			tx=0;
		}else 
		{
			tx++; 
			if(tx>3)	//����90ms��û��WKUP�ź�
			{
				LED0(1); 
				return 0;//����İ���,���´�������
			}
		}
		delay_ms(30);
		if(t>=100)		//���³���3����
		{
			LED0(1);;	//�ر�DS0 
			return 1;	//����3s������
		}
	}
}  
//�ж�,��⵽PA0�ŵ�һ��������.	  
//�ж���0���ϵ��жϼ��
void EXTI0_IRQHandler(void)
{ 		  
	EXTI_D1->PR1=1<<0;  	//���LINE0�ϵ��жϱ�־λ   
	if(Check_WKUP())//�ػ�?
	{		  
		Sys_Enter_Standby();  
	}
} 
//PA0 WKUP���ѳ�ʼ��
void WKUP_Init(void)
{	  		  
	RCC->AHB1ENR|=1<<0;		//ʹ��PORTAʱ��	   
	GPIO_Set(GPIOA,PIN0,GPIO_MODE_IN,0,0,GPIO_PUPD_PD);	//PA0���� 
	//(����Ƿ���������)��    		 
    if(Check_WKUP()==0)
	{ 
 		Sys_Enter_Standby();//���ǿ���,�������ģʽ  
	}
	Ex_NVIC_Config(GPIO_A,0,RTIR);	//PA0�����ش���	    										   		   
	MY_NVIC_Init(2,2,EXTI0_IRQn,2);	//��ռ2�������ȼ�2����2
}
















