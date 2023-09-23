#include "clock.h"
#include "return.h" // Include return.h to access returntoHome
#include "main.h"
#include <WiFi.h>
#include "pins.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "time.windows.com", utcOffsetInSeconds);

extern int selectLastState;
extern int selectCurrentState;

void subMenuClock() {
    // Your subMenuClock code here
   timeClient.begin();
   
   if(WiFi.status() == WL_CONNECTED) {
    while (inSubMenu) {
    lcd.clear();
    timeClient.update();
    lcd.setCursor(0, 0);
    lcd.print(daysOfTheWeek[timeClient.getDay()]);
    lcd.setCursor(0, 1);
    lcd.print(timeClient.getFormattedTime());
    delay(1000);
    selectCurrentState = digitalRead(buttonSelectPin);
    if (selectLastState == LOW && selectCurrentState == HIGH) {
      Serial.println("The state changed from LOW to HIGH, button is pressed");
      returnMainMenu();
      lcd.clear();
      Serial.println("select-home");
      inSubMenu = false;
      break;
    }
    selectLastState = selectCurrentState;
      
    }
   }
  if (WiFi.status() != WL_CONNECTED) {
    lcd.print("network is disconnected");
  }
}
