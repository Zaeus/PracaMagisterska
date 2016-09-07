/* PracaMagisterska - czujnik LM35
by Zaeus
*/

#define LM35_POWER 0       // Cyfrowy Pin 0 zasilania +3,3V
#define LM35_DATA 2        // Analogowy Pin A2 odczytu sygnału
#define LM35_GROUND 3      // Cyfrowy PIN 3 masy 0V

bool isStartSignalReceived = false; // Flaga otrzymania komendy rozpoczęcia pracy
const String START_CMD = "START_CMD"; // Komenda rozpoczęcia pracy
const String STOP_CMD = "STOP_CMD"; // Komenda zakończenia pracy

void setup()
{
  delay(2000);
  
  // Pin napięcia zasilania +Vcc do LM35
  pinMode(LM35_POWER, OUTPUT);  
  digitalWrite(LM35_POWER, HIGH);
  // Pin masy GND
  pinMode(LM35_GROUND, OUTPUT); 
  digitalWrite(LM35_GROUND ,LOW);
  
  analogReference(INTERNAL);
  // Otworzenie portu szeregowego (9600 bps)
  Serial.begin(115200);
}

void loop() {
  // Zbieranie danych po otrzymaniu sygnału rozpoczęcia zbierania danych
  if (isStartSignalReceived) {
    // Odczyt temperatury z LM35
    int reading = analogRead(LM35_DATA);
    float tempC = reading / 9.31;
    
    Serial.println(tempC, 6);
  }
  
  delay(1000);
}

// Zdarzenie przyjścia sygnału przez port szeregowy
void serialEvent() {
  while (Serial.available()) {
    if (!isStartSignalReceived && Serial.available() > 0) {
      String message = Serial.readString();
      if (START_CMD == message) {
        isStartSignalReceived = true;
        Serial.println("LM35");
        Serial.println("Unit [*C]");
        return;
      }
    }
  
    if (isStartSignalReceived && Serial.available() > 0) {
      String message = Serial.readString();
      if (STOP_CMD == message) {
        isStartSignalReceived = false;
        return;
      }
    }
  }
}
