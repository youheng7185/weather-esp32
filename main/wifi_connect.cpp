#include "wifi_connect.h"
#include "return.h" // Include return.h to access returnMainMenu and returntoHome
#include <WiFi.h>
#include "main.h"

void subMenuWiFi() {
    // Your subMenuWiFi code here
    if (WiFi.status() != WL_CONNECTED) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Connecting ...");
    if (WiFi.status() == WL_CONNECTED) {
      lcd.clear();
      lcd.print("Connected");
      delay(200);
      returntoHome();
    }
    delay(1000); // 1 sec to connect to wifi
    returnMainMenu();
    } else if (WiFi.status() == WL_CONNECTED) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Wi-Fi connected");
    lcd.setCursor(0, 1);
    lcd.print("SSID:");
    lcd.setCursor(5, 1);
    lcd.print(ssid);
    lcd.setCursor(0, 2);
    lcd.print("IP:");
    lcd.setCursor(3, 2);
    lcd.print(WiFi.localIP());
    lcd.setCursor(0, 3);
    lcd.print("RRSI:");
    lcd.setCursor(5, 3);
    lcd.print(WiFi.RSSI());
    lcd.setCursor(8, 3);
    lcd.print("dBm");
    
    delay(200);
    returntoHome();
  }
}
