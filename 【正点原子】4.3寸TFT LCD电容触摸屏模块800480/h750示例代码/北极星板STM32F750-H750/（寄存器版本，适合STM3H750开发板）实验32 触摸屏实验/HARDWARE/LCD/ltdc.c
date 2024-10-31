#include "lcd.h"
#include "ltdc.h" 
#include "delay.h" 
#include "sdram.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32H7������
//LTDC ����	   	    
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2019/4/21
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved	 
//********************************************************************************
//20190514 V1.1
//�޸ĸ�����Ļ��ʱ������,�ĳ�25��Ƶ(1Mhzʱ��Դ)
//////////////////////////////////////////////////////////////////////////////////	 

//���ݲ�ͬ����ɫ��ʽ,����֡��������
#if LCD_PIXFORMAT==LCD_PIXFORMAT_ARGB8888||LCD_PIXFORMAT==LCD_PIXFORMAT_RGB888 
	u32 ltdc_lcd_framebuf[1280][800] __attribute__((at(LCD_FRAME_BUF_ADDR)));	//����������ֱ���ʱ,LCD�����֡���������С
#else
	u16 ltdc_lcd_framebuf[1280][800] __attribute__((at(LCD_FRAME_BUF_ADDR)));	//����������ֱ���ʱ,LCD�����֡���������С
#endif
	
u32 *ltdc_framebuf[2];					//LTDC LCD֡��������ָ��,����ָ���Ӧ��С���ڴ�����
_ltdc_dev lcdltdc;						//����LCD LTDC����Ҫ����

//LTDC����
//sw:1,��;0,�ر�
void LTDC_Switch(u8 sw)
{
	if(sw==1)LTDC->GCR|=1<<0;			//��LTDC
	else if(sw==0)LTDC->GCR&=~(1<<0);	//�ر�LTDC
}

//����ָ����
//layerx:���;0,��һ��;1,�ڶ���;
//sw:1,��;0,�ر�
void LTDC_Layer_Switch(u8 layerx,u8 sw)
{
	if(sw==1)
	{
		if(layerx==0)LTDC_Layer1->CR|=1<<0;		//������1
		else LTDC_Layer2->CR|=1<<0;				//������2
	}else if(sw==0)  
	{
		if(layerx==0)LTDC_Layer1->CR&=~(1<<0);	//�رղ�1
		else LTDC_Layer2->CR&=~(1<<0);			//�رղ�2
	}
	LTDC->SRCR|=1<<0;							//���¼�������
}

//ѡ���
//layerx:���;0,��һ��;1,�ڶ���;
void LTDC_Select_Layer(u8 layerx)
{
	lcdltdc.activelayer=layerx;
}

//����LCD��ʾ����
//dir:0,������1,����
void LTDC_Display_Dir(u8 dir)
{
    lcdltdc.dir=dir; 	//��ʾ����
	if(dir==0)			//����
	{
		lcdltdc.width=lcdltdc.pheight;
		lcdltdc.height=lcdltdc.pwidth;	
	}else if(dir==1)	//����
	{
		lcdltdc.width=lcdltdc.pwidth;
		lcdltdc.height=lcdltdc.pheight;
	}
}

//���㺯��
//x,y:д������
//color:��ɫֵ
void LTDC_Draw_Point(u16 x,u16 y,u32 color)
{ 
#if LCD_PIXFORMAT==LCD_PIXFORMAT_ARGB8888||LCD_PIXFORMAT==LCD_PIXFORMAT_RGB888
	if(lcdltdc.dir)	//����
	{
        *(u32*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*y+x))=color;
	}else 			//����
	{
        *(u32*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-x-1)+y))=color; 
	}
#else
	if(lcdltdc.dir)	//����
	{
        *(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*y+x))=color;
	}else 			//����
	{
        *(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-x-1)+y))=color; 
	}
#endif
}

