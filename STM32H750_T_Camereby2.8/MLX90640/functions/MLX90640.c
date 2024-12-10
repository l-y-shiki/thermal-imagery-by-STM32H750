
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "MYMLX90640_API.h"
#include "MYMLX90640_I2C_Driver.h"
#include "MYMLX90640.h"
#include "spi.h"
#include "lcd.h"
#include <stdio.h>
#include <math.h>
#include "rgb565.h"

DMA_HandleTypeDef hdma_memtomem_dma2_stream6={0};
extern SPI_HandleTypeDef g_spi2_handle;

extern uint8_t dmaTransferComplete;

uint16_t scount2;


//__attribute__((at(0x24000000))) uint16_t src_buf[10] = {0x0a, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
//__attribute__((at(0x24000170))) uint16_t dest_buf[10] = {0};


uint16_t cmt=15;
typedef union{
	uint16_t mlx90640_Zoom10[834];  
	uint8_t  DisBuf[10*320];
	uint8_t  SPI_FLASH_BUF[4096];
}UnionData;

//计算后的温度原始数组
typedef union{
	float mlx90640To[768];
	uint16_t databuf[640];  
	uint8_t  bbb_data[512];
}UnionData2;

UnionData data;
UnionData2 data2;

static uint16_t mlx90640_disp_buf[768];		//镜像显示数组
static uint16_t mlx90640_temp_buf[25];		//双线性内插法后的原始数组
static uint16_t mlx90640_lcd_buf[100];		//放大后单个网格显示的数组
static uint16_t mlx90640_line_buf[3200];	//单行的数据数组

//颜色转换
typedef struct
{
		uint8_t colorR;
		uint8_t colorG;
	  uint8_t colorB;
}RGBcolor;
RGBcolor color1;
RGBcolor *color=&color1;

//温度显示
float Ta;
float emissivity=0.95;

//屏幕显示
uint16_t x_line[768]={0},y_list[768]={0};
uint16_t pin_x[768]={0},pin_y[768]={0};

__attribute__((at(0x24000500))) unsigned short rgb[768] ={0};
int k=0,status,i=0;
uint16_t j[768]={0},m[768]={0};

float vdd;
uint16_t count;
uint16_t mlx90640_status = 0;

paramsMLX90640 mlx90640;



void  mlx90640_buf_copy(void)
{
				//数据镜像
		for(int i=0;i<24;i++)
		{
				for(int j=0;j<32;j++)
				{
						mlx90640_disp_buf[i*32+j] = data2.mlx90640To[i*32+j] + 0;
				}
		}
}

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
					
//					vdd = MLX90640_GetVdd(data.mlx90640_Zoom10, &mlx90640);  //计算 Vdd（这句可有可无）
//					printf("vdd: %f\r\n",vdd);
					
										//测试数据是否全部采集
//										for(status2=0;status2<768;status2++)
//					{
//						printf("data %d:%d\n",status2,data.mlx90640_Zoom10[status2]);
//					}

						Ta = MLX90640_GetTa(data.mlx90640_Zoom10, &mlx90640);  //计算实时外壳温度
						Ta = Ta - TA_SHIFT;
						MLX90640_CalculateTo(data.mlx90640_Zoom10, &mlx90640,0.95,Ta,data2.mlx90640To);		//计算像素点温度
//						MLX90640_BadPixelsCorrection(mlx90640.brokenPixels,data2.mlx90640To,1,&mlx90640);
//						MLX90640_BadPixelsCorrection(mlx90640.outlierPixels,data2.mlx90640To,1,&mlx90640);
					//测试数据是否全部采集
										for(status2=0;status2<768;status2++)
					{
						
						//坏点处理
						if(isnan(data2.mlx90640To[status2]))
							data2.mlx90640To[status2]=(data2.mlx90640To[status2-1]+data2.mlx90640To[status2+1]+data2.mlx90640To[status2-32]+data2.mlx90640To[status2+32])/4;
						
				//		printf("data %d:%f\n",status2,data2.mlx90640To[status2]);
					}

						flag=1;
//					printf("TO:%f",Ta);
				}
			
		
		return flag;
}

//原数据显示
void Disp_Temp_Pic(void)
{
	
		for(k=0;k<768;k++)
		{		
			
//			m=k*500+500;
//			k=k*500+1;
			
			count=(uint8_t)data2.mlx90640To[k]*8;
			if(count>250)
				count=255;
			GrayToPseColor(count,&(color->colorR),&(color->colorG),&(color->colorB));	

			rgb[k]=RGB565(color->colorR,color->colorG,color->colorB);

				x_line[k]=k%32;
				y_list[k]=k/32+1;
			
			pin_x[k]=x_line[k]*10;
			m[k]=pin_x[k]-10;
			pin_y[k]=y_list[k]*10;
			j[k]=pin_y[k]-10;
		
		}
		
			for(k=0;k<768;k++)
		LCD_Fill(m[k],j[k],pin_x[k],pin_y[k],rgb[k]);
	}
	
