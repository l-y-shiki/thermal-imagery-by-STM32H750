/**
 ****************************************************************************************************
 * @file        atk_mc5640.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MC5640ģ����������
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

#ifndef __ATK_MC5640_H
#define __ATK_MC5640_H

#include "./SYSTEM/sys/sys.h"

/* ����ATK-MC5640ģ���������Ƿ���OV5640���� */
#define ATK_MC5640_LED_CTL_BY_OV5640            1

/* ���Ŷ��� */
#define ATK_MC5640_RST_GPIO_PORT                GPIOA
#define ATK_MC5640_RST_GPIO_PIN                 GPIO_PIN_7
#define ATK_MC5640_RST_GPIO_CLK_ENABLE()        do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)
#define ATK_MC5640_PWDN_GPIO_PORT               GPIOC
#define ATK_MC5640_PWDN_GPIO_PIN                GPIO_PIN_4
#define ATK_MC5640_PWDN_GPIO_CLK_ENABLE()       do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)
#if (ATK_MC5640_LED_CTL_BY_OV5640 == 0)
#define ATK_MC5640_FLASH_GPIO_PORT              GPIOA
#define ATK_MC5640_FLASH_GPIO_PIN               GPIO_PIN_8
#define ATK_MC5640_FLASH_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)
#endif

/* IO���� */
#define ATK_MC5640_RST(x)                       do{ x ?                                                                                         \
                                                    HAL_GPIO_WritePin(ATK_MC5640_RST_GPIO_PORT, ATK_MC5640_RST_GPIO_PIN, GPIO_PIN_SET) :        \
                                                    HAL_GPIO_WritePin(ATK_MC5640_RST_GPIO_PORT, ATK_MC5640_RST_GPIO_PIN, GPIO_PIN_RESET);       \
                                                }while(0)
#define ATK_MC5640_PWDN(x)                      do{ x ?                                                                                         \
                                                    HAL_GPIO_WritePin(ATK_MC5640_PWDN_GPIO_PORT, ATK_MC5640_PWDN_GPIO_PIN, GPIO_PIN_SET) :      \
                                                    HAL_GPIO_WritePin(ATK_MC5640_PWDN_GPIO_PORT, ATK_MC5640_PWDN_GPIO_PIN, GPIO_PIN_RESET);     \
                                                }while(0)
#if (ATK_MC5640_LED_CTL_BY_OV5640 == 0)
#define ATK_MC5640_FLASH(x)                     do{ x ?                                                                                         \
                                                    HAL_GPIO_WritePin(ATK_MC5640_FLASH_GPIO_PORT, ATK_MC5640_FLASH_GPIO_PIN, GPIO_PIN_SET) :    \
                                                    HAL_GPIO_WritePin(ATK_MC5640_FLASH_GPIO_PORT, ATK_MC5640_FLASH_GPIO_PIN, GPIO_PIN_RESET);   \
                                                }while(0)
#endif

/* ATK-MC5640 SCCBͨѶ��ַ */
#define ATK_MC5640_SCCB_ADDR                    0x3C

/* ATK-MC5640ģ��ISP���봰�����ߴ� */
#define ATK_MC5640_ISP_INPUT_WIDTH_MAX          0x0A3F
#define ATK_MC5640_ISP_INPUT_HEIGHT_MAX         0x06A9

/* ATK-MC5640ģ��ƹ�ģʽö�� */
typedef enum
{
    ATK_MC5640_LIGHT_MODE_ADVANCED_AWB = 0x00,  /* Advanced AWB */
    ATK_MC5640_LIGHT_MODE_SIMPLE_AWB,           /* Simple AWB */
    ATK_MC5640_LIGHT_MODE_MANUAL_DAY,           /* Manual day */
    ATK_MC5640_LIGHT_MODE_MANUAL_A,             /* Manual A */
    ATK_MC5640_LIGHT_MODE_MANUAL_CWF,           /* Manual cwf */
    ATK_MC5640_LIGHT_MODE_MANUAL_CLOUDY         /* Manual cloudy */
} atk_mc5640_light_mode_t;