//���㺯��
//x,y:��ȡ�������
//����ֵ:��ɫֵ
u32 LTDC_Read_Point(u16 x,u16 y)
{ 
#if LCD_PIXFORMAT==LCD_PIXFORMAT_ARGB8888||LCD_PIXFORMAT==LCD_PIXFORMAT_RGB888
	if(lcdltdc.dir)	//����
	{
		return *(u32*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*y+x));
	}else 			//����
	{
		return *(u32*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-x-1)+y)); 
	}
#else
	if(lcdltdc.dir)	//����
	{
		return *(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*y+x));
	}else 			//����
	{
		return *(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-x-1)+y)); 
	}
#endif 
}

//LTDC������,DMA2D���
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//ע��:sx,ex,���ܴ���lcddev.width-1;sy,ey,���ܴ���lcddev.height-1!!!
//color:Ҫ������ɫ
void LTDC_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color)
{ 
	u32 psx,psy,pex,pey;	//��LCD���Ϊ��׼������ϵ,����������仯���仯
	u32 timeout=0; 
	u16 offline;
	u32 addr; 
	//����ϵת��
	if(lcdltdc.dir)	//����
	{
		psx=sx;psy=sy;
		pex=ex;pey=ey;
	}else			//����
	{
		psx=sy;psy=lcdltdc.pheight-ex-1;
		pex=ey;pey=lcdltdc.pheight-sx-1;
	} 
	offline=lcdltdc.pwidth-(pex-psx+1);
	addr=((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*psy+psx));
	RCC->AHB3ENR|=1<<4;				//ʹ��DMA2Dʱ��
	DMA2D->CR&=~(1<<0);				//��ֹͣDMA2D
	DMA2D->CR=3<<16;				//�Ĵ������洢��ģʽ
	DMA2D->OPFCCR=LCD_PIXFORMAT;	//������ɫ��ʽ
	DMA2D->OOR=offline;				//������ƫ�� 
	DMA2D->OMAR=addr;				//����洢����ַ
	DMA2D->NLR=(pey-psy+1)|((pex-psx+1)<<16);	//�趨�����Ĵ���
	DMA2D->OCOLR=color;				//�趨�����ɫ�Ĵ��� 
	DMA2D->CR|=1<<0;				//����DMA2D
	while((DMA2D->ISR&(1<<1))==0)	//�ȴ��������
	{
		timeout++;
		if(timeout>0X1FFFFF)break;	//��ʱ�˳�
	}  
	DMA2D->IFCR|=1<<1;				//���������ɱ�־ 	
}

//��ָ�����������ָ����ɫ��,DMA2D���	
//�˺�����֧��u16,RGB565��ʽ����ɫ�������.
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)  
//ע��:sx,ex,���ܴ���lcddev.width-1;sy,ey,���ܴ���lcddev.height-1!!!
//color:Ҫ������ɫ�����׵�ַ
void LTDC_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{
	u32 psx,psy,pex,pey;	//��LCD���Ϊ��׼������ϵ,����������仯���仯
	u32 timeout=0; 
	u16 offline;
	u32 addr; 
	//����ϵת��
	if(lcdltdc.dir)	//����
	{
		psx=sx;psy=sy;
		pex=ex;pey=ey;
	}else			//����
	{
		psx=sy;psy=lcdltdc.pheight-ex-1;
		pex=ey;pey=lcdltdc.pheight-sx-1;
	}
	offline=lcdltdc.pwidth-(pex-psx+1);
	addr=((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*psy+psx));
	RCC->AHB3ENR|=1<<4;				//ʹ��DMA2Dʱ��
	DMA2D->CR&=~(1<<0);				//��ֹͣDMA2D
	DMA2D->CR=0<<16;				//�洢�����洢��ģʽ
	DMA2D->FGPFCCR=LCD_PIXFORMAT;	//������ɫ��ʽ
	DMA2D->FGOR=0;					//ǰ������ƫ��Ϊ0
	DMA2D->OOR=offline;				//������ƫ�� 
	DMA2D->FGMAR=(u32)color;		//Դ��ַ
	DMA2D->OMAR=addr;				//����洢����ַ
	DMA2D->NLR=(pey-psy+1)|((pex-psx+1)<<16);	//�趨�����Ĵ��� 
	DMA2D->CR|=1<<0;				//����DMA2D
	while((DMA2D->ISR&(1<<1))==0)	//�ȴ��������
	{
		timeout++;
		if(timeout>0X1FFFFF)break;	//��ʱ�˳�
	} 
	DMA2D->IFCR|=1<<1;				//���������ɱ�־  	
}  

