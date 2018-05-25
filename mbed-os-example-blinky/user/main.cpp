#include "mbed.h"
#include "USART3.h"
#include "iwdg.h"
#include "main.h"

DigitalOut led1(LED1);
Thread thread_u3;

PwmOut pwm1(PWM_OUT1);
PwmOut pwm2(PWM_OUT2);
PwmOut pwm3(PWM_OUT3);
PwmOut pwm4(PWM_OUT4);
PwmOut pwm5(PWM_OUT5);
PwmOut pwm6(PWM_OUT6);
PwmOut pwm7(PWM_OUT7);
PwmOut pwm8(PWM_OUT8);
PwmOut pwm9(PWM_OUT9);
PwmOut pwm10(PWM_OUT10);
PwmOut pwm11(PWM_OUT11);
PwmOut pwm12(PWM_OUT12);



// main() runs in its own thread in the OS
int main() {
	SetSysClock();
#ifdef USING_IWDG
//	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST)!=RESET)
//	{
//    RCC_ClearFlag();
//	}
  IWDG_Configuration();
#endif
	Init_U3_485();
//	PwmInit();
	
//	Thread thread1(ValveCtrlThread,NULL,osPriorityNormal,DEFAULT_STACK_SIZE);
//	Thread thread2 = new Thread(USART3_Thread,NULL,osPriorityNormal);

	thread_u3.start(callback(USART3_Thread,(void *)NULL));
	while (true) {
			led1 = !led1;
		Thread::wait(500);//wait(0.5);
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
			pwm1.write(0);
			pwm2.write(0);
			pwm3.write(0);
			pwm4.write(0);
			pwm5.write(0);
			pwm6.write(0);
			pwm7.write(0);
			pwm8.write(0);
			pwm9.write(0);
			pwm10.write(0);
			pwm11.write(0);
			pwm12.write(0);
			break;
		case 1:
			pwm1.write(VDUTY);
			pwm2.write(0);
			pwm3.write(0);
			pwm4.write(0);
			pwm5.write(0);
			pwm6.write(0);
			pwm7.write(0);
			pwm8.write(0);
			pwm9.write(0);
			pwm10.write(0);
			pwm11.write(0);
			pwm12.write(0);
			break;
		case 2:
			pwm1.write(0);
			pwm2.write(VDUTY);
			pwm3.write(0);
			pwm4.write(0);
			pwm5.write(0);
			pwm6.write(0);
			pwm7.write(0);
			pwm8.write(0);
			pwm9.write(0);
			pwm10.write(0);
			pwm11.write(0);
			pwm12.write(0);
			break;
		case 3:
			pwm1.write(0);
			pwm2.write(0);
			pwm3.write(VDUTY);
			pwm4.write(0);
			pwm5.write(0);
			pwm6.write(0);
			pwm7.write(0);
			pwm8.write(0);
			pwm9.write(0);
			pwm10.write(0);
			pwm11.write(0);
			pwm12.write(0);
			break;
		case 4:
			pwm1.write(0);
			pwm2.write(0);
			pwm3.write(0);
			pwm4.write(VDUTY);
			pwm5.write(0);
			pwm6.write(0);
			pwm7.write(0);
			pwm8.write(0);
			pwm9.write(0);
			pwm10.write(0);
			pwm11.write(0);
			pwm12.write(0);
			break;
		case 5:
			pwm1.write(0);
			pwm2.write(0);
			pwm3.write(0);
			pwm4.write(0);
			pwm5.write(VDUTY);
			pwm6.write(0);
			pwm7.write(0);
			pwm8.write(0);
			pwm9.write(0);
			pwm10.write(0);
			pwm11.write(0);
			pwm12.write(0);
			break;
		case 6:
			pwm1.write(0);
			pwm2.write(0);
			pwm3.write(0);
			pwm4.write(0);
			pwm5.write(0);
			pwm6.write(VDUTY);
			pwm7.write(0);
			pwm8.write(0);
			pwm9.write(0);
			pwm10.write(0);
			pwm11.write(0);
			pwm12.write(0);
			break;
		case 7:
			pwm1.write(0);
			pwm2.write(0);
			pwm3.write(0);
			pwm4.write(0);
			pwm5.write(0);
			pwm6.write(0);
			pwm7.write(VDUTY);
			pwm8.write(0);
			pwm9.write(0);
			pwm10.write(0);
			pwm11.write(0);
			pwm12.write(0);
			break;
		case 8:
			pwm1.write(0);
			pwm2.write(0);
			pwm3.write(0);
			pwm4.write(0);
			pwm5.write(0);
			pwm6.write(0);
			pwm7.write(0);
			pwm8.write(VDUTY);
			pwm9.write(0);
			pwm10.write(0);
			pwm11.write(0);
			pwm12.write(0);
			break;
		case 9:
			pwm1.write(0);
			pwm2.write(0);
			pwm3.write(0);
			pwm4.write(0);
			pwm5.write(0);
			pwm6.write(0);
			pwm7.write(0);
			pwm8.write(0);
			pwm9.write(VDUTY);
			pwm10.write(0);
			pwm11.write(0);
			pwm12.write(0);
			break;
		case 10:
			pwm1.write(0);
			pwm2.write(0);
			pwm3.write(0);
			pwm4.write(0);
			pwm5.write(0);
			pwm6.write(0);
			pwm7.write(0);
			pwm8.write(0);
			pwm9.write(0);
			pwm10.write(VDUTY);
			pwm11.write(0);
			pwm12.write(0);
			break;
		case 11:
			pwm1.write(0);
			pwm2.write(0);
			pwm3.write(0);
			pwm4.write(0);
			pwm5.write(0);
			pwm6.write(0);
			pwm7.write(0);
			pwm8.write(0);
			pwm9.write(0);
			pwm10.write(0);
			pwm11.write(VDUTY);
			pwm12.write(0);
			break;
		case 12:
			pwm1.write(0);
			pwm2.write(0);
			pwm3.write(0);
			pwm4.write(0);
			pwm5.write(0);
			pwm6.write(0);
			pwm7.write(0);
			pwm8.write(0);
			pwm9.write(0);
			pwm10.write(0);
			pwm11.write(0);
			pwm12.write(VDUTY);
			break;
		default:
			break;
	}
}

void PwmInit(void)
{
	pwm1.period_ms(10);
	pwm2.period_ms(10);
	pwm3.period_ms(10);
	pwm4.period_ms(10);
	pwm5.period_ms(10);
	pwm6.period_ms(10);
	pwm7.period_ms(10);
	pwm8.period_ms(10);
	pwm9.period_ms(10);
	pwm10.period_ms(10);
	pwm11.period_ms(10);
	pwm12.period_ms(10);
	
	pwm1.write(0);
	pwm2.write(0);
	pwm3.write(0);
	pwm4.write(0);
	pwm5.write(0);
	pwm6.write(0);
	pwm7.write(0);
	pwm8.write(0);
	pwm9.write(0);
	pwm10.write(0);
	pwm11.write(0);
	pwm12.write(0);
}


