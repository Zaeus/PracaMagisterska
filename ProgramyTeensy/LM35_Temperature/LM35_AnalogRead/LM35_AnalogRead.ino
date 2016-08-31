// LM35 Temperature sensor

float tempC;
int reading;
int tempPin = 0;

void setup()
{
  analogReference(INTERNAL);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  reading = analogRead(tempPin);
  tempC = reading / 9.31;
  Serial.println(tempC);
  delay(1000);
}
