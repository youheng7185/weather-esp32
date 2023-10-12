#include <LiquidCrystal_I2C.h>/* decarling LCD the library */
#include <Arduino.h>
#include "main.h"
#include "pins.h"
#include "return.h"
#include "game.h"


const int groundLevel = 3; // Adjust the ground level
int playerPos = groundLevel;  // Initial player position
int obstaclePos = 19;  // Initial obstacle position
int score = 0;
bool isJumping = false;
extern int upCurrentState;
extern int upLastState;



void playerJump() {
  if (playerPos == groundLevel) {
    playerPos = groundLevel - 3;  // Move the player upward
  }
}

void updatePlayer() {
  if (playerPos < groundLevel) {
    // Simulate gravity by moving the player down
    playerPos++;
  } else {
    isJumping = false;
  }
}

void gameOver() {
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Game Over!");
  lcd.setCursor(3, 1);
  lcd.print("Score: ");
  lcd.print(score);
  while (true) {
    // Freeze the game
  }
    returnMainMenu();
  
}

void updateObstacle() {
  obstaclePos--;

  if (obstaclePos <= 0) {
    obstaclePos = 19;  // Reset obstacle position
  }

  if (obstaclePos == 3 && playerPos == groundLevel) {
    // Player collided with the obstacle
    gameOver();
  }
}

void updateScore() {
  // Increase the score when the obstacle passes the player
  if (obstaclePos == 10) {
    score++;
  }
}

void displayGame() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Score: ");
  lcd.print(score);

  //lcd.setCursor(0, groundLevel);
  //lcd.print("   /\\   ");  // Display the ground

  lcd.setCursor(2, playerPos);
  lcd.print("*");  // Display the player character

  lcd.setCursor(obstaclePos, groundLevel);
  lcd.print("O");  // Display the obstacle
}



void gameInit() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dino Run Game");
  Serial.println("done init");
  delay(800);
}

void subMenuGame() {
  while (inSubMenu) {
      upCurrentState = digitalRead(buttonUpPin);
  
  if (upLastState == LOW && upCurrentState == HIGH && playerPos == groundLevel) {
    // Jump when the button is pressed
    isJumping = true;
    playerJump();
    tone(BUZZZER_PIN,1000);
    delay(100);
    noTone(BUZZZER_PIN);    
    Serial.println("down");
  }
  upLastState = upCurrentState;
  // Update the player position
  updatePlayer();
  updateObstacle();
  // Display the game on the LCD
  updateScore();
  displayGame();

  // Introduce some delay to control game speed
  delay(100);
  }
}
