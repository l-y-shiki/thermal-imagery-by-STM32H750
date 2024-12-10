//#ifndef __TOUCH_H__
//#define __TOUCH_H__
//#include "./SYSTEM/sys/sys.h"

//#define TP_PRES_DOWN 0x80  //触屏被按下	  
//#define TP_CATH_PRES 0x40  //有按键按下了 	  
//										    
////触摸屏控制器
//typedef struct
//{
//	uint8_t (*init)(void);			//初始化触摸屏控制器
//	uint8_t (*scan)(uint8_t);				//扫描触摸屏.0,屏幕扫描;1,物理坐标;	 
//	void (*adjust)(void);		//触摸屏校准
//	uint16_t x0;						//原始坐标(第一次按下时的坐标)
//	uint16_t y0;
//	uint16_t x; 						//当前坐标(此次扫描时,触屏的坐标)
//	uint16_t y;						   	    
//	uint8_t  sta;					//笔的状态 
//								//b7:按下1/松开0; 
//	                            //b6:0,没有按键按下;1,有按键按下.         			  
//////////////////////////触摸屏校准参数/////////////////////////								
//	float xfac;					
//	float yfac;
//	short xoff;
//	short yoff;	   
////新增的参数,当触摸屏的左右上下完全颠倒时需要用到.
////touchtype=0的时候,适合左右为X坐标,上下为Y坐标的TP.
////touchtype=1的时候,适合左右为Y坐标,上下为X坐标的TP.
//	uint8_t touchtype;
//}_m_tp_dev;

//extern _m_tp_dev tp_dev;	 	//触屏控制器在touch.c里面定义

////与触摸屏芯片连接引脚	   
////与触摸屏芯片连接引脚	   
////#define PEN  PCin(10)  //8    //PC10  INT
////#define DOUT PCin(2)   //7     //PC2  MISO		 PC2--PB14


////#define TDIN PCout(1)  //pc1  //PC3  MOSI		 PC3--PB15
////#define TCLK PCout(4)  //pc4  //PC0  SCLK	 PC0--PB13
////#define TCS  PCout(5)  //pc5     //PC13 CS    



//#define TDIN0()            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1,GPIO_PIN_RESET);
//#define TDIN1()            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1,GPIO_PIN_SET);

//#define TCLK0()            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4,GPIO_PIN_RESET);
//#define TCLK1()            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4,GPIO_PIN_SET);

//#define TCS0()            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5,GPIO_PIN_RESET);
//#define TCS1()            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5,GPIO_PIN_SET);

//#define DOUT_READ()             HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8);
//#define PEN_READ()             HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7);


//     

//void TP_Write_Byte(uint8_t num);						//向控制芯片写入一个数据
//uint16_t TP_Read_AD(uint8_t CMD);							//读取AD转换值
//uint16_t TP_Read_XOY(uint8_t xy);							//带滤波的坐标读取(X/Y)
//uint8_t TP_Read_XY(uint16_t *x,uint16_t *y);					//双方向读取(X+Y)
//uint8_t TP_Read_XY2(uint16_t *x,uint16_t *y);					//带加强滤波的双方向坐标读取
//void TP_Drow_Touch_Point(uint16_t x,uint16_t y,uint16_t color);//画一个坐标校准点
//void TP_Draw_Big_Point(uint16_t x,uint16_t y,uint16_t color);	//画一个大点
//uint8_t TP_Scan(uint8_t tp);								//扫描
//void TP_Save_Adjdata(void);						//保存校准参数
//uint8_t TP_Get_Adjdata(void);						//读取校准参数
//void TP_Adjust(void);							//触摸屏校准
//uint8_t TP_Init(void);								//初始化
//																 
//void TP_Adj_Info_Show(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t x3,uint16_t y3,uint16_t fac);//显示校准信息
// 		  
//#endif

////moSI   din  pc1
////clk   pc4
////  cs  pc5

////pen pc7
////MISO PC8
