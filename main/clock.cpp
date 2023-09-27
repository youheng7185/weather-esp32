/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-date-time-ntp-client-server-arduino/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include "clock.h"
#include "return.h" // Include return.h to access returntoHome
#include "main.h"
#include <WiFi.h>
#include "pins.h"
#include "time.h"

extern int selectLastState;
extern int selectCurrentState;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 28800;

void clockInit() {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void subMenuClock() {
  if (WiFi.status() == WL_CONNECTED) {
    struct tm timeinfo;
    lcd.clear();
    clockInit(); // Initialize time synchronization (configTime)

    while (inSubMenu) {
      // Get current time
      if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        delay(1000);
        continue; // Skip the rest of the loop iteration
      }

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(&timeinfo, "%H:%M:%S");
      lcd.setCursor(0, 1);
      lcd.print(&timeinfo, "%A");
      lcd.setCursor(0, 2);
      lcd.print(&timeinfo, "%d %B");
      delay(1000);

      selectCurrentState = digitalRead(buttonSelectPin);
      if (selectLastState == LOW && selectCurrentState == HIGH) {
        Serial.println("The state changed from LOW to HIGH, button is pressed");
        lcd.clear();
        returnMainMenu();
        Serial.println("select-home");
        inSubMenu = false;
        break;
      }
      selectLastState = selectCurrentState;
    }
  }
}
