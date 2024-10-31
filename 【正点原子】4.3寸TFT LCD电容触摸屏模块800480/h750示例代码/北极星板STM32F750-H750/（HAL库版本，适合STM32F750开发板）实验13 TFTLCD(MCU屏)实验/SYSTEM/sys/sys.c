#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//ϵͳʱ�ӳ�ʼ��	
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2019/6/10
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved
//********************************************************************************
//�޸�˵��
//��
////////////////////////////////////////////////////////////////////////////////// 

//ʹ��CPU��L1-Cache
void Cache_Enable(void)
{
    SCB_EnableICache();//ʹ��I-Cache
    SCB_EnableDCache();//ʹ��D-Cache
    SCB->CACR|=1<<2;   //ǿ��D-Cache͸д,�粻����,ʵ��ʹ���п���������������    
}

//ʱ�����ú���
//Fvco=Fs*(plln/pllm);
//Fsys=Fvco/pllp=Fs*(plln/(pllm*pllp));
//Fusb=Fvco/pllq=Fs*(plln/(pllm*pllq));

//Fvco:VCOƵ��
//Fsys:ϵͳʱ��Ƶ��
//Fusb:USB,SDIO,RNG�ȵ�ʱ��Ƶ��
//Fs:PLL����ʱ��Ƶ��,������HSI,HSE��. 
//plln:��PLL��Ƶϵ��(PLL��Ƶ),ȡֵ��Χ:64~432.
//pllm:��PLL����ƵPLL��Ƶϵ��(PLL֮ǰ�ķ�Ƶ),ȡֵ��Χ:2~63.
//pllp:ϵͳʱ�ӵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ),ȡֵ��Χ:2,4,6,8.(������4��ֵ!)
//pllq:USB/SDIO/������������ȵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ),ȡֵ��Χ:2~15.

//�ⲿ����Ϊ25M��ʱ��,�Ƽ�ֵ:plln=432,pllm=25,pllp=2,pllq=9.
//�õ�:Fvco=25*(432/25)=432Mhz
//     Fsys=432/2=216Mhz
//     Fusb=432/9=48Mhz
//����ֵ:0,�ɹ�;1,ʧ��
void Stm32_Clock_Init(u32 plln,u32 pllm,u32 pllp,u32 pllq)
{
    HAL_StatusTypeDef ret = HAL_OK;
    RCC_OscInitTypeDef RCC_OscInitStructure; 
    RCC_ClkInitTypeDef RCC_ClkInitStructure;
	
    __HAL_RCC_PWR_CLK_ENABLE(); //ʹ��PWRʱ��
 
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);//���õ�ѹ�������ѹ�����Ա�������δ�����Ƶ�ʹ���
      
    RCC_OscInitStructure.OscillatorType=RCC_OSCILLATORTYPE_HSE;    //ʱ��ԴΪHSE
    RCC_OscInitStructure.HSEState=RCC_HSE_ON;                      //��HSE
    RCC_OscInitStructure.PLL.PLLState=RCC_PLL_ON;				   //��PLL
    RCC_OscInitStructure.PLL.PLLSource=RCC_PLLSOURCE_HSE;          //PLLʱ��Դѡ��HSE
    RCC_OscInitStructure.PLL.PLLM=pllm;	//��PLL����ƵPLL��Ƶϵ��(PLL֮ǰ�ķ�Ƶ)
    RCC_OscInitStructure.PLL.PLLN=plln; //��PLL��Ƶϵ��(PLL��Ƶ)
    RCC_OscInitStructure.PLL.PLLP=pllp; //ϵͳʱ�ӵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ)
    RCC_OscInitStructure.PLL.PLLQ=pllq; //USB/SDIO/������������ȵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ)
    ret=HAL_RCC_OscConfig(&RCC_OscInitStructure);//��ʼ��
    if(ret!=HAL_OK) while(1);
    
    QSPI_Enable_Memmapmode(); //ʹ��QSPI��ַӳ��	
    
    ret=HAL_PWREx_EnableOverDrive(); //����Over-Driver����
    if(ret!=HAL_OK) while(1);
    
    //ѡ��PLL��Ϊϵͳʱ��Դ��������HCLK,PCLK1��PCLK2
    RCC_ClkInitStructure.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStructure.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;//����ϵͳʱ��ʱ��ԴΪPLL
    RCC_ClkInitStructure.AHBCLKDivider=RCC_SYSCLK_DIV1;//AHB��Ƶϵ��Ϊ1
    RCC_ClkInitStructure.APB1CLKDivider=RCC_HCLK_DIV4;//APB1��Ƶϵ��Ϊ4
    RCC_ClkInitStructure.APB2CLKDivider=RCC_HCLK_DIV2;//APB2��Ƶϵ��Ϊ2
    
    ret=HAL_RCC_ClockConfig(&RCC_ClkInitStructure,FLASH_LATENCY_7);//ͬʱ����FLASH��ʱ����Ϊ7WS��Ҳ����8��CPU���ڡ�
    if(ret!=HAL_OK) while(1);
}

