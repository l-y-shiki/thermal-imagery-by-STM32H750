#include "my_i2c.h"
#include "./SYSTEM/delay/delay.h"

/*****************************************************************************
 * @name       :void IIC_Init(void)
 * @date       :2018-08-09 
 * @function   :Initialize IIC
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void IIC_Init(void)
{					     
		GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_GPIOH_CLK_ENABLE();           //开启GPIOB时钟
	 
	GPIO_Initure.Pin=GPIO_PIN_4; //scl PH4
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
		GPIO_Initure.Pin=GPIO_PIN_5; //sda PH5
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_OD;  //开漏输出
		GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);	 
	
 
	IIC_SCL1();
	IIC_SDA1();

}

/*****************************************************************************
 * @name       :void IIC_Start(void)
 * @date       :2018-08-09 
 * @function   :Generating IIC starting signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void IIC_Start(void)
{
//	SDA_OUT();     //sda线输出
	IIC_SDA1();	  	  
	IIC_SCL1();
	delay_us(4);
 	IIC_SDA0();//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL0();//钳住I2C总线，准备发送或接收数据 
}	

/*****************************************************************************
 * @name       :void IIC_Stop(void)
 * @date       :2018-08-09 
 * @function   :Generating IIC stop signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/  
void IIC_Stop(void)
{
//	SDA_OUT();//sda线输出
	IIC_SCL0();
  IIC_SDA0();//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL1(); 
	IIC_SDA1();//发送I2C总线结束信号
	delay_us(4);							   	
}

/*****************************************************************************
 * @name       :u8 IIC_Wait_Ack(void)
 * @date       :2018-08-09 
 * @function   :Wait for the response signal
 * @parameters :None
 * @retvalue   :0-receive response signal successfully
								1-receive response signal unsuccessfully
******************************************************************************/ 
uint8_t IIC_Wait_Ack(void)
{
	uint8_t m;
	uint8_t ucErrTime=0;
//	SDA_IN();      //SDA设置为输入  
	IIC_SDA1();delay_us(1);	   
	IIC_SCL1();delay_us(1);	 
	
	while(m)
	{
		m=IIC_SDA_READ2();
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL0();//时钟输出0 	   
	return 0;  
} 

/*****************************************************************************
 * @name       :void IIC_Ack(void)
 * @date       :2018-08-09 
 * @function   :Generate ACK response signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/ 
void IIC_Ack(void)
{
	IIC_SCL0();
//	SDA_OUT();
  IIC_SDA0();
	delay_us(2);
	IIC_SCL1();
	delay_us(2);
	IIC_SCL0();
}

/*****************************************************************************
 * @name       :void IIC_NAck(void)
 * @date       :2018-08-09 
 * @function   :Don't generate ACK response signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/ 	    
void IIC_NAck(void)
{
	IIC_SCL0();
//	SDA_OUT();
	IIC_SDA1();
	delay_us(2);
	IIC_SCL1();
	delay_us(2);
	IIC_SCL0();
}	

/*****************************************************************************
 * @name       :void IIC_Send_Byte(u8 txd)
 * @date       :2018-08-09 
 * @function   :send a byte data by IIC bus
 * @parameters :txd:Data to be sent
 * @retvalue   :None
******************************************************************************/				 				     	  
void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
//	SDA_OUT(); 	    
    IIC_SCL0();//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {
				if((txd&0x80)>>7==1)
					IIC_SDA1();
				if((txd&0x80)>>7!=1)
					IIC_SDA0();
//        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
	IIC_SCL1();
		delay_us(2); 
		IIC_SCL0();	
		delay_us(2);
    }	 
} 

/*****************************************************************************
 * @name       :u8 IIC_Read_Byte(unsigned char ack)
 * @date       :2018-08-09 
 * @function   :read a byte data by IIC bus
 * @parameters :ack:0-send nACK
									  1-send ACK
 * @retvalue   :Data to be read
******************************************************************************/		    
uint8_t IIC_Read_Byte(unsigned char ack)
{
	uint8_t m;
	unsigned char i,receive=0;
//	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL0(); 
        delay_us(2);
	IIC_SCL1();
        receive<<=1;
		m=IIC_SDA_READ2();
        if(m)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}
