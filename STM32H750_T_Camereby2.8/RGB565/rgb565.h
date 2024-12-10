#ifndef __RGB565_H__
#define __RGB565_H__

void GrayToPseColor(uint8_t grayValue, uint8_t *colorR,uint8_t *colorG,uint8_t *colorB);
unsigned short RGB565(unsigned char red, unsigned char green, unsigned char blue);

#endif