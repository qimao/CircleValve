#ifndef __VALVE_H__
#define __VALVE_H__

#include "GPIO_STM32F10x.h"

extern int32_t  Valve_Initialize   (void);
extern int32_t  Valve_Uninitialize (void);
extern int32_t  Valve_On           (uint32_t num);
extern int32_t  Valve_Off          (uint32_t num);
extern int32_t  Valve_SetOut       (uint32_t val);
extern uint32_t Valve_GetCount     (void);
int32_t Valve_Set(uint32_t num,uint32_t val);
void ValveCtrl(unsigned char valve,unsigned char onoff);

#endif
