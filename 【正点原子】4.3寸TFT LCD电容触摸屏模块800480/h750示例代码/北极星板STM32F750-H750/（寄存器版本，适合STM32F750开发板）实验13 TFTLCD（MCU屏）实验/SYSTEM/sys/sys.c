#include "sys.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//系统时钟初始化	
//包括时钟设置/中断管理/GPIO设置等
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2019/5/23
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved
//********************************************************************************
//修改说明
//无
//////////////////////////////////////////////////////////////////////////////////  


//设置向量表偏移地址
//NVIC_VectTab:基址
//Offset:偏移量		 
void MY_NVIC_SetVectorTable(u32 NVIC_VectTab,u32 Offset)	 
{ 	   	  
	SCB->VTOR=NVIC_VectTab|(Offset&(u32)0xFFFFFE00);//设置NVIC的向量表偏移寄存器,VTOR低9位保留,即[8:0]保留。
}
//设置NVIC分组
//NVIC_Group:NVIC分组 0~4 总共5组 		   
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group)	 
{ 
	u32 temp,temp1;	  
	temp1=(~NVIC_Group)&0x07;//取后三位
	temp1<<=8;
	temp=SCB->AIRCR;  //读取先前的设置
	temp&=0X0000F8FF; //清空先前分组
	temp|=0X05FA0000; //写入钥匙
	temp|=temp1;	   
	SCB->AIRCR=temp;  //设置分组	    	  				   
}
//设置NVIC 
//NVIC_PreemptionPriority:抢占优先级
//NVIC_SubPriority       :响应优先级
//NVIC_Channel           :中断编号
//NVIC_Group             :中断分组 0~4
//注意优先级不能超过设定的组的范围!否则会有意想不到的错误
//组划分:
//组0:0位抢占优先级,4位响应优先级
//组1:1位抢占优先级,3位响应优先级
//组2:2位抢占优先级,2位响应优先级
//组3:3位抢占优先级,1位响应优先级
//组4:4位抢占优先级,0位响应优先级
//NVIC_SubPriority和NVIC_PreemptionPriority的原则是,数值越小,越优先	   
void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group)	 
{ 
	u32 temp;	  
	MY_NVIC_PriorityGroupConfig(NVIC_Group);//设置分组
	temp=NVIC_PreemptionPriority<<(4-NVIC_Group);	  
	temp|=NVIC_SubPriority&(0x0f>>NVIC_Group);
	temp&=0xf;								//取低四位
	NVIC->ISER[NVIC_Channel/32]|=1<<NVIC_Channel%32;//使能中断位(要清除的话,设置ICER对应位为1即可)
	NVIC->IP[NVIC_Channel]|=temp<<4;				//设置响应优先级和抢断优先级   	    	  				   
} 
//外部中断配置函数
//只针对GPIOA~I;不包括PVD,RTC,USB_OTG,USB_HS,以太网唤醒等
//参数:
//GPIOx:0~8,代表GPIOA~I
//BITx:需要使能的位;
//TRIM:触发模式,1,下升沿;2,上降沿;3，任意电平触发
//该函数一次只能配置1个IO口,多个IO口,需多次调用
//该函数会自动开启对应中断,以及屏蔽线   	    
void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM) 
{ 
	u8 EXTOFFSET=(BITx%4)*4;  
	RCC->APB2ENR|=1<<14;  						//使能SYSCFG时钟  
	SYSCFG->EXTICR[BITx/4]&=~(0x000F<<EXTOFFSET);//清除原来设置！！！
	SYSCFG->EXTICR[BITx/4]|=GPIOx<<EXTOFFSET;	//EXTI.BITx映射到GPIOx.BITx 
	//自动设置
	EXTI->IMR|=1<<BITx;					//开启line BITx上的中断(如果要禁止中断，则反操作即可)
	if(TRIM&0x01)EXTI->FTSR|=1<<BITx;	//line BITx上事件下降沿触发
	if(TRIM&0x02)EXTI->RTSR|=1<<BITx;	//line BITx上事件上升降沿触发
} 	
//GPIO复用设置
//GPIOx:GPIOA~GPIOI.
//BITx:0~15,代表IO引脚编号.
//AFx:0~15,代表AF0~AF15.
//AF0~15设置情况(这里仅是列出常用的,详细的请见429/746数据手册,Table 12):
//AF0:MCO/SWD/SWCLK/RTC   		AF1:TIM1/TIM2;            		AF2:TIM3~5;               		AF3:TIM8~11
//AF4:I2C1~I2C4;          		AF5:SPI1~SPI6;            		AF6:SPI3/SAI1;            	 	AF7:SPI2/3/USART1~3/UART5/SPDIFRX;
//AF8:USART4~8/SPDIFRX/SAI2; 	AF9;CAN1~2/TIM12~14/LCD/QSPI; 	AF10:USB_OTG/USB_HS/SAI2/QSPI  	AF11:ETH
//AF12:FMC/SDMMC/OTG/HS   		AF13:DCIM                 		AF14:LCD;                  		AF15:EVENTOUT
void GPIO_AF_Set(GPIO_TypeDef* GPIOx,u8 BITx,u8 AFx)
{  
	GPIOx->AFR[BITx>>3]&=~(0X0F<<((BITx&0X07)*4));
	GPIOx->AFR[BITx>>3]|=(u32)AFx<<((BITx&0X07)*4);
}   
//GPIO通用设置 
//GPIOx:GPIOA~GPIOI.
//BITx:0X0000~0XFFFF,位设置,每个位代表一个IO,第0位代表Px0,第1位代表Px1,依次类推.比如0X0101,代表同时设置Px0和Px8.
//MODE:0~3;模式选择,0,输入(系统复位默认状态);1,普通输出;2,复用功能;3,模拟输入.
//OTYPE:0/1;输出类型选择,0,推挽输出;1,开漏输出.
//OSPEED:0~3;输出速度设置,0,低速;1,中速;2,快速;3,高速. 
//PUPD:0~3:上下拉设置,0,不带上下拉;1,上拉;2,下拉;3,保留.
//注意:在输入模式(普通输入/模拟输入)下,OTYPE和OSPEED参数无效!!
void GPIO_Set(GPIO_TypeDef* GPIOx,u32 BITx,u32 MODE,u32 OTYPE,u32 OSPEED,u32 PUPD)
{  
	u32 pinpos=0,pos=0,curpin=0;
	for(pinpos=0;pinpos<16;pinpos++)
	{
		pos=1<<pinpos;	//一个个位检查 
		curpin=BITx&pos;//检查引脚是否要设置
		if(curpin==pos)	//需要设置
		{
			GPIOx->MODER&=~(3<<(pinpos*2));	//先清除原来的设置
			GPIOx->MODER|=MODE<<(pinpos*2);	//设置新的模式 
			if((MODE==0X01)||(MODE==0X02))	//如果是输出模式/复用功能模式
			{  
				GPIOx->OSPEEDR&=~(3<<(pinpos*2));	//清除原来的设置
				GPIOx->OSPEEDR|=(OSPEED<<(pinpos*2));//设置新的速度值  
				GPIOx->OTYPER&=~(1<<pinpos) ;		//清除原来的设置
				GPIOx->OTYPER|=OTYPE<<pinpos;		//设置新的输出模式
			}  
			GPIOx->PUPDR&=~(3<<(pinpos*2));	//先清除原来的设置
			GPIOx->PUPDR|=PUPD<<(pinpos*2);	//设置新的上下拉
		}
	}
} 
//设置GPIO某个引脚的输出状态
//GPIOx:GPIOA~GPIOI.
//pinx:引脚编号,范围:0~15
//status:引脚状态(仅最低位有效),0,输出低电平;1,输出高电平
void GPIO_Pin_Set(GPIO_TypeDef* GPIOx,u16 pinx,u8 status)
{
	if(status&0X01)GPIOx->BSRR=pinx;	//设置GPIOx的pinx为1
	else GPIOx->BSRR=pinx<<16;			//设置GPIOx的pinx为0
}
//读取GPIO某个引脚的状态
//GPIOx:GPIOA~GPIOI.
//pinx:引脚编号,范围:0~15
//返回值:引脚状态,0,引脚低电平;1,引脚高电平
u8 GPIO_Pin_Get(GPIO_TypeDef* GPIOx,u16 pinx)
{ 
	if(GPIOx->IDR&pinx)return 1;		//pinx的状态为1
	else return 0;						//pinx的状态为0
}
//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//关闭所有中断(但是不包括fault和NMI中断)
void INTX_DISABLE(void)
{
	__ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}
