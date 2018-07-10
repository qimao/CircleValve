#include "iwdg.h"
#include "mbed.h"
#include "rtos.h"

void IWDG_Configuration(void)
{
 /* 写入0x5555,用于允许狗狗寄存器写入功能 */
// IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  IWDG->KR = 0x5555;
 
 /* 狗狗时钟分频,40K/256=156HZ(6.4ms)*/
// IWDG_SetPrescaler(IWDG_Prescaler_256);
  IWDG->PR = 0x06;
 
 /* 喂狗时间 5s/6.4MS=781 .注意不能大于0xfff*/
 //IWDG_SetReload(781);

 /* 喂狗时间 26.208s/6.4MS=4095=0xfff .注意不能大于0xfff*/
// IWDG_SetReload(4095);//测试出大约31s
  IWDG->RLR = 1000;
 
 /* 喂狗*/
// IWDG_ReloadCounter();
  IWDG->KR = 0xAAAA;
 
 /* 使能狗狗*/
// IWDG_Enable();
  IWDG->KR = 0xCCCC;
}



