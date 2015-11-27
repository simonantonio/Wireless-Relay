
#ifndef _COMMANDS_H_
#define _COMMANDS_H_

//#include <String.h>
#include "RemoteFunctions.h"
#include "config.h"

const char ADVERTISE_COMMAND[] PROGMEM = "70D9A386-9D32-46A6-910D-E029EF3B0E9E/R/ADVERTISE/0/";

//set station, on/off
//read station
//read version
//set scheudle, {schedule] {station number, duration}

extern void SetPinOn(int);
extern void SetPinOff(int);
extern void GetVersion(int);
extern void GetPinState(int);
extern void GetCommands(int);
extern void GetConnected(int);

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
  {"HELP", GetCommands},
  {"CONNECTED", GetConnected }
};

class Commander{
  public:
    Commander(ESP8266wifi *wifi)
    {
      this->mWifi = wifi;
      this->mTimeout = 30 * 1000;//30 seconds
      //InitCommands(wifi);

      this->previousMillis = 0;
      this->interval = 5000;
    }
    
    void update()
    {
      //ask Wifi for commands
      uint8_t buffer[128] = {0};

      unsigned long currenMillis = millis();

      if(currenMillis - this->previousMillis >= this->interval) {
        this->previousMillis = currenMillis;
        static int errorCount = 0;
        if(!mWifi->send(SERVER, ADVERTISE_COMMAND)) {
          Serial.println("Unable to send message to server");
          
          if(errorCount >= 5) {
            Serial.println("Reset the wifi device");
            mWifi->begin();//reset this bastard
            errorCount = 0;
          }
          errorCount++;        
        }   
        //delay(200);     
      }
      
      String msg = String(mWifi->readIncomingMessage());
      this->parse(msg);
    }
  private:
    void parse(String stz) {

      if(stz.length() == 0)
      {
        return;
      }
      
      Serial.print("Received message: ");
      Serial.println(stz);
        
      int cmdLocation = stz.indexOf("/R/");

      if(cmdLocation == -1)//try /W/
        cmdLocation = stz.indexOf("/W/");
        
      if(cmdLocation > -1) {
        String cmd = stz.substring(cmdLocation);       
        
        this->execute(cmd);
      }
      else {
        Serial.print("Not a valid command (");
        Serial.print(cmdLocation);
        Serial.println(")");
      }
    }
    
    void execute(String cmd) {
      //"PIN_ON/5/" example

      //  "/SERVER/R/<CMD>/<param1>


      Serial.print("Command: ");
      Serial.println(cmd);

      //remove sender
      int idx = cmd.indexOf("/R/");
      if(idx == -1)
        idx = cmd.indexOf("/W");
        
      cmd = cmd.substring(idx);
    
      String command = cmd.substring(cmd.indexOf('/')+1, cmd.lastIndexOf('/'));
      command = command.substring(command.indexOf('/')+1);
      String param1 = cmd.substring(cmd.lastIndexOf('/')+1);
      
      Serial.print("command: ");
      Serial.print(command);
      Serial.print("\t");
      Serial.print("parameter: ");
      Serial.println(param1);
      
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

    int StringContains(String s, String search) {
        int max = s.length() - search.length();
        int lgsearch = search.length();
    
        for (int i = 0; i <= max; i++) {
            if (s.substring(i, i + lgsearch) == search) return i;
        }
    
     return -1;
    }
  
    ESP8266wifi *mWifi;
    uint8_t mTimeout;
    unsigned long previousMillis;
    long interval;
};

#endif //_COMMANDS_H_
