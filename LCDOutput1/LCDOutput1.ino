/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <LiquidCrystal_I2C.h>

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 1;
Adafruit_BMP085 bmp;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

void setup(){
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();

  Serial.begin(9600);
  if (!bmp.begin()) {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  while (1) {}
  }
}

void loop(){
  // set cursor to first column, first row
  lcd.setCursor(0, 0);
  // print message
  lcd.print("Temp: ");
  lcd.print(bmp.readTemperature());
  lcd.print(" C");
  delay(2000);
  lcd.clear();

  lcd.print("AtmP: ");
  lcd.print(bmp.readPressure());
  lcd.print(" Pa");
  delay(2000);
  lcd.clear();

  lcd.print("Alt: ");
  lcd.print(bmp.readAltitude());
  lcd.print(" M");
  delay(2000);
  lcd.clear();
}
