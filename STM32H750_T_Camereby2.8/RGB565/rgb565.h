#ifndef __RGB565_H__
#define __RGB565_H__

#include <math.h>

typedef struct
{
		uint8_t colorR;
		uint8_t colorG;
	  uint8_t colorB;
}RGBcolor;


#define GCM_Pseudo1 1
#define GCM_Pseudo2 2
#define GCM_Metal1 3
#define GCM_Metal2 4
#define GCM_Rainbow1 5
#define GCM_Rainbow2 6
#define GCM_Rainbow3 7
#define GCM_Zhou 8
#define GCM_Ning 9
#define GCM_Gray 10


void GrayToPseColor(uint8_t converMethod,uint8_t grayValue, uint8_t *colorR,uint8_t *colorG,uint8_t *colorB);
unsigned short RGB565(unsigned char red, unsigned char green, unsigned char blue);
uint16_t RGB565_change(uint16_t color1,uint16_t color2,float level);

#endif