
#include "./SYSTEM/sys/sys.h"
#include "./BSP/ATK_MD0430/atk_md0430.h"
#include "./SYSTEM/delay/delay.h"
#include "MYMLX90640_API.h"
#include "MYMLX90640_I2C_Driver.h"
#include "MYMLX90640.h"
#include <stdio.h>
#include <math.h>

DMA_HandleTypeDef hdma_memtomem_dma2_stream6;
uint16_t src_buf[10] = {0x0a, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
//__attribute__((at(0x30000000)))
uint16_t dest_buf[10] = {0};

uint16_t cmt=15;
unsigned short EE[832];
typedef union{
	uint16_t mlx90640_Zoom10[834];  
	uint8_t  DisBuf[10*320];
	uint8_t  SPI_FLASH_BUF[4096];
}UnionData;

typedef union{
	float mlx90640To[768];
	uint16_t databuf[640];  
	uint8_t  bbb_data[512];
}UnionData2;

UnionData data;
UnionData2 data2;

typedef struct
{
		uint8_t colorR;
		uint8_t colorG;
	  uint8_t colorB;
}RGBcolor;
RGBcolor color1;
RGBcolor *color=&color1;


float Ta;
float emissivity=0.95;
int x_line=0,y_list=0;
int pin_x=0,pin_y=0;

unsigned short rgb = 0;
int k=0,j=0,m=0,status,i=0;
float vdd;
uint16_t count;
uint16_t mlx90640_status = 0;

paramsMLX90640 mlx90640;


void	MLX90640_Init(void)
	{
	MLX90640_I2CInit();										//MLX90640I2C初始化
		delay_ms(50);												//等待初始化
	MLX90640_SetRefreshRate(MLX90640_ADDR, FPS16HZ);   //设置帧率	
	MLX90640_SetChessMode(MLX90640_ADDR);				//MLX90640设置成棋盘模式
	
			status = MLX90640_DumpEE(MLX90640_ADDR, data.mlx90640_Zoom10);		//读取像素校正参数 
		if (status != 0) printf("load system parameters error with code:%d\r\n",mlx90640_status);
	
		status = MLX90640_ExtractParameters(data.mlx90640_Zoom10, &mlx90640);		//解析校正参数
		if (status != 0) printf("Parameter extraction failed with error code:%d\r\n",mlx90640_status);

		for(i=0;i<3;i++)
		{
				MLX90640_I2CRead(MLX90640_ADDR, 0x8000, 1, &mlx90640_status);

				if(mlx90640_status & 0x0008)
				{
						int status = MLX90640_GetFrameData(MLX90640_ADDR, data.mlx90640_Zoom10);  //读取一帧原始数据
						if (status < 0)		printf("GetFrame Error: %d\r\n",status);
						
						vdd = MLX90640_GetVdd(data.mlx90640_Zoom10, &mlx90640);  //计算 Vdd（这句可有可无）
						Ta = MLX90640_GetTa(data.mlx90640_Zoom10, &mlx90640);  //计算实时外壳温度
						Ta = Ta - TA_SHIFT;
						MLX90640_CalculateTo(data.mlx90640_Zoom10, &mlx90640,0.95,Ta,data2.mlx90640To);		//计算像素点温度

						printf("vdd: %f Tr: %f\r\n",vdd,Ta);
				}
		}
	}


uint8_t Mlx90640_Get_Frame(void)
{
		uint8_t flag = 0;
		uint16_t status = 0;
	
				MLX90640_I2CRead(MLX90640_ADDR, 0x8000, 1, &status);

				if(status & 0x0008)
				{
						int status2 = MLX90640_GetFrameData(MLX90640_ADDR, data.mlx90640_Zoom10);  //读取一帧原始数据
						if (status2 < 0)		printf("GetFrame Error: %d\r\n",status2);
						
						Ta = MLX90640_GetTa(data.mlx90640_Zoom10, &mlx90640);  //计算实时外壳温度
						Ta = Ta - TA_SHIFT;
						MLX90640_CalculateTo(data.mlx90640_Zoom10, &mlx90640,0.95,Ta,data2.mlx90640To);		//计算像素点温度
				}
				flag=1;
		
		return flag;
}


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



void Disp_Temp_Pic(void)
{
		for(k=0;k<768;k++)
		{
			//printf("dddddd:%f\n",data2.mlx90640To[k]);
			count=(uint8_t)data2.mlx90640To[k]*8;
			if(count>250)
				count=255;
		//	count=160;
	//		printf("count:%d\n",count);
			GrayToPseColor(count,&(color->colorR),&(color->colorG),&(color->colorB));
	//		printf("RGB:%d  %d  %d\n",color->colorR,color->colorG,color->colorB);
				
			rgb=RGB565(color->colorR,color->colorG,color->colorB);
		//	printf("rgb:%8x\n",rgb);
				x_line=k%32;
				y_list=k/32+1;
			
			pin_x=x_line*6;
			m=pin_x-6;
			pin_y=y_list*5;
			j=pin_y-5;
			//显示
			
			atk_md0430_fill(m,j,pin_x,pin_y,rgb);
			
//			for(j;j<pin_y;pin_y--)
//			{
//						for(m;m<pin_x;pin_x--)
//						atk_md0430_draw_point(x_line+200,y_list+500,rgb);//画点
//				pin_x=x_line*25;
//			m=pin_x-25;
//			}

//			if(data2.mlx90640To[k]>25)
//			  atk_md0430_draw_point(x_line+200,y_list+500,ATK_MD0430_BLACK);//画点
//			else
//				atk_md0430_draw_point(x_line+200,y_list+500,ATK_MD0430_RED);//画点
		}
	}

	

	

	
	
void FSMC_DMA_Init()
{
	int status;
    __HAL_RCC_DMA2_CLK_ENABLE();
 
    hdma_memtomem_dma2_stream6.Instance = DMA2_Stream0;
		hdma_memtomem_dma2_stream6.Init.Request = DMA_REQUEST_MEM2MEM;
    hdma_memtomem_dma2_stream6.Init.Direction = DMA_MEMORY_TO_MEMORY;            //传输方向，存储器-》存储器
    hdma_memtomem_dma2_stream6.Init.PeriphInc = DMA_PINC_ENABLE;//源地址自增
    hdma_memtomem_dma2_stream6.Init.MemInc = DMA_MINC_DISABLE;  //目的地址不自增，因为LCD的地址是固定的
	//hdma_memtomem_dma2_stream6.Init.MemInc = DMA_MINC_ENABLE;  //目的地址自增，因为LCD的地址是固定的
	
    hdma_memtomem_dma2_stream6.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_memtomem_dma2_stream6.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_memtomem_dma2_stream6.Init.Mode = DMA_NORMAL;//传输模式，此即传输一次停止
    hdma_memtomem_dma2_stream6.Init.Priority = DMA_PRIORITY_MEDIUM;
//    hdma_memtomem_dma2_stream6.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
	    hdma_memtomem_dma2_stream6.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    hdma_memtomem_dma2_stream6.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_memtomem_dma2_stream6.Init.MemBurst = DMA_MBURST_INC8;   //源地址,16太快了，否则屏幕无反应
    hdma_memtomem_dma2_stream6.Init.PeriphBurst = DMA_MBURST_INC8;//目的地址,16太快了，否则白屏
    if (HAL_DMA_Init(&hdma_memtomem_dma2_stream6) != HAL_OK)
    {
    //    Error_Handler();
			printf("errorrrrrr\n");
    }
		HAL_DMA_Start(&hdma_memtomem_dma2_stream6,(uint32_t)&src_buf[9],(uint32_t)&dest_buf[5],1); 
		HAL_DMA_PollForTransfer(&hdma_memtomem_dma2_stream6,HAL_DMA_FULL_TRANSFER,20);
		HAL_DMA_Start(&hdma_memtomem_dma2_stream6,(uint32_t)&src_buf[8],(uint32_t)&dest_buf[4],1); 
		HAL_DMA_PollForTransfer(&hdma_memtomem_dma2_stream6,HAL_DMA_FULL_TRANSFER,20);
		HAL_DMA_Start(&hdma_memtomem_dma2_stream6,(uint32_t)&src_buf[7],(uint32_t)&dest_buf[3],1); 
				HAL_DMA_PollForTransfer(&hdma_memtomem_dma2_stream6,HAL_DMA_FULL_TRANSFER,20);
		HAL_DMA_Start(&hdma_memtomem_dma2_stream6,(uint32_t)src_buf,(uint32_t)dest_buf,10); 
		for(status=0;status<10;status++)
		printf("eeeeeeeeeeeeee:%d\n",dest_buf[status]);
		
		
//    /*不受FreeRTOS调度*/
//    HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 3, 0);
//    HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
//    HAL_DMA_RegisterCallback(&hdma_memtomem_dma2_stream6, HAL_DMA_XFER_CPLT_CB_ID, LVGL_LCD_FSMC_DMA_pCallback);
}

void display()
{
	int status;
		for(status=0;status<10;status++)
		printf("eeeeeeeeeeeeee:%d\n",dest_buf[status]);
}



