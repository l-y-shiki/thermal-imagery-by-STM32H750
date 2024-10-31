#include "fdcan.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32H7������
//FDCAN���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2019/5/4
//�汾��V1.0 
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
  
  
FDCAN_HandleTypeDef FDCAN1_Handler;
FDCAN_RxHeaderTypeDef FDCAN1_RxHeader;
FDCAN_TxHeaderTypeDef FDCAN1_TxHeader;


//CAN��ʼ��
//tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:2~128;
//tseg1:ʱ���1��ʱ�䵥Ԫ.��Χ:2~256;
//tseg2:ʱ���2��ʱ�䵥Ԫ.��Χ:2~128;
//tnbrp :�����ʷ�Ƶ��.��Χ:1~512;
//mode:0,��ͨģʽ;1,����ģʽ;2,���߼��ģʽ;3,�ڲ�����ģʽ;4,�ⲿ�ػ�ģʽ.
//mode�ļ���������stm32h7xx_hal_fdcan.h�����ж�Ӧ�ĺ궨��

//����CANʱ��ԴΪ:pll1_q_ck,һ��Ϊ200Mhz
//����CAN��ƵΪ:FDCAN_CLOCK_DIV4
//�õ�:fdcan_tq_ck=50Mhz(���APB1Ƶ��)
//������=fdcan_tq_ck/((tseg1+tseg2+1)*tnbrp);
//������:FDCAN1_Mode_Init(8,8,11,5,3)
//���ɵõ�500Kbps��FDCAN������

//����ֵ:0,��ʼ��OK;
//    ����,��ʼ��ʧ��; 
u8 FDCAN1_Mode_Init(u8 tnsjw,u16 tseg1,u8 tseg2,u16 tnbrp,u8 mode)
{
  	FDCAN_ClkCalUnitTypeDef FDCAN1_ClkCalUnit;
	FDCAN_FilterTypeDef FDCAN1_RXFilter;
        
    //��ʼ��FDCAN1
    HAL_FDCAN_DeInit(&FDCAN1_Handler);                              //�������ǰ������
    FDCAN1_Handler.Instance=FDCAN1;
    FDCAN1_Handler.Init.FrameFormat=FDCAN_FRAME_CLASSIC;            //��ͳģʽ
    FDCAN1_Handler.Init.Mode=mode;                                  //ģʽ����
    FDCAN1_Handler.Init.AutoRetransmission=DISABLE;                 //�ر��Զ��ش�����ͳģʽ��һ��Ҫ�رգ�����
    FDCAN1_Handler.Init.TransmitPause=DISABLE;                      //�رմ�����ͣ
    FDCAN1_Handler.Init.ProtocolException=DISABLE;                  //�ر�Э���쳣����
    FDCAN1_Handler.Init.NominalPrescaler=tnbrp;                     //��Ƶϵ��
    FDCAN1_Handler.Init.NominalSyncJumpWidth=tnsjw;                 //����ͬ����Ծ��ȷ�Χ:2~128
    FDCAN1_Handler.Init.NominalTimeSeg1=tseg1;                      //tseg1��Χ:2~256
    FDCAN1_Handler.Init.NominalTimeSeg2=tseg2;                      //tseg2��Χ:2~128
    FDCAN1_Handler.Init.MessageRAMOffset=0;                         //��ϢRAMƫ��
    FDCAN1_Handler.Init.StdFiltersNbr=0;                            //��׼��ϢID�˲������
    FDCAN1_Handler.Init.ExtFiltersNbr=0;                            //��չ��ϢID�˲������
    FDCAN1_Handler.Init.RxFifo0ElmtsNbr=1;                          //����FIFO0Ԫ�ر��
    FDCAN1_Handler.Init.RxFifo0ElmtSize=FDCAN_DATA_BYTES_8;         //����FIFO0Ԫ�ش�С��8�ֽ�
    FDCAN1_Handler.Init.RxBuffersNbr=0;                             //���ջ�����
    FDCAN1_Handler.Init.TxEventsNbr=0;                              //�����¼����
    FDCAN1_Handler.Init.TxBuffersNbr=0;                             //���ͻ�����
    FDCAN1_Handler.Init.TxFifoQueueElmtsNbr=1;                      //����FIFO����Ԫ�ر��
    FDCAN1_Handler.Init.TxFifoQueueMode=FDCAN_TX_FIFO_OPERATION;    //����FIFO����ģʽ
    FDCAN1_Handler.Init.TxElmtSize=FDCAN_DATA_BYTES_8;              //���ʹ�С:8�ֽ�
    if(HAL_FDCAN_Init(&FDCAN1_Handler)!=HAL_OK) return 1;           //��ʼ��FDCAN
	
  	FDCAN1_ClkCalUnit.ClockCalibration=DISABLE;						//��ʹ��ʱ��У׼����
	FDCAN1_ClkCalUnit.ClockDivider=FDCAN_CLOCK_DIV4;				//4��Ƶ,�õ�50Mhz��fdca_tq_ck
	HAL_FDCAN_ConfigClockCalibration(&FDCAN1_Handler,&FDCAN1_ClkCalUnit);	//���÷�Ƶ

    //����RX�˲���   
    FDCAN1_RXFilter.IdType=FDCAN_STANDARD_ID;                       //��׼ID
    FDCAN1_RXFilter.FilterIndex=0;                                  //�˲�������                   
    FDCAN1_RXFilter.FilterType=FDCAN_FILTER_MASK;                   //�˲�������
    FDCAN1_RXFilter.FilterConfig=FDCAN_FILTER_TO_RXFIFO0;           //������0������FIFO0  
    FDCAN1_RXFilter.FilterID1=0x0000;                               //32λID
    FDCAN1_RXFilter.FilterID2=0x0000;                               //���FDCAN����Ϊ��ͳģʽ�Ļ���������32λ����
    if(HAL_FDCAN_ConfigFilter(&FDCAN1_Handler,&FDCAN1_RXFilter)!=HAL_OK) return 2;//�˲�����ʼ��
    HAL_FDCAN_Start(&FDCAN1_Handler);                               //����FDCAN
    HAL_FDCAN_ActivateNotification(&FDCAN1_Handler,FDCAN_IT_RX_FIFO0_NEW_MESSAGE,0);
    return 0;
}

