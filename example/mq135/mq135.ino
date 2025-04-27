const int ANALOG_PIN = 34;
const int DIGITAL_PIN = 18;

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.print("The sensor is warming up\n[");
  for (int i = 0; i < 6; i++)
  {
    Serial.print("=");
    delay(500);
  }
  Serial.println("]");
  pinMode(DIGITAL_PIN, INPUT);
}

void loop()
{
  delay(5000);

  const uint16_t rawGasVal = analogRead(ANALOG_PIN);
  const int isGas = digitalRead(DIGITAL_PIN);

  const int gasVal = map(rawGasVal, 0, 4096, 0, 100);

  Serial.print("Gas: ");
  Serial.print(gasVal);
  Serial.print("% [");
  Serial.print(rawGasVal);
  Serial.print("] isGas? ");
  if (isGas)
  { // 1
    Serial.println("no");
  }
  else
  { // 0
    Serial.println("yes");
  }
}
