#include "lcd.h"
#include "ltdc.h" 
#include "delay.h" 
#include "sdram.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//LTDC ����	   	    
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2019/5/27
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved	 
//********************************************************************************
//�޸�˵��:
//20190527   V1.1
//�޸����10.1��7��1024*600 RGB��������ʱ��Ƶ��,��Ϊ60Mhz(�ȶ�����)
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
	RCC->AHB1ENR|=1<<23;			//ʹ��DM2Dʱ��
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
	RCC->AHB1ENR|=1<<23;			//ʹ��DM2Dʱ��
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
//Fvco=Fin*pllsain; 
//Fdclk=Fvco/pllsair/2*2^pllsaidivr=Fin*pllsain/pllsair/2*2^pllsaidivr;

//Fvco:VCOƵ��
//Fin:����ʱ��Ƶ��һ��Ϊ1Mhz(����ϵͳʱ��PLLM��Ƶ���ʱ��,��ʱ����ͼ)
//pllsain:SAIʱ�ӱ�Ƶϵ��N,ȡֵ��Χ:50~432.  
//pllsair:SAIʱ�ӵķ�Ƶϵ��R,ȡֵ��Χ:2~7
//pllsaidivr:LCDʱ�ӷ�Ƶϵ��,ȡֵ��Χ:0~3,��Ӧ��Ƶ2^(pllsaidivr+1) 
//����:�ⲿ����Ϊ25M,pllm=25��ʱ��,Fin=1Mhz.
//����:Ҫ�õ�33M��LTDCʱ��,���������:pllsain=396,pllsair=3,pllsaidivr=1
//Fdclk=1*396/3/2*2^1=396/12=33Mhz
//����ֵ:0,�ɹ�;1,ʧ�ܡ�
u8 LTDC_Clk_Set(u32 pllsain,u32 pllsair,u32 pllsaidivr)
{ 
	u16 retry=0;
	u8 status=0;
	u32 tempreg=0;
	RCC->CR&=~(1<<28);					//�ر�SAIʱ�� 
	while(((RCC->CR&(1<<29)))&&(retry<0X1FFF))retry++;//�ȴ�SAIʱ��ʧ��
 	if(retry==0X1FFF)status=1;			//LTDCʱ�ӹر�ʧ��
	else   
	{ 
		tempreg|=pllsain<<6;
		tempreg|=pllsair<<28;
		RCC->PLLSAICFGR=tempreg;		//����LTDC�ı�Ƶ�ͷ�Ƶ
		RCC->DCKCFGR1&=~(3<<16);		//���ԭ��������.
		RCC->DCKCFGR1|=pllsaidivr<<16;	//����fdclk��Ƶ

		RCC->CR|=1<<28;					//����SAIʱ�� 
		while(((RCC->CR&(1<<29))==0)&&(retry<0X1FFF))retry++;//�ȴ�SAIʱ������
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
		LTDC_Layer1->CFBLR=(width*temp<<16)|(width*temp+3);		//֡��������������(�ֽ�Ϊ��λ)
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
		LTDC_Layer2->CFBLR=(width*temp<<16)|(width*temp+3);		//֡��������������(�ֽ�Ϊ��λ)
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
	RCC->AHB1ENR|=1<<9|1<<10; 			//ʹ��PJ/PKʱ��   
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

	RCC->APB2ENR|=1<<26;			//����LTDCʱ��
	RCC->AHB1ENR|=1<<1;				//ʹ��PORTBʱ�� 
	RCC->AHB1ENR|=0X7<<8; 			//ʹ��PI/PJ/PKʱ��    
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
		LTDC_Clk_Set(288,4,2); 		//��������ʱ��  9Mhz 
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
		LTDC_Clk_Set(396,3,1); 		//��������ʱ�� 33M(�����˫��,��Ҫ����DCLK��:18.75Mhz  300/4/4,�Ż�ȽϺ�)
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
		LTDC_Clk_Set(360,3,0); 		//��������ʱ��  60Mhz   
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
		LTDC_Clk_Set(396,3,1); 		//��������ʱ�� 33M
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
		LTDC_Clk_Set(360,3,0); 		//��������ʱ��  60Mhz   
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

 






















