#include <WiFi.h>
#include <AsyncUDP.h>

#include "main.h"
#include "udp_server.h"
#include "return.h"
#include <Arduino.h>

AsyncUDP udp;

int counter = 1;

void udpInit() {
   if(udp.listen(8888)) {
        Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.localIP());
   }
}

void udpBroadcast() {
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;
    counter = counter + 1;
    String message = String(counter);
    udp.broadcast(message.c_str());
  }
}

void udpControl() {
  if(udp.listen(8888)) {
        udp.onPacket([](AsyncUDPPacket packet) {
            Serial.print("UDP Packet Type: ");
            Serial.print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
            Serial.print(", From: ");
            Serial.print(packet.remoteIP());
            Serial.print(":");
            Serial.print(packet.remotePort());
            Serial.print(", To: ");
            Serial.print(packet.localIP());
            Serial.print(":");
            Serial.print(packet.localPort());
            Serial.print(", Length: ");
            Serial.print(packet.length());
            Serial.print(", Data: ");
            Serial.write(packet.data(), packet.length());
            Serial.println();
            //reply to the client
            packet.printf("Got %u bytes of data", packet.length());

      //String receivedData = String(reinterpret_cast<char*>(packet.data()));
      if (strcmp(reinterpret_cast<const char*>(packet.data()), "up") == 0) {
          menu--;
          updateMenu();
          Serial.println("up");
      }
      if (strcmp(reinterpret_cast<const char*>(packet.data()), "down") == 0) {
        menu++;
        updateMenu();
        Serial.println("down");
      }

      if (strcmp(reinterpret_cast<const char*>(packet.data()), "home") == 0) {
        returnMainMenu();
        Serial.println("home");
      }
      if (strcmp(reinterpret_cast<const char*>(packet.data()), "select") == 0) {
        subMenu();
        Serial.println("select");
      }      
        });
    }
}
