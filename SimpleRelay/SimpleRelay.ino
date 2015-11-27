#include "ESP8266wifi.h"

#include "Commands.h"
#include "config.h"

//#define DEBUG
#define VERBOSE_SETUP

ESP8266wifi *wifi;
Commander *commander;

void setup() {
  
  // put your setup code here, to run once:
#ifdef VERBOSE_SETUP
  Serial.println("Turning off relays...");
#endif
  pinMode(D18, OUTPUT);
  pinMode(D19, OUTPUT);
  pinMode(D20, OUTPUT);
  pinMode(D21, OUTPUT);
  pinMode(D22, OUTPUT);
  pinMode(D23, OUTPUT);
  pinMode(D24, OUTPUT);

  digitalWrite(D18, 0);
  digitalWrite(D19, 0);
  digitalWrite(D20, 0);
  digitalWrite(D21, 0);
  digitalWrite(D22, 0);
  digitalWrite(D23, 0);
  digitalWrite(D24, 0);

#ifdef VERBOSE_SETUP
  Serial.println("Waiting for system to settle...");
#endif

  delay(5000);

  Serial.println("Starting Simple Relay Setup");

  Serial3.begin(9600);
  
  //force MUX mode seems like it doesnt get forced by library
  Serial3.write("AT\r\n");
  delay(100);
  Serial3.write("AT+CIPMUX=1\r\n");
  delay(100);

#ifdef VERBOSE_SETUP
  Serial.println("Creating Wifi Device...");
#endif
  
#ifdef DEBUG
  wifi = new ESP8266wifi(Serial3, Serial3, 10, Serial);
#else
  wifi = new ESP8266wifi(Serial3, Serial3, 10);
#endif
  wifi->begin();
  
  delay(2000);

#ifdef VERBOSE_SETUP
  Serial.println("Connecting to AP...");
#endif
  if(!wifi->connectToAP(SSID,PASSWORD)) {
    Serial.println("Unable to connect to access point");
  }

#ifdef VERBOSE_SETUP
  Serial.println("Set to UDP...");
#endif
  wifi->setTransportToUDP();

#ifdef VERBOSE_SETUP
  Serial.println("Connect to host...");
#endif
  if(!wifi->connectToServer(HOST, PORT)) {
    Serial.println("Unable to connect to server");
  }

#ifdef VERBOSE_SETUP
  Serial.println("Setup commander...");
#endif
  commander = new Commander(wifi);

  Serial.println("Simple Relay Driver r1 Now Ready...");
}

void loop() {
    
  //make sure wifi has started
  if(!wifi->isStarted()) {
    wifi->begin();
  }
    
  commander->update();
}