//进入待机模式	  
void Sys_Standby(void)
{ 
	SCB->SCR|=1<<2;		//使能SLEEPDEEP位 (SYS->CTRL)	   
	RCC->APB1ENR|=1<<28;//使能电源时钟 
	PWR->CSR2|=1<<8;	//设置PA0用于WKUP唤醒
	PWR->CR2|=0<<8;		//设置PA0为上升沿唤醒
	PWR->CR1|=1<<1;		//PDDS置位  
	PWR->CR1|=1<<0;		//LPDS置位   	
	WFI_SET();			//执行WFI指令,进入待机模式		 
}	     
//系统软复位   
void Sys_Soft_Reset(void)
{   
	SCB->AIRCR =0X05FA0000|(u32)0x04;	  
} 	
//使能STM32F7的L1-Cache,同时开启D cache的强制透写
void Cache_Enable(void)
{
    SCB_EnableICache();	//使能I-Cache,函数在core_cm7.h里面定义
    SCB_EnableDCache();	//使能D-Cache,函数在core_cm7.h里面定义 
	SCB->CACR|=1<<2;	//强制D-Cache透写,如不开启透写,实际使用中可能遇到各种问题
}
//时钟设置函数
//Fvco=Fs*(plln/pllm);
//Fsys=Fvco/pllp=Fs*(plln/(pllm*pllp));
//Fusb=Fvco/pllq=Fs*(plln/(pllm*pllq));

