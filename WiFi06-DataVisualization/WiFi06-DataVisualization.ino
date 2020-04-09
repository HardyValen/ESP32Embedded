#include <WiFi.h>
#include <Adafruit_BMP085.h>
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"

Adafruit_BMP085 bmp;

const char* ssid      = "TP-LINK_890E7A";
const char* password  = "suryanaga";

AsyncWebServer server(80);

String readBMP180Temperature(){
  float t = bmp.readTemperature();
  return String(t);
}

String readBMP180Pressure(){
  float t = bmp.readPressure();
  return String(t);
}

void setup() {
  Serial.begin(115200);

  // Hardware Checking
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }

  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi Connected");
  Serial.println("IP Address: ");

  // Local IP Address
  Serial.println(WiFi.localIP());

  // Routing
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readBMP180Temperature().c_str());
  });

  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readBMP180Pressure().c_str());
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/script.js", "text/javascript");
  });

  server.begin();

}

void loop() {

}
