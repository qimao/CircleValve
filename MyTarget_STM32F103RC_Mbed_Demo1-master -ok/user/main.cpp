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
unsigned int OnOffFlag = 0;

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

/*
valve:待操作的环阀通道，即通道地址
onoff:0xFF 或者 0x00
*/
void ValveCtrl(unsigned char valve,unsigned char onoff)
{
	switch(valve)
	{
		case 0:
			PWM1 = (onoff>>7);
		if(onoff == 0xFF)
			OnOffFlag |= (0x0001<<0);
		else OnOffFlag &= ~(0x0001<<0);
			break;
		case 1:
			PWM2 = (onoff>>7);
		if(onoff == 0xFF)
			OnOffFlag |= (0x0001<<1);
		else OnOffFlag &= ~(0x0001<<1);
			break;
		case 2:
			PWM3 = (onoff>>7);
		if(onoff == 0xFF)
			OnOffFlag |= (0x0001<<2);
		else OnOffFlag &= ~(0x0001<<2);
			break;
		case 3:
			PWM4 = (onoff>>7);
		if(onoff == 0xFF)
			OnOffFlag |= (0x0001<<3);
		else OnOffFlag &= ~(0x0001<<3);
			break;
		case 4:
			PWM5 = (onoff>>7);
		if(onoff == 0xFF)
			OnOffFlag |= (0x0001<<4);
		else OnOffFlag &= ~(0x0001<<4);
			break;
		case 5:
			PWM6 = (onoff>>7);
		if(onoff == 0xFF)
			OnOffFlag |= (0x0001<<5);
		else OnOffFlag &= ~(0x0001<<5);
			break;
		case 6:
			PWM7 = (onoff>>7);
		if(onoff == 0xFF)
			OnOffFlag |= (0x0001<<6);
		else OnOffFlag &= ~(0x0001<<6);
			break;
		case 7:
			PWM8 = (onoff>>7);
		if(onoff == 0xFF)
			OnOffFlag |= (0x0001<<7);
		else OnOffFlag &= ~(0x0001<<7);
			break;
		case 8:
			PWM9 = (onoff>>7);
		if(onoff == 0xFF)
			OnOffFlag |= (0x0001<<8);
		else OnOffFlag &= ~(0x0001<<8);
			break;
		case 9:
			PWM10 = (onoff>>7);
		if(onoff == 0xFF)
			OnOffFlag |= (0x0001<<9);
		else OnOffFlag &= ~(0x0001<<9);
			break;
		case 10:
			PWM11 = (onoff>>7);
		if(onoff == 0xFF)
			OnOffFlag |= (0x0001<<10);
		else OnOffFlag &= ~(0x0001<<10);
			break;
		case 11:
			PWM12 = (onoff>>7);
		if(onoff == 0xFF)
			OnOffFlag |= (0x0001<<11);
		else OnOffFlag &= ~(0x0001<<11);
			break;
		case 0x11:
			if(onoff == 0xFF)
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
				OnOffFlag = 0;
			}
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

