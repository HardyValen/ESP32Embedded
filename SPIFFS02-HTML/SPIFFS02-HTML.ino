#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include "SPIFFS.h"

Adafruit_BMP085 bmp;

const char* ssid      = "TP-LINK_890E7A";
const char* password  = "suryanaga";

WiFiServer server(80);
String header;

void setup() {
  Serial.begin(115200);
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi Connected");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  // Block Loop1: Listen to incoming cliennts
  WiFiClient client = server.available();

  // Block Loop2: Client connection checker, invoked when a new client connects
  if (client) {
    Serial.println("New Client");   // Prints the connected client status to the serial
    boolean isBlank = true;

    // Block Loop2.1: Do a loop while the client is connected
    while (client.connected()) {
      // Block Loop2.1.1: Loop while the client is connected
      if (client.available()) {     // If there's byte to read from the client,
        char c = client.read();     // Read a byte then
        header += c;                // The bytes are stored for the HTTP Header

        // Block Loop2.1.1.1: If the byte read is \n, continue
        if (c == '\n' && isBlank) {
          client.println("HTTP/1.1 200 OK");
          if (header.indexOf("update_readings") >= 0) {
            client.println("Content-Type:text/xml");
            client.println("Connection: keep-alive");
            client.println();
            sendXMLFile(client);
          } else {
            client.println("Content-Type:text/html");
            client.println("Connection: keep-alive");
            client.println();

            File html = SPIFFS.open("/index.html");
            if(!html){
              client.println("<html><h1>Internal Server Error 500</h1></html>");
            } else {
              while(html.available()){
                client.write(html.read());
              }  
            }
          }

          break;

          if (c == '\n') {
            isBlank = true;
          } else if (c != '\r') {
            isBlank += false;
          }
        }
      }
    }

    // Block 2.2. Disconnect the client and the server
    header = "";
    client.stop();
    Serial.println("Client disconnected\n");
  }
}

void sendXMLFile(WiFiClient cl) {
  cl.print("<?xml version = \"1.0\" ?>");
  cl.print("<inputs>");
  cl.print("<read>");
  cl.print(bmp.readTemperature());
  cl.print("</read>");
  cl.print("<read>");
  cl.print(bmp.readPressure());
  cl.print("</read>");
  cl.print("</inputs>");
}
