#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32H7开发板
//OLED 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2019/4/21
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	  
 
 
//OLED模式设置
//0: 4线串行模式  （模块的BS1，BS2均接GND）
//1: 并行8080模式 （模块的BS1，BS2均接VCC）
#define OLED_MODE 	1 
		    						  
//-----------------OLED端口定义----------------  					   
#define OLED_CS(x) 		GPIO_Pin_Set(GPIOB,PIN7,x)	 
#define OLED_RST(x)  	GPIO_Pin_Set(GPIOG,PIN12,x) 
#define OLED_RS(x) 		GPIO_Pin_Set(GPIOB,PIN4,x)	 
#define OLED_WR(x) 		GPIO_Pin_Set(GPIOA,PIN4,x)	 		  
#define OLED_RD(x) 		GPIO_Pin_Set(GPIOB,PIN3,x)	 
 
//使用4线串行接口时使用 
#define OLED_SCLK(x) 	GPIO_Pin_Set(GPIOC,PIN6,x) 
#define OLED_SDIN(x) 	GPIO_Pin_Set(GPIOC,PIN7,x) 
		     
#define OLED_CMD  	0		//写命令
#define OLED_DATA 	1		//写数据
//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);		   
							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size);	 
#endif  
	 



