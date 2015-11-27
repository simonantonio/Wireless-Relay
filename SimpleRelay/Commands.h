
#ifndef _COMMANDS_H_
#define _COMMANDS_H_

//#include <String.h>
#include "RemoteFunctions.h"
#include "config.h"

//set station, on/off
//read station
//read version
//set scheudle, {schedule] {station number, duration}

extern void SetPinOn(int);
extern void SetPinOff(int);
extern void GetDeviceState(int);
extern void Identify(int);

struct command {
        char* functionName;
        void (*function)(int);
};

struct command commands[] = {
  {"PIN_ON", SetPinOn},
  {"PIN_OFF", SetPinOff},
  {"STATE", GetDeviceState},
  {"IDENTIFY", Identify}
};

class Commander{
  public:
    Commander(ESP8266wifi *wifi)
    {
      this->mWifi = wifi;
    }
    
    void update()
    {
      //device is a slave     
      String msg = String(mWifi->readIncomingMessage());
      this->parse(msg);
    }
  private:
    void parse(String stz) 
    {

      if(stz.length() == 0)
      {
        return;
      }
      
      Serial.print("Received message: ");
      Serial.println(stz);
       
      this->execute(stz);     
    }
    
    void execute(String cmd) 
    {
      //"PIN_ON/5/" example

      //  "/SERVER/<CMD>/<param1>

      Serial.print("Command: ");
      Serial.println(cmd);

      String command = cmd.substring(cmd.indexOf('/')+1, cmd.lastIndexOf('/'));
      command = command.substring(command.indexOf('/')+1);
      String param1 = cmd.substring(cmd.lastIndexOf('/')+1);
      
      Serial.print("command: ");
      Serial.print(command);
      Serial.print("\t");
      Serial.print("parameter: ");
      Serial.println(param1);

      return;
      
      struct command *scan;
      
      for(scan=commands; scan->function; scan++)
      {
        if(!strcmp(command.c_str(), scan->functionName))
        {
          scan->function(param1.toInt());
          break;
        }
      }
    }
  
    ESP8266wifi *mWifi;   
};

#endif //_COMMANDS_H_
