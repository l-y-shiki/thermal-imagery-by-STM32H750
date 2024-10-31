/**
 ****************************************************************************************************
 * @file        atk_mc5640_dcmi.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MC5640ģ��DCMI�ӿ���������
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

#ifndef __ATK_MC5640_DCMI_H
#define __ATK_MC5640_DCMI_H

#include "./BSP/ATK_MC5640/atk_mc5640.h"

/* DCMI�ӿڶ��� */
#define ATK_MC5640_DCMI_INTERFACE                   DCMI
#define ATK_MC5640_DCMI_IRQn                        DCMI_IRQn
#define ATK_MC5640_DCMI_IRQHandler                  DCMI_IRQHandler
#define ATK_MC5640_DCMI_CLK_ENABLE()                do{ __HAL_RCC_DCMI_CLK_ENABLE(); }while(0)

/* DCMI�ӿ�DMA���� */
#define ATK_MC5640_DCMI_DMA_INTERFACE               DMA2_Stream1
#define ATK_MC5640_DCMI_DMA_IRQn                    DMA2_Stream1_IRQn
#define ATK_MC5640_DCMI_DMA_IRQHandler              DMA2_Stream1_IRQHandler
#define ATK_MC5640_DCMI_DMA_CLK_ENABLE()            do{ __HAL_RCC_DMA2_CLK_ENABLE(); }while(0)

/* ���Ŷ��� */
#define ATK_MC5640_DCMI_VSYNC_GPIO_PORT             GPIOB
#define ATK_MC5640_DCMI_VSYNC_GPIO_PIN              GPIO_PIN_7
#define ATK_MC5640_DCMI_VSYNC_GPIO_AF               GPIO_AF13_DCMI
#define ATK_MC5640_DCMI_VSYNC_GPIO_CLK_ENABLE()     do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)
#define ATK_MC5640_DCMI_HSYNC_GPIO_PORT             GPIOA
#define ATK_MC5640_DCMI_HSYNC_GPIO_PIN              GPIO_PIN_4
#define ATK_MC5640_DCMI_HSYNC_GPIO_AF               GPIO_AF13_DCMI
#define ATK_MC5640_DCMI_HSYNC_GPIO_CLK_ENABLE()     do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)
#define ATK_MC5640_DCMI_PIXCLK_GPIO_PORT            GPIOA
#define ATK_MC5640_DCMI_PIXCLK_GPIO_PIN             GPIO_PIN_6
#define ATK_MC5640_DCMI_PIXCLK_GPIO_AF              GPIO_AF13_DCMI
#define ATK_MC5640_DCMI_PIXCLK_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)
#define ATK_MC5640_DCMI_D0_GPIO_PORT                GPIOC
#define ATK_MC5640_DCMI_D0_GPIO_PIN                 GPIO_PIN_6
#define ATK_MC5640_DCMI_D0_GPIO_AF                  GPIO_AF13_DCMI
#define ATK_MC5640_DCMI_D0_GPIO_CLK_ENABLE()        do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)
#define ATK_MC5640_DCMI_D1_GPIO_PORT                GPIOC
#define ATK_MC5640_DCMI_D1_GPIO_PIN                 GPIO_PIN_7
#define ATK_MC5640_DCMI_D1_GPIO_AF                  GPIO_AF13_DCMI
#define ATK_MC5640_DCMI_D1_GPIO_CLK_ENABLE()        do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)
#define ATK_MC5640_DCMI_D2_GPIO_PORT                GPIOC
#define ATK_MC5640_DCMI_D2_GPIO_PIN                 GPIO_PIN_8
#define ATK_MC5640_DCMI_D2_GPIO_AF                  GPIO_AF13_DCMI
#define ATK_MC5640_DCMI_D2_GPIO_CLK_ENABLE()        do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)
#define ATK_MC5640_DCMI_D3_GPIO_PORT                GPIOC
#define ATK_MC5640_DCMI_D3_GPIO_PIN                 GPIO_PIN_9
#define ATK_MC5640_DCMI_D3_GPIO_AF                  GPIO_AF13_DCMI
#define ATK_MC5640_DCMI_D3_GPIO_CLK_ENABLE()        do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)
#define ATK_MC5640_DCMI_D4_GPIO_PORT                GPIOC
#define ATK_MC5640_DCMI_D4_GPIO_PIN                 GPIO_PIN_11
#define ATK_MC5640_DCMI_D4_GPIO_AF                  GPIO_AF13_DCMI
#define ATK_MC5640_DCMI_D4_GPIO_CLK_ENABLE()        do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)
#define ATK_MC5640_DCMI_D5_GPIO_PORT                GPIOD
#define ATK_MC5640_DCMI_D5_GPIO_PIN                 GPIO_PIN_3
#define ATK_MC5640_DCMI_D5_GPIO_AF                  GPIO_AF13_DCMI
#define ATK_MC5640_DCMI_D5_GPIO_CLK_ENABLE()        do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)
#define ATK_MC5640_DCMI_D6_GPIO_PORT                GPIOB
#define ATK_MC5640_DCMI_D6_GPIO_PIN                 GPIO_PIN_8
#define ATK_MC5640_DCMI_D6_GPIO_AF                  GPIO_AF13_DCMI
#define ATK_MC5640_DCMI_D6_GPIO_CLK_ENABLE()        do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)
#define ATK_MC5640_DCMI_D7_GPIO_PORT                GPIOB
#define ATK_MC5640_DCMI_D7_GPIO_PIN                 GPIO_PIN_9
#define ATK_MC5640_DCMI_D7_GPIO_AF                  GPIO_AF13_DCMI
#define ATK_MC5640_DCMI_D7_GPIO_CLK_ENABLE()        do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)

/* ������� */
#define ATK_MC5640_DCMI_EOK     0   /* û�д��� */
#define ATK_MC5640_DCMI_ERROR   1   /* ���� */
#define ATK_MC5640_DCMI_EINVAL  2   /* �Ƿ����� */

/* �������� */
void atk_mc5640_dcmi_init(void);                                                                            /* ��ʼ��ATK-MC5640ģ��DCMI�ӿ� */
void atk_mc5640_dcmi_start(uint32_t dts_addr, uint32_t meminc, uint32_t memdataalignment, uint32_t len);    /* ��ʼATK-MC5640ģ��DCMI�ӿ�DMA���� */

#endif

