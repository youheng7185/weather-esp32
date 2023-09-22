#include <HTTPClient.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

int lcdColumns = 20;
int lcdRows = 4;
const char *ssid     = "Galaxy S23 75A3";
const char *password = "qcomqcom";

const String endpoint = "http://api.openweathermap.org/data/2.5/weather?q=Kuantan,malaysia&APPID=";
const String key = "fd6d749c37dbfa09fe77a3299ee6ea80";


LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 

void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    lcd.print("Connecting to WiFi..");
  }
 
  lcd.print("Connected to the WiFi network");

}

void loop() {
  
  // put your main code here, to run repeatedly:
  if ((WiFi.status() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;
    
    http.begin(client, endpoint + key);
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      // Stream& input;

    StaticJsonDocument<1024> doc;

     DeserializationError error = deserializeJson(doc, payload);

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
    return;
    }

  float coord_lon = doc["coord"]["lon"]; // 101.6067
  float coord_lat = doc["coord"]["lat"]; // 3.1073

  JsonObject weather_0 = doc["weather"][0];
  const char* weather_0_main = weather_0["main"]; // "Clouds"

  JsonObject main = doc["main"];
  float main_temp = main["temp"]; // 303.05
  float main_feels_like = main["feels_like"]; // 310.05
  float main_temp_min = main["temp_min"]; // 300.07
  float main_temp_max = main["temp_max"]; // 305.6
  int main_pressure = main["pressure"]; // 1008
  int main_humidity = main["humidity"]; // 81

  const char* name = doc["name"]; // "Petaling Jaya"
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(name);
      lcd.setCursor(0, 1);
      lcd.print("Temp:");
      lcd.print((main_temp-273.15), 1);
      lcd.setCursor(10, 1);
      lcd.print("Feel:");
      lcd.print((main_feels_like-273.15), 1);
      lcd.setCursor(0, 2);
      lcd.print("Min:");
      lcd.print((main_temp_min-273.15), 1);
      lcd.setCursor(10, 2);
      lcd.print("Max:");
      lcd.print((main_temp_max-273.15), 1);
      lcd.setCursor(0, 3);
      lcd.print("Weather:");
      lcd.print(weather_0_main);
      
    } else {
      lcd.setCursor(0, 0);
      lcd.print("error on http request");
      
    }

    http.end();
  }

  delay(600000); //delay for 10 minutes
}
