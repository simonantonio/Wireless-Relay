
#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include <String.h>
#include "RemoteFunctions.h"

//set station, on/off
//read station
//read version
//set scheudle, {schedule] {station number, duration}

extern void SetPinOn(int);
extern void SetPinOff(int);
extern void GetVersion(int);
extern void GetPinState(int);
extern void GetCommands(int);

struct command {
        char* functionName;
        void (*function)(int);
};

struct command commands[] = {
  {"PIN_ON", SetPinOn},
  {"PIN_OFF", SetPinOff},
  {"PIN_STATE", GetPinState},
  {"VERSION", GetVersion},
  {"COMMANDS", GetCommands},
  {"HELP", GetCommands}
};

class Commander{
  public:
    Commander(ESP8266 *wifi)
    {
      this->mWifi = wifi;
      this->mTimeout = 30 * 1000;//30 seconds
      //InitCommands(wifi);
    }
    
    void update()
    {
      //ask Wifi for commands
      uint8_t buffer[128] = {0};
      
      String command = mWifi->getLocalIP() + "/R/Commands/0/";
      mWifi->send((const uint8_t*)command.c_str(), strlen(command.c_str()));

      unsigned long end = millis() + this->mTimeout;
      
      //wait for wifi to send response
      uint32_t len = mWifi->recv(buffer, sizeof(buffer), this->mTimeout);
      
      if(len > 0) {
        this->parse(buffer);
      }
    }
  private:
    void parse(const uint8_t *buffer) {
      String stz = String(*buffer);
      int cmdLocation = stz.indexOf("/W/");
      if(cmdLocation > 0) {
        String cmd = stz.substring(cmdLocation);
        this->execute(cmd);
      }
    }
    void execute(String cmd) {
      //"PIN_ON/5/" example

      String command = cmd.substring(0, cmd.indexOf('/'));
      String param1 = cmd.substring(cmd.indexOf('/'), cmd.lastIndexOf('/'));
      
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
  
    ESP8266 *mWifi;
    uint8_t mTimeout;
};

#endif //_COMMANDS_H_
