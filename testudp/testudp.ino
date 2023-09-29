#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "Galaxy S23 75A3";
const char* password = "qcomqcom";

WiFiUDP udp;
unsigned int localPort = 8888; // Port to listen on

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Start UDP server
  udp.begin(localPort);
}

void loop() {
  // Receive UDP packet
  int packetSize = udp.parsePacket();
  if (packetSize) {
    Serial.print("Received packet from ");
    Serial.print(udp.remoteIP());
    Serial.print(":");
    Serial.print(udp.remotePort());
    Serial.print(", length=");
    Serial.println(packetSize);

    // Read the packet into a buffer
    char packetBuffer[255];
    int len = udp.read(packetBuffer, sizeof(packetBuffer));
    if (len > 0) {
      packetBuffer[len] = 0; // Null-terminate the string
      Serial.println("Received packet data: " + String(packetBuffer));

      // Send a response back to the sender
      const char* responseMessage = "Response from ESP32"; // Your response data
      udp.beginPacket(udp.remoteIP(), 8887); // Destination IP and port
      udp.write((uint8_t*)responseMessage, strlen(responseMessage)); // Convert string to bytes
      udp.endPacket();
    }
  }

  // Your other code here
}
