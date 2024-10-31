#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F7开发板
//系统时钟初始化	
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2019/6/10
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved
//********************************************************************************
//修改说明
//无
////////////////////////////////////////////////////////////////////////////////// 

//使能CPU的L1-Cache
void Cache_Enable(void)
{
    SCB_EnableICache();//使能I-Cache
    SCB_EnableDCache();//使能D-Cache
    SCB->CACR|=1<<2;   //强制D-Cache透写,如不开启,实际使用中可能遇到各种问题    
}

//时钟设置函数
//Fvco=Fs*(plln/pllm);
//Fsys=Fvco/pllp=Fs*(plln/(pllm*pllp));
//Fusb=Fvco/pllq=Fs*(plln/(pllm*pllq));

//Fvco:VCO频率
//Fsys:系统时钟频率
//Fusb:USB,SDIO,RNG等的时钟频率
//Fs:PLL输入时钟频率,可以是HSI,HSE等. 
//plln:主PLL倍频系数(PLL倍频),取值范围:64~432.
//pllm:主PLL和音频PLL分频系数(PLL之前的分频),取值范围:2~63.
//pllp:系统时钟的主PLL分频系数(PLL之后的分频),取值范围:2,4,6,8.(仅限这4个值!)
//pllq:USB/SDIO/随机数产生器等的主PLL分频系数(PLL之后的分频),取值范围:2~15.

//外部晶振为25M的时候,推荐值:plln=432,pllm=25,pllp=2,pllq=9.
//得到:Fvco=25*(432/25)=432Mhz
//     Fsys=432/2=216Mhz
//     Fusb=432/9=48Mhz
//返回值:0,成功;1,失败
void Stm32_Clock_Init(u32 plln,u32 pllm,u32 pllp,u32 pllq)
{
    HAL_StatusTypeDef ret = HAL_OK;
    RCC_OscInitTypeDef RCC_OscInitStructure; 
    RCC_ClkInitTypeDef RCC_ClkInitStructure;
	
    __HAL_RCC_PWR_CLK_ENABLE(); //使能PWR时钟
 
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);//设置调压器输出电压级别，以便在器件未以最大频率工作
      
    RCC_OscInitStructure.OscillatorType=RCC_OSCILLATORTYPE_HSE;    //时钟源为HSE
    RCC_OscInitStructure.HSEState=RCC_HSE_ON;                      //打开HSE
    RCC_OscInitStructure.PLL.PLLState=RCC_PLL_ON;				   //打开PLL
    RCC_OscInitStructure.PLL.PLLSource=RCC_PLLSOURCE_HSE;          //PLL时钟源选择HSE
    RCC_OscInitStructure.PLL.PLLM=pllm;	//主PLL和音频PLL分频系数(PLL之前的分频)
    RCC_OscInitStructure.PLL.PLLN=plln; //主PLL倍频系数(PLL倍频)
    RCC_OscInitStructure.PLL.PLLP=pllp; //系统时钟的主PLL分频系数(PLL之后的分频)
    RCC_OscInitStructure.PLL.PLLQ=pllq; //USB/SDIO/随机数产生器等的主PLL分频系数(PLL之后的分频)
    ret=HAL_RCC_OscConfig(&RCC_OscInitStructure);//初始化
    if(ret!=HAL_OK) while(1);
    
    QSPI_Enable_Memmapmode(); //使能QSPI地址映射	
    
    ret=HAL_PWREx_EnableOverDrive(); //开启Over-Driver功能
    if(ret!=HAL_OK) while(1);
    
    //选中PLL作为系统时钟源并且配置HCLK,PCLK1和PCLK2
    RCC_ClkInitStructure.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStructure.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;//设置系统时钟时钟源为PLL
    RCC_ClkInitStructure.AHBCLKDivider=RCC_SYSCLK_DIV1;//AHB分频系数为1
    RCC_ClkInitStructure.APB1CLKDivider=RCC_HCLK_DIV4;//APB1分频系数为4
    RCC_ClkInitStructure.APB2CLKDivider=RCC_HCLK_DIV2;//APB2分频系数为2
    
    ret=HAL_RCC_ClockConfig(&RCC_ClkInitStructure,FLASH_LATENCY_7);//同时设置FLASH延时周期为7WS，也就是8个CPU周期。
    if(ret!=HAL_OK) while(1);
}

#ifdef  USE_FULL_ASSERT
//当编译提示出错的时候此函数用来报告错误的文件和所在行
//file：指向源文件
//line：指向在文件中的行数
void assert_failed(uint8_t* file, uint32_t line)
{ 
	while (1)
	{
	}
}
#endif

//判断I_Cache是否打开
//返回值:0 关闭，1 打开
u8 Get_ICahceSta(void)
{
    u8 sta;
    sta=((SCB->CCR)>>17)&0X01;
    return sta;
}

//判断I_Dache是否打开
//返回值:0 关闭，1 打开
u8 Get_DCahceSta(void)
{
    u8 sta;
    sta=((SCB->CCR)>>16)&0X01;
    return sta;
}