/* ATK-MC5640ģ��ɫ�ʱ��Ͷ�ö�� */
typedef enum
{
    ATK_MC5640_COLOR_SATURATION_0 = 0x00,       /* +4 */
    ATK_MC5640_COLOR_SATURATION_1,              /* +3 */
    ATK_MC5640_COLOR_SATURATION_2,              /* +2 */
    ATK_MC5640_COLOR_SATURATION_3,              /* +1 */
    ATK_MC5640_COLOR_SATURATION_4,              /* 0 */
    ATK_MC5640_COLOR_SATURATION_5,              /* -1 */
    ATK_MC5640_COLOR_SATURATION_6,              /* -2 */
    ATK_MC5640_COLOR_SATURATION_7,              /* -3 */
    ATK_MC5640_COLOR_SATURATION_8,              /* -4 */
} atk_mc5640_color_saturation_t;

/* ATK-MC5640ģ������ö�� */
typedef enum
{
    ATK_MC5640_BRIGHTNESS_0 = 0x00,             /* +4 */
    ATK_MC5640_BRIGHTNESS_1,                    /* +3 */
    ATK_MC5640_BRIGHTNESS_2,                    /* +2 */
    ATK_MC5640_BRIGHTNESS_3,                    /* +1 */
    ATK_MC5640_BRIGHTNESS_4,                    /* 0 */
    ATK_MC5640_BRIGHTNESS_5,                    /* -1 */
    ATK_MC5640_BRIGHTNESS_6,                    /* -2 */
    ATK_MC5640_BRIGHTNESS_7,                    /* -3 */
    ATK_MC5640_BRIGHTNESS_8,                    /* -4 */
} atk_mc5640_brightness_t;

/* ATK-MC5640ģ��Աȶ�ö�� */
typedef enum
{
    ATK_MC5640_CONTRAST_0 = 0x00,               /* +4 */
    ATK_MC5640_CONTRAST_1,                      /* +3 */
    ATK_MC5640_CONTRAST_2,                      /* +2 */
    ATK_MC5640_CONTRAST_3,                      /* +1 */
    ATK_MC5640_CONTRAST_4,                      /* 0 */
    ATK_MC5640_CONTRAST_5,                      /* -1 */
    ATK_MC5640_CONTRAST_6,                      /* -2 */
    ATK_MC5640_CONTRAST_7,                      /* -3 */
    ATK_MC5640_CONTRAST_8,                      /* -4 */
} atk_mc5640_contrast_t;

/* ATK-MC5640ģ��ɫ��ö�� */
typedef enum
{
    ATK_MC5640_HUE_0 = 0x00,                    /* -180 degree */
    ATK_MC5640_HUE_1,                           /* -150 degree */
    ATK_MC5640_HUE_2,                           /* -120 degree */
    ATK_MC5640_HUE_3,                           /* -90 degree */
    ATK_MC5640_HUE_4,                           /* -60 degree */
    ATK_MC5640_HUE_5,                           /* -30 degree */
    ATK_MC5640_HUE_6,                           /* 0 degree */
    ATK_MC5640_HUE_7,                           /* +30 degree */
    ATK_MC5640_HUE_8,                           /* +60 degree */
    ATK_MC5640_HUE_9,                           /* +90 degree */
    ATK_MC5640_HUE_10,                          /* +120 degree */
    ATK_MC5640_HUE_11,                          /* +150 degree */
} atk_mc5640_hue_t;

/* ATK-MC5640ģ������Ч��ö�� */
typedef enum
{
    ATK_MC5640_SPECIAL_EFFECT_NORMAL = 0x00,    /* Normal */
    ATK_MC5640_SPECIAL_EFFECT_BW,               /* B&W */
    ATK_MC5640_SPECIAL_EFFECT_BLUISH,           /* Bluish */
    ATK_MC5640_SPECIAL_EFFECT_SEPIA,            /* Sepia */
    ATK_MC5640_SPECIAL_EFFECT_REDDISH,          /* Reddish */
    ATK_MC5640_SPECIAL_EFFECT_GREENISH,         /* Greenish */
    ATK_MC5640_SPECIAL_EFFECT_NEGATIVE,         /* Negative */
} atk_mc5640_special_effect_t;

