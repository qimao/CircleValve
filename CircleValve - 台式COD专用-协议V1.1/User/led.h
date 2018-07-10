#include "stm32f10x.h"

#define LED1_CROSS  	 GPIOA->ODR ^= GPIO_Pin_12
#define LED1_OFF         GPIO_SetBits(GPIOA,GPIO_Pin_12)
#define LED1_ON          GPIO_ResetBits(GPIOA,GPIO_Pin_12)

void LED_Init(void);
