#include "wifi_connect.h"
#include "return.h" // Include return.h to access returnMainMenu and returntoHome
#include <WiFi.h>
#include "main.h"

void subMenuWiFi() {
    // Your subMenuWiFi code here
    if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Connecting ...");
    delay(7000); // 7 sec to connect to wifi
    returnMainMenu();
    } else if (WiFi.status() == WL_CONNECTED) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Wi-Fi connected");
    lcd.setCursor(0, 1);
    lcd.print("IP address:");
    lcd.setCursor(0, 2);
    lcd.print(WiFi.localIP());
    delay(200);
    returntoHome();
  }
}
