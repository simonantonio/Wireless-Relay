
#ifndef _REMOTE_FUNCTIONS_H_
#define _REMOTE_FUNCTIONS_H_

#include "ESP8266wifi.h"
#include "config.h"

void SetPinOn(int);
void SetPinOff(int);
void GetDeviceState(int);
void Identify(int);

#endif //_REMOTE_FUNCTIONS_H_