//LCD����
//color:��ɫֵ
void LTDC_Clear(u32 color)
{
	LTDC_Fill(0,0,lcdltdc.width-1,lcdltdc.height-1,color);
}

//LTDCʱ��(Fdclk)���ú���
//Fvco=Fs*(pll3n/pll3m); 
//Fr=Fvco/pll3r=Fs*(pll3n/(pll3m*pll3r));
//Fdclk=Fr;

//Fvco:VCOƵ��
//Fr:PLL3��r��Ƶ���ʱ��Ƶ��
//Fs:PLL3����ʱ��Ƶ��,������HSI,CSI,HSE��. 
//Fdclk:LTDC������ʱ��

//pll3n:PLL3��Ƶϵ��(PLL��Ƶ),ȡֵ��Χ:4~512.
//pll3m:PLL3Ԥ��Ƶϵ��(��PLL֮ǰ�ķ�Ƶ),ȡֵ��Χ:2~63.
//pll3r:PLL3��r��Ƶϵ��(PLL֮��ķ�Ƶ),ȡֵ��Χ:1~128.

//��ΪLTDC��SAI����pll3,����,��ʹ��SAI��ʱ��,���л���ͬ��pll3n(һ���
//����300),����LTDC��pllnҲ�仯��,�����������Ϊ300,�õ�������ʱ����
//����SAI�޸�pll3n��ʱ��,ֻ���,����С.���ή��LCD��ˢ��Ƶ��.
//���,LTDC,����һ������pll3m=25,pll3n=300,����,���Եõ�Fvco=300Mhz  
//Ȼ��,ֻ��Ҫͨ���޸�pll3r,��ƥ�䲻ͬ��Һ����ʱ�򼴿�.

