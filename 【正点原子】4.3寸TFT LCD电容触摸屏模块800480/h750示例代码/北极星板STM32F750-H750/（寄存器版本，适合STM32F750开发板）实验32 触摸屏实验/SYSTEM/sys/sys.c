#include "sys.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//ϵͳʱ�ӳ�ʼ��	
//����ʱ������/�жϹ���/GPIO���õ�
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2019/5/23
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved
//********************************************************************************
//�޸�˵��
//��
//////////////////////////////////////////////////////////////////////////////////  


//����������ƫ�Ƶ�ַ
//NVIC_VectTab:��ַ
//Offset:ƫ����		 
void MY_NVIC_SetVectorTable(u32 NVIC_VectTab,u32 Offset)	 
{ 	   	  
	SCB->VTOR=NVIC_VectTab|(Offset&(u32)0xFFFFFE00);//����NVIC��������ƫ�ƼĴ���,VTOR��9λ����,��[8:0]������
}
//����NVIC����
//NVIC_Group:NVIC���� 0~4 �ܹ�5�� 		   
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group)	 
{ 
	u32 temp,temp1;	  
	temp1=(~NVIC_Group)&0x07;//ȡ����λ
	temp1<<=8;
	temp=SCB->AIRCR;  //��ȡ��ǰ������
	temp&=0X0000F8FF; //�����ǰ����
	temp|=0X05FA0000; //д��Կ��
	temp|=temp1;	   
	SCB->AIRCR=temp;  //���÷���	    	  				   
}
//����NVIC 
//NVIC_PreemptionPriority:��ռ���ȼ�
//NVIC_SubPriority       :��Ӧ���ȼ�
//NVIC_Channel           :�жϱ��
//NVIC_Group             :�жϷ��� 0~4
//ע�����ȼ����ܳ����趨����ķ�Χ!����������벻���Ĵ���
//�黮��:
//��0:0λ��ռ���ȼ�,4λ��Ӧ���ȼ�
//��1:1λ��ռ���ȼ�,3λ��Ӧ���ȼ�
//��2:2λ��ռ���ȼ�,2λ��Ӧ���ȼ�
//��3:3λ��ռ���ȼ�,1λ��Ӧ���ȼ�
//��4:4λ��ռ���ȼ�,0λ��Ӧ���ȼ�
//NVIC_SubPriority��NVIC_PreemptionPriority��ԭ����,��ֵԽС,Խ����	   
void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group)	 
{ 
	u32 temp;	  
	MY_NVIC_PriorityGroupConfig(NVIC_Group);//���÷���
	temp=NVIC_PreemptionPriority<<(4-NVIC_Group);	  
	temp|=NVIC_SubPriority&(0x0f>>NVIC_Group);
	temp&=0xf;								//ȡ����λ
	NVIC->ISER[NVIC_Channel/32]|=1<<NVIC_Channel%32;//ʹ���ж�λ(Ҫ����Ļ�,����ICER��ӦλΪ1����)
	NVIC->IP[NVIC_Channel]|=temp<<4;				//������Ӧ���ȼ����������ȼ�   	    	  				   
} 
//�ⲿ�ж����ú���
//ֻ���GPIOA~I;������PVD,RTC,USB_OTG,USB_HS,��̫�����ѵ�
//����:
//GPIOx:0~8,����GPIOA~I
//BITx:��Ҫʹ�ܵ�λ;
//TRIM:����ģʽ,1,������;2,�Ͻ���;3�������ƽ����
//�ú���һ��ֻ������1��IO��,���IO��,���ε���
//�ú������Զ�������Ӧ�ж�,�Լ�������   	    
void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM) 
{ 
	u8 EXTOFFSET=(BITx%4)*4;  
	RCC->APB2ENR|=1<<14;  						//ʹ��SYSCFGʱ��  
	SYSCFG->EXTICR[BITx/4]&=~(0x000F<<EXTOFFSET);//���ԭ�����ã�����
	SYSCFG->EXTICR[BITx/4]|=GPIOx<<EXTOFFSET;	//EXTI.BITxӳ�䵽GPIOx.BITx 
	//�Զ�����
	EXTI->IMR|=1<<BITx;					//����line BITx�ϵ��ж�(���Ҫ��ֹ�жϣ��򷴲�������)
	if(TRIM&0x01)EXTI->FTSR|=1<<BITx;	//line BITx���¼��½��ش���
	if(TRIM&0x02)EXTI->RTSR|=1<<BITx;	//line BITx���¼��������ش���
} 	
//GPIO��������
//GPIOx:GPIOA~GPIOI.
//BITx:0~15,����IO���ű��.
//AFx:0~15,����AF0~AF15.
//AF0~15�������(��������г����õ�,��ϸ�����429/746�����ֲ�,Table 12):
//AF0:MCO/SWD/SWCLK/RTC   		AF1:TIM1/TIM2;            		AF2:TIM3~5;               		AF3:TIM8~11
//AF4:I2C1~I2C4;          		AF5:SPI1~SPI6;            		AF6:SPI3/SAI1;            	 	AF7:SPI2/3/USART1~3/UART5/SPDIFRX;
//AF8:USART4~8/SPDIFRX/SAI2; 	AF9;CAN1~2/TIM12~14/LCD/QSPI; 	AF10:USB_OTG/USB_HS/SAI2/QSPI  	AF11:ETH
//AF12:FMC/SDMMC/OTG/HS   		AF13:DCIM                 		AF14:LCD;                  		AF15:EVENTOUT
void GPIO_AF_Set(GPIO_TypeDef* GPIOx,u8 BITx,u8 AFx)
{  
	GPIOx->AFR[BITx>>3]&=~(0X0F<<((BITx&0X07)*4));
	GPIOx->AFR[BITx>>3]|=(u32)AFx<<((BITx&0X07)*4);
}   
//GPIOͨ������ 
//GPIOx:GPIOA~GPIOI.
//BITx:0X0000~0XFFFF,λ����,ÿ��λ����һ��IO,��0λ����Px0,��1λ����Px1,��������.����0X0101,����ͬʱ����Px0��Px8.
//MODE:0~3;ģʽѡ��,0,����(ϵͳ��λĬ��״̬);1,��ͨ���;2,���ù���;3,ģ������.
//OTYPE:0/1;�������ѡ��,0,�������;1,��©���.
//OSPEED:0~3;����ٶ�����,0,����;1,����;2,����;3,����. 
//PUPD:0~3:����������,0,����������;1,����;2,����;3,����.
//ע��:������ģʽ(��ͨ����/ģ������)��,OTYPE��OSPEED������Ч!!
void GPIO_Set(GPIO_TypeDef* GPIOx,u32 BITx,u32 MODE,u32 OTYPE,u32 OSPEED,u32 PUPD)
{  
	u32 pinpos=0,pos=0,curpin=0;
	for(pinpos=0;pinpos<16;pinpos++)
	{
		pos=1<<pinpos;	//һ����λ��� 
		curpin=BITx&pos;//��������Ƿ�Ҫ����
		if(curpin==pos)	//��Ҫ����
		{
			GPIOx->MODER&=~(3<<(pinpos*2));	//�����ԭ��������
			GPIOx->MODER|=MODE<<(pinpos*2);	//�����µ�ģʽ 
			if((MODE==0X01)||(MODE==0X02))	//��������ģʽ/���ù���ģʽ
			{  
				GPIOx->OSPEEDR&=~(3<<(pinpos*2));	//���ԭ��������
				GPIOx->OSPEEDR|=(OSPEED<<(pinpos*2));//�����µ��ٶ�ֵ  
				GPIOx->OTYPER&=~(1<<pinpos) ;		//���ԭ��������
				GPIOx->OTYPER|=OTYPE<<pinpos;		//�����µ����ģʽ
			}  
			GPIOx->PUPDR&=~(3<<(pinpos*2));	//�����ԭ��������
			GPIOx->PUPDR|=PUPD<<(pinpos*2);	//�����µ�������
		}
	}
} 
//����GPIOĳ�����ŵ����״̬
//GPIOx:GPIOA~GPIOI.
//pinx:���ű��,��Χ:0~15
//status:����״̬(�����λ��Ч),0,����͵�ƽ;1,����ߵ�ƽ
void GPIO_Pin_Set(GPIO_TypeDef* GPIOx,u16 pinx,u8 status)
{
	if(status&0X01)GPIOx->BSRR=pinx;	//����GPIOx��pinxΪ1
	else GPIOx->BSRR=pinx<<16;			//����GPIOx��pinxΪ0
}
//��ȡGPIOĳ�����ŵ�״̬
//GPIOx:GPIOA~GPIOI.
//pinx:���ű��,��Χ:0~15
//����ֵ:����״̬,0,���ŵ͵�ƽ;1,���Ÿߵ�ƽ
u8 GPIO_Pin_Get(GPIO_TypeDef* GPIOx,u16 pinx)
{ 
	if(GPIOx->IDR&pinx)return 1;		//pinx��״̬Ϊ1
	else return 0;						//pinx��״̬Ϊ0
}
//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//�ر������ж�(���ǲ�����fault��NMI�ж�)
void INTX_DISABLE(void)
{
	__ASM volatile("cpsid i");
}
//���������ж�
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}
//�������ģʽ	  
void Sys_Standby(void)
{ 
	SCB->SCR|=1<<2;		//ʹ��SLEEPDEEPλ (SYS->CTRL)	   
	RCC->APB1ENR|=1<<28;//ʹ�ܵ�Դʱ�� 
	PWR->CSR2|=1<<8;	//����PA0����WKUP����
	PWR->CR2|=0<<8;		//����PA0Ϊ�����ػ���
	PWR->CR1|=1<<1;		//PDDS��λ  
	PWR->CR1|=1<<0;		//LPDS��λ   	
	WFI_SET();			//ִ��WFIָ��,�������ģʽ		 
}	     
//ϵͳ��λ   
void Sys_Soft_Reset(void)
{   
	SCB->AIRCR =0X05FA0000|(u32)0x04;	  
} 	
//ʹ��STM32F7��L1-Cache,ͬʱ����D cache��ǿ��͸д
void Cache_Enable(void)
{
    SCB_EnableICache();	//ʹ��I-Cache,������core_cm7.h���涨��
    SCB_EnableDCache();	//ʹ��D-Cache,������core_cm7.h���涨�� 
	SCB->CACR|=1<<2;	//ǿ��D-Cache͸д,�粻����͸д,ʵ��ʹ���п���������������
}
//ʱ�����ú���
//Fvco=Fs*(plln/pllm);
//Fsys=Fvco/pllp=Fs*(plln/(pllm*pllp));
//Fusb=Fvco/pllq=Fs*(plln/(pllm*pllq));

