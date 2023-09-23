#include "weather.h"
#include "return.h"
#include "main.h"
#include <Arduino.h>
#include "pins.h"

extern int menu;
extern int downLastState;
extern int upLastState;
extern int selectLastState;
extern bool inSubMenu;
extern int selectCurrentState;

void subMenuWeather() {
    // Your subMenuWeather code here
  inSubMenu = true;
  lcd.clear();
  lcd.print("weather here");
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
    // Add any additional code for the sub-menu here
  }
}
