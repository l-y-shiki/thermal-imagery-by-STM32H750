#include "adc.h"
#include "delay.h"		 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32H7������
//ADC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2019/4/29
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
//********************************************************************************
//20190429 	V1.1
//1,�޸�Adc3_Init/Get_Adc3/Get_Adc3_Average����,֧���ڲ��¶ȴ��������ݲɼ�.
//2,����Get_Temprate����,���ڻ�ȡ�ڲ��¶ȴ������ɼ������¶�ֵ
////////////////////////////////////////////////////////////////////////////////// 


//��ʼ��ADC
//�������ǽ��Թ���ͨ��Ϊ��
//����Ĭ�Ͻ�����ADC1_CH19P
//16λת��ʱ��λ:��������+8.5��ADC����
void  Adc_Init(void)
{    
	//�ȳ�ʼ��IO��
 	RCC->AHB1ENR|=1<<5;    		//ʹ��ADC1/2ʱ�� 
	RCC->AHB4ENR|=1<<0;    		//ʹ��PORTAʱ��	  
	GPIO_Set(GPIOA,PIN5,GPIO_MODE_AIN,0,0,GPIO_PUPD_NONE);//PA5,ģ������,����������   

	RCC->AHB1RSTR|=1<<5;   		//ADC1/2��λ
	RCC->AHB1RSTR&=~(1<<5);		//��λ����	
	RCC->D3CCIPR&=~(3<<16);		//ADCSEL[1:0]����
	RCC->D3CCIPR|=2<<16;		//ADCSEL[1:0]=2,per_ck��ΪADCʱ��Դ,Ĭ��ѡ��hsi_ker_ck��Ϊper_ck,Ƶ��:64Mhz
	ADC12_COMMON->CCR|=1<<18;	//PRESC[3:0]=1,����ʱ��2��Ƶ,��adc_ker_ck=per_ck/2=64/2=32Mhz(���ܳ���36Mhz)
	
	ADC1->CR=0;   				//CR�Ĵ�������,DEEPPWD����,�����˯�߻���.
	ADC1->CR|=1<<28;  			//ADVREGEN=1,ʹ��ADC��ѹ��
	delay_ms(10);				//�ȴ���ѹ���������,Լ10us,������ʱ��һ��,û��ϵ.
	ADC1->CR|=1<<8;  			//BOOST=1,ADC������boostģʽ(ADCƵ�ʴ���20M��ʱ��,��������boostλ)	
	ADC1->CFGR&=~(1<<13);		//CONT=0,����ת��ģʽ
	ADC1->CFGR|=1<<12;			//OVRMOD=1,��дģʽ(DR�Ĵ����ɱ���д)	
	ADC1->CFGR&=~(3<<10);		//EXTEN[1:0]=0,�������
	ADC1->CFGR&=~(7<<2);		//RES[2:0]λ����
	ADC1->CFGR|=0<<2;			//RES[2:0]=0,16λ�ֱ���(0,16λ;1,14λ;2,12λ;3,10λ;4,8λ.)

	ADC1->CFGR2&=~((u32)15<<28);//LSHIFT[3:0]=0,������,�����Ҷ���.
	ADC1->CFGR2&=~((u32)0X3FF<<16);//OSR[9:0]=0,��ʹ�ù�����
	
	
	ADC1->CR&=~((u32)1<<30);	//ADCALDIF=0,У׼����ת��ͨ��
	ADC1->CR|=1<<16;			//ADCALLIN=1,����У׼
	ADC1->CR|=(u32)1<<31;		//����У׼
	while(ADC1->CR&((u32)1<<31));//�ȴ�У׼���
	 
	ADC1->SQR1&=~(0XF<<0);		//L[3:0]����
	ADC1->SQR1|=0<<0;     		//L[3:0]=0,1��ת���ڹ��������� Ҳ����ֻת����������1 
	
	//����ͨ��19�Ĳ���ʱ��
	ADC1->SMPR2&=~(7<<(3*9));	//ͨ��19����ʱ�����	  
 	ADC1->SMPR2|=7<<(3*9); 		//ͨ��19 810.5������,��߲���ʱ�������߾�ȷ��		
 	ADC1->CR|=1<<0;	   			//����ADת����	  
}				  
//���ADCֵ
//ch:ͨ��ֵ 0~19
//����ֵ:ת�����
u16 Get_Adc(u8 ch)   
{
	ADC1->PCSEL|=1<<ch;			//ADCת��ͨ��Ԥѡ�� 
	//����ת������	  		 
	ADC1->SQR1&=~(0X1F<<6*1);	//��������1ͨ������
	ADC1->SQR1|=ch<<6*1;		//���ù�������1��ת��ͨ��Ϊch 
	ADC1->CR|=1<<2;       		//��������ת��ͨ�� 
	while(!(ADC1->ISR&1<<2));	//�ȴ�ת������	 	   
	return ADC1->DR;			//����adcֵ	
}
//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
}  


