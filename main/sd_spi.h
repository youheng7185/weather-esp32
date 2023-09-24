#ifndef SD_SPI_H
#define SD_SPI_H

#include <Arduino.h>
#include <SD.h>

void sd_setup();
void sd_read_wifi(String &ssid, String &password);

#endif
