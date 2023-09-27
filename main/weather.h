#ifndef WEATHER_H
#define WEATHER_H

#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;
extern int selectLastState;
extern bool inSubMenu;
extern unsigned int currentMills;

extern float coord_lon;
extern unsigned long currentMillis;
extern const char* weather_0_main;
extern float main_temp;
extern float main_feels_like;
extern float main_temp_min;
extern float main_temp_max;
extern int main_pressure;
extern int main_humidity;
extern const char* name;

void weatherInit();
void subMenuWeather();

#endif