//Fvco:VCOƵ��
//Fsys:ϵͳʱ��Ƶ��
//Fusb:USB,SDMMC,RNG�ȵ�ʱ��Ƶ��
//Fs:PLL����ʱ��Ƶ��,������HSI,HSE��. 
//plln:��PLL��Ƶϵ��(PLL��Ƶ),ȡֵ��Χ:50~432.
//pllm:��PLL����ƵPLL��Ƶϵ��(PLL֮ǰ�ķ�Ƶ),ȡֵ��Χ:2~63.
//pllp:ϵͳʱ�ӵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ),ȡֵ��Χ:2,4,6,8.(������4��ֵ!)
//pllq:USB/SDMMC/������������ȵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ),ȡֵ��Χ:2~15.

//�ⲿ����Ϊ25M��ʱ��,�Ƽ�ֵ:plln=432,pllm=25,pllp=2,pllq=9.
//�õ�:Fvco=25*(432/25)=432Mhz
//     Fsys=432/2=216Mhz
//     Fusb=432/9=48Mhz
//����ֵ:0,�ɹ�;1,ʧ�ܡ�
u8 Sys_Clock_Set(u32 plln,u32 pllm,u32 pllp,u32 pllq)
{ 
	u16 retry=0;
	u8 status=0;
	RCC->CR|=1<<16;				//HSE ���� 
	while(((RCC->CR&(1<<17))==0)&&(retry<0X1FFF))retry++;//�ȴ�HSE RDY
	if(retry==0X1FFF)status=1;	//HSE�޷�����
	else   
	{
		RCC->APB1ENR|=1<<28;	//��Դ�ӿ�ʱ��ʹ��
		PWR->CR1|=3<<14; 		//������ģʽ,ʱ�ӿɵ�180Mhz
		PWR->CR1|=1<<16; 		//ʹ�ܹ�����,Ƶ�ʿɵ�216Mhz
		PWR->CR1|=1<<17; 		//ʹ�ܹ������л�
		RCC->CFGR|=(0<<4)|(5<<10)|(4<<13);//HCLK ����Ƶ;APB1 4��Ƶ;APB2 2��Ƶ. 
		RCC->CR&=~(1<<24);		//�ر���PLL
		RCC->PLLCFGR=pllm|(plln<<6)|(((pllp>>1)-1)<<16)|(pllq<<24)|(1<<22);//������PLL,PLLʱ��Դ����HSE
		RCC->CR|=1<<24;			//����PLL
		while((RCC->CR&(1<<25))==0);//�ȴ�PLL׼���� 
		FLASH->ACR|=1<<8;		//ָ��Ԥȡʹ��.
		FLASH->ACR|=1<<9;		//ʹ��ART Accelerator 
		FLASH->ACR|=7<<0;		//8��CPU�ȴ�����. 
		RCC->CFGR&=~(3<<0);		//����
		RCC->CFGR|=2<<0;		//ѡ����PLL��Ϊϵͳʱ��	 
		while((RCC->CFGR&(3<<2))!=(2<<2));//�ȴ���PLL��Ϊϵͳʱ�ӳɹ�. 
	} 
	return status;
}  

