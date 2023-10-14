#include <Wire.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <NTPClient.h>
//#include <WiFiUdp.h>
#include <AsyncUDP.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "wifi_connect.h"
#include "clock.h"
#include "return.h"
#include "weather.h"
#include "pins.h"
#include "sensor.h"
#include "udp_server.h"
#include "sdcardLog.h"
#include "http_server.h"
#include "game.h"

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

byte customCharSelect[] = {
  B00000,
  B00100,
  B00010,
  B11111,
  B00010,
  B00100,
  B00000,
  B00000
};

byte customCharDown[] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100,
  B00000
};

byte customCharUp[] = {
  B00000,
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00100
};

byte customCharReturn[] = {
  B00100,
  B01000,
  B11110,
  B01001,
  B00101,
  B00001,
  B01110,
  B00000
};

byte customCharyuan1[] = {
  B00011,
  B00100,
  B00101,
  B01001,
  B01001,
  B01001,
  B10010,
  B00101
};

byte customCharyuan2[] = {
  B11110,
  B10000,
  B11100,
  B11100,
  B00100,
  B11100,
  B10100,
  B10010
};

byte customCharshen1[] = {
  B01000,
  B00100,
  B01110,
  B00100,
  B01100,
  B10110,
  B00101,
  B00100
};

byte customCharshen2[] = {
  B00100,
  B11111,
  B10101,
  B11111,
  B10101,
  B11111,
  B00100,
  B00100
};
void setup() {
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, customCharSelect);
  lcd.createChar(1, customCharDown);
  lcd.createChar(2, customCharUp);
  lcd.createChar(3, customCharReturn);
  lcd.createChar(4, customCharyuan1);
  lcd.createChar(5, customCharyuan2);
  lcd.createChar(6, customCharshen1);
  lcd.createChar(7, customCharshen2);  
  lcd.setCursor(8, 1);
  lcd.write((uint8_t)4);
  lcd.setCursor(9, 1);
  lcd.write((uint8_t)5);
  lcd.setCursor(10, 1);
  lcd.write((uint8_t)6);
  lcd.setCursor(11, 1);
  lcd.write((uint8_t)7);  
  Serial.begin(9600);
  pinMode(buttonDownPin, INPUT_PULLUP);
  pinMode(buttonUpPin, INPUT_PULLUP);
  pinMode(buttonSelectPin, INPUT_PULLUP);

  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);

  pinMode(BUZZZER_PIN, OUTPUT);
  
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
    //udpBroadcast();
  }
  // Read the state of the switch/button:
  downCurrentState = digitalRead(buttonDownPin);
  upCurrentState = digitalRead(buttonUpPin);
  selectCurrentState = digitalRead(buttonSelectPin);

  if (downLastState == LOW && downCurrentState == HIGH) {
    Serial.println("The state changed from LOW to HIGH, button is pressed");
    if (menu <= 8) {
      menu++;
    } else {
      menu = menu;
      Serial.println("end of menu");
    }
    updateMenu();
    tone(BUZZZER_PIN,1000);
    delay(100);
    noTone(BUZZZER_PIN);
    Serial.println("down");
  }
  downLastState = downCurrentState;
  if (upLastState == LOW && upCurrentState == HIGH) {
    Serial.println("The state changed from LOW to HIGH, button is pressed");
    if (menu >= 2) {
      menu--;
    } else {
      menu = menu;
      Serial.println("end of menu");
    }
    updateMenu();
    tone(BUZZZER_PIN,1000);
    delay(100);
    noTone(BUZZZER_PIN);    
    Serial.println("up");
  }
  upLastState = upCurrentState;
  
  if (selectLastState == LOW && selectCurrentState == HIGH) {
    Serial.println("The state changed from LOW to HIGH, button is pressed");

    // Toggle the inSubMenu flag when the "Select" button is pressed
    inSubMenu = !inSubMenu;
    
    if (inSubMenu) {
      subMenu();
      tone(BUZZZER_PIN,1000);
      delay(100);
      noTone(BUZZZER_PIN);
      Serial.println("select");      
    } else {
      returnMainMenu();
      tone(BUZZZER_PIN,1000);
      delay(100);
      noTone(BUZZZER_PIN);
      Serial.println("select");      
    }

  }
  selectLastState = selectCurrentState;

}