//QSPI进入内存映射模式（执行QSPI代码必备前提）
void QSPI_Enable_Memmapmode(void)
{
	u32 tempreg=0; 
	vu32 *data_reg=&QUADSPI->DR;
	GPIO_InitTypeDef qspi_gpio;
	
	__HAL_RCC_GPIOB_CLK_ENABLE();    			//使能PORTB时钟	   
	__HAL_RCC_GPIOF_CLK_ENABLE();    			//使能PORTF时钟	   
	__HAL_RCC_QSPI_CLK_ENABLE();				//QSPI时钟使能

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
	
	//QSPI设置，参考QSPI实验的QSPI_Init函数
	RCC->AHB3RSTR|=1<<14;			//复位QSPI
	RCC->AHB3RSTR&=~(1<<14);		//停止复位QSPI
	while(QUADSPI->SR&(1<<5));		//等待BUSY位清零 
	QUADSPI->CR=0X01000310;			//设置CR寄存器,这些值怎么来的，请参考QSPI实验/看H750参考手册寄存器描述分析
	QUADSPI->DCR=0X00160401;		//设置DCR寄存器
	QUADSPI->CR|=1<<0;				//使能QSPI 

	//注意:QSPI QE位的使能，在QSPI烧写算法里面，就已经设置了
	//所以,这里可以不用设置QE位，否则需要加入对QE位置1的代码
	//不过,代码必须通过仿真器下载,直接烧录到外部QSPI FLASH,是不可用的
	//如果想直接烧录到外部QSPI FLASH也可以用,则需要在这里添加QE位置1的代码
	
	//W25QXX进入QPI模式（0X38指令）
	while(QUADSPI->SR&(1<<5));		//等待BUSY位清零 
	QUADSPI->CCR=0X00000138;		//发送0X38指令，W25QXX进入QPI模式
	while((QUADSPI->SR&(1<<1))==0);	//等待指令发送完成
	QUADSPI->FCR|=1<<1;				//清除发送完成标志位 	

	//W25QXX写使能（0X06指令）
	while(QUADSPI->SR&(1<<5));		//等待BUSY位清零 
	QUADSPI->CCR=0X00000106;		//发送0X06指令，W25QXX写使能
	while((QUADSPI->SR&(1<<1))==0);	//等待指令发送完成
	QUADSPI->FCR|=1<<1;				//清除发送完成标志位 
	
	//W25QXX设置QPI相关读参数（0XC0）
	while(QUADSPI->SR&(1<<5));		//等待BUSY位清零 
	QUADSPI->CCR=0X030003C0;		//发送0XC0指令，W25QXX读参数设置
	QUADSPI->DLR=0;
	while((QUADSPI->SR&(1<<2))==0);	//等待FTF
	*(vu8 *)data_reg=3<<4;			//设置P4&P5=11,8个dummy clocks,104M
	QUADSPI->CR|=1<<2;				//终止传输 
	while((QUADSPI->SR&(1<<1))==0);	//等待数据发送完成
	QUADSPI->FCR|=1<<1;				//清除发送完成标志位  
	while(QUADSPI->SR&(1<<5));		//等待BUSY位清零 	 

	//MemroyMap 模式设置
	while(QUADSPI->SR&(1<<5));		//等待BUSY位清零 
	QUADSPI->ABR=0;					//交替字节设置为0，实际上就是W25Q 0XEB指令的,M0~M7=0
	tempreg=0XEB;					//INSTRUCTION[7:0]=0XEB,发送0XEB指令（Fast Read QUAD I/O）
	tempreg|=3<<8;					//IMODE[1:0]=3,四线传输指令
	tempreg|=3<<10;					//ADDRESS[1:0]=3,四线传输地址
	tempreg|=2<<12;					//ADSIZE[1:0]=2,24位地址长度
	tempreg|=3<<14;					//ABMODE[1:0]=3,四线传输交替字节
	tempreg|=0<<16;					//ABSIZE[1:0]=0,8位交替字节(M0~M7)
	tempreg|=6<<18;					//DCYC[4:0]=6,6个dummy周期
	tempreg|=3<<24;					//DMODE[1:0]=3,四线传输数据
	tempreg|=3<<26;					//FMODE[1:0]=3,内存映射模式
	QUADSPI->CCR=tempreg;			//设置CCR寄存器
	
	//设置QSPI FLASH空间的MPU保护
	SCB->SHCSR&=~(1<<16);			//禁止MemManage 
	MPU->CTRL&=~(1<<0);				//禁止MPU
	MPU->RNR=0;						//设置保护区域编号为0(1~7可以给其他内存用)
	MPU->RBAR=0X90000000;			//基地址为0X9000 000,即QSPI的起始地址
	MPU->RASR=0X0303002D;			//设置相关保护参数(禁止共用,允许cache,允许缓冲),详见MPU实验的解析
	MPU->CTRL=(1<<2)|(1<<0);		//使能PRIVDEFENA,使能MPU 
	SCB->SHCSR|=1<<16;				//使能MemManage
}

//系统软复位   
void Sys_Soft_Reset(void)
{   
	SCB->AIRCR =0X05FA0000|(u32)0x04;	  
} 

//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
__asm void WFI_SET(void)
{
	WFI;		  
}
//关闭所有中断(但是不包括fault和NMI中断)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//开启所有中断
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}
