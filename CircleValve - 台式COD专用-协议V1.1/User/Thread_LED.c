#include "Thread_LED.h"
#include "led.h"

osThreadId tid_Thread_LED;												// thread id
osThreadDef(Thread_LED, osPriorityNormal, 1, 0);	// thread object

/*
File Name:创建Thread_LED线程并初始化
return:错误 -1
				成功 0
*/
int Init_Thread_LED (void)
{
	LED_Init();
	tid_Thread_LED = osThreadCreate(osThread(Thread_LED), NULL);
	if (!tid_Thread_LED)
		return(-1);
	
	return(0);
}

void Thread_LED(void const *argument)						//thread function
{
	
	for(;;)
	{
		osDelay(500);
		LED1_CROSS;
	}
}










