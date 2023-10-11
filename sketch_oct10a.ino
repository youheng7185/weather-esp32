#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "Galaxy S23 75A3";
const char* password = "qcomqcom";

const char* html = R"(
<!DOCTYPE HTML>
<html>
  <head>
    <title>BME680 Sensor Data</title>
  </head>
  <body>
    <h1>BME680 Sensor Data</h1>
    <p>Temperature: <span id="temperature">--</span> &#8451;</p>
    <p>Pressure: <span id="pressure">--</span> hPa</p>
    <p>Humidity: <span id="humidity">--</span> %</p>
    <p>Gas Resistance: <span id="gas_resistance">--</span> KOhms</p>
    <p>Approx. Altitude: <span id="altitude">--</span> m</p>
    <script>
      function updateData() {
        fetch("/data")
          .then(response => response.json())
          .then(data => {
            document.getElementById("temperature").textContent = data.temperature;
            document.getElementById("pressure").textContent = data.pressure;
            document.getElementById("humidity").textContent = data.humidity;
            document.getElementById("gas_resistance").textContent = data.gas_resistance;
            document.getElementById("altitude").textContent = data.altitude;
          });
      }
      setInterval(updateData, 2000);
    </script>
  </body>
</html>
)";

#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME680 bme;

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!bme.begin()) {
    Serial.println(F("Could not find a valid BME680 sensor, check wiring!"));
    while (1);
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Route for serving sensor data
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "{";
    json += "\"temperature\":" + String(bme.temperature) + ",";
    json += "\"pressure\":" + String(bme.pressure / 100.0) + ",";
    json += "\"humidity\":" + String(bme.humidity) + ",";
    json += "\"gas_resistance\":" + String(bme.gas_resistance / 1000.0) + ",";
    json += "\"altitude\":" + String(bme.readAltitude(SEALEVELPRESSURE_HPA)) + "}";
    request->send(200, "application/json", json);
  });

  // Route for serving HTML page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", html);
  });

  server.begin();
}

void loop() {
  // Nothing to do here
}
