const float V_IN = 3.3; // voltage
const uint8_t RESOLUTION = 12; // bit
const float R1 = 220;

const uint8_t ANALOG_PIN = 34;

void setup() {
  Serial.begin(115200);
  analogReadResolution(RESOLUTION);

  delay(1000);
}

void loop() {
  const int rawValue = analogRead(ANALOG_PIN);
  const float v_out = V_IN * (float)(rawValue / pow(2, RESOLUTION));

  const float R2 = v_out / (V_IN - v_out) * R1;

  Serial.print("readed value: ");
  Serial.print(rawValue);
  Serial.print("V; parsed value: ");
  Serial.print(v_out, 3);
  Serial.print("V; unknown resistence: ");
  Serial.print(R2, 0);
  Serial.println(" ohm");

  delay(2000);
}