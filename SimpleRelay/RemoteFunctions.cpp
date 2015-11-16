
#include "RemoteFunctions.h"
#include <Arduino.h>

extern ESP8266 *wifi;

//void InitCommands(ESP8266 *mwifi)
//{
//  wifi = mwifi;
//}

//add some constraints to digital writes
int validPins[] = 
{
  D18, D19, D20, D21, D22, D23, D24
};

bool find(int n, const int *array)
{
  for(int i = 0; i <= sizeof(validPins) * sizeof(int); i++)
  {
    if(array[i] == n)
    {
      return true;
    }
  }
  return false;
}

void SetPinOn(int p)
{
  if(find(p,validPins))
  {
    digitalWrite(p, 1);
  }
}
void SetPinOff(int p)
{
  if(find(p,validPins))
  {  
    digitalWrite(p, 0);
  }
}
void GetVersion(int)
{
  String command = wifi->getLocalIP() + "/W/Version/1/";
  wifi->send((const uint8_t*)command.c_str(), strlen(command.c_str()));
}

void GetPinState(int p )
{  
  int state = digitalRead(p);
  String command = wifi->getLocalIP() + "/W/PinState/" + state + "/";
  wifi->send((const uint8_t*)command.c_str(), strlen(command.c_str()));
}

void GetCommands(int)
{
  String command = wifi->getLocalIP() + "/W/COMMANDS/PIN_ON,PIN_OFF,PIN_STATE,VERSION,HELP/";
  wifi->send((const uint8_t*)command.c_str(), strlen(command.c_str()));
}