//��ʼ��ADC3 
//����Ĭ�Ͻ�����ADC3_CH18P(�ڲ��¶ȴ���������ͨ��)
//16λת��ʱ��λ:��������+8.5��ADC����
void  Adc3_Init(void)
{     
 	RCC->AHB4ENR|=1<<24;    	//ʹ��ADC3ʱ��   

	RCC->AHB4RSTR|=1<<24;   	//ADC3��λ
	RCC->AHB4RSTR&=~(1<<24);	//��λ����	
	RCC->D3CCIPR&=~(3<<16);		//ADCSEL[1:0]����
	RCC->D3CCIPR|=2<<16;		//ADCSEL[1:0]=2,per_ck��ΪADCʱ��Դ,Ĭ��ѡ��hsi_ker_ck��Ϊper_ck,Ƶ��:64Mhz
	ADC3_COMMON->CCR|=1<<18;	//PRESC[3:0]=1,����ʱ��2��Ƶ,��adc_ker_ck=per_ck/2=64/2=32Mhz(���ܳ���36Mhz)
	ADC3_COMMON->CCR|=1<<23;	//VSENSEEN=1,ʹ���ڲ��¶ȴ�����ͨ��
	
	ADC3->CR=0;   				//CR�Ĵ�������,DEEPPWD����,�����˯�߻���.
	ADC3->CR|=1<<28;  			//ADVREGEN=1,ʹ��ADC��ѹ��
	delay_ms(10);				//�ȴ���ѹ���������,Լ10us,������ʱ��һ��,û��ϵ.
	ADC3->CR|=1<<8;  			//BOOST=1,ADC������boostģʽ(ADCƵ�ʴ���20M��ʱ��,��������boostλ)	
	ADC3->CFGR&=~(1<<13);		//CONT=0,����ת��ģʽ
	ADC3->CFGR|=1<<12;			//OVRMOD=1,��дģʽ(DR�Ĵ����ɱ���д)	
	ADC3->CFGR&=~(3<<10);		//EXTEN[1:0]=0,�������
	ADC3->CFGR&=~(7<<2);		//RES[2:0]λ����
	ADC3->CFGR|=0<<2;			//RES[2:0]=0,16λ�ֱ���(0,16λ;1,14λ;2,12λ;3,10λ;4,8λ.)

	ADC3->CFGR2&=~((u32)15<<28);//LSHIFT[3:0]=0,������,�����Ҷ���.
	ADC3->CFGR2&=~((u32)0X3FF<<16);//OSR[9:0]=0,��ʹ�ù�����
	
	
	ADC3->CR&=~((u32)1<<30);	//ADCALDIF=0,У׼����ת��ͨ��
	ADC3->CR|=1<<16;			//ADCALLIN=1,����У׼
	ADC3->CR|=(u32)1<<31;		//����У׼
	while(ADC3->CR&((u32)1<<31));//�ȴ�У׼���
	 
	ADC3->SQR1&=~(0XF<<0);		//L[3:0]����
	ADC3->SQR1|=0<<0;     		//L[3:0]=0,1��ת���ڹ��������� Ҳ����ֻת����������1 
	
	//����ͨ��18�Ĳ���ʱ��
	ADC3->SMPR2&=~(7<<(3*8));	//ͨ��18����ʱ�����	  
 	ADC3->SMPR2|=7<<(3*8); 		//ͨ��18 810.5������,��߲���ʱ�������߾�ȷ��		
 	ADC3->CR|=1<<0;	   			//����ADת����	  
}	
				  
//���ADC3ĳ��ͨ����ֵ
//ch:ͨ��ֵ 0~19
//����ֵ:ת�����
u16 Get_Adc3(u8 ch)   
{
	ADC3->PCSEL|=1<<ch;			//ADCת��ͨ��Ԥѡ�� 
	//����ת������	  		 
	ADC3->SQR1&=~(0X1F<<6*1);	//��������1ͨ������
	ADC3->SQR1|=ch<<6*1;		//���ù�������1��ת��ͨ��Ϊch 
	ADC3->CR|=1<<2;       		//��������ת��ͨ�� 
	while(!(ADC3->ISR&1<<2));	//�ȴ�ת������	 	   
	return ADC3->DR;			//����adcֵ	
}

//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
u16 Get_Adc3_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc3(ch);
		delay_ms(5);
	}
	return temp_val/times;
}  

//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��,��λ:��.)
short Get_Temprate(void)
{
	u32 adcx;
	short result;
 	double temperate;
    float temp=0;
    u16 ts_cal1,ts_cal2;
    ts_cal1=*(vu16*)(0X1FF1E820);					//��ȡTS_CAL1
    ts_cal2=*(vu16*)(0X1FF1E840);					//��ȡTS_CAL2
    temp=(float)((110.0-30.0)/(ts_cal2-ts_cal1));	//��ȡ��������
	adcx=Get_Adc3_Average(ADC3_TEMPSENSOR,10);		//��ȡ�ڲ��¶ȴ�����ͨ��,10��ȡƽ��
    temperate=(float)(temp*(adcx-ts_cal1)+30);		//�����¶�
	result=temperate*=100;							//����100��.
	return result;

}