/* ATK-MC5640ģ���ع��ö�� */
typedef enum
{
    ATK_MC5640_EXPOSURE_LEVEL_0 = 0x00,         /* -1.7EV */
    ATK_MC5640_EXPOSURE_LEVEL_1,                /* -1.3EV */
    ATK_MC5640_EXPOSURE_LEVEL_2,                /* -1.0EV */
    ATK_MC5640_EXPOSURE_LEVEL_3,                /* -0.7EV */
    ATK_MC5640_EXPOSURE_LEVEL_4,                /* -0.3EV */
    ATK_MC5640_EXPOSURE_LEVEL_5,                /* default */
    ATK_MC5640_EXPOSURE_LEVEL_6,                /* 0.3EV */
    ATK_MC5640_EXPOSURE_LEVEL_7,                /* 0.7EV */
    ATK_MC5640_EXPOSURE_LEVEL_8,                /* 1.0EV */
    ATK_MC5640_EXPOSURE_LEVEL_9,                /* 1.3EV */
    ATK_MC5640_EXPOSURE_LEVEL_10,               /* 1.7EV */
} atk_mc5640_exposure_level_t;

/* ATK-MC5640ģ�����ö�� */
typedef enum
{
    ATK_MC5640_SHARPNESS_OFF = 0x00,            /* Sharpness OFF */
    ATK_MC5640_SHARPNESS_1,                     /* Sharpness 1 */
    ATK_MC5640_SHARPNESS_2,                     /* Sharpness 2 */
    ATK_MC5640_SHARPNESS_3,                     /* Sharpness 3 */
    ATK_MC5640_SHARPNESS_4,                     /* Sharpness 4 */
    ATK_MC5640_SHARPNESS_5,                     /* Sharpness 5 */
    ATK_MC5640_SHARPNESS_6,                     /* Sharpness 6 */
    ATK_MC5640_SHARPNESS_7,                     /* Sharpness 7 */
    ATK_MC5640_SHARPNESS_8,                     /* Sharpness 8 */
    ATK_MC5640_SHARPNESS_AUTO,                  /* Sharpness Auto */
} atk_mc5640_sharpness_t;

/* ATK-MC5640ģ�龵��/��תö�� */
typedef enum
{
    ATK_MC5640_MIRROR_FLIP_0 = 0x00,            /* MIRROR */
    ATK_MC5640_MIRROR_FLIP_1,                   /* FLIP */
    ATK_MC5640_MIRROR_FLIP_2,                   /* MIRROR & FLIP */
    ATK_MC5640_MIRROR_FLIP_3,                   /* Normal */
} atk_mc5640_mirror_flip_t;

/* ATK-MC5640ģ�����ͼ��ö�� */
typedef enum
{
    ATK_MC5640_TEST_PATTERN_OFF = 0x00,         /* OFF */
    ATK_MC5640_TEST_PATTERN_COLOR_BAR,          /* Color bar */
    ATK_MC5640_TEST_PATTERN_COLOR_SQUARE,       /* Color square */
} atk_mc5640_test_pattern_t;

/* ATK-MC5640���ͼ���ʽö�� */
typedef enum
{
    ATK_MC5640_OUTPUT_FORMAT_RGB565 = 0x00,     /* RGB565 */
    ATK_MC5640_OUTPUT_FORMAT_JPEG,              /* JPEG */
} atk_mc5640_output_format_t;

/* ATK-MC5640��ȡ֡���ݷ�ʽö�� */
typedef enum
{
    ATK_MC5640_GET_TYPE_DTS_8B_NOINC = 0x00,    /* ͼ���������ֽڷ�ʽд��Ŀ�ĵ�ַ��Ŀ�ĵ�ַ�̶����� */
    ATK_MC5640_GET_TYPE_DTS_8B_INC,             /* ͼ���������ֽڷ�ʽд��Ŀ�ĵ�ַ��Ŀ�ĵ�ַ�Զ����� */
    ATK_MC5640_GET_TYPE_DTS_16B_NOINC,          /* ͼ�������԰��ַ�ʽд��Ŀ�ĵ�ַ��Ŀ�ĵ�ַ�̶����� */
    ATK_MC5640_GET_TYPE_DTS_16B_INC,            /* ͼ�������԰��ַ�ʽд��Ŀ�ĵ�ַ��Ŀ�ĵ�ַ�Զ����� */
    ATK_MC5640_GET_TYPE_DTS_32B_NOINC,          /* ͼ���������ַ�ʽд��Ŀ�ĵ�ַ��Ŀ�ĵ�ַ�̶����� */
    ATK_MC5640_GET_TYPE_DTS_32B_INC,            /* ͼ���������ַ�ʽд��Ŀ�ĵ�ַ��Ŀ�ĵ�ַ�Զ����� */
} atk_mc5640_get_type_t;

