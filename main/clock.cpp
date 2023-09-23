#include "clock.h"
#include "return.h" // Include return.h to access returntoHome
#include "main.h"
#include <WiFi.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "time.windows.com", utcOffsetInSeconds);

void subMenuClock() {
    // Your subMenuClock code here
   timeClient.begin();
   lcd.clear();
   if(WiFi.status() == WL_CONNECTED) {
    while (WiFi.status() == WL_CONNECTED) {
    timeClient.update();
    lcd.setCursor(0, 0);
    lcd.print(daysOfTheWeek[timeClient.getDay()]);
    lcd.setCursor(0, 1);
    lcd.print(timeClient.getFormattedTime());
    delay(200);
    returntoHome();
    break;
    }
   } else if (WiFi.status() != WL_CONNECTED) {
    lcd.print("network is disconnected");
  }
  delay(200);
  returntoHome();
}
