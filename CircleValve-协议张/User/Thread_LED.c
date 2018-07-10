#include "Thread_LED.h"
#include "led.h"

osThreadId tid_Thread_LED;												// thread id
osThreadDef(Thread_LED, osPriorityNormal, 1, 0);	// thread object

/*
File Name:����Thread_LED�̲߳���ʼ��
return:���� -1
				�ɹ� 0
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










