
#include "RemoteFunctions.h"
#include <Arduino.h>

extern ESP8266wifi *wifi;

//void InitCommands(ESP8266 *mwifi)
//{
//  wifi = mwifi;
//}

//add some constraints to digital writes
int validPins[] = 
{
  D18, D19, D20, D21, D22, D23, D24,
  18, 19, 20, 21, 22, 23, 24
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
  Serial.print("Set Pin ON ");
  Serial.println(p);
  
  if(find(p,validPins))
  {
    digitalWrite(p, 1);
  }  
  else
  {
    Serial.print("Not a valid pin to turn ON: ");
    Serial.println(p);
  }
}
void SetPinOff(int p)
{
  Serial.print("Set Pin OFF ");
  Serial.println(p);
  
  if(find(p,validPins))
  {  
    digitalWrite(p, 0);
  }
  else
  {
    Serial.print("Not a valid pin to turn OFF: ");
    Serial.println(p);
  }
}
void GetVersion(int)
{
  String command(DEVICE_ID);
  command += "/W/VERSION/1/";
  wifi->send(SERVER,command.c_str());
}

void GetPinState(int p )
{  
  int state = digitalRead(p);
  
  //int state = digitalReadOutputPin(p);
  
  String command(DEVICE_ID);
  command += "/W/PIN_STATE/";
  command += p;
  command += "-";
  command += state;
  command += "/";
  wifi->send(SERVER,command.c_str());

  Serial.print("send: ");
  Serial.println(command.c_str());

  delay(2);
}

void GetCommands(int)
{
  String command(DEVICE_ID);
  command += "/W/COMMANDS/PIN_ON,PIN_OFF,PIN_STATE,VERSION,HELP/";
  wifi->send(SERVER,command.c_str());
}

void GetConnected(int)
{
  //loop each of validPins
  //appened to string
   String command(DEVICE_ID);
  command += "/W/CONNECTED/";

  for(int i = 0; i <= sizeof(validPins) / sizeof(int) -1; i++)
  {
    command += validPins[i] + ",";
  }

  command += "/";
  wifi->send(SERVER,command.c_str());
}


