#ifndef RETURN_H
#define RETURN_H

#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;
extern bool inSubMenu;
extern int menu;

void returnMainMenu();
void returntoHome();

#endif
