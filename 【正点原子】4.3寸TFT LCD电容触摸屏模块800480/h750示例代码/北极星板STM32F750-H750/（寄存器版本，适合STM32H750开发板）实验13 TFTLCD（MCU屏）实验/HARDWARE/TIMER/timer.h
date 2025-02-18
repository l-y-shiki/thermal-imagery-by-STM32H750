#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32H7开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2019/4/21
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
//********************************************************************************
//修改说明
//V1.1 20190421
//新增TIM3_PWM_Init函数,用于PWM输出	
//V1.2 20190421
//新增TIM5_CH1_Cap_Init函数,用于输入捕获	
////////////////////////////////////////////////////////////////////////////////// 	 

//通过改变TIM3->CCR4的值来改变占空比，从而控制LED0的亮度
#define LED0_PWM_VAL TIM3->CCR4    

void TIM3_Int_Init(u16 arr,u16 psc); 
void TIM3_PWM_Init(u32 arr,u32 psc);
void TIM5_CH1_Cap_Init(u32 arr,u16 psc);
#endif





















