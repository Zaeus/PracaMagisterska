/* PracaMagisterska
by Zaeus
*/

bool isStartSignalReceived = false; // Flaga otrzymania komendy rozpoczęcia pracy
bool isStopSignalReceived = false; // Flaga otrzymania komendy zakończenia pracy
const String START_CMD = "START_CMD"; // Komenda rozpoczęcia pracy
const String STOP_CMD = "STOP_CMD"; // Komenda zakończenia pracy

void setup() {
  // Otworzenie portu szeregowego (9600 bps)
  Serial.begin(9600);
}

void loop() {
  // Wykonywanie funkcji po otrzymaniu sygnału rozpoczęcia i przed otrzymaniem sygnału zakończenia
  if (isStartSignalReceived && !isStopSignalReceived) {
    Serial.println("Sensor working");
    delay(1000);
  }
}

// Zdarzenie przyjścia sygnału przez port szeregowy
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