//滤波数据显示
void Disp_Temp_Pia(void)
{
		uint16_t count = 0;
		mlx90640_buf_copy();
	unsigned int j;
	
			//数据显示（滤波优化显示）
		for(int i=0;i<24;i++)
		{
				for(j=0;j<32;j++)
				{
						//双线性插值算法
						Bilinear_Interpolation(i*32+j);
					
						//将数据转换为显示颜色
						for(int k=0;k<25;k++)
{
			count=(uint8_t)mlx90640_temp_buf[k]*6.7;
					if(count>250)
						count=255;
						GrayToPseColor(count,&(color->colorR),&(color->colorG),&(color->colorB));		
						mlx90640_temp_buf[k] = RGB565(color->colorR,color->colorG,color->colorB);
}				
					
						//数据数组放大
						count = 0;
						for(int k=0;k<5;k++)
						{
								for(int l=0;l<5;l++)
								{
										mlx90640_lcd_buf[l*2+k*20] = mlx90640_temp_buf[count];
										mlx90640_lcd_buf[l*2+k*20+1] = mlx90640_temp_buf[count];
									
										mlx90640_lcd_buf[l*2+k*20+10] = mlx90640_temp_buf[count];
										mlx90640_lcd_buf[l*2+k*20+11] = mlx90640_temp_buf[count];
										count++;
								}
						}
							
						//将单格数据排列入整行数据数组
						count = 0;
						for(int k=0;k<10;k++)
						{
								for(int l=0;l<10;l++)
								{
										mlx90640_line_buf[k*320+j*10+l] = mlx90640_lcd_buf[count];
										count++;
								}
								
						}
						
				}
				
				LCD_Fill_BUF(0,i*10,319,i*10+9,mlx90640_line_buf);
		}
		
}

	
	/*******************************************************************************
	* @name:						Bilinear_Interpolation	
	* @instructions:		双线性插值算法
	*
	* @input:						flag	位置信息
	* @output:					NULL
	*****************************************************************************/