//Fvco:VCO频率
//Fsys:系统时钟频率
//Fusb:USB,SDMMC,RNG等的时钟频率
//Fs:PLL输入时钟频率,可以是HSI,HSE等. 
//plln:主PLL倍频系数(PLL倍频),取值范围:50~432.
//pllm:主PLL和音频PLL分频系数(PLL之前的分频),取值范围:2~63.
//pllp:系统时钟的主PLL分频系数(PLL之后的分频),取值范围:2,4,6,8.(仅限这4个值!)
//pllq:USB/SDMMC/随机数产生器等的主PLL分频系数(PLL之后的分频),取值范围:2~15.

//外部晶振为25M的时候,推荐值:plln=432,pllm=25,pllp=2,pllq=9.
//得到:Fvco=25*(432/25)=432Mhz
//     Fsys=432/2=216Mhz
//     Fusb=432/9=48Mhz
//返回值:0,成功;1,失败。
u8 Sys_Clock_Set(u32 plln,u32 pllm,u32 pllp,u32 pllq)
{ 
	u16 retry=0;
	u8 status=0;
	RCC->CR|=1<<16;				//HSE 开启 
	while(((RCC->CR&(1<<17))==0)&&(retry<0X1FFF))retry++;//等待HSE RDY
	if(retry==0X1FFF)status=1;	//HSE无法就绪
	else   
	{
		RCC->APB1ENR|=1<<28;	//电源接口时钟使能
		PWR->CR1|=3<<14; 		//高性能模式,时钟可到180Mhz
		PWR->CR1|=1<<16; 		//使能过驱动,频率可到216Mhz
		PWR->CR1|=1<<17; 		//使能过驱动切换
		RCC->CFGR|=(0<<4)|(5<<10)|(4<<13);//HCLK 不分频;APB1 4分频;APB2 2分频. 
		RCC->CR&=~(1<<24);		//关闭主PLL
		RCC->PLLCFGR=pllm|(plln<<6)|(((pllp>>1)-1)<<16)|(pllq<<24)|(1<<22);//配置主PLL,PLL时钟源来自HSE
		RCC->CR|=1<<24;			//打开主PLL
		while((RCC->CR&(1<<25))==0);//等待PLL准备好 
		FLASH->ACR|=1<<8;		//指令预取使能.
		FLASH->ACR|=1<<9;		//使能ART Accelerator 
		FLASH->ACR|=7<<0;		//8个CPU等待周期. 
		RCC->CFGR&=~(3<<0);		//清零
		RCC->CFGR|=2<<0;		//选择主PLL作为系统时钟	 
		while((RCC->CFGR&(3<<2))!=(2<<2));//等待主PLL作为系统时钟成功. 
	} 
	return status;
}  

