#ifndef CLOCK_H
#define CLOCK_H

#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

extern LiquidCrystal_I2C lcd;
extern const long utcOffsetInSeconds;
extern char daysOfTheWeek[7][12];

void subMenuClock();

#endif
