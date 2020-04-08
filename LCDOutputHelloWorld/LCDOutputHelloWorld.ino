#include <LiquidCrystal_I2C.h>

int lcdColumns = 16;
int lcdRows = 1;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

byte dicc[8] = {
  0b00100,
  0b01010,
  0b01010,
  0b01010,
  0b01010,
  0b01010,
  0b10101,
  0b01010
};

void setup() {
  lcd.init();                     
  lcd.backlight();
  lcd.createChar(1, dicc);
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.write(1);
  lcd.print(' ');
  lcd.write(1);
  lcd.print(' ');
  lcd.write(1);
  lcd.print(' ');
  lcd.write(1);
  lcd.print(' ');
  lcd.write(1);
  lcd.print(' ');
  lcd.write(1);
}
