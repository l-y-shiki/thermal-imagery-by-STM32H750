/**
 ****************************************************************************************************
 * @file        atk_mc5640_sccb.c
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

#include "./BSP/ATK_MC5640/atk_mc5640_sccb.h"
#include "./SYSTEM/delay/delay.h"

/* SCCB�ӿڶ�дͨѶ��ַbit0 */
#define ATK_MC5640_SCCB_WRITE   0x00
#define ATK_MC5640_SCCB_READ    0x01

#if (ATK_MC5640_SCCB_GPIO_PULLUP != 0)
/**
 * @brief       ����SCCB�ӿ�SDA����Ϊ���ģʽ
 * @param       ��
 * @retval      ��
 */
static void atk_mc5640_sccb_set_sda_output(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};
    
    gpio_init_struct.Pin    = ATK_MC5640_SCCB_SDA_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MC5640_SCCB_SDA_GPIO_PORT, &gpio_init_struct);
}

/**
 * @brief       ����SCCB�ӿ�SDA����Ϊ����ģʽ
 * @param       ��
 * @retval      ��
 */
static void atk_mc5640_sccb_set_sda_input(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};
    
    gpio_init_struct.Pin    = ATK_MC5640_SCCB_SDA_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_INPUT;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MC5640_SCCB_SDA_GPIO_PORT, &gpio_init_struct);
}
#endif

/**
 * @brief       SCCB�ӿ���ʱ����
 * @param       ��
 * @retval      ��
 */
static inline void atk_mc5640_sccb_delay(void)
{
    delay_us(5);
}

/**
 * @brief       SCCB�ӿ���ʼ�ź�
 * @param       ��
 * @retval      ��
 */
static void atk_mc5640_sccb_start(void)
{
    ATK_MC5640_SCCB_SDA(1);
    ATK_MC5640_SCCB_SCL(1);
    atk_mc5640_sccb_delay();
    ATK_MC5640_SCCB_SDA(0);
    atk_mc5640_sccb_delay();
    ATK_MC5640_SCCB_SCL(0);
}

/**
 * @brief       SCCB�ӿ�ֹͣ�ź�
 * @param       ��
 * @retval      ��
 */
static void atk_mc5640_sccb_stop(void)
{
    ATK_MC5640_SCCB_SDA(0);
    atk_mc5640_sccb_delay();
    ATK_MC5640_SCCB_SCL(1);
    atk_mc5640_sccb_delay();
    ATK_MC5640_SCCB_SDA(1);
    atk_mc5640_sccb_delay();
}

/**
 * @brief       SCCB�ӿ�������ӻ�д��һ���ֽ�����
 * @param       dat: ��д���һ���ֽ�����
 * @retval      ��
 */
static void atk_mc5640_sccb_write_byte(uint8_t dat)
{
    int8_t dat_index;
    uint8_t dat_bit;
    
    for (dat_index=7; dat_index>=0; dat_index--)
    {
        dat_bit = (dat >> dat_index) & 0x01;
        ATK_MC5640_SCCB_SDA(dat_bit);
        atk_mc5640_sccb_delay();
        ATK_MC5640_SCCB_SCL(1);
        atk_mc5640_sccb_delay();
        ATK_MC5640_SCCB_SCL(0);
    }
    
    ATK_MC5640_SCCB_SDA(1);
    atk_mc5640_sccb_delay();
    ATK_MC5640_SCCB_SCL(1);
    atk_mc5640_sccb_delay();
    ATK_MC5640_SCCB_SCL(0);
}

/**
 * @brief       SCCB�ӿ�������ӻ���ȡһ���ֽ�����
 * @param       dat: ��ȡ����һ���ֽ�����
 * @retval      ��
 */
