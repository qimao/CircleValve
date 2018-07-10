/****************************************Copyright (c)***************************************
**                                江苏苏科畅联科技有限公司
**-------------------------------------------------------------------------------------------
** File name:           苏科畅联视频客流统计分析软件
** Modified by:         Qimao
** Modified date:      2015-5-16
** MCU:        	    STM32F407ZGT6
** Version:              V2.1.4
** Board Version:	    V1.2
** Descriptions:        客流统计/站点匹配/数据上传
**-------------------------------------------------------------------------------------------

********************************************************************************************/
#include "stm32f10x.h"
#include "iwdg.h"

__IO uint32_t LsiFreq = 42000;

void IWDG_Configuration(void)
{
 /* 写入0x5555,用于允许狗狗寄存器写入功能 */
 IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
 
 /* 狗狗时钟分频,40K/256=156HZ(6.4ms)*/
 IWDG_SetPrescaler(IWDG_Prescaler_256);//256
 
 /* 喂狗时间 5s/6.4MS=781 .注意不能大于0xfff*/
 IWDG_SetReload(781);

 /* 喂狗时间 26.208s/6.4MS=4095=0xfff .注意不能大于0xfff*/
// IWDG_SetReload(4095);//测试出大约31s
 
 /* 喂狗*/
 IWDG_ReloadCounter();
 
 /* 使能狗狗*/
 IWDG_Enable();
}



