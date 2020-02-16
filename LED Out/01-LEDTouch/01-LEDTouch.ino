const int ledPin = 0;
const int touchPin = 4;

int touchInput;

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode (ledPin, OUTPUT);
}

void loop() {
  touchInput = touchRead(touchPin);

  if(touchInput < 30){
    digitalWrite(ledPin, HIGH);
    Serial.print(touchInput);
    Serial.print(" - LED on \n");
  } else {
    digitalWrite(ledPin, LOW);
    Serial.print(touchInput);
    Serial.print(" - LED off \n");
  }

  delay(100);
}
