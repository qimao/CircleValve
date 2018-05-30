#include "mbed.h"
#include "rtos.h"
#include "USART3.h"
#include "crc.h"
#include "main.h"

static DigitalOut USART3EN(U3EN);
static Serial U3_485(U3TX,U3RX);
extern Thread thread_u3;

unsigned char U3RecLen = 0;
unsigned char U3RecBuf[U3RECLENS] = {0};
unsigned char U3RxBuf[U3RECLENS] = {0};
Mutex U3txMutex;
unsigned char U3SendBuf[U3RECLENS] = {0x01,0x05,0x00,0xFE,0x00,0x00,0xAC,0x3A};

uint32_t Init_U3_485(void)
{
	U3_485.baud(9600);
	U3_485.format(8,SerialBase::None,1);
	USART3EN = 0;
	USART3->CR1 |= 1<<5;       //RXNE interrupt enable
  USART3->CR1 |= 1<<8;       //PE interrupt enable
    
  U3_485.attach(&RX_U3_485,SerialBase::RxIrq);
	
	return 1;
}


static void RX_U3_485(void)
{
	volatile unsigned char temp;
	while(U3_485.readable())
	{
		U3RecBuf[U3RecLen++] = USART3->DR;//U3_485.getc();//采用getc()接收数据会卡死
		if(U3RecBuf[0] == SlaveAddr)
		{
			if(U3RecLen > 1)
			{
				if(U3RecBuf[1] == ModbusCmd5)
				{
					if(U3RecLen >= U3RECLENS)
					{
						memcpy(U3RxBuf,U3RecBuf,U3RECLENS);
						thread_u3.signal_set(0x01);
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
	}
}

uint32_t TX_U3_485(unsigned char *TxData, int length)
{
	U3txMutex.lock();
	short i = 0;
	
//	U3_485.attach(&RX_U3_485,SerialBase::RxIrq);
//	while(U3_485.readable())
//		U3_485.getc();
	
	USART3EN = 1;
	Thread::wait(5);
	for(;i<length;i++)
	{
		while(U3_485.writeable() == 0);
		U3_485.putc(TxData[i]);
	}
	while((USART3->SR & USART_FLAG_TC) == RESET);
	USART3EN = 0;
	Thread::wait(100);
	U3txMutex.unlock();
	
	return 1;
}

void USART3_Thread(void const *args)
{
	unsigned int crc16tem;
	
	for(;;)
	{
		Thread::signal_wait(0x01);
		U3_485.attach(NULL,SerialBase::RxIrq);
		crc16tem = crc16(U3RxBuf,U3RECLENS-2);
		if((U3RxBuf[U3RECLENS-2] != (unsigned char)((crc16tem >> 8 )& 0x00FF)) || (U3RxBuf[U3RECLENS-1] != (unsigned char)(crc16tem & 0x00FF)))
      return;
		if(U3RxBuf[2] != 0x00 || U3RxBuf[4] != 0xFF || U3RxBuf[5] != 0x00)
			return;
		ValveCtrl(U3RxBuf[3]);
//		Thread::wait(200);
		U3SendBuf[0] = SlaveAddr;
		TX_U3_485(U3SendBuf,U3RECLENS);//U3RxBuf
		U3_485.attach(&RX_U3_485,SerialBase::RxIrq);
	}
}

