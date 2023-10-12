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
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 28800;

void clockInit() {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

String logTime() {
  struct tm timeinfo;
  clockInit(); // Initialize time synchronization (configTime)

  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return "N/A"; // Return a default value or handle the error as needed
  }

  char timeString[64];
  strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(timeString);
}

void subMenuClock() {
  if (WiFi.status() == WL_CONNECTED) {
    lcd.clear();
    clockInit(); // Initialize time synchronization (configTime)

    while (inSubMenu) {
      // Get current time
      String currentTime = logTime();

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(currentTime);
      delay(1000);

      selectCurrentState = digitalRead(buttonSelectPin);
      if (selectLastState == LOW && selectCurrentState == HIGH) {
        Serial.println("The state changed from LOW to HIGH, button is pressed");
        lcd.clear();
        returnMainMenu();
        Serial.println("select-home");
        inSubMenu = false;
        tone(BUZZZER_PIN,1000);
        delay(100);
        noTone(BUZZZER_PIN);
        break;
      }
      selectLastState = selectCurrentState;
    }
  }
}