//FDCAN1�ײ��������������ã�ʱ��ʹ��
//HAL_FDCAN_Init()����
//hsdram:FDCAN1���
void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef* hfdcan)
{

	RCC->AHB4ENR|=1<<0;  		//ʹ��PORTA��ʱ�� 
	RCC->APB1HENR|=1<<8;		//ʹ��FDCANʱ��
	GPIO_Set(GPIOA,PIN11|PIN12,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_MID,GPIO_PUPD_PU);//PA11,PA12,���ù���,�������
 	GPIO_AF_Set(GPIOA,11,9);	//PA11,AF9
	GPIO_AF_Set(GPIOA,12,9);	//PA12,AF9 	   
 
	RCC->D2CCIP1R&=(u32)3<<28;	//FDCANSEL[1:0]����
	RCC->D2CCIP1R|=(u32)1<<28;	//FDCANSEL[1:0]=1,ѡ��pll1_q_ck��ΪFDCANʱ��Դ,һ��Ϊ200Mhz
	
#if FDCAN1_RX0_INT_ENABLE     
	MY_NVIC_Init(1,0,FDCAN1_IT0_IRQn,2);//��2
#endif	
}

//�˺����ᱻHAL_FDCAN_DeInit����
//hfdcan:fdcan���
void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef* hfdcan)
{
	RCC->APB1HRSTR|=1<<8;		//FDCANRST=1,��λFDCAN
	delay_ms(10);				//�ӳ�
	RCC->APB1HRSTR&=~(1<<8);	//FDCANRST=0,������λ 
}

//can����һ������(�̶���ʽ:IDΪ0X12,��׼֡,����֡)	
//len:���ݳ���(���Ϊ8),������ΪFDCAN_DLC_BYTES_2~FDCAN_DLC_BYTES_8				     
//msg:����ָ��,���Ϊ8���ֽ�.
//����ֵ:0,�ɹ�;
//		 ����,ʧ��;
u8 FDCAN1_Send_Msg(u8* msg,u32 len)
{	
    FDCAN1_TxHeader.Identifier=0x12;                           //32λID
    FDCAN1_TxHeader.IdType=FDCAN_STANDARD_ID;                  //��׼ID
    FDCAN1_TxHeader.TxFrameType=FDCAN_DATA_FRAME;              //����֡
    FDCAN1_TxHeader.DataLength=len;                            //���ݳ���
    FDCAN1_TxHeader.ErrorStateIndicator=FDCAN_ESI_ACTIVE;            
    FDCAN1_TxHeader.BitRateSwitch=FDCAN_BRS_OFF;               //�ر������л�
    FDCAN1_TxHeader.FDFormat=FDCAN_CLASSIC_CAN;                //��ͳ��CANģʽ
    FDCAN1_TxHeader.TxEventFifoControl=FDCAN_NO_TX_EVENTS;     //�޷����¼�
    FDCAN1_TxHeader.MessageMarker=0;                           
    
    if(HAL_FDCAN_AddMessageToTxFifoQ(&FDCAN1_Handler,&FDCAN1_TxHeader,msg)!=HAL_OK) return 1;//����
    return 0;	
}

//can�ڽ������ݲ�ѯ
//buf:���ݻ�����;	 
//����ֵ:0,�����ݱ��յ�;
//		 ����,���յ����ݳ���;
u8 FDCAN1_Receive_Msg(u8 *buf)
{	
    if(HAL_FDCAN_GetRxMessage(&FDCAN1_Handler,FDCAN_RX_FIFO0,&FDCAN1_RxHeader,buf)!=HAL_OK)return 0;//��������
	return FDCAN1_RxHeader.DataLength>>16;	
}

#if FDCAN1_RX0_INT_ENABLE  
//FDCAN1�жϷ�����
void FDCAN1_IT0_IRQHandler(void)
{
    HAL_FDCAN_IRQHandler(&FDCAN1_Handler);
}

//FIFO0�ص�����
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
    u8 i=0;
    u8 rxdata[8];
    if((RxFifo0ITs&FDCAN_IT_RX_FIFO0_NEW_MESSAGE)!=RESET)   //FIFO1�������ж�
    {
        //��ȡFIFO0�н��յ�������
        HAL_FDCAN_GetRxMessage(hfdcan,FDCAN_RX_FIFO0,&FDCAN1_RxHeader,rxdata);
        printf("id:%#x\r\n",FDCAN1_RxHeader.Identifier);
        printf("len:%d\r\n",FDCAN1_RxHeader.DataLength>>16);
        for(i=0;i<8;i++)
        printf("rxdata[%d]:%d\r\n",i,rxdata[i]);
        HAL_FDCAN_ActivateNotification(hfdcan,FDCAN_IT_RX_FIFO0_NEW_MESSAGE,0);
    }
}
#endif	














