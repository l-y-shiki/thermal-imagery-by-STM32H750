#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//按键输入驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2019/5/24
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

#define KEY0 		GPIO_Pin_Get(GPIOH,PIN3)   	//PH3
#define KEY1 		GPIO_Pin_Get(GPIOH,PIN2)	//PH2 
#define KEY2 		GPIO_Pin_Get(GPIOC,PIN13)	//PC13
#define WK_UP 		GPIO_Pin_Get(GPIOA,PIN0)	//PA0 

#define KEY0_PRES 	1	//KEY0按下
#define KEY1_PRES	2	//KEY1按下
#define KEY2_PRES	3	//KEY2按下
#define WKUP_PRES   4	//KEY_UP按下(即WK_UP)

void KEY_Init(void);	//IO初始化
u8 KEY_Scan(u8);  		//按键扫描函数					    
#endif