void updateMenu() {
  switch(menu) {
    case 1:
      lcd.clear();
      lcd.print(" Weather");
      lcd.setCursor(0, 0);
      lcd.write((uint8_t)0);
      lcd.setCursor(0, 1);
      lcd.print(" Clock");
      lcd.setCursor(0, 2);
      lcd.print(" Sensor");
      lcd.setCursor(0, 3);
      lcd.print(" Connect WiFi");
      lcd.setCursor(19, 3);
      lcd.write((uint8_t)1);
      Serial.println("menu1");
      break;
    case 2:
      lcd.clear();
      lcd.print(" Weather");
      lcd.setCursor(0, 1);
      lcd.print(" Clock");
      lcd.setCursor(0, 1);        
      lcd.write((uint8_t)0);    
      lcd.setCursor(0, 2);
      lcd.print(" Sensor");
      lcd.setCursor(0, 3);
      lcd.print(" Connect WiFi");
      lcd.setCursor(19, 3);
      lcd.write((uint8_t)1);      
      Serial.println("menu2");
      break;
   case 3:
      lcd.clear();
      lcd.print(" Weather");
      lcd.setCursor(0 ,1);
      lcd.print(" Clock");
      lcd.setCursor(0, 2);
      lcd.print(" Sensor");
      lcd.setCursor(0, 2);        
      lcd.write((uint8_t)0);       
      lcd.setCursor(0, 3);
      lcd.print(" Connect WiFi");
      lcd.setCursor(19, 3);
      lcd.write((uint8_t)1);      
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
      lcd.print(" Connect WiFi");
      lcd.setCursor(0, 3);        
      lcd.write((uint8_t)0);
      lcd.setCursor(19, 3);
      lcd.write((uint8_t)1);    
      Serial.println("menu4");
      break;
    case 5:
      lcd.clear();
      lcd.print(" SD card info");
      lcd.setCursor(0, 0);        
      lcd.write((uint8_t)0);       
      lcd.setCursor(0 ,1);
      lcd.print(" Dino run");
      lcd.setCursor(19, 0);
      lcd.write((uint8_t)2);
      lcd.setCursor(19, 3);
      lcd.write((uint8_t)1);       
      Serial.println("menu5");
      break;
    case 6:
      lcd.clear();
      lcd.print(" SD card info");
      lcd.setCursor(0 ,1);
      lcd.print(" Dino run");
      lcd.setCursor(0, 1);        
      lcd.write((uint8_t)0);
      lcd.setCursor(19, 0);
      lcd.write((uint8_t)2);
      lcd.setCursor(19, 3);
      lcd.write((uint8_t)1);       
      Serial.println("menu5");
      break;
    case 7:
      lcd.clear();
      lcd.print(" Disconnect WiFi");
      lcd.setCursor(0 ,1);
      lcd.print(" Restart");
      lcd.setCursor(0, 2);
      lcd.print(" Profile");      
      lcd.setCursor(0, 0);        
      lcd.write((uint8_t)0);
      lcd.setCursor(19, 0);
      lcd.write((uint8_t)2);       
      Serial.println("menu5");
      break;
    case 8:
      lcd.clear();
      lcd.print(" Disconnect WiFi");
      lcd.setCursor(0 ,1);
      lcd.print(" Restart");
      lcd.setCursor(0, 2);
      lcd.print(" Profile");      
      lcd.setCursor(0, 1);        
      lcd.write((uint8_t)0);
      lcd.setCursor(19, 0);
      lcd.write((uint8_t)2);       
      Serial.println("menu5");
      break;
    case 9:
      lcd.clear();
      lcd.print(" Disconnect WiFi");
      lcd.setCursor(0 ,1);
      lcd.print(" Restart");
      lcd.setCursor(0, 2);
      lcd.print(" Profile");
      lcd.setCursor(0, 2);        
      lcd.write((uint8_t)0);
      lcd.setCursor(19, 0);
      lcd.write((uint8_t)2);       
      Serial.println("menu5");
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
    case 5:
      subMenuSDcard();
      break;
    case 6:
      subMenuGame();
      break;
    case 7:
      WiFi.disconnect();
      break;
    case 8:
      ESP.restart();
      break;
    case 9:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("CHEK CHEE HIM");
      lcd.setCursor(0, 1);
      lcd.print("LEONG CHIN JUN");
      lcd.setCursor(0, 2);
      lcd.print("LAU ZHENG HONG");
      lcd.setCursor(0, 3);
      lcd.print("LIM YOU HENG");
      delay(1500);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Guru pembimbing:");
      lcd.setCursor(0, 1);
      lcd.print("Wan Sofian Bin Wan");
      lcd.setCursor(0, 2);
      lcd.print("Hamat @ Wan Safie");
      delay(1000);
      returnMainMenu();
      break;
        
}
}
