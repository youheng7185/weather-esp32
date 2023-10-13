#include <LiquidCrystal_I2C.h>
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
extern int selectCurrentState;
extern int selectLastState;
bool gameFinish = true;
bool inGame = false;  // New state to track whether the game is running
bool gameOverFlag = false; // Add this flag

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

  while (gameFinish) {
    selectCurrentState = digitalRead(buttonSelectPin);
    if (selectLastState == LOW && selectCurrentState == HIGH) {
      gameFinish = false;
      lcd.clear();
      returnMainMenu();
      inGame = false;  // Set inGame to false when returning to the main menu
      score = 0;
      ESP.restart();
    }
    selectLastState = selectCurrentState;
    inGame = false;
  }
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

  lcd.setCursor(0, groundLevel);
  lcd.print("   /   ");  // Display the ground

  lcd.setCursor(2, playerPos);
  lcd.print("*");  // Display the player character

  lcd.setCursor(obstaclePos, groundLevel);
  lcd.print("O");  // Display the obstacle
}

void gameInit() {
  playerPos = groundLevel;
  obstaclePos = 19;
  score = 0;
  inGame = true;  // Set inGame to true when starting the game
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dino Run Game");
  Serial.println("done init");
  delay(800);
}


void subMenuGame() {
  if (!inGame) {
    gameInit();
  }

  while (inGame) {
    upCurrentState = digitalRead(buttonUpPin);

    if (upLastState == LOW && upCurrentState == HIGH && playerPos == groundLevel) {
      // Jump when the button is pressed
      isJumping = true;
      playerJump();
      tone(BUZZZER_PIN, 1000);
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

    if (playerPos == groundLevel && obstaclePos == 3) {
      gameOver();
      gameOverFlag = true;
    }
    // Introduce some delay to control game speed
    delay(100);
  }
  //inGame = false;  // Set inGame to false when exiting the game
}
