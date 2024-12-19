/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-06-12
 * @brief       串口通信实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 * 
 * 实验平台:正点原子 北极星 H750开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 * 
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "lcd.h"
#include "gui.h"
#include "spi.h"
#include "spi_test.h"
#include "MYMLX90640_API.h"
#include "MYMLX90640_I2C_Driver.h"
#include "MYMLX90640.h"
#include "RGB565.h"
#include "string.h"

//摄像头
#include "ova5640.h"
#include "ova5640_demo.h"

extern DMA_HandleTypeDef hdma_memtomem_dma2_stream6;
extern SPI_HandleTypeDef g_spi2_handle;

//调用缓存数据
extern uint16_t receve[76800];
extern uint16_t temp[76800];
uint16_t mix[76800];

uint8_t dmaTransferComplete = 0;
uint16_t scount=0;

uint16_t ww[3200]; // __attribute__((section(".ARM.__at_0x24000000")));
// uint8_t ww[320] __attribute__((at(0x30000000)));

//SPI回调函数
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	printf("enter\n");
    if (hspi->Instance == SPI2)
    {
			dmaTransferComplete=1;
			scount++;
	printf("scount:%d            \n",scount);
		
    }
//		HAL_DMAEx_DisableMuxRequestGenerator(&hdma_memtomem_dma2_stream6);
			//HAL_SPI_DMAStop(&g_spi2_handle);
	//	HAL_SPI_Transmit_DMA(&g_spi2_handle,ww,320);
        // 传输完成后的处理
}




	
int main(void)
{
	    uint8_t mode=0,type=2;
	 uint16_t keyvalue=1;
	uint16_t q,nts=0,mts=0;
	
    sys_cache_enable();                     /* 使能L1-Cache */
    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(192, 5, 2, 4);     /* 配置时钟，480MHz */
    delay_init(480);                        /* 初始化延时 */
    usart_init(115200);                     /* 初始化串口 */
 //   led_init();                             
			key_init();														/*初始化按键*/
			MLX90640_Init();											/* 初始化MLX90640 */
			
		  LCD_Init();	   //液晶屏初始化
			SPI_DMA_Init();
	
			LCD_direction(3);
			VO5640_Init();												/*摄像头初始化*/
	
    while (1)
    {
		//	keyvalue=key_scan(&keyvalue);
			
		if(KEY0==0)
		{
				while(KEY0==0);
				keyvalue++;
		}
				if(keyvalue==4)
					keyvalue=1;
		
				if(KEY2==0)
				{
						while(KEY2==0);
						type+=2;
					mode++;
					if(mode==2)
						mode=0;
				}
						if(type==6)
							type=2;


//检查DMA传输是否完成
				
//			if(dmaTransferComplete==48)
//			{
//				dmaTransferComplete=0;			

//					if(Mlx90640_Get_Frame()==1)
//					{
//				
//					}
//			}

			
			if(keyvalue==1)
			{	
				demo_run();
				Mlx90640_Get_Frame();
				Disp_Temp_Pia(mode,type); 		//缓存温度数据
					for(nts=0;nts<24;nts++)
				{
						for(mts=0;mts<3200;mts++)
					{
						mix[mts+3200*nts]=RGB565_change(temp[mts+3200*nts],receve[mts+3200*nts],0.5);					//图像融合
					}
				}
			}
			
			else if(keyvalue==2)
			{			demo_run();
							for(nts=0;nts<24;nts++)
			{
			for(mts=0;mts<3200;mts++)
			{
				mix[mts+3200*nts]=receve[mts+3200*nts];					//显示可见光图像
			}
    }
			}
			
			else if(keyvalue==3)
			{
				Mlx90640_Get_Frame();
				Disp_Temp_Pia(mode,type);
					for(nts=0;nts<24;nts++)
			{
			for(mts=0;mts<3200;mts++)
			{
				mix[mts+3200*nts]=temp[mts+3200*nts];					//显示热成像
			}
    }
			}


					for(nts=0;nts<24;nts++)
			{
			for(mts=0;mts<3200;mts++)
			{
				ww[mts]=mix[mts+3200*nts];
			
//				printf("%d::::%x\n",mts+3200*nts,receve[mts+3200*nts]);
			}
				LCD_Fill_BUF(0,10*nts,319,9+10*nts,ww);
				delay_us(1);
    }
    }
}
