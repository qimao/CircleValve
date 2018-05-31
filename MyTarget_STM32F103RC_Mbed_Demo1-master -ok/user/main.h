#ifndef __MAIN_H__
#define __MAIN_H__

//#define VDUTY 0.5

extern unsigned int OnOffFlag;

//void ValveCtrlThread(void const *argument);
void ValveCtrl(unsigned char valve,unsigned char onoff);
void PwmInit(void);

#endif