//ϵͳʱ�ӳ�ʼ������
//plln:��PLL��Ƶϵ��(PLL��Ƶ),ȡֵ��Χ:50~432.
//pllm:��PLL����ƵPLL��Ƶϵ��(PLL֮ǰ�ķ�Ƶ),ȡֵ��Χ:2~63.
//pllp:ϵͳʱ�ӵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ),ȡֵ��Χ:2,4,6,8.(������4��ֵ!)
//pllq:USB/SDMMC/������������ȵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ),ȡֵ��Χ:2~15.
void Stm32_Clock_Init(u32 plln,u32 pllm,u32 pllp,u32 pllq)
{  
	RCC->CR|=0x00000001;		//����HISON,�����ڲ�����RC��
	RCC->CFGR=0x00000000;		//CFGR���� 
	RCC->CR&=0xFEF6FFFF;		//HSEON,CSSON,PLLON���� 
	RCC->PLLCFGR=0x24003010;	//PLLCFGR�ָ���λֵ 
	RCC->CR&=~(1<<18);			//HSEBYP����,�ⲿ������·
	RCC->CIR=0x00000000;		//��ֹRCCʱ���ж� 
	QSPI_Enable_Memmapmode();	//ʹ��QSPI�ڴ�ӳ��ģʽ
	Cache_Enable();				//ʹ��L1 Cache
	Sys_Clock_Set(plln,pllm,pllp,pllq);//����ʱ�� 
	//����������				  
#ifdef  VECT_TAB_RAM
	MY_NVIC_SetVectorTable(SRAM1_BASE,0x0);
#else   
	MY_NVIC_SetVectorTable(FLASH_BASE,0x0);
#endif 
}		    

