#ifndef WIFI_CONNECT_H
#define WIFI_CONNECT_H

#include <WiFi.h>
#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;
extern const char *ssid;
extern const char *password;
extern int menu;

void subMenuWiFi();

#endif
