
#ifndef _REMOTE_FUNCTIONS_H_
#define _REMOTE_FUNCTIONS_H_

#include "ESP8266.h"

void GetPinState(int);
void GetCommands(int);

void SetPinOn(int);
void SetPinOff(int);
void GetVersion(int);

//void InitCommands(ESP8266 *mwifi);


#endif //_REMOTE_FUNCTIONS_H_
