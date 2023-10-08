#include "return.h"
#include "main.h"
#include "pins.h"
#include <Arduino.h>

int selectCurrentState;
int selectLastState = LOW;

void returnMainMenu() {
    lcd.clear();
    // Other returnMainMenu code here
    int menu = 1;
    updateMenu();
}

void returntoHome() {
    // Your returntoHome code here
    // Keep checking the "Select" button to return to the main menu
    selectCurrentState = digitalRead(buttonSelectPin);
    if (selectLastState == LOW && selectCurrentState == HIGH) {
      Serial.println("The state changed from LOW to HIGH, button is pressed");
      returnMainMenu();
      lcd.clear();
      Serial.println("select-home");
      inSubMenu = false;
    }
    selectLastState = selectCurrentState;
    analogWrite(PIN_RED, 0);
    analogWrite(PIN_GREEN, 0);
    analogWrite(PIN_BLUE, 0);
    // Add any additional code for the sub-menu here
}
