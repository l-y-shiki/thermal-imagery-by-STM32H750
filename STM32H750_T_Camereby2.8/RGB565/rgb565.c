#include "./SYSTEM/sys/sys.h"
#include "rgb565.h"


//将温度区间以256的分辨率转化为RGB颜色
//void GrayToPseColor(uint8_t converMethod,uint8_t grayValue, uint8_t *colorR,uint8_t *colorG,uint8_t *colorB)  //灰度-伪彩色变换
//{
//		if( (grayValue>=0) && (grayValue<=63) )  
//		{
//			*colorR=0;
//			*colorG=0;
//			*colorB=round(grayValue*255/64);
//		}
//		else if( (grayValue>=64) && (grayValue<=127) )  
//		{
//			*colorR=0;
//			*colorG=round((grayValue-64)*255/64);
//			*colorB=round((127-grayValue)*255/64);
//		}
//		else if( (grayValue>=128) && (grayValue<=191) )  
//		{
//			*colorR=round((grayValue-128)*255/64);
//			*colorG=255;
//			*colorB=0;
//		}
//		else if( (grayValue>=192) && (grayValue<=255) )  
//		{
//			*colorR=255;
//			*colorG=round((255-grayValue)*255/64);
//			*colorB=0;
//		}		
//}

unsigned short RGB565(unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned short color = 0;
    color |= (red >> 3) << 11;      //将red的高5位放到color的高5位
    color |= (green >> 2) << 5;     //将green的高6位放到color的中间6位
    color |= blue >> 3;             //将blue的高5位放到color的低5位
    return color;
}

uint16_t RGB565_change(uint16_t color1,uint16_t color2,float level)
{
		unsigned short color=0;
	
	    // 提取R、G、B分量
    uint8_t r1 = (color1 >> 11) & 0x1F; // 提取红色分量
    uint8_t g1 = (color1 >> 5) & 0x3F;  // 提取绿色分量
    uint8_t b1 = color1 & 0x1F;         // 提取蓝色分量

    uint8_t r2 = (color2 >> 11) & 0x1F; // 提取红色分量
    uint8_t g2 = (color2 >> 5) & 0x3F;  // 提取绿色分量
    uint8_t b2 = color2 & 0x1F;         // 提取蓝色分量

    // 按分量相加
    uint8_t r_sum =r1*level + r2*(1-level);
    uint8_t g_sum =g1*level + g2*(1-level);
    uint8_t b_sum =b1*level + b2*(1-level);

    // 处理溢出
    r_sum = (r_sum > 31) ? 31 : r_sum; // R最大值为31
    g_sum = (g_sum > 63) ? 63 : g_sum; // G最大值为63
    b_sum = (b_sum > 31) ? 31 : b_sum; // B最大值为31

    // 合成新的RGB565值
     color = (r_sum << 11) | (g_sum << 5) | b_sum;

    return color;
}

