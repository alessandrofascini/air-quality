const int pin = 34;

void setup() {
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  int valueReaded = analogRead(pin);
  Serial.println(valueReaded);
  delay(500);
}