//����:Ҫ�õ�33M��LTDCʱ��,���������:pll3n=300,pll3m=25,pll3r=9
//Fdclk=300*(25/25)/9=300/9=33.33Mhz
//����ֵ:0,�ɹ�;1,ʧ�ܡ�
u8 LTDC_Clk_Set(u32 pll3n,u32 pll3m,u32 pll3r)
{ 
	u16 retry=0;
	u8 status=0; 
	RCC->CR&=~(1<<28);				//�ر�PLL3ʱ�� 
	while(((RCC->CR&(1<<29)))&&(retry<0X1FFF))retry++;//�ȴ�PLL3ʱ��ʧ��
 	if(retry==0X1FFF)status=1;		//PLL3ʱ�ӹر�ʧ��
	else   
	{ 
		RCC->PLLCKSELR&=~(0X3F<<20);	//���DIVM3[5:0]ԭ��������
		RCC->PLLCKSELR|=pll3m<<20;		//DIVM3[5:0]=25,����PLL3��Ԥ��Ƶϵ��
		RCC->PLL3DIVR&=~(0X1FF<<0);		//���DIVN3[8:0]ԭ��������
		RCC->PLL3DIVR|=(pll3n-1)<<0;	//DIVN3[8:0]=pll3n-1,����PLL3�ı�Ƶϵ��,����ֵ���1
		RCC->PLL3DIVR&=~(0X7F<<24);		//���DIVR2[6:0]ԭ��������
		RCC->PLL3DIVR|=(pll3r-1)<<24;	//DIVR3[8:0]=pll3r-1,����PLL3�ı�Ƶϵ��,����ֵ���1  

		RCC->PLLCFGR&=~(0X0F<<8);		//���PLL3RGE[1:0]/PLL3VCOSEL/PLL3FRACEN������
		RCC->PLLCFGR|=0<<10;			//PLL3RGE[1:0]=0,PLL3����ʱ��Ƶ����1~2Mhz֮��(25/25=1Mhz)
		RCC->PLLCFGR|=0<<9;				//PLL3VCOSEL=0,PLL3���VCO��Χ,192~836Mhz 
		RCC->PLLCFGR|=1<<24;			//DIVR3EN=1,ʹ��pll3_r_ck  
		RCC->CR|=1<<28;					//PLL3ON=1,ʹ��PLL3
		while(((RCC->CR&(1<<29))==0)&&(retry<0X1FFF))retry++;	//�ȴ�PLL3ʱ������
		if(retry==0X1FFF)status=2;	
 	} 
	return status;
}      
//LTDC,�㴰������,������LCD�������ϵΪ��׼
//ע��:�˺���������LTDC_Layer_Parameter_Config֮��������.����,�����õĴ���ֵ���������ĳ�
//��ʱ,GRAM�Ĳ���(��/д�㺯��),ҲҪ���ݴ��ڵĿ���������޸�,������ʾ������(�����̾�δ���޸�).
//layerx:��ֵ,0/1.
//sx,sy:��ʼ����
//width,height:��Ⱥ͸߶�
//pixformat:��ɫ��ʽ.0,ARGB8888;1,RGB888;2,RGB565;3,ARGB1555;4,ARGB4444;5,L8;6;AL44;7;AL88
void LTDC_Layer_Window_Config(u8 layerx,u16 sx,u16 sy,u16 width,u16 height,u8 pixformat)
{
	u32 temp; 
	if(layerx==0)
	{
		temp=(sx+width+((LTDC->BPCR&0X0FFF0000)>>16))<<16; 
		LTDC_Layer1->WHPCR=(sx+((LTDC->BPCR&0X0FFF0000)>>16)+1)|temp;//��������ʼ�ͽ���λ�� 
		temp=(sy+height+(LTDC->BPCR&0X7FF))<<16; 
		LTDC_Layer1->WVPCR=(sy+(LTDC->BPCR&0X7FF)+1)|temp;		//��������ʼ�ͽ���λ��
		if(pixformat==0)temp=4;									//ARGB8888,һ����4���ֽ�
		else if(pixformat==1)temp=3;							//RGB888,һ����3���ֽ�
		else if(pixformat==5||pixformat==6)temp=1;				//L8/AL44,һ����1���ֽ�
		else temp=2;											//������ʽ,һ����2���ֽ�
		LTDC_Layer1->CFBLR=(width*temp<<16)|(width*temp+7);		//֡��������������(�ֽ�Ϊ��λ)
		LTDC_Layer1->CFBLNR=height;								//֡��������������	
	}else
	{
		temp=(sx+width+((LTDC->BPCR&0X0FFF0000)>>16))<<16; 
		LTDC_Layer2->WHPCR=(sx+((LTDC->BPCR&0X0FFF0000)>>16)+1)|temp;//��������ʼ�ͽ���λ�� 
		temp=(sy+height+(LTDC->BPCR&0X7FF))<<16; 
		LTDC_Layer2->WVPCR=(sy+(LTDC->BPCR&0X7FF)+1)|temp;		//��������ʼ�ͽ���λ��
		if(pixformat==0)temp=4;									//ARGB8888,һ����4���ֽ�
		else if(pixformat==1)temp=3;							//RGB888,һ����3���ֽ�
		else if(pixformat==5||pixformat==6)temp=1;				//L8/AL44,һ����1���ֽ�
		else temp=2;											//������ʽ,һ����2���ֽ�
		LTDC_Layer2->CFBLR=(width*temp<<16)|(width*temp+7);		//֡��������������(�ֽ�Ϊ��λ)
		LTDC_Layer2->CFBLNR=height;								//֡��������������	
	}	 
	LTDC_Layer_Switch(layerx,1);								//��ʹ��		
}