static void atk_mc5640_sccb_read_byte(uint8_t *dat)
{
    int8_t dat_index;
    uint8_t dat_bit;
    
#if (ATK_MC5640_SCCB_GPIO_PULLUP != 0)
    atk_mc5640_sccb_set_sda_input();
#endif
    
    for (dat_index=7; dat_index>=0; dat_index--)
    {
        atk_mc5640_sccb_delay();
        ATK_MC5640_SCCB_SCL(1);
        dat_bit = ATK_MC5640_SCCB_READ_SDA();
        *dat |= (dat_bit << dat_index);
        atk_mc5640_sccb_delay();
        ATK_MC5640_SCCB_SCL(0);
    }
    
    atk_mc5640_sccb_delay();
    ATK_MC5640_SCCB_SCL(1);
    atk_mc5640_sccb_delay();
    ATK_MC5640_SCCB_SCL(0);
    atk_mc5640_sccb_delay();
    ATK_MC5640_SCCB_SDA(0);
    atk_mc5640_sccb_delay();
    
#if (ATK_MC5640_SCCB_GPIO_PULLUP != 0)
    atk_mc5640_sccb_set_sda_output();
#endif
}

/**
 * @brief       ATK-MC5640 SCCB�ӿڳ�ʼ��
 * @param       ��
 * @retval      ��
 */
void atk_mc5640_sccb_init(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};
    
    /* ʹ��SCL��SDA����GPIOʱ�� */
    ATK_MC5640_SCCB_SCL_GPIO_CLK_ENABLE();
    ATK_MC5640_SCCB_SDA_GPIO_CLK_ENABLE();
    
    /* ��ʼ��SCL���� */
    gpio_init_struct.Pin    = ATK_MC5640_SCCB_SCL_GPIO_PIN;
    gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MC5640_SCCB_SCL_GPIO_PORT, &gpio_init_struct);
    
    /* ��ʼ��SDA���� */
    gpio_init_struct.Pin    = ATK_MC5640_SCCB_SDA_GPIO_PIN;
#if (ATK_MC5640_SCCB_GPIO_PULLUP != 0)
    gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_PP;
#else
    gpio_init_struct.Mode   = GPIO_MODE_OUTPUT_OD;
#endif
    gpio_init_struct.Pull   = GPIO_PULLUP;
    gpio_init_struct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MC5640_SCCB_SDA_GPIO_PORT, &gpio_init_struct);
    
    atk_mc5640_sccb_stop();
}

/**
 * @brief       SCCB�ӿ�3�׶�д����
 * @param       id_addr : ID Address
 *              sub_addr: Sub-address
 *              dat     : Write Data
 * @retval      ��
 */
void atk_mc5640_sccb_3_phase_write(uint8_t id_addr, uint16_t sub_addr, uint8_t dat)
{
    atk_mc5640_sccb_start();
    atk_mc5640_sccb_write_byte((id_addr << 1) | ATK_MC5640_SCCB_WRITE);
    atk_mc5640_sccb_write_byte((uint8_t)(sub_addr >> 8) & 0xFF);
    atk_mc5640_sccb_write_byte((uint8_t)sub_addr & 0xFF);
    atk_mc5640_sccb_write_byte(dat);
    atk_mc5640_sccb_stop();
}

/**
 * @brief       SCCB�ӿ�2�׶�д����
 * @param       id_addr : ID Address
 *              sub_addr: Sub-address
 * @retval      ��
 */
void atk_mc5640_sccb_2_phase_write(uint8_t id_addr, uint16_t sub_addr)
{
    atk_mc5640_sccb_start();
    atk_mc5640_sccb_write_byte((id_addr << 1) | ATK_MC5640_SCCB_WRITE);
    atk_mc5640_sccb_write_byte((uint8_t)(sub_addr >> 8) & 0xFF);
    atk_mc5640_sccb_write_byte((uint8_t)sub_addr & 0xFF);
    atk_mc5640_sccb_stop();
}

/**
 * @brief       SCCB�ӿ�2�׶ζ�����
 * @param       id_addr: ID Address
 *              dat: ��ȡ��������
 * @retval      ��
 */
void atk_mc5640_sccb_2_phase_read(uint8_t id_addr, uint8_t *dat)
{
    atk_mc5640_sccb_start();
    atk_mc5640_sccb_write_byte((id_addr << 1) | ATK_MC5640_SCCB_READ);
    atk_mc5640_sccb_read_byte(dat);
    atk_mc5640_sccb_stop();
}
