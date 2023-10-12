#include <LiquidCrystal_I2C.h>/* decarling LCD the library */
#include <Arduino.h>
#include "main.h"
#include "pins.h"
#include "return.h"
#include "game.h"

extern int menu;
extern int downLastState;
extern int upLastState;
extern int selectLastState;
extern bool inSubMenu;
extern int selectCurrentState;
extern int downCurrentState;

int Button=8;/*assigning the Arduino pin for push button*/
/*variables for storing time to move the enemy and character*/
unsigned long a;
unsigned long b;
unsigned long c;
unsigned long d;
int Delay=500; /* defining the speed of enemy */
int DecreaseDelay=20; /* for decreasing the speed of enemy */
bool pac=0;/* variable that will save the value for character to detect the collision with the enemy*/
int i;/* variable that is used to move the enemy on the LCD*/
int points=0;/* variable to store the points achieved the player */


void setupGame() {
i=15;/* defining the number of columns for the enemy */
/* saving time in the time variables declared above */
a=millis();
b=millis();
c=millis();
d=millis();
/* displaying the game and character animation */
lcd.setCursor(3, 0);
lcd.print("Ardi-Pac");
lcd.setCursor(3, 1);
lcd.print("PRESS START");
lcd.setCursor(13, 0);
lcd.write(6);
delay(300);
lcd.setCursor(13, 0);
lcd.write(5);
delay(300);
 

}

void subMenuGame() {
  setupGame();
  while (inSubMenu) {

if((millis()-a)>=Delay) { /* start the motion of enemy on LCD*/
a=millis();
lcd.clear();
lcd.setCursor(i, 1);
lcd.write(7);
i=i-1;
if(i == -1) {/* condition to move the enemy on the LCD starting from 15 and going to zero*/
i=15;
}
lcd.setCursor(6, 0);
lcd.print("Points:");
lcd.print(points);/* displaying the points achieved by the player */
}
if((millis()-b)>=50) {
b=millis();
if (downLastState == LOW && downCurrentState == HIGH && pac == false) { /* displaying character on LCD by reading the state of button */
Serial.println("The state changed from LOW to HIGH, button is pressed");
    tone(BUZZZER_PIN,1000);
    delay(100);
    noTone(BUZZZER_PIN);
lcd.setCursor(1, 0);/* placing the character on first column and first row when button is pressed*/
lcd.write(6);
pac = true; /* updating that no collision with enemy */
b=millis();/* saving the time */
}
else if(pac == true) { /* if still no collision place it on the same place*/
lcd.setCursor(1, 0);
lcd.write(6);
}
else if(pac == false) {/* move the character to second row first column on LCD*/
lcd.setCursor(1, 0);
lcd.print(" ");
lcd.setCursor(1, 1);
lcd.write(6);
}
}
if((millis()-c)>=1500 && pac == true) {/* if button is pressed again move the character to the first row */
c=millis();
lcd.setCursor(1, 1);
lcd.write(6);
lcd.setCursor(1, 0);
lcd.print(" ");
pac = false;
}
if(((millis()-d)>=1000) && (i == 1)) { /* if the enemy takes its first round LCD without any collision*/
d=millis();
if (pac == true) {
points=points+1;/* collision is detected then add one in the points*/
if (points%5 == 0) {
Delay=Delay-DecreaseDelay;/* decrease the speed of enemy*/
if(Delay <= 200) { //changed the speed of enemy to 200
DecreaseDelay=0; /* do not increase the speed of enemy */
}
}
} else {/* when collision occurs clear the LCD and display game over */
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("GAME OVER!");
  lcd.setCursor(0, 1);
  lcd.print("START AGAIN");
  points=0;
  i=15;
  Delay=500; //toggles back to basic speed
  //State2 = digitalRead(Button);
  //while(State2 == HIGH) {/* keep reading the state of button when the state is HIGH*/
  //State2 = digitalRead(Button); /*if button pressed then start the game again*/
  }
  delay(500);
  }
  }
}
