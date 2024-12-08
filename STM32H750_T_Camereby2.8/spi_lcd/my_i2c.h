


#ifndef __MY_I2C_H
#define __MY_I2C_H
#include "./SYSTEM/sys/sys.h"
  	   		   
////IO方向设置
//#define SDA_IN()  {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=8<<12;}
//#define SDA_OUT() {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=3<<12;}




////IO操作函数	 
//#define IIC_SCL    PCout(12) //SCL
//#define IIC_SDA    PCout(11) //SDA	 
//#define READ_SDA   PCin(11)  //输入SDA 


//IO方向设置
//#define SDA_IN()  {GPIOC->MODER&=~(3<<(12*2));GPIOC->MODER|=0<<12*2;}		//PB12设置输入
//#define SDA_OUT() {GPIOC->MODER&=~(3<<(12*2));GPIOC->MODER|=1<<12*2;}		//PB12设置输出

//IO操作函数	
/*********************************** IIC_SCL ******************************/

#define IIC_SCL0()            HAL_GPIO_WritePin(GPIOH, GPIO_PIN_4,GPIO_PIN_RESET);
#define IIC_SCL1()            HAL_GPIO_WritePin(GPIOH, GPIO_PIN_4,GPIO_PIN_SET);

/*********************************** IIC_SDA ******************************/

#define IIC_SDA0()              HAL_GPIO_WritePin(GPIOH, GPIO_PIN_5,GPIO_PIN_RESET);
#define IIC_SDA1()             HAL_GPIO_WritePin(GPIOH, GPIO_PIN_5,GPIO_PIN_SET);
#define IIC_SDA_READ2()             HAL_GPIO_ReadPin(GPIOH, GPIO_PIN_5);

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
uint8_t IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	  
#endif
