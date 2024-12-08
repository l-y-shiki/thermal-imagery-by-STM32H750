

#ifndef _SPI_H_
#define _SPI_H_
#include "./SYSTEM/sys/sys.h"


#define SPI2_SPI SPI2
#define SPI2_SPI_CLK_ENABLE() do { __HAL_RCC_SPI2_CLK_ENABLE(); } while (0)
#define SPI2_SCK_GPIO_PORT GPIOB
#define SPI2_SCK_GPIO_PIN GPIO_PIN_13
#define SPI2_SCK_GPIO_AF GPIO_AF5_SPI2
#define SPI2_SCK_GPIO_CLK_ENABLE()  do { __HAL_RCC_GPIOB_CLK_ENABLE(); } while (0)
#define SPI2_MISO_GPIO_PORT GPIOB
#define SPI2_MISO_GPIO_PIN GPIO_PIN_14
#define SPI2_MISO_GPIO_AF GPIO_AF5_SPI2
#define SPI2_MISO_GPIO_CLK_ENABLE() do { __HAL_RCC_GPIOB_CLK_ENABLE(); } while (0)
#define SPI2_MOSI_GPIO_PORT GPIOB
#define SPI2_MOSI_GPIO_PIN GPIO_PIN_15
#define SPI2_MOSI_GPIO_AF GPIO_AF5_SPI2
#define SPI2_MOSI_GPIO_CLK_ENABLE() do { __HAL_RCC_GPIOB_CLK_ENABLE(); } while (0)

/* 函数声明 */
void spi2_init(void);                           /* 初始化SPI2 */
void spi2_set_speed(uint32_t speed);            /* 设置SPI2通信波特率 */
uint8_t spi2_read_write_byte(uint8_t txdata);   /* SPI2读写一字节数据 */


#endif
//SCK GPIOB_13
//MISO GPIOB_14
//MOSI GPIOB_15
