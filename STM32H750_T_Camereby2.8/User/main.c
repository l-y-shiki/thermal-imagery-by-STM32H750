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
#include "lcd.h"
#include "gui.h"
#include "spi.h"
#include "spi_test.h"
#include "MYMLX90640_API.h"
#include "MYMLX90640_I2C_Driver.h"
#include "MYMLX90640.h"
#include "string.h"

extern DMA_HandleTypeDef hdma_memtomem_dma2_stream6;
extern SPI_HandleTypeDef g_spi2_handle;

uint8_t dmaTransferComplete = 0;
uint16_t scount=0;

uint16_t ww[6400]; // __attribute__((section(".ARM.__at_0x24000000")));
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
	
	 uint16_t len;
    uint16_t times = 0;
	uint16_t q;


	for(q=0;q<6400;q++)
	{
			ww[q]=0xFFE0;
	}
  
	
    sys_cache_enable();                     /* 使能L1-Cache */
    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(192, 5, 2, 4);     /* 配置时钟，480MHz */
    delay_init(480);                        /* 初始化延时 */
    usart_init(115200);                     /* 初始化串口 */
 //   led_init();                             /* 初始化MLX90640 */
			MLX90640_Init();
			
		  LCD_Init();	   //液晶屏初始化
	SPI_DMA_Init();
	
	LCD_direction(3);
// 				LCD_Clear(RED);
//	
//			LCD_SetWindows(1,11,320,20);//设置显示窗口

	
//			LCD_Fill_BUF(0,0,319,9,ww);
//			LCD_Fill_BUF(0,160,319,169,ww);


	
    while (1)
    {
			
//							LCD_SetWindows(1,21,320,30);//设置显示窗口
//				DMA_Start(ww);
//				
//				LCD_SetWindows(1,41,320,50);//设置显示窗口
//				DMA_Start(ww);


			//检查DMA传输是否完成
//			if(dmaTransferComplete==48)
//			{
//				dmaTransferComplete=0;
//				
					if(Mlx90640_Get_Frame()==1)
					{
				//		printf("ok\n");

					}
//			}
//			delay_ms(200);
			Disp_Temp_Pia(); 

    }
}
