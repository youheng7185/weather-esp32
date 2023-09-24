#include "sd_spi.h"
#include <Arduino.h>
#include "SD.h"
#include "SPI.h"

File readFile(fs::FS &fs, const char *path) {
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path, FILE_READ);
    if (!file || file.isDirectory()) {
        Serial.println("Failed to open file for reading");
        return File();
    }

    return file;
}

void sd_setup() {
    if (!SD.begin()) {
        Serial.println("Card Mount Failed");
        return;
    }
}

void sd_read_wifi(String &ssid, String &password) {
    File file = readFile(SD, "/wifi_credentials.txt");

    if (file) {
        // Read SSID (first line)
        ssid = file.readStringUntil('\n');
        ssid.trim();

        // Read password (second line)
        password = file.readStringUntil('\n');
        password.trim();

        file.close();
    } else {
        Serial.println("Error opening wifi_credentials.txt");
    }
}
