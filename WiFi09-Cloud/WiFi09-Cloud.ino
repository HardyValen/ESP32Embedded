#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <HTTPClient.h>

Adafruit_BMP085 bmp;

// Replace with your SSID and Password
const char* ssid      = "TP-LINK_890E7A";
const char* password  = "suryanaga";

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "https://hardyvalen.my.id/post-esp-data.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 
String apiKeyValue = "tPmAT5Ab3j7F9";

String sensorName = "BMP180";
String sensorLocation = "Hardy(Bekasi)";

void setup() {
  Serial.begin(115200); 
  
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
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

}

void loop() {
    //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + sensorName
                          + "&location=" + sensorLocation + "&value1=" + String(bmp.readTemperature())
                          + "&value2=" + String(bmp.readPressure()/100000.0F) + "";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);

    //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&location=Office&value1=24.75&value2=1005.14";

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
    
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every hour
  delay(60 * 60 * 1000);
}
