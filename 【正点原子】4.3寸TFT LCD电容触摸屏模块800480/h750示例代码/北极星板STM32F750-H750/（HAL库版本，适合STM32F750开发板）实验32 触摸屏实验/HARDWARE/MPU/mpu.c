#include "mpu.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F7开发板
//MPU驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2019/6/1
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

//设置某个区域的MPU保护
//baseaddr:MPU保护区域的基址(首地址)
//size:MPU保护区域的大小(必须是32的倍数,单位为字节)
//rnum:MPU保护区编号,范围:0~7,最大支持8个保护区域
//de:禁止指令访问;0,允许指令访问;1,禁止指令访问
//ap:访问权限,访问关系如下:
//0,无访问（特权&用户都不可访问）
//1,仅支持特权读写访问
//2,禁止用户写访问（特权可读写访问）
//3,全访问（特权&用户都可访问）
//4,无法预测(禁止设置为4!!!)
//5,仅支持特权读访问
//6,只读（特权&用户都不可以写）
//详见:STM32H7编程手册.pdf,4.6.6节,Table 91.
//sen:是否允许共用;0,不允许;1,允许
//cen:是否允许cache;0,不允许;1,允许
//ben:是否允许缓冲;0,不允许;1,允许
//返回值;0,成功.
//    其他,错误.
u8 MPU_Set_Protection(u32 baseaddr,u32 size,u32 rnum,u8 de,u8 ap,u8 sen,u8 cen,u8 ben)
{
	MPU_Region_InitTypeDef MPU_Initure;
	HAL_MPU_Disable();								        //配置MPU之前先关闭MPU,配置完成以后再使能MPU

	MPU_Initure.Enable=MPU_REGION_ENABLE;			        //使能该保护区域 
	MPU_Initure.Number=rnum;			                    //设置保护区域
	MPU_Initure.BaseAddress=baseaddr;	                    //设置基址
	MPU_Initure.Size=size;				                    //设置保护区域大小
	MPU_Initure.SubRegionDisable=0X00;                      //禁止子区域
	MPU_Initure.TypeExtField=MPU_TEX_LEVEL0;                //设置类型扩展域为level0
	MPU_Initure.AccessPermission=(u8)ap;		            	//设置访问权限,
	MPU_Initure.DisableExec=de;								//是否允许指令访问
	MPU_Initure.IsShareable=sen;                            //是否允许共用
	MPU_Initure.IsCacheable=cen;                            //是否允许cache
	MPU_Initure.IsBufferable=ben;                           //是否允许缓冲
	HAL_MPU_ConfigRegion(&MPU_Initure);                     //配置MPU
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);			        //开启MPU
    return 0;
}


//设置需要保护的存储块
//必须对部分存储区域进行MPU保护,否则可能导致程序运行异常
//比如MCU屏不显示,摄像头采集数据出错等等问题...
void MPU_Memory_Protection(void)
{		
	MPU_Set_Protection(0x20000000,MPU_REGION_SIZE_64KB,1,0,MPU_REGION_FULL_ACCESS,0,1,1);	//保护整个DTCM,共64K字节,允许指令访问,禁止共用,允许cache,允许缓冲
	MPU_Set_Protection(0x20010000,MPU_REGION_SIZE_256KB,2,0,MPU_REGION_FULL_ACCESS,0,1,1);	//保护整个SRAM1/SRAM2,共256K字节,允许指令访问,禁止共用,允许cache,允许缓冲
	MPU_Set_Protection(0x00000000,MPU_REGION_SIZE_16KB,3,0,MPU_REGION_FULL_ACCESS,0,1,1);	//保护整个ITCM,共16K字节,允许指令访问,禁止共用,允许cache,允许缓冲 

	MPU_Set_Protection(0x60000000,MPU_REGION_SIZE_64MB,4,0,MPU_REGION_FULL_ACCESS,0,0,0);	//保护MCU LCD屏所在的FMC区域,,共64M字节,允许指令访问,禁止共用,禁止cache,禁止缓冲
	MPU_Set_Protection(0XC0000000,MPU_REGION_SIZE_64MB,5,0,MPU_REGION_FULL_ACCESS,0,1,1);	//保护SDRAM区域,共64M字节,允许指令访问,禁止共用,允许cache,允许缓冲
	MPU_Set_Protection(0X80000000,MPU_REGION_SIZE_256MB,6,1,MPU_REGION_FULL_ACCESS,0,0,0);	//保护整个NAND FLASH区域,共256M字节,禁止指令访问,禁止共用,禁止cache,禁止缓冲
}


//MemManage错误处理中断
//进入此中断以后,将无法恢复程序运行!!
void MemManage_Handler(void)
{ 
	LED1(0);							//点亮DS1
	printf("Mem Access Error!!\r\n"); 	//输出错误信息
	delay_ms(1000);	
	printf("Soft Reseting...\r\n");		//提示软件重启
	delay_ms(1000);	
	NVIC_SystemReset();					//软复位
}
