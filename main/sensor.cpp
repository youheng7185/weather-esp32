/***************************************************************************
  This is a library for the BME680 gas, humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME680 Breakout
  ----> http://www.adafruit.com/products/XXXX

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

/*
 This software, the ideas and concepts is Copyright (c) David Bird 2018. All rights to this software are reserved.
 
 Any redistribution or reproduction of any part or all of the contents in any form is prohibited other than the following:
 1. You may print or download to a local hard disk extracts for your personal and non-commercial use only.
 2. You may copy the content to individual third parties for their personal use, but only if you acknowledge the author David Bird as the source of the material.
 3. You may not, except with my express written permission, distribute or commercially exploit the content.
 4. You may not transmit it or store it in any other website or other form of electronic retrieval system for commercial purposes.

 The above copyright ('as annotated') notice and this permission notice shall be included in all copies or substantial portions of the Software and where the
 software use is visible to an end-user.
 
 THE SOFTWARE IS PROVIDED "AS IS" FOR PRIVATE USE ONLY, IT IS NOT FOR COMMERCIAL USE IN WHOLE OR PART OR CONCEPT. FOR PERSONAL USE IT IS SUPPLIED WITHOUT WARRANTY 
 OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHOR OR COPYRIGHT HOLDER BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 See more at http://www.dsbird.org.uk
*/  

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include "main.h"
#include "return.h"
#include "sensor.h"
#include "pins.h"
#include "clock.h"
#include "sdcardLog.h"

int ldrValue;

extern int selectLastState;
extern int selectCurrentState;
extern float hum_weighting = 0.25; // so hum effect is 25% of the total air quality score
extern float gas_weighting = 0.75; // so gas effect is 75% of the total air quality score

float hum_score, gas_score;
//extern float hum_score, gas_score;
extern float gas_reference = 250000;
extern float hum_reference = 40;
extern int   getgasreference_count = 0;

extern Adafruit_BME680 bme;

void GetGasReference(){
  // Now run the sensor for a burn-in period, then use combination of relative humidity and gas resistance to estimate indoor air quality as a percentage.
  Serial.println("Getting a new gas reference value");
  int readings = 10;
  for (int i = 1; i <= readings; i++){ // read gas for 10 x 0.150mS = 1.5secs
    gas_reference += bme.readGas();
  }
  gas_reference = gas_reference / readings;
}

void sensorInit() {
  while (!Serial);
  Serial.println(F("BME680 test"));
  
  Wire.begin();
  if (!bme.begin(0x77)) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  } else Serial.println("Found a sensor");

  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
  GetGasReference();
}



String CalculateIAQ(float score){
  String IAQ_text = "Air quality is ";
  score = (100-score)*5;
  if      (score >= 301)                  IAQ_text += "Hazardous";
  else if (score >= 201 && score <= 300 ) IAQ_text += "Very Unhealthy";
  else if (score >= 176 && score <= 200 ) IAQ_text += "Unhealthy";
  else if (score >= 151 && score <= 175 ) IAQ_text += "Unhealthy for Sensitive Groups";
  else if (score >=  51 && score <= 150 ) IAQ_text += "Moderate";
  else if (score >=  00 && score <=  50 ) IAQ_text += "Good";
  return IAQ_text;
}

void subMenuSensor() {
  while (inSubMenu) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp    :");
  lcd.setCursor(9, 0);
  lcd.print(bme.readTemperature());
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Pressure:");
  lcd.setCursor(9, 1);
  lcd.print(bme.readPressure() / 100.0F);
  lcd.print(" hPa");

  lcd.setCursor(0, 2);
  lcd.print("Humidity:");
  lcd.setCursor(9, 2);
  lcd.print(bme.readHumidity());
  lcd.print("%");

  //Serial.print("        Gas = ");
  //Serial.print(bme.readGas());
    // Update the sensor value
  //Calculate humidity contribution to IAQ index
  float current_humidity = bme.readHumidity();
  if (current_humidity >= 38 && current_humidity <= 42)
    hum_score = 0.25*100; // Humidity +/-5% around optimum 
  else
  { //sub-optimal
    if (current_humidity < 38) 
      hum_score = 0.25/hum_reference*current_humidity*100;
    else
    {
      hum_score = ((-0.25/(100-hum_reference)*current_humidity)+0.416666)*100;
    }
  }
  
  //Calculate gas contribution to IAQ index
  float gas_lower_limit = 5000;   // Bad air quality limit
  float gas_upper_limit = 50000;  // Good air quality limit 
  if (gas_reference > gas_upper_limit) gas_reference = gas_upper_limit;
  if (gas_reference < gas_lower_limit) gas_reference = gas_lower_limit;
  gas_score = (0.75/(gas_upper_limit-gas_lower_limit)*gas_reference -(gas_lower_limit*(0.75/(gas_upper_limit-gas_lower_limit))))*100;
  
  //Combine results for the final IAQ index value (0-100% where 100% is good quality air)
  float air_quality_score = hum_score + gas_score;
  lcd.setCursor(0, 3);
  lcd.print("IAQ     :");
  lcd.setCursor(9, 3);
  lcd.print(air_quality_score);
  Serial.println("Air Quality = "+String(air_quality_score,1)+"% derived from 25% of Humidity reading and 75% of Gas reading - 100% is good quality air");
  Serial.println("Humidity element was : "+String(hum_score/100)+" of 0.25");
  Serial.println("     Gas element was : "+String(gas_score/100)+" of 0.75");
  if (bme.readGas() < 120000) Serial.println("***** Poor air quality *****");
  Serial.println();
  if ((getgasreference_count++)%10==0) GetGasReference(); 
  Serial.println(CalculateIAQ(air_quality_score));
  Serial.println("------------------------------------------------");
  // Create a string containing sensor data, date, and time
  char sensorData[128];
  snprintf(sensorData, sizeof(sensorData), "%s, %.2f C, %.2f hPa, %.2f%%, %.2f IAQ",
           logTime(), bme.readTemperature(), bme.readPressure() / 100.0F, bme.readHumidity(), air_quality_score);

  // Log the sensor data to the SD card
  appendSensorData("/iaq_data.txt", sensorData);
  delay(200);
    // Check for "Select" button press
    //if (selectLastState == LOW && selectCurrentState == HIGH) {
    //  Serial.println("The state changed from LOW to HIGH, button is pressed");
    //  returnMainMenu();
    //  Serial.println("select-home");
    // inSubMenu = false;
    //  break;
    //}
    //selectLastState = selectCurrentState;
    returntoHome();
    // Delay for a short interval
    //delay(1000); // Adjust the delay interval as needed for your desired update rate
  }
}
