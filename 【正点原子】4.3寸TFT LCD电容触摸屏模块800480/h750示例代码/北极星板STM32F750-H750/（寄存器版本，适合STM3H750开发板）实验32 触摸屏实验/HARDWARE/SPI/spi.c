#include "spi.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32H7������
//SPI ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2019/5/1
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
//********************************************************************************
//����˵��
//��
////////////////////////////////////////////////////////////////////////////////// 	 


//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ 						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI2�ĳ�ʼ��
void SPI2_Init(void)
{	 
	u32 tempreg=0;
	RCC->AHB4ENR|=1<<1;			//ʹ��PORTBʱ�� 
	RCC->APB1LENR|=1<<14;		//SPI2ʱ��ʹ�� 
	GPIO_Set(GPIOB,7<<13,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_FAST,GPIO_PUPD_PU);	//PB13~15���ù������	
  	GPIO_AF_Set(GPIOB,13,5);	//PB13,AF5
 	GPIO_AF_Set(GPIOB,14,5);	//PB14,AF5
 	GPIO_AF_Set(GPIOB,15,5);	//PB15,AF5 
	//����SPI��ʱ��Դ
	RCC->D2CCIP1R&=~(7<<12);	//SPI123SEL[2:0]=0,���ԭ��������
	RCC->D2CCIP1R|=0<<12;		//SPI123SEL[2:0]=1,ѡ��pll1_q_ck��ΪSPI1/2/3��ʱ��Դ,һ��Ϊ200Mhz
								//��:spi_ker_ck=200Mhz 
	//����ֻ���SPI�ڳ�ʼ��
	RCC->APB1LRSTR|=1<<14;		//��λSPI2
	RCC->APB1LRSTR&=~(1<<14);	//ֹͣ��λSPI2
	
	SPI2->CR1|=1<<12;			//SSI=1,�����ڲ�SS�ź�Ϊ�ߵ�ƽ	
	SPI2->CFG1=7<<28;			//MBR[2:0]=7,����spi_ker_ckΪ256��Ƶ.
	SPI2->CFG1|=7<<0;			//DSIZE[4:0]=7,����SPI֡��ʽΪ8λ,���ֽڴ���
	tempreg=(u32)1<<31;			//AFCNTR=1,SPI���ֶ�IO�ڵĿ���
	tempreg|=0<<29;				//SSOE=0,��ֹӲ��NSS���
	tempreg|=1<<26;				//SSM=1,�������NSS��
	tempreg|=1<<25;				//CPOL=1,����״̬��,SCKΪ�ߵ�ƽ
	tempreg|=1<<24;				//CPHA=1,���ݲ����ӵ�2��ʱ����ؿ�ʼ
	tempreg|=0<<23;				//LSBFRST=0,MSB�ȴ���
	tempreg|=1<<22;				//MASTER=1,����ģʽ
	tempreg|=0<<19;				//SP[2:0]=0,Ħ��������ʽ
	tempreg|=0<<17;				//COMM[1:0]=0,ȫ˫��ͨ��
	SPI2->CFG2=tempreg;			//����CFG2�Ĵ���	
	SPI2->I2SCFGR&=~(1<<0);		//ѡ��SPIģʽ
	SPI2->CR1|=1<<0;			//SPE=1,ʹ��SPI2
 
	SPI2_ReadWriteByte(0xff);	//��������		 
}   

//SPI2�ٶ����ú���
//SpeedSet:0~7
//SPI�ٶ�=spi_ker_ck/2^(SpeedSet+1)
//spi_ker_ck����ѡ������pll1_q_ck,Ϊ200Mhz
void SPI2_SetSpeed(u8 SpeedSet)
{
	SpeedSet&=0X07;					//���Ʒ�Χ
 	SPI2->CR1&=~(1<<0); 			//SPE=0,SPI�豸ʧ��
	SPI2->CFG1&=~(7<<28); 			//MBR[2:0]=0,���ԭ���ķ�Ƶ����
	SPI2->CFG1|=(u32)SpeedSet<<28;	//MBR[2:0]=SpeedSet,����SPI2�ٶ�  
	SPI2->CR1|=1<<0; 				//SPE=1,SPI�豸ʹ��	 	  
} 

//SPI2 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI2_ReadWriteByte(u8 TxData)
{			 	   
	u8 RxData=0;	
	SPI2->CR1|=1<<0;				//SPE=1,ʹ��SPI2
	SPI2->CR1|=1<<9;  				//CSTART=1,��������
	
	while((SPI2->SR&1<<1)==0);		//�ȴ��������� 
	*(vu8 *)&SPI2->TXDR=TxData;		//����һ��byte,�Դ��䳤�ȷ���TXDR�Ĵ���   
	while((SPI2->SR&1<<0)==0);		//�ȴ�������һ��byte  
	RxData=*(vu8 *)&SPI2->RXDR;		//����һ��byte,�Դ��䳤�ȷ���RXDR�Ĵ���	
	
	SPI2->IFCR|=3<<3;				//EOTC��TXTFC��1,���EOT��TXTFCλ 
	SPI2->CR1&=~(1<<0);				//SPE=0,�ر�SPI2,��ִ��״̬����λ/FIFO���õȲ���
	return RxData;					//�����յ�������
}