//LTDC,������������.
//ע��:�˺���,������LTDC_Layer_Window_Config֮ǰ����.
//layerx:��ֵ,0/1.
//bufaddr:����ɫ֡������ʼ��ַ
//pixformat:��ɫ��ʽ.0,ARGB8888;1,RGB888;2,RGB565;3,ARGB1555;4,ARGB4444;5,L8;6;AL44;7;AL88
//alpha:����ɫAlphaֵ,0,ȫ͸��;255,��͸��
//alpha0:Ĭ����ɫAlphaֵ,0,ȫ͸��;255,��͸��
//bfac1:���ϵ��1,4(100),�㶨��Alpha;6(101),����Alpha*�㶨Alpha
//bfac2:���ϵ��2,5(101),�㶨��Alpha;7(111),����Alpha*�㶨Alpha
//bkcolor:��Ĭ����ɫ,32λ,��24λ��Ч,RGB888��ʽ
//����ֵ:��
void LTDC_Layer_Parameter_Config(u8 layerx,u32 bufaddr,u8 pixformat,u8 alpha,u8 alpha0,u8 bfac1,u8 bfac2,u32 bkcolor)
{
	if(layerx==0)
	{
		LTDC_Layer1->CFBAR=bufaddr;					//���ò���ɫ֡������ʼ��ַ
		LTDC_Layer1->PFCR=pixformat;				//���ò���ɫ��ʽ
		LTDC_Layer1->CACR=alpha;					//���ò���ɫAlphaֵ,255��Ƶ;����255,��͸��
		LTDC_Layer1->DCCR=((u32)alpha0<<24)|bkcolor;//����Ĭ����ɫAlphaֵ,�Լ�Ĭ����ɫ
		LTDC_Layer1->BFCR=((u32)bfac1<<8)|bfac2;	//���ò���ϵ��
	}else
	{
		LTDC_Layer2->CFBAR=bufaddr;					//���ò���ɫ֡������ʼ��ַ
		LTDC_Layer2->PFCR=pixformat;				//���ò���ɫ��ʽ
		LTDC_Layer2->CACR=alpha;					//���ò���ɫAlphaֵ,255��Ƶ;����255,��͸��
		LTDC_Layer2->DCCR=((u32)alpha0<<24)|bkcolor;//����Ĭ����ɫAlphaֵ,�Լ�Ĭ����ɫ
		LTDC_Layer2->BFCR=((u32)bfac1<<8)|bfac2;	//���ò���ϵ��
	}
} 

//��ȡ������
//PG6=R7(M0);PI2=G7(M1);PI7=B7(M2);
//M2:M1:M0
//0 :0 :0	//4.3��480*272 RGB��,ID=0X4342
//0 :0 :1	//7��800*480 RGB��,ID=0X7084
//0 :1 :0	//7��1024*600 RGB��,ID=0X7016
//0 :1 :1	//7��1280*800 RGB��,ID=0X7018
//1 :0 :0	//4.3��800*480 RGB��,ID=0X4384
//1 :0 :1	//10.1��1280*800 RGB��,ID=0X1018 
//����ֵ:LCD ID:0,�Ƿ�;����ֵ,ID;
u16 LTDC_PanelID_Read(void)
{
	u8 idx=0;
	RCC->AHB4ENR|=1<<9|1<<10; 			//ʹ��PJ/PKʱ��   
	GPIO_Set(GPIOJ,PIN6,GPIO_MODE_IN,0,0,GPIO_PUPD_PU); 			//PJ6�������� 
	GPIO_Set(GPIOK,PIN2|PIN6,GPIO_MODE_IN,0,0,GPIO_PUPD_PU); 		//PK2/PK6��������
	delay_ms(50);
	idx=GPIO_Pin_Get(GPIOJ,PIN6);		//��ȡM1
	idx|=GPIO_Pin_Get(GPIOK,PIN2)<<1;	//��ȡM0
	idx|=GPIO_Pin_Get(GPIOK,PIN6)<<2;	//��ȡM2 
	switch(idx)
	{
		case 0:return 0X4342;		//4.3����,480*272�ֱ���
		case 1:return 0X7084;		//7����,800*480�ֱ���
		case 2:return 0X7016;		//7����,1024*600�ֱ���
		case 3:return 0X7018;		//7����,1280*800�ֱ���
		case 4:return 0X4384;	    //4.3����,800*480�ֱ���
		case 5:return 0X1018; 		//10.1����,1280*800�ֱ���
		default:return 0;
	}
}

