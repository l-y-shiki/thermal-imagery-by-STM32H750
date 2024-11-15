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
#include "MYMLX90640_API.h"
#include "MYMLX90640_I2C_Driver.h"

#include "demo.h"


#define  FPS2HZ   0x02
#define  FPS4HZ   0x03
#define  FPS8HZ   0x04
#define  FPS16HZ  0x05
#define  FPS32HZ  0x06

#define  MLX90640_ADDR 0x33
#define	 RefreshRate FPS4HZ  //推荐用4Hz，干扰少
#define  TA_SHIFT 8          //Default shift for MLX90640 in open air


unsigned short EE[832];
typedef union{
	uint16_t mlx90640_Zoom10[834];  
	uint8_t  DisBuf[10*320];
	uint8_t  SPI_FLASH_BUF[4096];
}UnionData;

typedef union{
	float mlx90640To[768];
	uint16_t databuf[640];  
	uint8_t  bbb_data[512];
}UnionData2;

UnionData data;
UnionData2 data2;

float Ta;
float emissivity=0.95;  

paramsMLX90640 mlx90640;


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

int main(void)
{
	int k=0,status;
	float vdd;
    sys_cache_enable();                 /* 打开L1-Cache */
    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(240, 2, 2, 4); /* 设置时钟, 480Mhz */
    delay_init(480);                    /* 延时初始化 */
    usart_init(115200);                 /* 串口初始化为115200 */
    mpu_memory_protection();            /* 保护相关存储区域 */
//    led_init();                         /* 初始化LED */
//    key_init();                         /* 初始化按键 */
    show_mesg();                        /* 显示实验信息 */
  //  demo_run();                         /* 运行示例程序 */
	atk_md0430_init();
	
	MLX90640_I2CInit();										//MLX90640I2C初始化
		delay_ms(50);												//等待初始化
	MLX90640_SetRefreshRate(MLX90640_ADDR, FPS4HZ);   //设置帧率

	
	MLX90640_SetChessMode(MLX90640_ADDR);				//MLX90640设置成棋盘模式

//	
//	vdd=MLX90640_GetVdd(MLX90640_ADDR,&mlx90640);
//	printf("vdd;%f\n",vdd);
	

	
	
	atk_md0430_clear(ATK_MD0430_WHITE);  //填充区域，颜色
	while(1)
	{	
		status=MLX90640_DumpEE(MLX90640_ADDR, data.mlx90640_Zoom10);					//			读取像素校正参数
				printf("status11:%d\n",status);
		status=MLX90640_ExtractParameters(data.mlx90640_Zoom10, &mlx90640);				//解析校正参数
			printf("status22:%d\n",status);
		status=MLX90640_GetFrameData(MLX90640_ADDR, data.mlx90640_Zoom10);
		printf("status33:%d\n",status);
		Ta = MLX90640_GetTa(data.mlx90640_Zoom10, &mlx90640);    //读取MLX90640 外壳温度	
		MLX90640_CalculateTo(data.mlx90640_Zoom10, &mlx90640, 0.95 , Ta - TA_SHIFT, data2.mlx90640To);				//计算像素点温度
		
//		MLX90640_BadPixelsCorrection(mlx90640.brokenPixels,data2.mlx90640To,1,&mlx90640);
//		MLX90640_BadPixelsCorrection(mlx90640.outlierPixels,data2.mlx90640To,1,&mlx90640);
	//	Disp_TempPic();
//		for(k=0;k<765;k++)
//		{
//				printf("%d:%d\n",k,data.mlx90640_Zoom10[k]);
//			printf("\n");
//			printf("TO%d:%f\n",k,data2.mlx90640To[k]);
//		}
		
		printf("Ta:%f\n",Ta);
		
//		atk_md0430_set_disp_dir(ATK_MD0430_LCD_DISP_DIR_90);   //显示旋转
//		atk_md0430_fill(20,20,40,40,ATK_MD0430_BLACK);//区域填充
//		atk_md0430_draw_point(20,20,ATK_MD0430_BLACK);//画点
//		color=atk_md0430_read_point(20,20);//读取坐标点颜色
//		printf("%hx",color);
//		atk_md0430_draw_line(30,30,50,50,ATK_MD0430_YELLOW);  //画线
//		atk_md0430_show_char(60,60,"c",ATK_MD0430_LCD_FONT_16,ATK_MD0430_RED);//显示字符
//		atk_md0430_show_string(70,70,50,50,"show string ok",ATK_MD0430_LCD_FONT_16,ATK_MD0430_RED); //显示字符串
		
		
	atk_md0430_show_string(100,100,200,200,"tepmreture:",ATK_MD0430_LCD_FONT_32,ATK_MD0430_RED);
	atk_md0430_show_xnum(350,100,Ta,2,ATK_MD0430_NUM_SHOW_ZERO,ATK_MD0430_LCD_FONT_32,ATK_MD0430_RED);
	atk_md0430_show_xnum(350,200,data2.mlx90640To[400],3,ATK_MD0430_NUM_SHOW_ZERO,ATK_MD0430_LCD_FONT_32,ATK_MD0430_RED);
		atk_md0430_show_xnum(350,300,data.mlx90640_Zoom10[400],3,ATK_MD0430_NUM_SHOW_ZERO,ATK_MD0430_LCD_FONT_32,ATK_MD0430_RED);
		
	}

}