#ifdef  USE_FULL_ASSERT
//��������ʾ�����ʱ��˺����������������ļ���������
//file��ָ��Դ�ļ�
//line��ָ�����ļ��е�����
void assert_failed(uint8_t* file, uint32_t line)
{ 
	while (1)
	{
	}
}
#endif

//�ж�I_Cache�Ƿ��
//����ֵ:0 �رգ�1 ��
u8 Get_ICahceSta(void)
{
    u8 sta;
    sta=((SCB->CCR)>>17)&0X01;
    return sta;
}

//�ж�I_Dache�Ƿ��
//����ֵ:0 �رգ�1 ��
u8 Get_DCahceSta(void)
{
    u8 sta;
    sta=((SCB->CCR)>>16)&0X01;
    return sta;
}

//QSPI�����ڴ�ӳ��ģʽ��ִ��QSPI����ر�ǰ�ᣩ
void QSPI_Enable_Memmapmode(void)
{
	u32 tempreg=0; 
	vu32 *data_reg=&QUADSPI->DR;
	GPIO_InitTypeDef qspi_gpio;
	
	__HAL_RCC_GPIOB_CLK_ENABLE();    			//ʹ��PORTBʱ��	   
	__HAL_RCC_GPIOF_CLK_ENABLE();    			//ʹ��PORTFʱ��	   
	__HAL_RCC_QSPI_CLK_ENABLE();				//QSPIʱ��ʹ��

	qspi_gpio.Pin=GPIO_PIN_6;					//PB6 AF10	
	qspi_gpio.Mode=GPIO_MODE_AF_PP;
	qspi_gpio.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
	qspi_gpio.Pull=GPIO_NOPULL;
	qspi_gpio.Alternate=GPIO_AF10_QUADSPI;
	HAL_GPIO_Init(GPIOB,&qspi_gpio);
	
	qspi_gpio.Pin=GPIO_PIN_2;					//PB2 AF9	
	qspi_gpio.Alternate=GPIO_AF9_QUADSPI;
	HAL_GPIO_Init(GPIOB,&qspi_gpio);
	
	qspi_gpio.Pin=GPIO_PIN_6|GPIO_PIN_7;		//PF6,7 AF9	
	qspi_gpio.Alternate=GPIO_AF9_QUADSPI;
	HAL_GPIO_Init(GPIOF,&qspi_gpio);
	
	qspi_gpio.Pin=GPIO_PIN_8|GPIO_PIN_9;		//PF8,9 AF10		
	qspi_gpio.Alternate=GPIO_AF10_QUADSPI;
	HAL_GPIO_Init(GPIOF,&qspi_gpio);
	
	//QSPI���ã��ο�QSPIʵ���QSPI_Init����
	RCC->AHB3RSTR|=1<<14;			//��λQSPI
	RCC->AHB3RSTR&=~(1<<14);		//ֹͣ��λQSPI
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

//ϵͳ��λ   
void Sys_Soft_Reset(void)
{   
	SCB->AIRCR =0X05FA0000|(u32)0x04;	  
} 

//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
__asm void WFI_SET(void)
{
	WFI;		  
}
//�ر������ж�(���ǲ�����fault��NMI�ж�)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//���������ж�
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}
