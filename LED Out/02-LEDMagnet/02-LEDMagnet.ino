const int ledPin2 = 23;

int hallValue;

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode (ledPin2, OUTPUT);
}

void loop() {
  hallValue = hallRead();

  if(abs(hallValue) > 100){
    digitalWrite(ledPin2, HIGH);
    Serial.print(hallValue);
    Serial.print(" Hall - LED on \n");
  } else {
    digitalWrite(ledPin2, LOW);
    Serial.print(hallValue);
    Serial.print(" Hall - LED off \n");
  }

  delay(200);
}
