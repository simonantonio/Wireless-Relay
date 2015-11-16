#include "ESP8266.h"

#include "Commands.h"

#define SSID "medusa"
#define PASSWORD "miling03"
#define HOST "192.168.1.10"
#define PORT 15000

ESP8266 *wifi;
Commander *commander;

void setup() {
  
  // put your setup code here, to run once:
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

  delay(2000);

  wifi = new ESP8266(Serial3);

  delay(1000);

  Serial.println("Version");
  Serial.println(wifi->getVersion().c_str());

   if (wifi->setOprToStationSoftAP()) {
        Serial.print("to station + softap ok\r\n");
    } else {
        Serial.print("to station + softap err\r\n");
    }

    delay(1000);

    if (wifi->joinAP(SSID, PASSWORD)) {
        Serial.print("Join AP success\r\n");
        Serial.print("IP: ");
        Serial.println(wifi->getLocalIP().c_str());       
    } else {
        Serial.print("Join AP failure\r\n");
    }

    delay(1000);

   if (wifi->disableMUX()) {
        Serial.print("single ok\r\n");
    } else {
        Serial.print("single err\r\n");
    }

    commander = new Commander(wifi);
}

void loop() {
  uint8_t buffer[128] = {0};
  
  if (wifi->registerUDP(HOST, PORT)) {
        Serial.print("register udp ok\r\n");
  } else {
      Serial.print("register udp err\r\n");
  }


  uint32_t len = wifi->recv(buffer, sizeof(buffer), 10000);
    if (len > 0) {
        Serial.print("Received:[");
        for(uint32_t i = 0; i < len; i++) {
            Serial.print((char)buffer[i]);
        }
        Serial.print("]\r\n");
    }

    if (wifi->unregisterUDP()) {
        Serial.print("unregister udp ok\r\n");
    } else {
        Serial.print("unregister udp err\r\n");
    }
  
  // put your main code here, to run repeatedly:
  //digitalWrite(D18, 1);

  //delay (2000);

  //digitalWrite(D18, 0);

  //delay (2000);

  //digitalWrite(D19, 1);

  //delay (2000);

  //digitalWrite(D19, 0);
  commander->update();
}




