#ifndef __USART3_H__
#define __USART3_H__
#include "mbed.h"
 
#define U3RECLENS 8
#define SlaveAddr 0x06	//板子地址 根据不同板子设置不同的地址
#define ModbusCmd5	0x05

extern unsigned char U3RxBuf[U3RECLENS];

uint32_t Init_U3_485(void);
static void RX_U3_485(void);
uint32_t TX_U3_485(unsigned char *TxData, int length);
void USART3_Thread(void const *args);

#endif