//QSPI�����ڴ�ӳ��ģʽ��ִ��QSPI����ر�ǰ�ᣩ
void QSPI_Enable_Memmapmode(void)
{
	u32 tempreg=0; 
	vu32 *data_reg=&QUADSPI->DR; 
	RCC->AHB1ENR|=1<<1;    		//ʹ��PORTBʱ��	   
	RCC->AHB1ENR|=1<<5;    		//ʹ��PORTFʱ��	   
	RCC->AHB3ENR|=1<<1;   		//QSPIʱ��ʹ��
	//���IO������
	GPIO_Set(GPIOB,1<<2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_PU);	//PB2���ù������	
	GPIO_Set(GPIOB,1<<6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_PU);	//PB6���ù������	
	GPIO_Set(GPIOF,0XF<<6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_PU);	//PF6~9���ù������	
	GPIO_AF_Set(GPIOB,2,9);			//PB2,AF9
 	GPIO_AF_Set(GPIOB,6,10);		//PB6,AF10
 	GPIO_AF_Set(GPIOF,6,9);			//PF6,AF9 
 	GPIO_AF_Set(GPIOF,7,9);			//PF7,AF9
 	GPIO_AF_Set(GPIOF,8,10);		//PF8,AF10
 	GPIO_AF_Set(GPIOF,9,10);		//PF9,AF10
	//QSPI���ã��ο�QSPIʵ���QSPI_Init����
	RCC->AHB3RSTR|=1<<1;			//��λQSPI
	RCC->AHB3RSTR&=~(1<<1);			//ֹͣ��λQSPI
	while(QUADSPI->SR&(1<<5));		//�ȴ�BUSYλ���� 
	QUADSPI->CR=0X01000310;			//����CR�Ĵ���,��Щֵ��ô���ģ���ο�QSPIʵ��/��H750�ο��ֲ�Ĵ�����������
	QUADSPI->DCR=0X00160401;		//����DCR�Ĵ���
	QUADSPI->CR|=1<<0;				//ʹ��QSPI 

	//ע��:QSPI QEλ��ʹ�ܣ���QSPI��д�㷨���棬���Ѿ�������
	//����,������Բ�������QEλ��������Ҫ�����QEλ��1�Ĵ���
	//����,�������ͨ������������,ֱ����¼���ⲿQSPI FLASH,�ǲ����õ�
	//�����ֱ����¼���ⲿQSPI FLASHҲ������,����Ҫ���������QEλ��1�Ĵ���
	
	//W25QXX����QPIģʽ��0X38ָ�
	while(QUADSPI->SR&(1<<5));		//�ȴ�BUSYλ���� 
	QUADSPI->CCR=0X00000138;		//����0X38ָ�W25QXX����QPIģʽ
	while((QUADSPI->SR&(1<<1))==0);	//�ȴ�ָ������
	QUADSPI->FCR|=1<<1;				//���������ɱ�־λ 	

	//W25QXXдʹ�ܣ�0X06ָ�
	while(QUADSPI->SR&(1<<5));		//�ȴ�BUSYλ���� 
	QUADSPI->CCR=0X00000106;		//����0X06ָ�W25QXXдʹ��
	while((QUADSPI->SR&(1<<1))==0);	//�ȴ�ָ������
	QUADSPI->FCR|=1<<1;				//���������ɱ�־λ 
	
	//W25QXX����QPI��ض�������0XC0��
	while(QUADSPI->SR&(1<<5));		//�ȴ�BUSYλ���� 
	QUADSPI->CCR=0X030003C0;		//����0XC0ָ�W25QXX����������
	QUADSPI->DLR=0;
	while((QUADSPI->SR&(1<<2))==0);	//�ȴ�FTF
	*(vu8 *)data_reg=3<<4;			//����P4&P5=11,8��dummy clocks,104M
	QUADSPI->CR|=1<<2;				//��ֹ���� 
	while((QUADSPI->SR&(1<<1))==0);	//�ȴ����ݷ������
	QUADSPI->FCR|=1<<1;				//���������ɱ�־λ  
	while(QUADSPI->SR&(1<<5));		//�ȴ�BUSYλ���� 	 

	//MemroyMap ģʽ����
	while(QUADSPI->SR&(1<<5));		//�ȴ�BUSYλ���� 
	QUADSPI->ABR=0;					//�����ֽ�����Ϊ0��ʵ���Ͼ���W25Q 0XEBָ���,M0~M7=0
	tempreg=0XEB;					//INSTRUCTION[7:0]=0XEB,����0XEBָ�Fast Read QUAD I/O��
	tempreg|=3<<8;					//IMODE[1:0]=3,���ߴ���ָ��
	tempreg|=3<<10;					//ADDRESS[1:0]=3,���ߴ����ַ
	tempreg|=2<<12;					//ADSIZE[1:0]=2,24λ��ַ����
	tempreg|=3<<14;					//ABMODE[1:0]=3,���ߴ��佻���ֽ�
	tempreg|=0<<16;					//ABSIZE[1:0]=0,8λ�����ֽ�(M0~M7)
	tempreg|=6<<18;					//DCYC[4:0]=6,6��dummy����
	tempreg|=3<<24;					//DMODE[1:0]=3,���ߴ�������
	tempreg|=3<<26;					//FMODE[1:0]=3,�ڴ�ӳ��ģʽ
	QUADSPI->CCR=tempreg;			//����CCR�Ĵ���
	
	//����QSPI FLASH�ռ��MPU����
	SCB->SHCSR&=~(1<<16);			//��ֹMemManage 
	MPU->CTRL&=~(1<<0);				//��ֹMPU
	MPU->RNR=0;						//���ñ���������Ϊ0(1~7���Ը������ڴ���)
	MPU->RBAR=0X90000000;			//����ַΪ0X9000 000,��QSPI����ʼ��ַ
	MPU->RASR=0X0303002D;			//������ر�������(��ֹ����,����cache,������),���MPUʵ��Ľ���
	MPU->CTRL=(1<<2)|(1<<0);		//ʹ��PRIVDEFENA,ʹ��MPU 
	SCB->SHCSR|=1<<16;				//ʹ��MemManage
}






































