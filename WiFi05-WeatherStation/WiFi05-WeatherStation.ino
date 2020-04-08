#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

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
          Serial.println(header.indexOf("update_readings"));
          client.println("HTTP/1.1 200 OK");
          if (header.indexOf("update_readings") >= 0) {
            client.println("Content-Type:text/xml");
            client.println("Connection: keep-alive");
            client.println();
            sendXMLFile(client);
            Serial.println(bmp.readTemperature());
          } else {
            client.println("Content-Type:text/html");
            client.println("Connection: keep-alive");
            client.println();
            client.println("<!doctype html>");
            client.println("<html lang='en'>");
            client.println("<head>");
            client.println("<meta charset='utf-8'>");
            client.println("<meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'>");
            client.println("<title>BMP180 Weather Station</title>");

            client.println("<script>");

            client.println("function GetReadings(){");
              client.println("nocache = '&nocache';");
              client.println("var request = new XMLHttpRequest();");
              client.println("request.onreadystatechange = function() {");
                client.println("if (this.status == 200) {");
                  client.println("if (this.responseXML != null){");
                      client.println("var num_an = this.responseXML.getElementsByTagName('read').length;");
                      client.println("for (var i = 0; i < num_an; i++){document.getElementsByClassName('read')[i].innerHTML = this.responseXML.getElementsByTagName('read')[i].childNodes[0].nodeValue;}");
                  client.println("}");
                client.println("}");
              client.println("}");

              client.println("request.open('GET', '?update_readings' + nocache, true);");
              client.println("request.send(null);");
              client.println("setTimeout('GetReadings()', 1000);");
            client.println("}");
            
            client.println("document.addEventListener('DOMContentLoaded', function(){GetReadings()}, false);");
            client.println("</script>");
            client.println("</head>");

            
            client.println("<body>");
              client.println("<h1>BMP180 Weather Station</h1>");
              client.println("<h2>Real-Time Update</h2>");
              client.println("Temperature Today: ");
              client.println("<span class='read'></span>");        
              client.println(" &#8451<br>");
              client.println("Pressure Today: ");
              client.println("<span class='read'></span>");
              client.println(" Pa<br>");
              client.print("Altitude Today: ");
              client.print("<span class='read'></span>");
              client.println(" Meters<br>");
            client.println("</body>");
            client.println("</html>");
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
  cl.print("<read>");
  cl.print(bmp.readAltitude());
  cl.print("</read>");
  cl.print("</inputs>");
}
