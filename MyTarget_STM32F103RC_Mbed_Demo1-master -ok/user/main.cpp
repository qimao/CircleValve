#include "mbed.h"
#include "rtos.h"
#include "USART3.h"
#include "iwdg.h"
#include "main.h"

DigitalOut led1(LED1);
DigitalOut PWM1(PWM_OUT1);
//PwmOut pwmout1(PWM_OUT1);
DigitalOut PWM2(PWM_OUT2);
DigitalOut PWM3(PWM_OUT3);
DigitalOut PWM4(PWM_OUT4);
DigitalOut PWM5(PWM_OUT5);
DigitalOut PWM6(PWM_OUT6);
DigitalOut PWM7(PWM_OUT7);
DigitalOut PWM8(PWM_OUT8);
DigitalOut PWM9(PWM_OUT9);
DigitalOut PWM10(PWM_OUT10);
DigitalOut PWM11(PWM_OUT11);
DigitalOut PWM12(PWM_OUT12);

Thread thread_u3;

int main()
{
	#ifdef USING_IWDG
//	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST)!=RESET)
//	{
//    RCC_ClearFlag();
//	}
  IWDG_Configuration();
#endif
	Init_U3_485();
	PwmInit();
	
//    Thread thread(print_thread, NULL, osPriorityNormal, DEFAULT_STACK_SIZE);
  thread_u3.start(callback(USART3_Thread,(void *)NULL));
	for(;;)
	{
		led1 = !led1;
		Thread::wait(500);
#ifdef USING_IWDG
		IWDG->KR = 0xAAAA;
#endif
  }
}

void ValveCtrl(unsigned char valve)
{
	switch(valve)
	{
		case 0:
			PWM1 = 0;
			PWM2 = 0;
			PWM3 = 0;
			PWM4 = 0;
			PWM5 = 0;
			PWM6 = 0;
			PWM7 = 0;
			PWM8 = 0;
			PWM9 = 0;
			PWM10 = 0;
			PWM11 = 0;
			PWM12 = 0;
			break;
		case 1:
			PWM1 = 1;
			PWM2 = 0;
			PWM3 = 0;
			PWM4 = 0;
			PWM5 = 0;
			PWM6 = 0;
			PWM7 = 0;
			PWM8 = 0;
			PWM9 = 0;
			PWM10 = 0;
			PWM11 = 0;
			PWM12 = 0;
			break;
		case 2:
			PWM1 = 0;
			PWM2 = 1;
			PWM3 = 0;
			PWM4 = 0;
			PWM5 = 0;
			PWM6 = 0;
			PWM7 = 0;
			PWM8 = 0;
			PWM9 = 0;
			PWM10 = 0;
			PWM11 = 0;
			PWM12 = 0;
			break;
		case 3:
			PWM1 = 0;
			PWM2 = 0;
			PWM3 = 1;
			PWM4 = 0;
			PWM5 = 0;
			PWM6 = 0;
			PWM7 = 0;
			PWM8 = 0;
			PWM9 = 0;
			PWM10 = 0;
			PWM11 = 0;
			PWM12 = 0;
			break;
		case 4:
			PWM1 = 0;
			PWM2 = 0;
			PWM3 = 0;
			PWM4 = 1;
			PWM5 = 0;
			PWM6 = 0;
			PWM7 = 0;
			PWM8 = 0;
			PWM9 = 0;
			PWM10 = 0;
			PWM11 = 0;
			PWM12 = 0;
			break;
		case 5:
			PWM1 = 0;
			PWM2 = 0;
			PWM3 = 0;
			PWM4 = 0;
			PWM5 = 1;
			PWM6 = 0;
			PWM7 = 0;
			PWM8 = 0;
			PWM9 = 0;
			PWM10 = 0;
			PWM11 = 0;
			PWM12 = 0;
			break;
		case 6:
			PWM1 = 0;
			PWM2 = 0;
			PWM3 = 0;
			PWM4 = 0;
			PWM5 = 0;
			PWM6 = 1;
			PWM7 = 0;
			PWM8 = 0;
			PWM9 = 0;
			PWM10 = 0;
			PWM11 = 0;
			PWM12 = 0;
			break;
		case 7:
			PWM1 = 0;
			PWM2 = 0;
			PWM3 = 0;
			PWM4 = 0;
			PWM5 = 0;
			PWM6 = 0;
			PWM7 = 1;
			PWM8 = 0;
			PWM9 = 0;
			PWM10 = 0;
			PWM11 = 0;
			PWM12 = 0;
			break;
		case 8:
			PWM1 = 0;
			PWM2 = 0;
			PWM3 = 0;
			PWM4 = 0;
			PWM5 = 0;
			PWM6 = 0;
			PWM7 = 0;
			PWM8 = 1;
			PWM9 = 0;
			PWM10 = 0;
			PWM11 = 0;
			PWM12 = 0;
			break;
		case 9:
			PWM1 = 0;
			PWM2 = 0;
			PWM3 = 0;
			PWM4 = 0;
			PWM5 = 0;
			PWM6 = 0;
			PWM7 = 0;
			PWM8 = 0;
			PWM9 = 1;
			PWM10 = 0;
			PWM11 = 0;
			PWM12 = 0;
			break;
		case 10:
			PWM1 = 0;
			PWM2 = 0;
			PWM3 = 0;
			PWM4 = 0;
			PWM5 = 0;
			PWM6 = 0;
			PWM7 = 0;
			PWM8 = 0;
			PWM9 = 0;
			PWM10 = 1;
			PWM11 = 0;
			PWM12 = 0;
			break;
		case 11:
			PWM1 = 0;
			PWM2 = 0;
			PWM3 = 0;
			PWM4 = 0;
			PWM5 = 0;
			PWM6 = 0;
			PWM7 = 0;
			PWM8 = 0;
			PWM9 = 0;
			PWM10 = 0;
			PWM11 = 1;
			PWM12 = 0;
			break;
		case 12:
			PWM1 = 0;
			PWM2 = 0;
			PWM3 = 0;
			PWM4 = 0;
			PWM5 = 0;
			PWM6 = 0;
			PWM7 = 0;
			PWM8 = 0;
			PWM9 = 0;
			PWM10 = 0;
			PWM11 = 0;
			PWM12 = 1;
			break;
		default:
			break;
	}
}

void PwmInit(void)
{
	PWM1 = 0;
	PWM2 = 0;
	PWM3 = 0;
	PWM4 = 0;
	PWM5 = 0;
	PWM6 = 0;
	PWM7 = 0;
	PWM8 = 0;
	PWM9 = 0;
	PWM10 = 0;
	PWM11 = 0;
	PWM12 = 0;
}