//彩色变换模式选择
void GrayToPseColor(uint8_t converMethod,uint8_t grayValue, uint8_t *colorR,uint8_t *colorG,uint8_t *colorB)  //灰度-伪彩色变换
{
	switch(converMethod)
	{
		case GCM_Pseudo1:
			*colorR=abs(0-grayValue);
			*colorG=abs(127-grayValue);
			*colorB=abs(255-grayValue);		
			break;
			
		case GCM_Pseudo2:	
			if( (grayValue>=0) && (grayValue<=63) )  
			{
				*colorR=0;
				*colorG=0;
				*colorB=round(grayValue/64*255);
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
			break;

		case GCM_Metal1:	
			if( (grayValue>=0) && (grayValue<=63) )  
			{
				*colorR=0;
				*colorG=0;
				*colorB=round(grayValue*255/64);
			}
			else if( (grayValue>=64) && (grayValue<=95) )  
			{
				*colorR=round((grayValue-63)*127/32);
				*colorG=round((grayValue-63)*127/32);
				*colorB=255;
			}
			else if( (grayValue>=96) && (grayValue<=127) )  
			{
				*colorR=round((grayValue-95)*127/32)+128;
				*colorG=round((grayValue-95)*127/32)+128;
				*colorB=round((127-grayValue)*255/32);
			}
			else if( (grayValue>=128) && (grayValue<=191) )  
			{
				*colorR=255;
				*colorG=255;
				*colorB=0;
			}
			else if( (grayValue>=192) && (grayValue<=255) )  
			{
				*colorR=255;
				*colorG=255;
				*colorB=round((grayValue-192)*255/64);
			}
			break;
			
		case GCM_Metal2:	
			*colorR=0;*colorG=0;*colorB=0;
			if( (grayValue>=0) && (grayValue<=16) )  
			{
				*colorR=0;
			}
			else if( (grayValue>=17) && (grayValue<=140) )  
			{
				*colorR=round((grayValue-16)*255/(140-16));
			}
			else if( (grayValue>=141) && (grayValue<=255) )  
			{
				*colorR=255;
			}

			if( (grayValue>=0) && (grayValue<=101) )  
			{
				*colorG=0;
			}
			else if( (grayValue>=102) && (grayValue<=218) )  
			{
				*colorG=round((grayValue-101)*255/(218-101));
			}
			else if( (grayValue>=219) && (grayValue<=255) )  
			{
				*colorG=255;
			}

			if( (grayValue>=0) && (grayValue<=91) )  
			{
				*colorB=28+round((grayValue-0)/(91-0)*100);
			}
			else if( (grayValue>=92) && (grayValue<=120) )  
			{
				*colorB=round((120-grayValue)*128/(120-91));
			}
			else if( (grayValue>=129) && (grayValue<=214) )  
			{
				*colorB=0;
			}
			else if( (grayValue>=215) && (grayValue<=255) )  
			{
				*colorB=round((grayValue-214)*255/(255-214));
			}
			break;		
			
		case GCM_Rainbow1:
			if( (grayValue>=0) && (grayValue<=31) )  
			{
				*colorR=0;
				*colorG=0;
				*colorB=round(grayValue/32*255);
			}
			else if( (grayValue>=32) && (grayValue<=63) )  
			{
				*colorR=0;
				*colorG=round((grayValue-32)*255/32);
				*colorB=255;
			}
			else if( (grayValue>=64) && (grayValue<=95) )  
			{
				*colorR=0;
				*colorG=255;
				*colorB=round((95-grayValue)*255/32);
			}
			else if( (grayValue>=96) && (grayValue<=127) )  
			{
				*colorR=round((grayValue-96)*255/32);
				*colorG=255;
				*colorB=0;
			}
			else if( (grayValue>=128) && (grayValue<=191) )  
			{
				*colorR=255;
				*colorG=round((191-grayValue)*255/64);
				*colorB=0;
			}
			else if( (grayValue>=192) && (grayValue<=255) )  
			{
				*colorR=255;
				*colorG=round((grayValue-192)*255/64);//0
				*colorB=round((grayValue-192)*255/64);
			}		
			break;
			
		case GCM_Rainbow2:
			if( (grayValue>=0) && (grayValue<=63) )  
			{
				*colorR=0;
				*colorG=round((grayValue-0)*255/64);
				*colorB=255;
			}
			else if( (grayValue>=64) && (grayValue<=95) )  
			{
				*colorR=0;
				*colorG=255;
				*colorB=round((95-grayValue)*255/32);
			}
			else if( (grayValue>=96) && (grayValue<=127) )  
			{
				*colorR=round((grayValue-96)*255/32);
				*colorG=255;
				*colorB=0;
			}
			else if( (grayValue>=128) && (grayValue<=191) )  
			{
				*colorR=255;
				*colorG=round((191-grayValue)*255/64);
				*colorB=0;
			}
			else if( (grayValue>=192) && (grayValue<=255) )  
			{
				*colorR=255;
				*colorG=round((grayValue-192)*255/64);
				*colorB=round((grayValue-192)*255/64);
			}
			break;
			
		case GCM_Rainbow3:
			if( (grayValue>=0) && (grayValue<=51) )  
			{
				*colorR=0;
				*colorG=grayValue*5;
				*colorB=255;
			}
			else if( (grayValue>=52) && (grayValue<=102) )  
			{
				*colorR=0;
				*colorG=255;
				*colorB=255-(grayValue-51)*5;
			}
			else if( (grayValue>=103) && (grayValue<=153) )  
			{
				*colorR=(grayValue-102)*5;
				*colorG=255;
				*colorB=0;
			}
			else if( (grayValue>=154) && (grayValue<=204) )  
			{
				*colorR=255;
				*colorG=round(255-128*(grayValue-153)/51);
				*colorB=0;
			}
			else if( (grayValue>=205) && (grayValue<=255) )  
			{
				*colorR=255;
				*colorG=round(127-127*(grayValue-204)/51);
				*colorB=0;
			}
			break;
		
		case GCM_Zhou:
			if( (grayValue>=0) && (grayValue<=63) )  
			{
				*colorR=0;
				*colorG=round((64-grayValue)*255/64);
				*colorB=255;
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
			break;
			
		case GCM_Ning:
			if ((grayValue>=0) && (grayValue<=63))  
			{
				*colorR=0;
				*colorG=254-4*grayValue;
				*colorB=255;
			}
			else if ((grayValue>=64) && (grayValue<=127))  
			{
				*colorR=0;
				*colorG=4*grayValue-254;
				*colorB=510-4*grayValue;
			}
			else if ((grayValue>=128) && (grayValue<=191))  
			{
				*colorR=4*grayValue-510;
				*colorG=255;
				*colorB=0;
			}
			else if ((grayValue>=192) && (grayValue<=255))  
			{
				*colorR=255;
				*colorG=1022-4*grayValue;
				*colorB=0;
			}
			break;
			
		case GCM_Gray:	
			*colorR=grayValue;
			*colorG=grayValue;
			*colorB=grayValue;
			break;
			
		default:
			break;
	}
}