//系统时钟初始化函数
//plln:主PLL倍频系数(PLL倍频),取值范围:50~432.
//pllm:主PLL和音频PLL分频系数(PLL之前的分频),取值范围:2~63.
//pllp:系统时钟的主PLL分频系数(PLL之后的分频),取值范围:2,4,6,8.(仅限这4个值!)
//pllq:USB/SDMMC/随机数产生器等的主PLL分频系数(PLL之后的分频),取值范围:2~15.
void Stm32_Clock_Init(u32 plln,u32 pllm,u32 pllp,u32 pllq)
{  
	RCC->CR|=0x00000001;		//设置HISON,开启内部高速RC振荡
	RCC->CFGR=0x00000000;		//CFGR清零 
	RCC->CR&=0xFEF6FFFF;		//HSEON,CSSON,PLLON清零 
	RCC->PLLCFGR=0x24003010;	//PLLCFGR恢复复位值 
	RCC->CR&=~(1<<18);			//HSEBYP清零,外部晶振不旁路
	RCC->CIR=0x00000000;		//禁止RCC时钟中断 
	QSPI_Enable_Memmapmode();	//使能QSPI内存映射模式
	Cache_Enable();				//使能L1 Cache
	Sys_Clock_Set(plln,pllm,pllp,pllq);//设置时钟 
	//配置向量表				  
#ifdef  VECT_TAB_RAM
	MY_NVIC_SetVectorTable(SRAM1_BASE,0x0);
#else   
	MY_NVIC_SetVectorTable(FLASH_BASE,0x0);
#endif 
}		    

//QSPI进入内存映射模式（执行QSPI代码必备前提）
void QSPI_Enable_Memmapmode(void)
{
	u32 tempreg=0; 
	vu32 *data_reg=&QUADSPI->DR; 
	RCC->AHB1ENR|=1<<1;    		//使能PORTB时钟	   
	RCC->AHB1ENR|=1<<5;    		//使能PORTF时钟	   
	RCC->AHB3ENR|=1<<1;   		//QSPI时钟使能
	//相关IO口设置
	GPIO_Set(GPIOB,1<<2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_PU);	//PB2复用功能输出	
	GPIO_Set(GPIOB,1<<6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_PU);	//PB6复用功能输出	
	GPIO_Set(GPIOF,0XF<<6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_PU);	//PF6~9复用功能输出	
	GPIO_AF_Set(GPIOB,2,9);			//PB2,AF9
 	GPIO_AF_Set(GPIOB,6,10);		//PB6,AF10
 	GPIO_AF_Set(GPIOF,6,9);			//PF6,AF9 
 	GPIO_AF_Set(GPIOF,7,9);			//PF7,AF9
 	GPIO_AF_Set(GPIOF,8,10);		//PF8,AF10
 	GPIO_AF_Set(GPIOF,9,10);		//PF9,AF10
	//QSPI设置，参考QSPI实验的QSPI_Init函数
	RCC->AHB3RSTR|=1<<1;			//复位QSPI
	RCC->AHB3RSTR&=~(1<<1);			//停止复位QSPI
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






































