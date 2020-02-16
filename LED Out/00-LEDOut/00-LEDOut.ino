const int ledPin = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  Serial.println("Hello World");
}

void loop() {
  digitalWrite(ledPin, HIGH);
  Serial.println("Hello World");
  delay(1000);
  digitalWrite(ledPin, LOW);
  Serial.println("Hello World");
  delay(1000);
}
