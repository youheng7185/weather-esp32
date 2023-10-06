#include <Wire.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <NTPClient.h>
//#include <WiFiUdp.h>
#include <AsyncUDP.h>
#include <Adafruit_Sensor.h>
#include "wifi_connect.h"
#include "clock.h"
#include "return.h"
#include "weather.h"
#include "pins.h"
#include "sensor.h"
#include "udp_server.h"

#include "Adafruit_BME680.h"

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 20 chars and 4 line display

extern const char *ssid     = "Galaxy S23 75A3";
extern const char *password = "qcomqcom";
extern int menu;
extern int downLastState;
extern int upLastState;
extern int selectLastState;
extern bool inSubMenu;
extern int menu = 1;
int downLastState = HIGH;
int upLastState = HIGH;
extern int downCurrentState;
extern int upCurrentState;
int upCurrentState = LOW;
int downCurrentState = LOW;
// int selectLastState = HIGH;
extern int selectCurrentState;
//extern unsigned long buttonPressStartTime = 0;
extern bool inSubMenu = false;

#define I2C_LCD_ADDR 0x27
#define BME680_ADDR 0x77

Adafruit_BME680 bme; // I2C

void setup() {
  lcd.init();
  lcd.backlight();

  Serial.begin(9600);
  pinMode(buttonDownPin, INPUT_PULLUP);
  pinMode(buttonUpPin, INPUT_PULLUP);
  pinMode(buttonSelectPin, INPUT_PULLUP);

  
  

  sensorInit();
  updateMenu();
  if(WiFi.status() == WL_CONNECTED) {
    udpInit();
    clockInit();
  }
}

void loop() {

  if(WiFi.status() == WL_CONNECTED) {
    udpControl();
    udpBroadcast();
  }
  // Read the state of the switch/button:
  downCurrentState = digitalRead(buttonDownPin);
  upCurrentState = digitalRead(buttonUpPin);
  selectCurrentState = digitalRead(buttonSelectPin);

  if (downLastState == LOW && downCurrentState == HIGH) {
    Serial.println("The state changed from LOW to HIGH, button is pressed");
    menu++;
    updateMenu();
    Serial.println("down");
  }
  downLastState = downCurrentState;
  if (upLastState == LOW && upCurrentState == HIGH) {
    Serial.println("The state changed from LOW to HIGH, button is pressed");
    menu--;
    updateMenu();
    Serial.println("up");
  }
  upLastState = upCurrentState;
  
  if (selectLastState == LOW && selectCurrentState == HIGH) {
    Serial.println("The state changed from LOW to HIGH, button is pressed");
    
    // Toggle the inSubMenu flag when the "Select" button is pressed
    inSubMenu = !inSubMenu;
    
    if (inSubMenu) {
      subMenu();
    } else {
      returnMainMenu();
    }
    
    Serial.println("select");
  }
  selectLastState = selectCurrentState;

}

void updateMenu() {
  switch(menu) {
    case 1:
      lcd.clear();
      lcd.print(">Weather");
      lcd.setCursor(0 ,1);
      lcd.print(" Clock");
      lcd.setCursor(0, 2);
      lcd.print(" Sensor");
      lcd.setCursor(0, 3);
      lcd.print(" WiFi settings");
      Serial.println("menu1");
      break;
    case 2:
      lcd.clear();
      lcd.print(" Weather");
      lcd.setCursor(0 ,1);
      lcd.print(">Clock");
      lcd.setCursor(0, 2);
      lcd.print(" Sensor");
      lcd.setCursor(0, 3);
      lcd.print(" WiFi settings");
      Serial.println("menu2");
      break;
   case 3:
      lcd.clear();
      lcd.print(" Weather");
      lcd.setCursor(0 ,1);
      lcd.print(" Clock");
      lcd.setCursor(0, 2);
      lcd.print(">Sensor");
      lcd.setCursor(0, 3);
      lcd.print(" WiFi settings");
      Serial.println("menu3");
      break;
    case 4:
      lcd.clear();
      lcd.print(" Weather");
      lcd.setCursor(0 ,1);
      lcd.print(" Clock");
      lcd.setCursor(0, 2);
      lcd.print(" Sensor");
      lcd.setCursor(0, 3);
      lcd.print(">WiFi settings");
      Serial.println("menu4");
      break;
  }
}

void subMenu() {
  switch (menu) {
    case 1:
      subMenuWeather();
      break;
    case 2:
      subMenuClock();
      break;
    case 3:
      subMenuSensor();
      break;
    case 4:
      subMenuWiFi();
      break;
}
}