//LTDC��ʼ������
void LTDC_Init(void)
{   
	u32 tempreg=0;
	u16 lcdid=0;

	lcdid=LTDC_PanelID_Read();		//��ȡLCD���ID  PE   
	if(lcdid==0)return;				//��RGB��

	RCC->APB3ENR|=1<<3;				//����LTDCʱ��
	RCC->AHB4ENR|=1<<1;				//ʹ��PORTBʱ�� 
	RCC->AHB4ENR|=0X7<<8; 			//ʹ��PI/PJ/PKʱ��    
	GPIO_Set(GPIOB,PIN5,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_MID,GPIO_PUPD_PU);				//PB5 �������,���Ʊ��� 
	GPIO_Set(GPIOI,7<<12,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_PU);				//PI12/13/14
	GPIO_Set(GPIOJ,(0X1F<<2)|(0X07<<9)|(1<<15),GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_PU);//PJ2~6,9~11,15
	GPIO_Set(GPIOK,0XFF,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_PU);				//PK0~7
	 
	

 	GPIO_AF_Set(GPIOI,12,14);		//PI12,AF14
 	GPIO_AF_Set(GPIOI,13,14);		//PI13,AF14
 	GPIO_AF_Set(GPIOI,14,14);		//PI14,AF14  
	 
 	GPIO_AF_Set(GPIOJ,2,14);		//PJ2,AF14
 	GPIO_AF_Set(GPIOJ,3,14);		//PJ3,AF14
 	GPIO_AF_Set(GPIOJ,4,14);		//PJ4,AF14
 	GPIO_AF_Set(GPIOJ,5,14);		//PJ5,AF14
 	GPIO_AF_Set(GPIOJ,6,14);		//PJ6,AF14 
 	GPIO_AF_Set(GPIOJ,9,14);		//PJ9,AF14
 	GPIO_AF_Set(GPIOJ,10,14);		//PJ10,AF14 
 	GPIO_AF_Set(GPIOJ,11,14);		//PJ11,AF14  
 	GPIO_AF_Set(GPIOJ,15,14);		//PJ15,AF14
	
 	GPIO_AF_Set(GPIOK,0,14);		//PK0,AF14 
 	GPIO_AF_Set(GPIOK,1,14);		//PK1,AF14  
 	GPIO_AF_Set(GPIOK,2,14);		//PK2,AF14
 	GPIO_AF_Set(GPIOK,3,14);		//PK3,AF14 
 	GPIO_AF_Set(GPIOK,4,14);		//PK4,AF14  
 	GPIO_AF_Set(GPIOK,5,14);		//PK5,AF14
 	GPIO_AF_Set(GPIOK,6,14);		//PK6,AF14
 	GPIO_AF_Set(GPIOK,7,14);		//PK7,AF14
	if(lcdid==0X4342)
	{
		lcdltdc.pwidth=480;			//�����,��λ:����
		lcdltdc.pheight=272;		//���߶�,��λ:����
		lcdltdc.hsw=1;				//ˮƽͬ�����
		lcdltdc.vsw=1;				//��ֱͬ�����
		lcdltdc.hbp=40;				//ˮƽ����
		lcdltdc.vbp=8;				//��ֱ����
		lcdltdc.hfp=5;				//ˮƽǰ��
		lcdltdc.vfp=8;				//��ֱǰ��
		LTDC_Clk_Set(300,25,33);	//��������ʱ��  9Mhz 
	}else if(lcdid==0X7084)
	{
		lcdltdc.pwidth=800;			//�����,��λ:����
		lcdltdc.pheight=480;		//���߶�,��λ:����
		lcdltdc.hsw=1;				//ˮƽͬ�����
		lcdltdc.vsw=1;				//��ֱͬ�����
		lcdltdc.hbp=46;				//ˮƽ����
		lcdltdc.vbp=23;				//��ֱ����
		lcdltdc.hfp=210;			//ˮƽǰ��
		lcdltdc.vfp=22;				//��ֱǰ��
		LTDC_Clk_Set(300,25,9);		//��������ʱ�� 33M(�����˫��,��Ҫ����DCLK��:18.75Mhz  300/25/16,�Ż�ȽϺ�)
	}else if(lcdid==0X7016)		
	{
		lcdltdc.pwidth=1024;		//�����,��λ:����
		lcdltdc.pheight=600;		//���߶�,��λ:����
		lcdltdc.hsw=20;				//ˮƽͬ�����
		lcdltdc.vsw=3;				//��ֱͬ�����
		lcdltdc.hbp=140;			//ˮƽ����
		lcdltdc.vbp=20;				//��ֱ����
		lcdltdc.hfp=160;			//ˮƽǰ��
		lcdltdc.vfp=12;				//��ֱǰ��
		LTDC_Clk_Set(300,25,6);		//��������ʱ��  50Mhz 
	}else if(lcdid==0X7018)		
	{
		lcdltdc.pwidth=1280;		//�����,��λ:����
		lcdltdc.pheight=800;		//���߶�,��λ:����
		//������������.
	}else if(lcdid==0X4384)		
	{
		lcdltdc.pwidth=800;		    //�����,��λ:����
		lcdltdc.pheight=480;	    //���߶�,��λ:����
		lcdltdc.hbp=88;			    //ˮƽ����
		lcdltdc.hfp=40;			   	//ˮƽǰ��
		lcdltdc.hsw=48;			    //ˮƽͬ�����
		lcdltdc.vbp=32;			    //��ֱ����
		lcdltdc.vfp=13;				//��ֱǰ��
		lcdltdc.vsw=3;				//��ֱͬ�����
		LTDC_Clk_Set(300,25,9);		//��������ʱ�� 33M
		//������������.
	}else if(lcdid==0X1018)			//10.1��1280*800 RGB��	
	{
		lcdltdc.pwidth=1280;		//�����,��λ:����
		lcdltdc.pheight=800;		//���߶�,��λ:����
		lcdltdc.hbp=140;			//ˮƽ����
		lcdltdc.hfp=10;			    //ˮƽǰ��
		lcdltdc.hsw=10;				//ˮƽͬ�����
		lcdltdc.vbp=10;				//��ֱ����
		lcdltdc.vfp=10;				//��ֱǰ��
		lcdltdc.vsw=3;				//��ֱͬ����� 
		LTDC_Clk_Set(300,25,4);		//��������ʱ��  75Mhz     
	}  
	tempreg=0<<28;					//����ʱ�Ӽ���:������
	if(lcdid==0X1018)tempreg=1<<28;	//����ʱ�Ӽ���:����
	tempreg|=0<<29;					//����ʹ�ܼ���:�͵�ƽ��Ч
	tempreg|=0<<30;					//��ֱͬ������:�͵�ƽ��Ч
	tempreg|=0<<31;					//ˮƽͬ������:�͵�ƽ��Ч
	LTDC->GCR=tempreg;				//����ȫ�ֿ��ƼĴ��� 
	tempreg=(lcdltdc.vsw-1)<<0;		//��ֱ����-1
	tempreg|=(lcdltdc.hsw-1)<<16;	//ˮƽ����-1
	LTDC->SSCR=tempreg;				//����ͬ����С���üĴ���

	tempreg=(lcdltdc.vsw+lcdltdc.vbp-1)<<0;		//�ۼӴ�ֱ����=��ֱ����+��ֱ����-1
	tempreg|=(lcdltdc.hsw+lcdltdc.hbp-1)<<16;	//�ۼ�ˮƽ����=ˮƽ����+ˮƽ����-1
	LTDC->BPCR=tempreg;							//���ú������üĴ���
	
	tempreg=(lcdltdc.vsw+lcdltdc.vbp+lcdltdc.pheight-1)<<0;	//�ۼ���Ч�߶�=��ֱ����+��ֱ����+��ֱ�ֱ���-1
	tempreg|=(lcdltdc.hsw+lcdltdc.hbp+lcdltdc.pwidth-1)<<16;//�ۼ���Ч���=ˮƽ����+ˮƽ����+ˮƽ�ֱ���-1
	LTDC->AWCR=tempreg;							//������Ч������üĴ���

 	tempreg=(lcdltdc.vsw+lcdltdc.vbp+lcdltdc.pheight+lcdltdc.vfp-1)<<0;	//�ܸ߶�=��ֱ����+��ֱ����+��ֱ�ֱ���+��ֱǰ��-1
	tempreg|=(lcdltdc.hsw+lcdltdc.hbp+lcdltdc.pwidth+lcdltdc.hfp-1)<<16;//�ܿ��=ˮƽ����+ˮƽ����+ˮƽ�ֱ���+ˮƽǰ��-1
	LTDC->TWCR=tempreg;				//�����ܿ�����üĴ���
	
	LTDC->BCCR=LTDC_BACKLAYERCOLOR;	//���ñ�������ɫ�Ĵ���(RGB888��ʽ)
	LTDC_Switch(1);					//����LTDC
	

#if LCD_PIXFORMAT==LCD_PIXFORMAT_ARGB8888||LCD_PIXFORMAT==LCD_PIXFORMAT_RGB888 
	ltdc_framebuf[0]=(u32*)&ltdc_lcd_framebuf;
	lcdltdc.pixsize=4;				//ÿ������ռ4���ֽ�
#else 
	ltdc_framebuf[0]=(u32*)&ltdc_lcd_framebuf;
	//ltdc_framebuf[1]=(u32*)&ltdc_lcd_framebuf1;
	lcdltdc.pixsize=2;				//ÿ������ռ2���ֽ�
#endif 	
	//������
	LTDC_Layer_Parameter_Config(0,(u32)ltdc_framebuf[0],LCD_PIXFORMAT,255,0,6,7,0X000000);	//���������
	LTDC_Layer_Window_Config(0,0,0,lcdltdc.pwidth,lcdltdc.pheight,LCD_PIXFORMAT);			//�㴰������,��LCD�������ϵΪ��׼,��Ҫ����޸�!
	
	//LTDC_Layer_Parameter_Config(1,(u32)ltdc_framebuf[1],LCD_PIXFORMAT,127,0,6,7,0X000000);//���������
	//LTDC_Layer_Window_Config(1,0,0,lcdltdc.pwidth,lcdltdc.pheight,LCD_PIXFORMAT);			//�㴰������,��LCD�������ϵΪ��׼,��Ҫ����޸�!

	
	
	lcddev.width=lcdltdc.pwidth;	//����lcddev�Ŀ�Ȳ���
	lcddev.height=lcdltdc.pheight; 	//����lcddev�Ŀ�Ȳ���
	//LTDC_Display_Dir(0);			//Ĭ����������LCD_Init������������
	LTDC_Select_Layer(0); 			//ѡ���1��
    LCD_LED(1);         		 	//��������
    LTDC_Clear(0XFFFFFFFF);			//����
}

 






















