#include <SimpleBLE.h>

  #include <WiFi.h>

const int ledPin1 = 0;
const int ledPin2 = 23;
const int ledPin3 = 26;
const int ledPin4 = 27;
const int touchPin = 4;
const char* ssid     = "inihotspot";
const char* password = "dibagihotspot";

int hallValue;
int touchInput;

WiFiServer server(80);

String header;
String ledPin3State = "off";
String ledPin4State = "off";

void setup() {
  Serial.begin(9600);
  Serial.begin(115200);
  delay(1000);
  Serial.println("Sensor Test");
  
  pinMode (ledPin1, OUTPUT);
  pinMode (ledPin2, OUTPUT);
  pinMode (ledPin3, OUTPUT);
  pinMode (ledPin4, OUTPUT);

  digitalWrite(ledPin3, LOW);
  digitalWrite(ledPin4, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  touchInput = touchRead(touchPin);
  hallValue = hallRead();

  // Touch Function
  if(touchInput < 30){
    digitalWrite(ledPin1, HIGH);
    Serial.print(touchInput);
    Serial.print(" - LED on \n");
  } else {
    digitalWrite(ledPin1, LOW);
    Serial.print(touchInput);
    Serial.print(" - LED off \n");
  }

  // Hall Function
  if(abs(hallValue) > 100){
    digitalWrite(ledPin2, HIGH);
    Serial.print(hallValue);
    Serial.print(" Hall - LED on \n");
  } else {
    digitalWrite(ledPin2, LOW);
    Serial.print(hallValue);
    Serial.print(" Hall - LED off \n");
  }

  // Wifi
    WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("GPIO 26 on");
              ledPin3State = "on";
              digitalWrite(ledPin3, HIGH);
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              ledPin3State = "off";
              digitalWrite(ledPin3, LOW);
            } else if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("GPIO 27 on");
              ledPin4State = "on";
              digitalWrite(ledPin4, HIGH);
            } else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("GPIO 27 off");
              ledPin4State = "off";
              digitalWrite(ledPin4, LOW);
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 26  
            client.println("<p>GPIO 26 - State " + ledPin3State + "</p>");
            // If the ledPin3State is off, it displays the ON button       
            if (ledPin3State=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 27  
            client.println("<p>GPIO 27 - State " + ledPin4State + "</p>");
            // If the ledPin4State is off, it displays the ON button       
            if (ledPin4State=="off") {
              client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
  
  delay(200);
}
