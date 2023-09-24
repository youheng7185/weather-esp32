#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "main.h"
#include "return.h"
#include "sensor.h"
#include "pins.h"

int ldrValue;

extern int selectLastState;
extern int selectCurrentState;

void subMenuSensor() {
  while (inSubMenu) {
    // Update the sensor value
    ldrValue = analogRead(ldrPin);
    
    // Clear the LCD and display the sensor value
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Sensor value:");
    lcd.setCursor(0, 2);
    lcd.print(ldrValue);
    delay(200);
    // Check for "Select" button press
    //if (selectLastState == LOW && selectCurrentState == HIGH) {
    //  Serial.println("The state changed from LOW to HIGH, button is pressed");
    //  returnMainMenu();
    //  Serial.println("select-home");
    // inSubMenu = false;
    //  break;
    //}
    //selectLastState = selectCurrentState;
    returntoHome();
    // Delay for a short interval
    //delay(1000); // Adjust the delay interval as needed for your desired update rate
  }
}
