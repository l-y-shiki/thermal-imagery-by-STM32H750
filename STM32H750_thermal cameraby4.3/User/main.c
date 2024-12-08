/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MD0430模块测试实验（FMC）
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 Mini Pro H750开发板
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
#include "./BSP/MPU/mpu.h"
#include "./BSP/ATK_MD0430/atk_md0430.h"
#include "lcd.h"
#include "MYMLX90640_API.h"
#include "MYMLX90640_I2C_Driver.h"
#include "MYMLX90640.h"

#include "demo.h"

extern DMA_HandleTypeDef hdma_memtomem_dma2_stream6;
extern unsigned short rgb[768];


//#define  FPS2HZ   0x02
//#define  FPS4HZ   0x03
//#define  FPS8HZ   0x04
//#define  FPS16HZ  0x05
//#define  FPS32HZ  0x06

//#define  MLX90640_ADDR 0x33
//#define	 RefreshRate FPS4HZ  //推荐用4Hz，干扰少
//#define  TA_SHIFT 8          //Default shift for MLX90640 in open air


//typedef union{
//	uint16_t mlx90640_Zoom10[834];  
//	uint8_t  DisBuf[10*320];
//	uint8_t  SPI_FLASH_BUF[4096];
//}UnionData;

//typedef union{
//	float mlx90640To[768];
//	uint16_t databuf[640];  
//	uint8_t  bbb_data[512];
//}UnionData2;

//UnionData data;
//UnionData2 data2;

//float Ta;
//float emissivity=0.95;
//int x_line=0,y_list=0;
//int pin_x=0,pin_y=0;

//paramsMLX90640 mlx90640;


/**
 * @brief       显示实验信息
 * @param       无
 * @retval      无
 */
void show_mesg(void)
{
    /* 串口输出实验信息 */
    printf("\n");
    printf("********************************\r\n");
    printf("STM32\r\n");
    printf("ATK-MD0430\r\n");
    printf("ATOM@ALIENTEK\r\n");
    printf("********************************\r\n");
    printf("\r\n");
}


//uint8_t Mlx90640_Get_Frame(void)
//{
//		uint8_t flag = 0;
//		uint16_t status = 0;
//	
//				MLX90640_I2CRead(MLX90640_ADDR, 0x8000, 1, &status);

//				if(status & 0x0008)
//				{
//						int status2 = MLX90640_GetFrameData(MLX90640_ADDR, data.mlx90640_Zoom10);  //读取一帧原始数据
//						if (status2 < 0)		printf("GetFrame Error: %d\r\n",status2);
//						
//						Ta = MLX90640_GetTa(data.mlx90640_Zoom10, &mlx90640);  //计算实时外壳温度
//						Ta = Ta - TA_SHIFT;
//						MLX90640_CalculateTo(data.mlx90640_Zoom10, &mlx90640,0.95,Ta,data2.mlx90640To);		//计算像素点温度
//				}
//				flag=1;
//		
//		return flag;
//}


int main(void)
{
//	unsigned short rgb = 0;
//	int k=0,j=0,m=0,status,i=0;
//	float vdd;
//	uint8_t count;
    sys_cache_enable();                 /* 打开L1-Cache */
    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(192, 5, 2, 4); /* 设置时钟, 480Mhz */
    delay_init(480);                    /* 延时初始化 */
    usart_init(115200);                 /* 串口初始化为115200 */
    mpu_memory_protection();            /* 保护相关存储区域 */
//    led_init();                         /* 初始化LED */
//    key_init();                         /* 初始化按键 */
    show_mesg();                        /* 显示实验信息 */
//    demo_run();                         /* 运行示例程序 */
	
		MLX90640_Init();
	//	FSMC_DMA_Init();
//	atk_md0430_init();
	
	  LCD_Init();	   //液晶屏初始化

//	atk_md0430_set_disp_dir(ATK_MD0430_LCD_DISP_DIR_90);   //显示旋转
//	atk_md0430_clear(ATK_MD0430_WHITE);  //填充区域，颜色
	while(1)
	{	
		LCD_Clear(BLACK);
		LCD_Clear(ATK_MD0430_WHITE);
		LCD_Clear(ATK_MD0430_RED);
		LCD_Clear(ATK_MD0430_GREEN);
		
		if(Mlx90640_Get_Frame()==1)
		{
			//printf("ok\n");
			LCD_Fill(1,1,100,100,BLACK);
		}
//		printf("ok\n");

//		atk_md0430_fill(20,20,40,40,ATK_MD0430_BLACK);//区域填充
//		atk_md0430_draw_point(20,20,ATK_MD0430_BLACK);//画点
//		color=atk_md0430_read_point(20,20);//读取坐标点颜色
//		printf("%hx",color);
//		atk_md0430_draw_line(30,30,50,50,ATK_MD0430_YELLOW);  //画线
//		atk_md0430_show_char(60,60,"c",ATK_MD0430_LCD_FONT_16,ATK_MD0430_RED);//显示字符
//		atk_md0430_show_string(70,70,50,50,"show string ok",ATK_MD0430_LCD_FONT_16,ATK_MD0430_RED); //显示字符串
		
		
//	atk_md0430_show_string(100,100,200,200,"tepmreture:",ATK_MD0430_LCD_FONT_32,ATK_MD0430_RED);
//	atk_md0430_show_xnum(350,100,Ta,2,ATK_MD0430_NUM_SHOW_ZERO,ATK_MD0430_LCD_FONT_32,ATK_MD0430_RED);
//	atk_md0430_show_xnum(350,200,data2.mlx90640To[400],3,ATK_MD0430_NUM_SHOW_ZERO,ATK_MD0430_LCD_FONT_32,ATK_MD0430_RED);
//		atk_md0430_show_xnum(350,300,data.mlx90640_Zoom10[400],3,ATK_MD0430_NUM_SHOW_ZERO,ATK_MD0430_LCD_FONT_32,ATK_MD0430_RED);
	//	Disp_Temp_Pic2(); 
		
//		HAL_DMA_Start(&hdma_memtomem_dma2_stream6,(uint32_t)&dat,(uint32_t)ATK_MD0430_FMC_DAT_ADDR,1);
//  	HAL_DMA_PollForTransfer(&hdma_memtomem_dma2_stream6,HAL_DMA_FULL_TRANSFER,1);
		
		
//		display();
//		atk_md0430_clear(ATK_MD0430_WHITE);  //填充区域，颜色
//		atk_md0430_clear(ATK_MD0430_RED);  //填充区域，颜色
//		atk_md0430_clear(ATK_MD0430_BLACK);  //填充区域，颜色
//		atk_md0430_clear(ATK_MD0430_GREEN);  //填充区域，颜色
		
//		
//		atk_md0430_fill(1,1,280,320,ATK_MD0430_WHITE);
//		atk_md0430_fill(1,1,280,320,ATK_MD0430_RED);
//		atk_md0430_fill(1,1,280,320,ATK_MD0430_BLACK);
//			atk_md0430_fill2(1,1,800,480,ATK_MD0430_RED);
		//	atk_md0430_fill2(1,1,800,480,ATK_MD0430_BLACK);

	}

}
