#include "sys.h" 
#include "usart.h" 
#include "delay.h" 
#include "led.h" 
#include "mpu.h"    
#include "lcd.h" 
//ALIENTEK 北极星STM32H750/F750开发板 实验13
//TFTLCD（MCU屏）实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司

int main(void)
{  
 	u8 x=0,led0sta=1;
	u8 lcd_id[12];	
	Stm32_Clock_Init(432,25,2,9);//设置时钟,216Mhz
    delay_init(216);		//延时初始化  
	uart_init(108,115200);	//初始化串口波特率为115200
	LED_Init();		  		//初始化与LED连接的硬件接口
	MPU_Memory_Protection();//保护相关存储区域
	LCD_Init();				//初始化LCD
	POINT_COLOR=RED; 
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//将LCD ID打印到lcd_id数组。				 	
  	while(1) 
	{		 
		switch(x)
		{
			case 0:LCD_Clear(WHITE);break;
			case 1:LCD_Clear(BLACK);break;
			case 2:LCD_Clear(BLUE);break;
			case 3:LCD_Clear(RED);break;
			case 4:LCD_Clear(MAGENTA);break;
			case 5:LCD_Clear(GREEN);break;
			case 6:LCD_Clear(CYAN);break; 
			case 7:LCD_Clear(YELLOW);break;
			case 8:LCD_Clear(BRRED);break;
			case 9:LCD_Clear(GRAY);break;
			case 10:LCD_Clear(LGRAY);break;
			case 11:LCD_Clear(BROWN);break;
		}
		POINT_COLOR=RED;	  
		LCD_ShowString(10,40,240,32,32,"POLARIS STM32"); 	
		LCD_ShowString(10,80,240,24,24,"TFTLCD TEST");
		LCD_ShowString(10,110,240,16,16,"ATOM@ALIENTEK");
 		LCD_ShowString(10,130,240,16,16,lcd_id);		//显示LCD ID	      					 
		LCD_ShowString(10,150,240,12,12,"2019/5/24");	      					 
	    x++;
		if(x==12)x=0;
		LED0(led0sta^=1);	//LED0闪烁	 
		delay_ms(1000);	
	} 
}









