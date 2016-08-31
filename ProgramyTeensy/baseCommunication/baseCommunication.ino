
bool isStartSignalReceived = false;
bool isStopSignalReceived = false;
const String START_CMD = "START_CMD";
const String STOP_CMD = "STOP_CMD";

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (isStartSignalReceived && !isStopSignalReceived) {
    Serial.println("Sensor working");
    delay(1000);
  }
}

void serialEvent() {
  while (Serial.available()) {
    if (!isStartSignalReceived && Serial.available() > 0) {
      String message = Serial.readString();
      if (START_CMD == message) {
        isStartSignalReceived = true;
        Serial.println("Sensor X detection started");
        return;
      }
    }
  
    if (!isStopSignalReceived && Serial.available() > 0) {
      String message = Serial.readString();
      if (STOP_CMD == message) {
        isStopSignalReceived = true;
        return;
      }
    }
  }
}
