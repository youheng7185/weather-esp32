#include "weather.h"
#include "return.h"
#include "main.h"
#include <Arduino.h>
#include "pins.h"
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>

extern int menu;
extern int downLastState;
extern int upLastState;
extern int selectLastState;
extern bool inSubMenu;
extern int selectCurrentState;

const String endpoint = "http://api.openweathermap.org/data/2.5/weather?q=Kuantan,malaysia&APPID=";
const String key = "fd6d749c37dbfa09fe77a3299ee6ea80";

//unsigned long previousMillis = 0;
//const long interval = 600000;  // Interval in milliseconds (10 minutes)
//unsigned long currentMillis;
//bool isWeatherDataUpdated = false; // Flag to indicate if weather data is updated

// weather.cpp
// ...
float coord_lon = 0.0;
float coord_lat = 0.0;
const char* weather_0_main = "";
float main_temp = 0.0;
float main_feels_like = 0.0;
float main_temp_min = 0.0;
float main_temp_max = 0.0;
int main_pressure = 0;
int main_humidity = 0;
const char* name = "";
// ...

void subMenuWeather() {
  // Your subMenuWeather code here
  inSubMenu = true;
  WiFiClient client;
  HTTPClient http;

  if ((WiFi.status() == WL_CONNECTED)) {

      // make the HTTP GET request
      http.begin(client, endpoint + key);
      int httpCode = http.GET();

      if (httpCode > 0) {
        String payload = http.getString();

        StaticJsonDocument<1024> doc;

        DeserializationError error = deserializeJson(doc, payload);

        if (error) {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.f_str());
          return;
        }

        float coord_lon = doc["coord"]["lon"];
        float coord_lat = doc["coord"]["lat"];

        JsonObject weather_0 = doc["weather"][0];
        const char* weather_0_main = weather_0["main"];

        JsonObject main = doc["main"];
        float main_temp = main["temp"];
        float main_feels_like = main["feels_like"];
        float main_temp_min = main["temp_min"];
        float main_temp_max = main["temp_max"];
        int main_pressure = main["pressure"];
        int main_humidity = main["humidity"];

        const char* name = doc["name"];
        http.end();

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(name);
        lcd.setCursor(0, 1);
        lcd.print("Temp:");
        lcd.print((main_temp - 273.15), 1);
        lcd.setCursor(10, 1);
        lcd.print("Feel:");
        lcd.print((main_feels_like - 273.15), 1);
        lcd.setCursor(0, 2);
        lcd.print("Min :");
        lcd.print((main_temp_min - 273.15), 1);
        lcd.setCursor(10, 2);
        lcd.print("Max:");
        lcd.print((main_temp_max - 273.15), 1);
        lcd.setCursor(0, 3);
        lcd.print("Weather:");
        lcd.print(weather_0_main);

        delay(200);
        
  while (inSubMenu) {
    // Keep checking the "Select" button to return to the main menu
    selectCurrentState = digitalRead(buttonSelectPin);
    if (selectLastState == LOW && selectCurrentState == HIGH) {
      Serial.println("The state changed from LOW to HIGH, button is pressed");
      returnMainMenu();
      Serial.println("select-home");
      inSubMenu = false;
    }
    selectLastState = selectCurrentState;
      } 
    }
   }
}
