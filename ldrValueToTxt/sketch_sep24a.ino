
#include "FS.h"
#include "SD.h"
#include <SPI.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define ldrPin 34
int ldrValue;
String dayStamp;
String timeStamp;
// Replace with your network credentials
const char* ssid     = "Galaxy S23 75A3";
const char* password = "qcomqcom";
// Define CS pin for the SD card module
#define SD_CS 5

String dataMessage;
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  // Initialize a NTPClient to get time
  timeClient.begin();
  timeClient.setTimeOffset(19800);
  // Initialize SD card
  SD.begin(SD_CS);  
  if(!SD.begin(SD_CS)) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();
  if(cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }
  Serial.println("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("ERROR - SD card initialization failed!");
    return;    // init failed
  }
  File file = SD.open("/data.txt");
  if(!file) {
    Serial.println("File doens't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/data.txt", "Time, LDR value \r\n");
  }
  else {
    Serial.println("File already exists");  
  }
  file.close();
}

void loop() {
    readLdrValue();
    getTimeStamp();
    logSDCard();
    delay(1000); //Wait for 5 seconds before writing the next data 
}

// Function to get temperature
void readLdrValue()
{
  ldrValue = analogRead(ldrPin);
  Serial.print("ldr reading = ");
  Serial.println(ldrValue);
}

// Function to get date and time from NTPClient
void getTimeStamp() {
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  // Extract time
  timeStamp = timeClient.getFormattedTime();
  Serial.println(timeStamp);
}
// Write the sensor readings on the SD card
void logSDCard() {
  dataMessage =  String(timeStamp) + "," + 
                String(ldrValue) + "\r\n";
  Serial.print("Save data: ");
  Serial.println(dataMessage);
  appendFile(SD, "/data.txt", dataMessage.c_str());
}

// Write to the SD card (DON'T MODIFY THIS FUNCTION)
void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);
  File file = fs.open(path, FILE_WRITE);
  if(!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

// Append data to the SD card (DON'T MODIFY THIS FUNCTION)
void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);
  File file = fs.open(path, FILE_APPEND);
  if(!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}
