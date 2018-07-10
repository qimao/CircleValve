#include "cmsis_os.h"
#include "stm32f10x.h" 
#include "Thread_USART.h"
#include "string.h"
#include "crc.h"
#include "Valve.h"

osThreadId tid_Thread_USART;												// thread id
osThreadDef(Thread_USART, osPriorityNormal, 1, 0);	// thread object

unsigned char U3RecLen = 0;
unsigned char U3RecBuf[U3RECLENS] = {0};
unsigned char U3RxBuf[U3RECLENS] = {0};
unsigned char U3SendBuf[ModbusCmd4Len] = {SlaveAddr,ModbusCmd4,0x1C};
extern unsigned int OnOffFlag;

void USART_Config(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;   //USART3对应的TX引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;   //USART3对应的RX引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
		
	//U3EN
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_1);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStruct);

	USART_InitStructure.USART_BaudRate = 57600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_DeInit(USART3);
	USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//使能中断
	USART_ITConfig(USART3,USART_IT_ERR,ENABLE);//必须要分开写
  USART_ITConfig(USART3,USART_IT_PE,ENABLE);//必须要分开写 
	//解决第1个字节无法正确发送出去的问题
	USART_ClearFlag(USART3, USART_FLAG_TC); /* 清发送成功标志，Transmission Complete flag */

	USART_Cmd(USART3, ENABLE);    //打开串口
	
	U3_RECEIVE;
}

//中断处理函数 bug:先接收CC 03 00 00 00 DD AC 01再接收01 05 00 0E FF 00 ED F9时出错,需要发送两次
void USART3_IRQHandler (void)
{
	volatile unsigned char temp;
	
	while(USART_GetFlagStatus(USART3,USART_FLAG_RXNE) == SET)
	{
		U3RecBuf[U3RecLen++] = USART3->DR;//U3_485.getc();//采用getc()接收数据会卡死
		if(U3RecBuf[0] == SlaveAddr)
		{
			if(U3RecLen > 1)
			{
				if((U3RecBuf[1] == ModbusCmd5) || (U3RecBuf[1] == ModbusCmd4))
				{
					if(U3RecLen >= U3RECLENS)
					{
						memcpy(U3RxBuf,U3RecBuf,U3RECLENS);
						osSignalSet(tid_Thread_USART,0x01);
						U3RecLen = 0;
						U3RecBuf[0] = 0;
						U3RecBuf[1] = 0;
					}
				}
				else U3RecLen = 0;
			}
		}
		else
		{
			U3RecLen = 0;
		}
		
		if(((USART3->SR) & USART_FLAG_ORE) != RESET)//OverRun Error interrupt
		{
				temp = USART3->SR;
				temp = USART3->DR;
		}
		if(((USART3->SR) & USART_FLAG_NE) != RESET)//Noise Error interrupt
		{
				temp = USART3->SR;
				temp = USART3->DR;
		}
		if(((USART3->SR) & USART_FLAG_FE) != RESET)//Framing error interrupt
		{
				temp = USART3->SR;
				temp = USART3->DR;
		}
		if(((USART3->SR) & USART_FLAG_PE) != RESET)//Parity error interrupt
		{
				temp = USART3->SR;
				temp = USART3->DR;
		}

//		while (!USART_GetFlagStatus(USART3,USART_FLAG_TC));
	}
	
	USART_ClearITPendingBit(USART3,USART_IT_RXNE);//清中断标志位
}

void ParseRecieve(unsigned char RecLen)	 //包解析
{
  unsigned int  uIndex;
  unsigned int crc16tem;
          
  if(U3RxBuf[2] == 0x00)
  {
    crc16tem=((unsigned int)(U3RxBuf[U3RECLENS-2]) << 8) | U3RxBuf[U3RECLENS-1]; // calculate CRC check, erase two CRC byte
    uIndex = crc16(U3RxBuf,U3RECLENS-2);
    
    if(crc16tem==uIndex)// crc16检校正确
    {	 
			if(U3RxBuf[1] == ModbusCmd5)
				{
					if((U3RxBuf[3] == 0x11) && (U3RxBuf[4] == 0x00));
					else
					{
						ValveCtrl(U3RxBuf[3],U3RxBuf[4]);
						TX_U3_485(U3RxBuf,U3RECLENS);
					}
				}
				else if(U3RxBuf[1] == ModbusCmd4)
				{
					if((U3RxBuf[3] == 0x0A) && (U3RxBuf[4] == 0x00) && (U3RxBuf[5] == 0x01))//01 03 00 0A 00 01 xx xx
					{
						U3SendBuf[25] = (unsigned char)((OnOffFlag>>8) & 0xFF);
						U3SendBuf[26] = (unsigned char)(OnOffFlag & 0xFF);
						crc16tem = crc16(U3SendBuf,31);
						U3SendBuf[31] = (unsigned char)((crc16tem >> 8 ) & 0x0FF);
						U3SendBuf[32] = (unsigned char)(crc16tem & 0x0FF);
						TX_U3_485(U3SendBuf,ModbusCmd4Len);
					}
				}
		}
	}
}


void USART_Send(USART_TypeDef* USARTx, u8 *buf, u32 len)
{       
	u32 i=0;
	for(;i<len;i++)//for(u32 i=0;i<len;i++)Keil不兼容
	{
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET); //
		USART_SendData(USARTx,*(buf+i)); 
	}          		
}

uint32_t TX_U3_485(unsigned char *TxData, int length)
{
	 int i = 0;

	 U3_SEND;
	 osDelay(5);//5ms
	 for(i=0;i<length;i++)
	 {
			while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET); //
			USART_SendData(USART3,*(TxData+i));
	 }
	 while((USART3->SR & USART_FLAG_TC) == RESET);
	 U3_RECEIVE;
	 
	 return 1;
}


int Init_Thread_USART(void)
{
	USART_Config();
	tid_Thread_USART = osThreadCreate(osThread(Thread_USART), NULL);
	if (!tid_Thread_USART)
		return(-1);
	
	return(0);
}

void Thread_USART(void const *argument)
{
	for(;;)
	{
		osSignalWait(0x01, osWaitForever);
		USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);
		ParseRecieve(U3RECLENS);
		USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
		osDelay(100);
	}
}




