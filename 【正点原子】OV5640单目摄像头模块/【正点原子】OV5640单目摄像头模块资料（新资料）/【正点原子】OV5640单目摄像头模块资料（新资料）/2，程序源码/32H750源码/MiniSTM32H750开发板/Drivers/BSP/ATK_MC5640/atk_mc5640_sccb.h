/**
 ****************************************************************************************************
 * @file        atk_mc5640_sccb.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MC5640ģ��SCCB�ӿ���������
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

#ifndef __ATK_MC5640_SCCB_H
#define __ATK_MC5640_SCCB_H

#include "./SYSTEM/sys/sys.h"

/* ����ATK-MC5640��SCCBͨѶ�����Ƿ�������������� */
#define ATK_MC5640_SCCB_GPIO_PULLUP             1

/* ���Ŷ��� */
#define ATK_MC5640_SCCB_SCL_GPIO_PORT           GPIOB
#define ATK_MC5640_SCCB_SCL_GPIO_PIN            GPIO_PIN_10
#define ATK_MC5640_SCCB_SCL_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)
#define ATK_MC5640_SCCB_SDA_GPIO_PORT           GPIOB
#define ATK_MC5640_SCCB_SDA_GPIO_PIN            GPIO_PIN_11
#define ATK_MC5640_SCCB_SDA_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)

/* IO���� */
#define ATK_MC5640_SCCB_SCL(x)                  do{ x ?                                                                                             \
                                                    HAL_GPIO_WritePin(ATK_MC5640_SCCB_SCL_GPIO_PORT, ATK_MC5640_SCCB_SCL_GPIO_PIN, GPIO_PIN_SET) :  \
                                                    HAL_GPIO_WritePin(ATK_MC5640_SCCB_SCL_GPIO_PORT, ATK_MC5640_SCCB_SCL_GPIO_PIN, GPIO_PIN_RESET); \
                                                }while(0)
#define ATK_MC5640_SCCB_SDA(x)                  do{ x ?                                                                                             \
                                                    HAL_GPIO_WritePin(ATK_MC5640_SCCB_SDA_GPIO_PORT, ATK_MC5640_SCCB_SDA_GPIO_PIN, GPIO_PIN_SET) :  \
                                                    HAL_GPIO_WritePin(ATK_MC5640_SCCB_SDA_GPIO_PORT, ATK_MC5640_SCCB_SDA_GPIO_PIN, GPIO_PIN_RESET); \
                                                }while(0)
#define ATK_MC5640_SCCB_READ_SDA()              HAL_GPIO_ReadPin(ATK_MC5640_SCCB_SDA_GPIO_PORT, ATK_MC5640_SCCB_SDA_GPIO_PIN)

/* �������� */
void atk_mc5640_sccb_init(void);                                                        /* ATK-MC5640 SCCB�ӿڳ�ʼ�� */
void atk_mc5640_sccb_3_phase_write(uint8_t id_addr, uint16_t sub_addr, uint8_t dat);    /* SCCB�ӿ�3�׶�д���� */
void atk_mc5640_sccb_2_phase_write(uint8_t id_addr, uint16_t sub_addr);                 /* SCCB�ӿ�2�׶�д���� */
void atk_mc5640_sccb_2_phase_read(uint8_t id_addr, uint8_t *dat);                       /* SCCB�ӿ�2�׶ζ����� */

#endif
