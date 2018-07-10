/*仿Board Support中的LED_MCBSTM32E.c*/
#include "GPIO_STM32F10x.h"
#include "Valve.h"

unsigned int OnOffFlag = 0;

const GPIO_PIN_ID Pin_Valve[] = {
  { GPIOB, 12 },
  { GPIOB, 13 },
  { GPIOB, 14 },
  { GPIOB, 15 },
  { GPIOA,  8 },
  { GPIOA,  9 },
  { GPIOA, 10 },
  { GPIOA, 11 },
	{ GPIOA, 15 },
	{ GPIOB,  3 },
	{ GPIOB,  4 },
	{ GPIOB,  5 },
	{ GPIOB,  6 },
	{ GPIOB,  7 },
	{ GPIOB,  8 },
	{ GPIOB,  9 },
};

#define Valve_COUNT (sizeof(Pin_Valve)/sizeof(GPIO_PIN_ID))

/**
  \fn          int32_t LED_Initialize (void)
  \brief       Initialize LEDs
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Valve_Initialize (void) {
  uint32_t n;
	
	//PA15 PB3 PB4需要打开复用功能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//打开复用时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//JTAG-DP Disabled and SW-DP Enabled
	
  /* Configure pins: Push-pull Output Mode (50 MHz) with Pull-down resistors */
  for (n = 0; n < Valve_COUNT; n++) {
#ifdef USB_SOFT_CONNECT
    if (n == 6) continue;
#endif
    GPIO_PortClock   (Pin_Valve[n].port, true);
    GPIO_PinWrite    (Pin_Valve[n].port, Pin_Valve[n].num, 0);
    GPIO_PinConfigure(Pin_Valve[n].port, Pin_Valve[n].num,
                      GPIO_OUT_PUSH_PULL, 
                      GPIO_MODE_OUT2MHZ);
  }

  return 0;
}

/**
  \fn          int32_t LED_Uninitialize (void)
  \brief       De-initialize LEDs
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Valve_Uninitialize (void) {
  uint32_t n;

  /* Configure pins: Input mode, without Pull-up/down resistors */
  for (n = 0; n < Valve_COUNT; n++) {
#ifdef USB_SOFT_CONNECT
    if (n == 6) continue;
#endif
    GPIO_PinConfigure(Pin_Valve[n].port, Pin_Valve[n].num,
                      GPIO_IN_FLOATING,
                      GPIO_MODE_INPUT);
  }

  return 0;
}

/**
  \fn          int32_t LED_On (uint32_t num)
  \brief       Turn on requested LED
  \param[in]   num  LED number
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Valve_On (uint32_t num) {
#ifdef USB_SOFT_CONNECT
  if (num == 6) return;
#endif
  GPIO_PinWrite(Pin_Valve[num].port, Pin_Valve[num].num, 1);
  return 0;
}

/**
  \fn          int32_t LED_Off (uint32_t num)
  \brief       Turn off requested LED
  \param[in]   num  LED number
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Valve_Off (uint32_t num) {
#ifdef USB_SOFT_CONNECT
  if (num == 6) return;
#endif
  GPIO_PinWrite(Pin_Valve[num].port, Pin_Valve[num].num, 0);
  return 0;
}

/**
  \fn          int32_t LED_SetOut (uint32_t val)
  \brief       Write value to LEDs
  \param[in]   val  value to be displayed on LEDs
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Valve_SetOut (uint32_t val) {
  uint32_t n;

  for (n = 0; n < Valve_COUNT; n++) {
#ifdef USB_SOFT_CONNECT
    if (n == 6) continue;
#endif
    if (val & (1<<n)) {
      Valve_On (n);
    } else {
      Valve_Off(n);
    }
  }
	return 0;
}

int32_t Valve_Set(uint32_t num,uint32_t val)
{
	if(val == 0)
		Valve_Off(num);
	else
		Valve_On(num);
	
  return 0;
}

/**
  \fn          uint32_t LED_GetCount (void)
  \brief       Get number of LEDs
  \return      Number of available LEDs
*/
uint32_t Valve_GetCount (void) {
  return Valve_COUNT;
}

/*
valve:待操作的环阀通道，即通道地址
onoff:0xFF 或者 0x00
*/
void ValveCtrl(unsigned char valve,unsigned char onoff)
{
	if(onoff == 0xFF)
	{
		Valve_SetOut(0);//全关
		if(valve != 0x0E)
		{
			if(valve == 0x1E)
				Valve_On(0x0E);
			else
				Valve_On(valve);
		}
	}
	
//	if(valve == 0x0E)
//	{
//		if(onoff == 0xFF)
//		{
//			Valve_SetOut(0);
//			OnOffFlag = 0;
//		}
//	}
//	else
//	{
//		Valve_Set(valve,(onoff>>7));
//		if(onoff == 0xFF)
//			OnOffFlag |= (0x0001<<valve);
//		else OnOffFlag &= ~(0x0001<<valve);
//	}
}
























