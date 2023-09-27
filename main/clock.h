#ifndef CLOCK_H
#define CLOCK_H

#include <WiFi.h>
#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;

void subMenuClock();
void clockInit();

#endif
