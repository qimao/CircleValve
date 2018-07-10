/**
  ******************************************************************************
  * @file   : USARTtest.h
  * @author : wind
  * @version: 
  * @date   : 2015,03,29
  * @brief  :
  ******************************************************************************
  */
/* ---------------------------------------------------------------------------*/

#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include "stdio.h"

#define SlaveAddr 0x01	//板子地址
#define U3RECLENS 8
#define ModbusCmd5	0x05
#define ModbusCmd4	0x04
#define ModbusCmd4Len 33

#define U3_SEND		GPIO_SetBits(GPIOB,GPIO_Pin_1)
#define U3_RECEIVE	GPIO_ResetBits(GPIOB,GPIO_Pin_1)

int Init_Thread_USART(void);
void Thread_USART(void const *argument);
uint32_t TX_U3_485(unsigned char *TxData, int length);
void USART_Config(void);
void ParseRecieve(unsigned char rxlen);

#endif