void Bilinear_Interpolation(uint16_t flag)
{
		uint16_t scale_x = 102;		//512/5=102
		uint16_t scale_y = 102;		//512/5=102
	
		uint16_t cbufx[2],cbufy[2];
		int x,y;
	
	if(flag<=763)
	{//边界内处理
	
    for (y = 0; y < 5; y++)
    {
        cbufy[0] = 512-y*scale_y;
        cbufy[1] = 512-cbufy[0];
		
        for (x = 0; x < 5; x++)
        {
						cbufx[0] = 512-x*scale_x;
						cbufx[1] = 512-cbufx[0];

						mlx90640_temp_buf[y*5+x] = (mlx90640_disp_buf[flag   ] * cbufx[0] * cbufy[0] +               //Q21
																				mlx90640_disp_buf[flag+32] * cbufx[0] * cbufy[1] +               //Q11
																				mlx90640_disp_buf[flag+1 ] * cbufx[1] * cbufy[0] +               //Q22
																				mlx90640_disp_buf[flag+33] * cbufx[1] * cbufy[1] )>>18;					 //Q12
        }
		}
	}
	if(flag>736)
	{//边界处理
		
				    for (y = 0; y < 5; y++)
    {
        cbufy[0] = 512-y*scale_y;
        cbufy[1] = 512-cbufy[0];
		
        for (x = 0; x < 5; x++)
        {
						cbufx[0] = 512-x*scale_x;
						cbufx[1] = 512-cbufx[0];

						mlx90640_temp_buf[y*5+x] = (mlx90640_disp_buf[flag   ] * cbufx[0] * cbufy[0] +               //Q21
																				mlx90640_disp_buf[flag-32] * cbufx[0] * cbufy[1] +               //Q11
																				mlx90640_disp_buf[flag+1 ] * cbufx[1] * cbufy[0] +               //Q22
																				mlx90640_disp_buf[flag-33] * cbufx[1] * cbufy[1] )>>18;					 //Q12
        }
		}
	}
}

	
void SPI_DMA_Init(void)
{
//	int status;
    __HAL_RCC_DMA2_CLK_ENABLE();
 
    hdma_memtomem_dma2_stream6.Instance = DMA2_Stream3;
		hdma_memtomem_dma2_stream6.Init.Request = DMA_REQUEST_SPI2_TX;
    hdma_memtomem_dma2_stream6.Init.Direction = DMA_MEMORY_TO_PERIPH;            //传输方向，存储器->外设
	  hdma_memtomem_dma2_stream6.Init.PeriphInc = DMA_PINC_DISABLE;//外设地址不自增，因为LCD的地址是固定的
    hdma_memtomem_dma2_stream6.Init.MemInc = DMA_MINC_ENABLE;  //存储地址自增
	
    hdma_memtomem_dma2_stream6.Init.PeriphDataAlignment = DMA_MDATAALIGN_BYTE;          //外设数据宽度
    hdma_memtomem_dma2_stream6.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;							//内存数据宽度
    hdma_memtomem_dma2_stream6.Init.Mode = DMA_NORMAL;//传输模式，此即传输一次停止
    hdma_memtomem_dma2_stream6.Init.Priority = DMA_PRIORITY_VERY_HIGH;
//    hdma_memtomem_dma2_stream6.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
	
	    hdma_memtomem_dma2_stream6.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    hdma_memtomem_dma2_stream6.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_1QUARTERFULL;
    hdma_memtomem_dma2_stream6.Init.MemBurst = DMA_MBURST_INC8;   //源地址,16太快了，否则屏幕无反应
    hdma_memtomem_dma2_stream6.Init.PeriphBurst = DMA_MBURST_INC8;//目的地址,16太快了，否则白屏
    if (HAL_DMA_Init(&hdma_memtomem_dma2_stream6) != HAL_OK)
    {
    //    Error_Handler();
			printf("errorrrrrr\n");
    }
		
		__HAL_LINKDMA(&g_spi2_handle,hdmatx,hdma_memtomem_dma2_stream6);
		
		
				//使能DMA中断

		HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
		
//		HAL_DMA_Start(&hdma_memtomem_dma2_stream6,(uint32_t)&src_buf[9],(uint32_t)&dest_buf[5],1); 
//		HAL_DMA_PollForTransfer(&hdma_memtomem_dma2_stream6,HAL_DMA_FULL_TRANSFER,20);
//		HAL_DMA_Start(&hdma_memtomem_dma2_stream6,(uint32_t)&src_buf[8],(uint32_t)&dest_buf[4],1); 
//		HAL_DMA_PollForTransfer(&hdma_memtomem_dma2_stream6,HAL_DMA_FULL_TRANSFER,20);
//		HAL_DMA_Start(&hdma_memtomem_dma2_stream6,(uint32_t)&src_buf[7],(uint32_t)&dest_buf[3],1); 
//		
//				HAL_DMA_PollForTransfer(&hdma_memtomem_dma2_stream6,HAL_DMA_FULL_TRANSFER,20);
//		HAL_DMA_Start(&hdma_memtomem_dma2_stream6,(uint32_t)src_buf,(uint32_t)dest_buf,5); 
//						HAL_DMA_PollForTransfer(&hdma_memtomem_dma2_stream6,HAL_DMA_FULL_TRANSFER,20);
//		
//		for(status=0;status<10;status++)
//		printf("eeeeeeeeeeeeee:%d\n",dest_buf[status]);
}

void DMA2_Stream3_IRQHandler(void)																							
{
  HAL_DMA_IRQHandler(&hdma_memtomem_dma2_stream6);
		dmaTransferComplete++;
//	// 检查 DMA 状态
//    if (HAL_DMA_GetState(&hdma_memtomem_dma2_stream6) == HAL_DMA_STATE_READY) {
//        printf("DMA is ready\n");
//    }
}

void DMA_Start(uint8_t *ww)
{
//					HAL_SPI_StateTypeDef spiState = HAL_SPI_GetState(&g_spi2_handle);
//if (spiState == HAL_SPI_STATE_READY) {
//   printf("the spistatus is ok!!!!!!!!\n"); // SPI可以使用
//		
//}
//else {
//	printf("the spistatsu was busy...........\n");
//    // SPI正在忙碌，不能使用
//}
				LCD_CS_CLR;
				LCD_RS_SET;

//				if(HAL_SPI_Transmit_DMA(&g_spi2_handle,ww,6400)!=HAL_OK)
//					printf("ttttttttttrasmit  error!!!!\n");
					
//				else
//					printf("ttrasnprot okkkk\n");
	
	//DMA传输
				HAL_SPI_Transmit_DMA(&g_spi2_handle,ww,6400);
				delay_us(900);
				HAL_SPI_Abort(&g_spi2_handle);
				
				
		//for循环写入
		
//	for(int q=0;q<6400;q++)
//{
//				spi2_read_write_byte(ww[q]);
//				spi2_read_write_byte(ww[q]);
//}			
					LCD_CS_SET;

}







