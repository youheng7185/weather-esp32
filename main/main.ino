#include <LiquidCrystal_I2C.h>

#define buttonDownPin 14
#define buttonUpPin 12
#define buttonSelectPin 27

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 20 chars and 4 line display

int menu = 1;
int downLastState = HIGH;
int downCurrentState;
int upLastState = HIGH;
int upCurrentState;
int selectLastState = HIGH;
int selectCurrentState;
unsigned long buttonPressStartTime = 0;
bool inSubMenu = false;
const int debounceDelay = 50; // Debounce delay in milliseconds
const int longPressDuration = 200; // Long press duration in milliseconds

void setup() {
  lcd.init();
  lcd.backlight();

  Serial.begin(9600);
  pinMode(buttonDownPin, INPUT_PULLUP);
  pinMode(buttonUpPin, INPUT_PULLUP);
  pinMode(buttonSelectPin, INPUT_PULLUP);

  updateMenu();
}

void loop() {
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
      lcd.print(" Settings");
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
      lcd.print(" Settings");
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
      lcd.print(" Settings");
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
      lcd.print(">Settings");
      Serial.println("menu4");
      break;
  }
}

void subMenu() {
  switch (menu) {
    case 1:
      subMenuWeather();
      break;
}
}

void subMenuWeather() {
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

void returnMainMenu() {
  lcd.clear();
  menu = 1;
  updateMenu();
}
