#include "wdg.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32H7������
//���Ź� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2019/4/21
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved		  
//********************************************************************************
//V1.1 20190421
//�����˴��ڿ��Ź���غ�����									  
////////////////////////////////////////////////////////////////////////////////// 	 


//��ʼ���������Ź�
//prer:��Ƶ��:0~7(ֻ�е�3λ��Ч!)
//rlr:�Զ���װ��ֵ,0~0XFFF.
//��Ƶ����=4*2^prer.�����ֵֻ����256!
//rlr:��װ�ؼĴ���ֵ:��11λ��Ч.
//ʱ�����(���):Tout=((4*2^prer)*rlr)/32 (ms).
void IWDG_Init(u8 prer,u16 rlr) 
{
	IWDG1->KR=0X5555;//ʹ�ܶ�IWDG->PR��IWDG->RLR��д		 										  
  	IWDG1->PR=prer;  //���÷�Ƶϵ��   
  	IWDG1->RLR=rlr;  //�Ӽ��ؼĴ��� IWDG->RLR  
	IWDG1->KR=0XAAAA;//reload											   
  	IWDG1->KR=0XCCCC;//ʹ�ܿ��Ź�	
}
//ι�������Ź�
void IWDG_Feed(void)
{
	IWDG1->KR=0XAAAA;//reload											   
}

//����WWDG������������ֵ,Ĭ��Ϊ���. 
u8 WWDG_CNT=0x7f; 
//��ʼ�����ڿ��Ź� 	
//tr   :T[6:0],������ֵ 
//wr   :W[6:0],����ֵ 
//fprer:��Ƶϵ����WDGTB��,��Χ:0~7,��ʾ2^WDGTB��Ƶ
//Fwwdg=PCLK3/(4096*2^fprer). һ��PCLK3=100Mhz
//���ʱ��=(4096*2^fprer)*(tr-0X3F)/PCLK3
//����fprer=4,tr=7f,PCLK3=100Mhz
//�����ʱ��=4096*16*64/100Mhz=41.94ms
void WWDG_Init(u8 tr,u8 wr,u8 fprer) 
{     
	RCC->APB3ENR|=1<<6; 	//ʹ��wwdgʱ�� 
	WWDG_CNT=tr&WWDG_CNT;   //��ʼ��WWDG_CNT.  
	RCC->GCR|=1<<0;			//WW1RSC=1,��λWWDG1,Ӳ������ 
	WWDG1->CFR|=fprer<<11;	//PCLK1/4096�ٳ�2^fprer 
	WWDG1->CFR&=0XFF80;      
	WWDG1->CFR|=wr;			//�趨����ֵ      
	WWDG1->CR|=WWDG_CNT; 	//�趨������ֵ 
	WWDG1->CR|=1<<7;  		//�������Ź�      
	MY_NVIC_Init(2,3,WWDG_IRQn,2);//��ռ2�������ȼ�3����2     
	WWDG1->SR=0X00;			//�����ǰ�����жϱ�־λ 
	WWDG1->CFR|=1<<9;		//ʹ����ǰ�����ж� 
} 
//������WWDG��������ֵ 
void WWDG_Set_Counter(u8 cnt) 
{ 
	WWDG1->CR=(cnt&0x7F);	//������7λ������ 
} 
//���ڿ��Ź��жϷ������ 
void WWDG_IRQHandler(void) 
{      
	static u8 led1sta=1;
	if(WWDG1->SR&0X01)		//���ж��Ƿ�����WWDG��ǰ�����ж�
	{
		WWDG1->SR=0X00;				//�����ǰ�����жϱ�־λ 
		WWDG_Set_Counter(WWDG_CNT);	//���贰�ڿ��Ź���ֵ!         
		LED1(led1sta^=1); 			//LED1��˸
	}
}







