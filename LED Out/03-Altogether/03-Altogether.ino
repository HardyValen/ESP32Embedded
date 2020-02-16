const int ledPin1 = 0;
const int ledPin2 = 23;
const int touchPin = 4;

int hallValue;
int touchInput;

void setup() {
  Serial.begin(9600);
  Serial.begin(115200);
  delay(1000);
  Serial.println("Touch sensor test from GPIO Pin 4");
  pinMode (ledPin1, OUTPUT);
  pinMode (ledPin2, OUTPUT);
}

void loop() {
  touchInput = touchRead(touchPin);
  hallValue = hallRead();

  if(touchInput < 30){
    digitalWrite(ledPin1, HIGH);
    Serial.print(touchInput);
    Serial.print(" - LED on \n");
  } else {
    digitalWrite(ledPin1, LOW);
    Serial.print(touchInput);
    Serial.print(" - LED off \n");
  }

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
