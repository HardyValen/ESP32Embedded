#include <Wire.h>
#include <Adafruit_BMP085.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
Adafruit_BMP085 bmp;
  
void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32HardyHudanAbbel"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  if (!bmp.begin()) {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  while (1) {}
  }
}
  
void loop() {
    SerialBT.print("Temperature = ");
    SerialBT.print(bmp.readTemperature());
    SerialBT.println(" *C");
    delay(400);
}
