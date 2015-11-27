
#include "RemoteFunctions.h"
#include <Arduino.h>

extern ESP8266wifi *wifi;

void SetPinOn(int p)
{  
  Serial.print("Set Pin ON ");
  Serial.println(p);
  
  digitalWrite(p, 1);
}

void SetPinOff(int p)
{
  Serial.print("Set Pin OFF ");
  Serial.println(p);
  
  digitalWrite(p, 0);
}

void GetDeviceState(int)
{  
  int portA = GPIOA->regs->BSRR;
  
  String command(DEVICE_ID);
  command += "/STATE/";
  command += portA;
  wifi->send(SERVER,command.c_str());

  Serial.print("send: ");
  Serial.println(command.c_str());

  delay(2);
}

void Identify(int) 
{
  wifi->send(SERVER,DEVICE_ID);
}

