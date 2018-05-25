#include "iwdg.h"
#include "mbed.h"
#include "rtos.h"

void IWDG_Configuration(void)
{
 /* д��0x5555,�����������Ĵ���д�빦�� */
// IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  IWDG->KR = 0x5555;
 
 /* ����ʱ�ӷ�Ƶ,40K/256=156HZ(6.4ms)*/
// IWDG_SetPrescaler(IWDG_Prescaler_256);
  IWDG->PR = 0x06;
 
 /* ι��ʱ�� 5s/6.4MS=781 .ע�ⲻ�ܴ���0xfff*/
 //IWDG_SetReload(781);

 /* ι��ʱ�� 26.208s/6.4MS=4095=0xfff .ע�ⲻ�ܴ���0xfff*/
// IWDG_SetReload(4095);//���Գ���Լ31s
  IWDG->RLR = 1000;
 
 /* ι��*/
// IWDG_ReloadCounter();
  IWDG->KR = 0xAAAA;
 
 /* ʹ�ܹ���*/
// IWDG_Enable();
  IWDG->KR = 0xCCCC;
}



