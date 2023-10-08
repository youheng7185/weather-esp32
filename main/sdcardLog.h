#ifndef SDCARDLOG_H
#define SDCARDLOG_H

#include "FS.h"  // Include the FS.h header for the 'fs::FS' data type
#include "SD.h"  // Include the SD.h header for the 'File' data type

void subMenuSDcard();
void appendSensorData(const char *filename, const char *data);
void writeFile(fs::FS &fs, const char * path, const char * message);
void appendFile(fs::FS &fs, const char * path, const char * message);



#endif
