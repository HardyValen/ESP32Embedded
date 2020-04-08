#include <WiFi.h>

const char* ssid      = "TP-LINK_890E7A";
const char* password  = "suryanaga";

WiFiServer server(80);
String header;

void setup() {
  Serial.begin(115200);
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
    String currentLine = "";        // To hold the incoming data from the client

    // Block Loop2.1: Do a loop while the client is connected
    while(client.connected()){

      // Block Loop2.1.1: Loop while the client is connected
      if(client.available()){       // If there's byte to read from the client,
        char c = client.read();     // Read a byte then
        Serial.write(c);            // Print the byte on the serial monitor
        header += c;                // The bytes are stored for the HTTP Header

        // Block Loop2.1.1.1: If the byte read is \n, continue
        if (c == '\n'){

          // Block Loop2.1.1.1.1: If the current line is empty, indicates the end of the header
          if (currentLine.length() == 0){
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            client.println("<!doctype html>");
            client.println("<html lang='en'>");
            client.println("<head>");
            client.println("<meta charset='utf-8'>");
            client.println("<meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'>");
            client.println("<link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css' integrity='sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T' crossorigin='anonymous'>");
            client.println("<title>Hello, world!</title>");
            client.println("</head>");
            client.println("<body>");
            client.println("<h1>Hello, world!</h1>");
            client.println("<script src='https://code.jquery.com/jquery-3.3.1.slim.min.js' integrity='sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo' crossorigin='anonymous'></script>");
            client.println("<script src='https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.7/umd/popper.min.js' integrity='sha384-UO2eT0CpHqdSJQ6hJty5KVphtPhzWj9WO1clHTMGa3JDZwrnQq4sF86dIHNDz0W1' crossorigin='anonymous'></script>");
            client.println("<script src='https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js' integrity='sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM' crossorigin='anonymous'></script>");
            client.println("</body>");
            client.println("</html>");
            break;
          } 
          
          // Block Loop 2.1.1.1.2: If the current line is not empty
          else {
            currentLine = "";  
          }  
        } 

        // Block Loop2.1.1.2: If the byte is not '/r', concatenate the byte to currentLine
        else if (c != '\r'){
          currentLine += c;  
        }
      }
    }

    // Block 2.2. Disconnect the client and the server
    header = "";
    client.stop();
    Serial.println("Client disconnected\n");
  } 
}
