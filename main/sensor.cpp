#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "main.h"
#include "return.h"
#include "sensor.h"
#include "pins.h"

int ldrValue;
unsigned long lastUpdateTime = 0; // Store the last update time
const unsigned long updateInterval = 200; // Update interval in milliseconds

void subMenuSensor() {
  // Check if it's time to update the sensor value
  while (inSubMenu) {
  unsigned long currentTime = millis();
  if (currentTime - lastUpdateTime >= updateInterval) {
    ldrValue = analogRead(ldrPin);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("sensor value:");
    lcd.setCursor(0, 2);
    lcd.print(ldrValue);
    Serial.print(ldrValue);
    delay(200);
    lastUpdateTime = currentTime; // Update the last update time
  }
  }
  
  // Check for button press to return to the main menu
  returntoHome();
  
}
