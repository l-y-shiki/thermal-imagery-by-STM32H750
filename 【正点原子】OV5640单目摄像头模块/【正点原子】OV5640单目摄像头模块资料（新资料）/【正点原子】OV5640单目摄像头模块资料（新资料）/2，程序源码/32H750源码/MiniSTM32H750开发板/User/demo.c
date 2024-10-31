/**
 ****************************************************************************************************
 * @file        demo.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MC5640ģ�����ʵ�飨DCMI��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� Mini Pro H750������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "demo.h"
#include "./BSP/ATK_MC5640/atk_mc5640.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/lcd/lcd.h"
#include "./BSP/led/led.h"

/**
 * @brief       ��λLCD��д��λ��������Ϊ(0, 0)�����ص�
 * @param       ��
 * @retval      ��
 */
static void demo_reset_lcd(void)
{
    lcd_set_cursor(0, 0);
    lcd_write_ram_prepare();
}

/**
 * @brief       ������ʾ��ں���
 * @param       ��
 * @retval      ��
 */
void demo_run(void)
{
    uint8_t ret;
    uint16_t output_height;
    
    ret  = atk_mc5640_init();                                               /* ��ʼ��ATK-MC5640ģ�� */
    ret += atk_mc5640_set_output_format(ATK_MC5640_OUTPUT_FORMAT_RGB565);   /* ����ATK-MC5640���RGB565ͼ������ */
    ret += atk_mc5640_auto_focus_init();                                    /* ��ʼ��ATK-MC5640ģ���Զ��Խ� */
    ret += atk_mc5640_auto_focus_continuance();                             /* ATK-MC5640ģ������Զ��Խ� */
    ret += atk_mc5640_set_light_mode(ATK_MC5640_LIGHT_MODE_ADVANCED_AWB);   /* ����ATK-MC5640ģ��ƹ�ģʽ */
    ret += atk_mc5640_set_color_saturation(ATK_MC5640_COLOR_SATURATION_4);  /* ����ATK-MC5640ģ��ɫ�ʱ��� */
    ret += atk_mc5640_set_brightness(ATK_MC5640_BRIGHTNESS_4);              /* ����ATK-MC5640ģ������ */
    ret += atk_mc5640_set_contrast(ATK_MC5640_CONTRAST_4);                  /* ����ATK-MC5640ģ��Աȶ� */
    ret += atk_mc5640_set_hue(ATK_MC5640_HUE_6);                            /* ����ATK-MC5640ģ��ɫ�� */
    ret += atk_mc5640_set_special_effect(ATK_MC5640_SPECIAL_EFFECT_NORMAL); /* ����ATK-MC5640ģ������Ч�� */
    ret += atk_mc5640_set_exposure_level(ATK_MC5640_EXPOSURE_LEVEL_5);      /* ����ATK-MC5640ģ���ع�� */
    ret += atk_mc5640_set_sharpness_level(ATK_MC5640_SHARPNESS_OFF);        /* ����ATK-MC5640ģ����� */
    ret += atk_mc5640_set_mirror_flip(ATK_MC5640_MIRROR_FLIP_1);            /* ����ATK-MC5640ģ�龵��/��ת */
    ret += atk_mc5640_set_test_pattern(ATK_MC5640_TEST_PATTERN_OFF);        /* ����ATK-MC5640ģ�����ͼ�� */
    if (lcddev.width == 480)
    {
        output_height = 640;
    }
    else
    {
        output_height = lcddev.height;
    }
    ret += atk_mc5640_set_output_size(lcddev.width, output_height);         /* ����ATK-MC5640ģ�����ͼ��ߴ� */
    if (ret != 0)
    {
        printf("ATK-MC5640 init failed!\r\n");
        while (1)
        {
            LED0_TOGGLE();
            delay_ms(200);
        }
    }
    
    while (1)
    {
        /* ����ȡ����ͼ�����ݣ���ʾ��LCD */
        atk_mc5640_get_frame((uint32_t)&LCD->LCD_RAM, ATK_MC5640_GET_TYPE_DTS_16B_NOINC, demo_reset_lcd);
    }
}
