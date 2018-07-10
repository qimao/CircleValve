#include "cmsis_os.h"
#include "stm32f10x.h" 
#include "led.h"
#include "Thread_LED.h"
#include "IWDG.h"
#include "Thread_USART.h"
#include "Valve.h"

osThreadId tid_IWDG;
void Thread_IWDG(void const *argument) {
	for (;;) {	
		IWDG_ReloadCounter(); //手动喂狗
		osDelay(200);
	}
}
osThreadDef(Thread_IWDG, osPriorityNormal, 1, 0);


int main(void)
{
	osKernelInitialize();                     /* initialize CMSIS-RTOS          */
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST)!=RESET)
	{
    RCC_ClearFlag();
	}
	IWDG_Configuration();
	tid_IWDG = osThreadCreate(osThread(Thread_IWDG), NULL);
	Valve_Initialize();//阀通道控制口初始化
	Init_Thread_LED();
	Init_Thread_USART();
	
	osKernelStart();                          /* start thread execution         */
	
	for(;;)
	{
//		LED1_CROSS;
		osDelay(100);
	}
}



