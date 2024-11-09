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
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/MPU/mpu.h"
#include "./BSP/ATK_MD0430/atk_md0430.h"
#include "demo.h"


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
	int color;
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
	atk_md0430_clear(ATK_MD0430_BLUE);  //填充区域，颜色
	while(1)
	{
		
		//atk_md0430_set_disp_dir(ATK_MD0430_LCD_DISP_DIR_90);   //显示旋转
		atk_md0430_fill(20,20,40,40,ATK_MD0430_RED);//区域填充
		atk_md0430_draw_point(20,20,ATK_MD0430_BLACK);//画点
		color=atk_md0430_read_point(20,20);//读取坐标点颜色
		printf("%hx",color);
		atk_md0430_draw_line(30,30,50,50,ATK_MD0430_YELLOW);  //画线
	//	atk_md0430_show_char(60,60,"c",ATK_MD0430_LCD_FONT_16,ATK_MD0430_RED);//显示字符
		atk_md0430_show_string(70,70,50,50,"show string ok",ATK_MD0430_LCD_FONT_16,ATK_MD0430_RED); //显示字符串
		
		
		
	}

}
