#ifndef _MYMLX90640_H_
#define _MYMLX90640_H_

#include "./SYSTEM/sys/sys.h"


#define  FPS2HZ   0x02
#define  FPS4HZ   0x03
#define  FPS8HZ   0x04
#define  FPS16HZ  0x05
#define  FPS32HZ  0x06

#define  MLX90640_ADDR 0x33
#define	 RefreshRate FPS4HZ  //推荐用4Hz，干扰少
#define  TA_SHIFT 8          //Default shift for MLX90640 in open air

//typedef struct
//{
//		uint8_t colorR;
//		uint8_t colorG;
//	  uint8_t colorB;
//}RGBcolor;
//RGBcolor color1;
//RGBcolor *color=&color1;


void	MLX90640_Init(void);
uint8_t Mlx90640_Get_Frame(void);
void GrayToPseColor(uint8_t grayValue, uint8_t *colorR,uint8_t *colorG,uint8_t *colorB);  //灰度-伪彩色变换
unsigned short RGB565(unsigned char red, unsigned char green, unsigned char blue);
void Disp_Temp_Pic(void);
void FSMC_DMA_Init();
void display();

#endif
