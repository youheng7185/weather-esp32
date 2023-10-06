#ifndef CLOCK_H
#define CLOCK_H

#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

extern LiquidCrystal_I2C lcd;
String logTime();

void subMenuClock();
void clockInit();

#endif
