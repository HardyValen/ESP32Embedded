#include <WiFi.h>
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"

// Replace with your SSID and Password
const char* ssid      = "TP-LINK_890E7A";
const char* password  = "suryanaga";

AsyncWebServer server(80);

// Replace with your unique IFTTT URL resource
const char* resource = "/trigger/esp32_form/with/key/dO1uekEnRGjVGzi245KCYn";

// How your resource variable should look like, but with your own API KEY (that API KEY below is just an example):
//const char* resource = "/trigger/bme280_readings/with/key/nAZjOphL3d-ZO4N3k64-1A7gTlNSrxMJdmqy3";

// Maker Webhooks IFTTT
const char* IFTTTserver = "maker.ifttt.com";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

// Make an HTTP request to the IFTTT web service
void makeIFTTTRequest(String value1, String value2, String value3) {
  Serial.print("Connecting to "); 
  Serial.print(IFTTTserver);
  
  WiFiClient client;
  int retries = 5;
  while(!!!client.connect(IFTTTserver, 80) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();
  if(!!!client.connected()) {
    Serial.println("Failed to connect...");
  }
  
  Serial.print("Request resource: "); 
  Serial.println(resource);
  
  String jsonObject = String("{\"value1\":\"") + value1 + "\",\"value2\":\"" + value2 + "\",\"value3\":\"" + value3 + "\"}";              
                      
  client.println(String("POST ") + resource + " HTTP/1.1");
  client.println(String("Host: ") + IFTTTserver); 
  client.println("Connection: close\r\nContent-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonObject.length());
  client.println();
  client.println(jsonObject);
        
  int timeout = 5 * 10; // 5 seconds             
  while(!!!client.available() && (timeout-- > 0)){
    delay(100);
  }
  if(!!!client.available()) {
    Serial.println("No response...");
  }
  while(client.available()){
    Serial.write(client.read());
  }
  
  Serial.println("\nclosing connection");
  client.stop(); 
}

void setup() {
  Serial.begin(115200);

  // Hardware Checking
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

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server.on("/success.svg", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/success.svg", "image/svg+xml");
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/script.js", "text/javascript");
  });

  server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request){
    String nama = "";
    String feedback = "";
    String rating = "";
    
    if(request->hasParam("nama", true)) {
      AsyncWebParameter* p = request->getParam("nama", true);
      nama = p->value().c_str();
    }

    if(request->hasParam("feedback", true)) {
      AsyncWebParameter* p = request->getParam("feedback", true);
      feedback = p->value().c_str();
    }

    if(request->hasParam("rating", true)) {
      AsyncWebParameter* p = request->getParam("rating", true);
      rating = p->value().c_str();
    }

    Serial.println(nama);
    Serial.println(feedback);
    Serial.println(rating);

    makeIFTTTRequest(nama, feedback, rating);
    
    request->send(SPIFFS, "/success.html");
  });

  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

}
