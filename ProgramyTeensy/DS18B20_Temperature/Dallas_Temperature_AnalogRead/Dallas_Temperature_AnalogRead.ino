// Dallas DS18B20 Temperature sensor

const int readPin = A2; // ADC1
float value = 0;

void setup() {
    pinMode(readPin, INPUT);
    Serial.begin(9600);
    Serial.println("End Setup");
}

void loop() {
    value = 100*3.3*(float)analogRead(A2)/1024;
    //Serial.print(" Value stopnie!: ");
    Serial.println(value);
    delay(950);
}
