/**
 ****************************************************************************************************
 * @file        demo.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MC5640模块测试实验（DCMI）
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

#include "ova5640_demo.h"
#include "ova5640.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "lcd.h"
#include "gui.h"
#include "MYMLX90640.h"


//__attribute__((at(0x24002000)))
uint16_t receve[76800]	__attribute__((at(0x30000000)));
uint16_t receve_t[3200]={0};
uint16_t mts=5,nts=0;

/**
 * @brief       复位LCD的写入位置至坐标为(0, 0)的像素点
 * @param       无
 * @retval      无
 */
static void demo_reset_lcd()
{
	//printf("yesssssssss\n");
		//LCD_SetCursor(0,0);
    //lcd_write_ram_prepare();
	
//		LCD_SetWindows(0,0,100,100);//设置显示窗口
//		DMA_Start(ww);
}

/**
 * @brief       例程演示入口函数
 * @param       无
 * @retval      无
 */
void demo_run(void)
{
//    uint8_t ret;
//    uint16_t output_height;
//    
//    ret  = atk_mc5640_init();                                               /* 初始化ATK-MC5640模块 */
//	 printf("%d\r\n",ret);
//    ret += atk_mc5640_set_output_format(ATK_MC5640_OUTPUT_FORMAT_RGB565);   /* 设置ATK-MC5640输出RGB565图像数据 */
//    ret += atk_mc5640_auto_focus_init();                                    /* 初始化ATK-MC5640模块自动对焦 */
//    ret += atk_mc5640_auto_focus_continuance();                             /* ATK-MC5640模块持续自动对焦 */
//    ret += atk_mc5640_set_light_mode(ATK_MC5640_LIGHT_MODE_ADVANCED_AWB);   /* 设置ATK-MC5640模块灯光模式 */
//    ret += atk_mc5640_set_color_saturation(ATK_MC5640_COLOR_SATURATION_4);  /* 设置ATK-MC5640模块色彩饱度 */
//    ret += atk_mc5640_set_brightness(ATK_MC5640_BRIGHTNESS_4);              /* 设置ATK-MC5640模块亮度 */
//    ret += atk_mc5640_set_contrast(ATK_MC5640_CONTRAST_4);                  /* 设置ATK-MC5640模块对比度 */
//    ret += atk_mc5640_set_hue(ATK_MC5640_HUE_6);                            /* 设置ATK-MC5640模块色相 */
//    ret += atk_mc5640_set_special_effect(ATK_MC5640_SPECIAL_EFFECT_NORMAL); /* 设置ATK-MC5640模块特殊效果 */
//    ret += atk_mc5640_set_exposure_level(ATK_MC5640_EXPOSURE_LEVEL_5);      /* 设置ATK-MC5640模块曝光度 */
//    ret += atk_mc5640_set_sharpness_level(ATK_MC5640_SHARPNESS_OFF);        /* 设置ATK-MC5640模块锐度 */
//    ret += atk_mc5640_set_mirror_flip(ATK_MC5640_MIRROR_FLIP_1);            /* 设置ATK-MC5640模块镜像/翻转 */
//	 printf("%d\r\n",ret);
//    ret += atk_mc5640_set_test_pattern(ATK_MC5640_TEST_PATTERN_OFF);        /* 设置ATK-MC5640模块测试图案 */
//    if (lcddev.width == 480)
//    {
//        output_height = 640;
//    }
//    else
//    {
//        output_height = lcddev.height;
//    }
//    ret += atk_mc5640_set_output_size(lcddev.width, output_height);         /* 设置ATK-MC5640模块输出图像尺寸 */
//    if (ret != 0)
//    {
//        printf("ATK-MC5640 init failed!\r\n");
//        while (1)
//        {
//            printf("nononoononononono\n");
//            delay_ms(200);
//        }
//    }
//		else printf("ATK-MC5640 init okkkkkkk!\r\n");
//		
//    /* 将获取到的图像数据，显示至LCD */
//       
//	//	delay_ms(200);
//    while (1)
//    {
      atk_mc5640_get_frame((uint32_t)receve, ATK_MC5640_GET_TYPE_DTS_16B_INC, demo_reset_lcd);
			for(nts=0;nts<24;nts++)
			{
			for(mts=0;mts<3200;mts++)
			{
				receve_t[mts]=receve[mts+3200*nts];
			
			//	printf("%d::::%x\n",mts+3200*nts,receve[mts+3200*nts]);
			}
//				LCD_Fill_BUF(0,10*nts,319,9+10*nts,receve_t);
//				delay_us(1500);
//		}
    }
}

void VO5640_Init(void)
{
		    uint8_t ret;
    uint16_t output_height;
    
    ret  = atk_mc5640_init();                                               /* 初始化ATK-MC5640模块 */
	 printf("%d\r\n",ret);
    ret += atk_mc5640_set_output_format(ATK_MC5640_OUTPUT_FORMAT_RGB565);   /* 设置ATK-MC5640输出RGB565图像数据 */
    ret += atk_mc5640_auto_focus_init();                                    /* 初始化ATK-MC5640模块自动对焦 */
    ret += atk_mc5640_auto_focus_continuance();                             /* ATK-MC5640模块持续自动对焦 */
    ret += atk_mc5640_set_light_mode(ATK_MC5640_LIGHT_MODE_ADVANCED_AWB);   /* 设置ATK-MC5640模块灯光模式 */
    ret += atk_mc5640_set_color_saturation(ATK_MC5640_COLOR_SATURATION_4);  /* 设置ATK-MC5640模块色彩饱度 */
    ret += atk_mc5640_set_brightness(ATK_MC5640_BRIGHTNESS_4);              /* 设置ATK-MC5640模块亮度 */
    ret += atk_mc5640_set_contrast(ATK_MC5640_CONTRAST_4);                  /* 设置ATK-MC5640模块对比度 */
    ret += atk_mc5640_set_hue(ATK_MC5640_HUE_6);                            /* 设置ATK-MC5640模块色相 */
    ret += atk_mc5640_set_special_effect(ATK_MC5640_SPECIAL_EFFECT_NORMAL); /* 设置ATK-MC5640模块特殊效果 */
    ret += atk_mc5640_set_exposure_level(ATK_MC5640_EXPOSURE_LEVEL_5);      /* 设置ATK-MC5640模块曝光度 */
    ret += atk_mc5640_set_sharpness_level(ATK_MC5640_SHARPNESS_OFF);        /* 设置ATK-MC5640模块锐度 */
    ret += atk_mc5640_set_mirror_flip(ATK_MC5640_MIRROR_FLIP_1);            /* 设置ATK-MC5640模块镜像/翻转 */
	 printf("%d\r\n",ret);
    ret += atk_mc5640_set_test_pattern(ATK_MC5640_TEST_PATTERN_OFF);        /* 设置ATK-MC5640模块测试图案 */
    if (lcddev.width == 480)
    {
        output_height = 640;
    }
    else
    {
        output_height = lcddev.height;
    }
    ret += atk_mc5640_set_output_size(lcddev.width, output_height);         /* 设置ATK-MC5640模块输出图像尺寸 */
    if (ret != 0)
    {
        printf("ATK-MC5640 init failed!\r\n");
        while (1)
        {
            printf("nononoononononono\n");
            delay_ms(200);
        }
    }
		else printf("ATK-MC5640 init okkkkkkk!\r\n");
}
