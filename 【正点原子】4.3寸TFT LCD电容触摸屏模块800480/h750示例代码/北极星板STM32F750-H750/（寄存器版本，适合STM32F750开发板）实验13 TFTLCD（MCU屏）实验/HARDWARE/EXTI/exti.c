#include "exti.h"
#include "delay.h" 
#include "led.h" 
#include "key.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//外部中断 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2019/5/24
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

//外部中断0服务程序
void EXTI0_IRQHandler(void)
{ 
	static u8 led0sta=1,led1sta=1;
	delay_ms(10);	//消抖
	EXTI->PR=1<<0;  //清除LINE0上的中断标志位  
	if(WK_UP==1)	 
	{
		led1sta=!led1sta;
		led0sta=!led1sta;
		LED1(led1sta);
		LED0(led0sta); 
	}		 
}	
//外部中断2服务程序
void EXTI2_IRQHandler(void)
{
	static u8 led1sta=1;
	delay_ms(10);	//消抖
	EXTI->PR=1<<2;  //清除LINE2上的中断标志位  
	if(KEY1==0)	  
	{	 
		led1sta=!led1sta;
		LED1(led1sta);	 
	}		 
}
//外部中断3服务程序
void EXTI3_IRQHandler(void)
{
	static u8 led0sta=1,led1sta=1;
	delay_ms(10);	//消抖
	EXTI->PR=1<<3;	//清除LINE3上的中断标志位  
	if(KEY0==0)	 
	{
		led1sta=!led1sta;
		led0sta=!led0sta;
		LED1(led1sta);
		LED0(led0sta); 
	}		 
}
//外部中断10~15服务程序
void EXTI15_10_IRQHandler(void)
{
	static u8 led0sta=1;
	delay_ms(10);	//消抖
	EXTI->PR=1<<13; //清除LINE13上的中断标志位 
	if(KEY2==0)	 
	{		
		led0sta=!led0sta;
		LED0(led0sta);	 		
	}		  
}
	   
//外部中断初始化程序
//初始化PH2,PH3,PC13,PA0为中断输入.
void EXTIX_Init(void)
{
	KEY_Init(); 
	Ex_NVIC_Config(GPIO_C,13,FTIR);		//下降沿触发
	Ex_NVIC_Config(GPIO_H,2,FTIR); 		//下降沿触发
	Ex_NVIC_Config(GPIO_H,3,FTIR); 		//下降沿触发
 	Ex_NVIC_Config(GPIO_A,0,RTIR); 	 	//上升沿触发 
	MY_NVIC_Init(3,2,EXTI2_IRQn,2);		//抢占3，子优先级2，组2
	MY_NVIC_Init(2,2,EXTI3_IRQn,2);		//抢占2，子优先级2，组2	   
	MY_NVIC_Init(1,2,EXTI15_10_IRQn,2);	//抢占1，子优先级2，组2	   
	MY_NVIC_Init(0,2,EXTI0_IRQn,2);		//抢占0，子优先级2，组2	   
}












