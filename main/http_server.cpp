#include "main.h"
#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include "sensor.h"

AsyncWebServer server(80);

extern Adafruit_BME680 bme;

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

void httpServerInit() {
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