/* ������� */
#define ATK_MC5640_EOK      0   /* û�д��� */
#define ATK_MC5640_ERROR    1   /* ���� */
#define ATK_MC5640_EINVAL   2   /* �Ƿ����� */
#define ATK_MC5640_ENOMEM   3   /* �ڴ治�� */
#define ATK_MC5640_EEMPTY   4   /* ��ԴΪ�� */
#define ATK_MC5640_ETIMEOUT 5   /* ������ʱ */

/* �������� */
uint8_t atk_mc5640_init(void);                                                                              /* ��ʼ��ATK-MC5640ģ�� */
uint8_t atk_mc5640_auto_focus_init(void);                                                                   /* ��ʼ��ATK-MC5640ģ���Զ��Խ� */
uint8_t atk_mc5640_auto_focus_once(void);                                                                   /* ATK-MC5640ģ���Զ��Խ�һ�� */
uint8_t atk_mc5640_auto_focus_continuance(void);                                                            /* ATK-MC5640ģ������Զ��Խ� */
void atk_mc5640_led_on(void);                                                                               /* ����ATK-MC5640ģ������� */
void atk_mc5640_led_off(void);                                                                              /* �ر�ATK-MC5640ģ������� */
uint8_t atk_mc5640_set_light_mode(atk_mc5640_light_mode_t mode);                                            /* ����ATK-MC5640ģ��ƹ�ģʽ */
uint8_t atk_mc5640_set_color_saturation(atk_mc5640_color_saturation_t saturation);                          /* ����ATK-MC5640ģ��ɫ�ʱ��Ͷ� */
uint8_t atk_mc5640_set_brightness(atk_mc5640_brightness_t brightness);                                      /* ����ATK-MC5640ģ������ */
uint8_t atk_mc5640_set_contrast(atk_mc5640_contrast_t contrast);                                            /* ����ATK-MC5640ģ��Աȶ� */
uint8_t atk_mc5640_set_hue(atk_mc5640_hue_t hue);                                                           /* ����ATK-MC5640ģ��ɫ�� */
uint8_t atk_mc5640_set_special_effect(atk_mc5640_special_effect_t effect);                                  /* ����ATK-MC5640ģ������Ч�� */
uint8_t atk_mc5640_set_exposure_level(atk_mc5640_exposure_level_t level);                                   /* ����ATK-MC5640ģ���ع�� */
uint8_t atk_mc5640_set_sharpness_level(atk_mc5640_sharpness_t sharpness);                                   /* ����ATK-MC5640ģ����� */
uint8_t atk_mc5640_set_mirror_flip(atk_mc5640_mirror_flip_t mirror_flip);                                   /* ����ATK-MC5640ģ�龵��/��ת */
uint8_t atk_mc5640_set_test_pattern(atk_mc5640_test_pattern_t pattern);                                     /* ����ATK-MC5640ģ�����ͼ�� */
uint8_t atk_mc5640_set_output_format(atk_mc5640_output_format_t format);                                    /* ����ATK-MC5640ģ�����ͼ���ʽ */
uint8_t atk_mc5640_set_isp_input_window(uint16_t x, uint16_t y, uint16_t width, uint16_t height);           /* ����ATK-MC5640ģ��ISP���봰�ڳߴ� */
uint8_t atk_mc5640_set_pre_scaling_window(uint16_t x_offset, uint16_t y_offset);                            /* ����ATK-MC5640ģ��Ԥ���Ŵ���ƫ�� */
uint8_t atk_mc5640_set_output_size(uint16_t width, uint16_t height);                                        /* ����ATK-MC5640ģ�����ͼ��ߴ� */
uint8_t atk_mc5640_get_frame(uint32_t dts_addr, atk_mc5640_get_type_t type, void (*before_transfer)(void)); /* ��ȡATK-MC5640ģ�������һ֡ͼ������ */

#endif
