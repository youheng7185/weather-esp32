#ifndef WEATHER_H
#define WEATHER_H

#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;
extern int selectLastState;
extern bool inSubMenu;

void subMenuWeather();

#endif
