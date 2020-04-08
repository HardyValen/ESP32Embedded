#include "BluetoothSerial.h"
#include <LiquidCrystal_I2C.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

int lcdColumns = 16;
int lcdRows = 1;

BluetoothSerial SerialBT;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

String message = "";
char incomingChar;

void setup() {
  lcd.init();
  lcd.backlight();

  Serial.begin(115200);
  SerialBT.begin("ESP32HardyHudanAbbel"); //Bluetooth device name
}
  
void loop() {
  if (SerialBT.available()){
    char incomingChar = SerialBT.read();
    if (incomingChar != '\n'){
      lcd.clear();  
      message += String(incomingChar);
    } else {
      message = "";
    }

      lcd.setCursor(0, 0);
      lcd.print(message);
    
      delay(20);
  }
}
