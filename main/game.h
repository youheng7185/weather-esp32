#ifndef GAME_H
#define GAME_H

#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;
extern int selectLastState;
extern bool inSubMenu;

void subMenuGame();

#endif
