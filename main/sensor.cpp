#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "main.h"
#include "return.h"
#include "sensor.h"
#include "pins.h"

int ldrValue;
unsigned long lastUpdateTime = 0; // Store the last update time
const unsigned long updateInterval = 1000; // Update interval in milliseconds

extern int selectLastState;
extern int selectCurrentState;

void subMenuSensor() {
  // Check if it's time to update the sensor value
  while (inSubMenu) {
  unsigned long currentTime = millis();
    ldrValue = analogRead(ldrPin);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("sensor value:");
    lcd.setCursor(0, 2);
    lcd.print(ldrValue);
    Serial.print(ldrValue);
    delay(100);
  
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
