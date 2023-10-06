#ifndef SENSOR_H
#define SENSOR_H

#define buttonDownPin 14
#define buttonUpPin 12
#define buttonSelectPin 27

#define SEALEVELPRESSURE_HPA (1013.25)


void subMenuSensor();
void sensorInit();
void GetGasReference();
#endif
