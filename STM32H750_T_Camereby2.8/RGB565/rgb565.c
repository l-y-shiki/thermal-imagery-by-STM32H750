#include "./SYSTEM/sys/sys.h"



//将温度区间以256的分辨率转化为RGB颜色
void GrayToPseColor(uint8_t grayValue, uint8_t *colorR,uint8_t *colorG,uint8_t *colorB)  //灰度-伪彩色变换
{
		if( (grayValue>=0) && (grayValue<=63) )  
		{
			*colorR=0;
			*colorG=0;
			*colorB=round(grayValue*255/64);
		}
		else if( (grayValue>=64) && (grayValue<=127) )  
		{
			*colorR=0;
			*colorG=round((grayValue-64)*255/64);
			*colorB=round((127-grayValue)*255/64);
		}
		else if( (grayValue>=128) && (grayValue<=191) )  
		{
			*colorR=round((grayValue-128)*255/64);
			*colorG=255;
			*colorB=0;
		}
		else if( (grayValue>=192) && (grayValue<=255) )  
		{
			*colorR=255;
			*colorG=round((255-grayValue)*255/64);
			*colorB=0;
		}		
}

unsigned short RGB565(unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned short color = 0;
    color |= (red >> 3) << 11;      //将red的高5位放到color的高5位
    color |= (green >> 2) << 5;     //将green的高6位放到color的中间6位
    color |= blue >> 3;             //将blue的高5位放到color的低5位
    return color;
}