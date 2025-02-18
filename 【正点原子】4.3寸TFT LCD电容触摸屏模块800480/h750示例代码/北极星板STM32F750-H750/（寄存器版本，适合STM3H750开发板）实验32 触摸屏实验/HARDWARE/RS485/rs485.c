#include "sys.h"		    
#include "rs485.h"	 
#include "delay.h"
#include "pcf8574.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32H7开发板
//RS485 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2019/5/4
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 


#if EN_USART2_RX   		//如果使能了接收   	  
//接收缓存区 	
u8 RS485_RX_BUF[64];  	//接收缓冲,最大64个字节.
//接收到的数据长度
u8 RS485_RX_CNT=0;   
void USART2_IRQHandler(void)
{
	u8 res;	    
	if(USART2->ISR&(1<<5))//接收到数据
	{	 
		res=USART2->RDR; 			 
		if(RS485_RX_CNT<64)
		{
			RS485_RX_BUF[RS485_RX_CNT]=res;		//记录接收到的值
			RS485_RX_CNT++;						//接收数据增加1 
		} 
	}  											 
} 
#endif										 
//初始化IO 串口2
//pclk1:PCLK1时钟频率(Mhz),APB1一般为100Mhz
//bound:波特率	  
void RS485_Init(u32 pclk1,u32 bound)
{  	 
	u32 temp;   
	temp=(pclk1*1000000+bound/2)/bound;	//得到USARTDIV@OVER8=0,采用四舍五入计算
	PCF8574_Init();				//初始化PCF8574,用于控制RE脚	
	RCC->AHB4ENR|=1<<0;   		//使能PORTA口时钟   
	GPIO_Set(GPIOA,PIN2|PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_MID,GPIO_PUPD_PU);	//PA2,PA3,复用功能,上拉 
 	GPIO_AF_Set(GPIOA,2,7);		//PA2,AF7
	GPIO_AF_Set(GPIOA,3,7);		//PA3,AF7  	   
 
	RCC->APB1LENR|=1<<17;  		//使能串口2时钟  
	RCC->APB1LRSTR|=1<<17;   	//复位串口2
	RCC->APB1LRSTR&=~(1<<17);	//停止复位	   	   
	//波特率设置
 	USART2->BRR=temp; 			//波特率设置	
	USART2->CR1=0;		 		//清零CR1寄存器
	USART2->CR1|=0<<28;	 		//设置M1=0
	USART2->CR1|=0<<12;	 		//设置M0=0&M1=0,选择8位字长 
	USART2->CR1|=0<<15; 		//设置OVER8=0,16倍过采样 
	USART2->CR1|=1<<3;  		//串口发送使能 
#if EN_USART2_RX		  		//如果使能了接收
	//使能接收中断 
	USART2->CR1|=1<<2;  		//串口接收使能
	USART2->CR1|=1<<5;    		//接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3,3,USART2_IRQn,2);//组2，最低优先级 
#endif
	USART2->CR1|=1<<0;  		//串口使能
	RS485_TX_Set(0);			//默认设置为接收模式	
}

//RS485发送len个字节.
//buf:发送区首地址
//len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
void RS485_Send_Data(u8 *buf,u8 len)
{
	u8 t;
	RS485_TX_Set(1);			//设置为发送模式
  	for(t=0;t<len;t++)			//循环发送数据
	{
		while((USART2->ISR&0X40)==0);//等待发送结束		  
		USART2->TDR=buf[t];
	}	 
	while((USART2->ISR&0X40)==0);//等待发送结束	
	RS485_RX_CNT=0;	  
	RS485_TX_Set(0);			//设置为接收模式	
}
//RS485查询接收到的数据
//buf:接收缓存首地址
//len:读到的数据长度
void RS485_Receive_Data(u8 *buf,u8 *len)
{
	u8 rxlen=RS485_RX_CNT;
	u8 i=0;
	*len=0;				//默认为0
	delay_ms(10);		//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束
	if(rxlen==RS485_RX_CNT&&rxlen)//接收到了数据,且接收完成了
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_RX_BUF[i];	
		}		
		*len=RS485_RX_CNT;	//记录本次数据长度
		RS485_RX_CNT=0;		//清零
	}
} 
//RS485模式控制.
//en:0,接收;1,发送.
void RS485_TX_Set(u8 en)
{
	PCF8574_WriteBit(RS485_RE_IO,en);
}




