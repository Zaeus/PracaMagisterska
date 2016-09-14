/* PracaMagisterska - czujnik LM35
by Zaeus
*/

// Pin zasilania +3,3V
#define LM35_POWER 0
// Pin odczytu sygnału - Analogowy Pin A2
#define LM35_DATA 2
// PIN masy 0V
#define LM35_GROUND 3

// Flaga otrzymania komendy rozpoczęcia pracy
bool isStartSignalReceived = false;   
// Komenda rozpoczęcia pracy
const String START_CMD = "START_CMD"; 
// Komenda zakończenia pracy
const String STOP_CMD = "STOP_CMD"; 

void setup()
{
  // Ustawienie pinu 0 na pin wyjściowy - zasilania Vcc
  pinMode(LM35_POWER, OUTPUT);  
  // Ustawienie stanu pinu 0 na napięcie 3,3V
  digitalWrite(LM35_POWER, HIGH);
  // Ustawienie pinu 3 na pin wyjściowy - zasilania GND
  pinMode(LM35_GROUND, OUTPUT); 
  // Ustawienie stanu pinu 3 na napięcie 0V
  digitalWrite(LM35_GROUND ,LOW);

  // Ustawienie referencji dla wejścia analogowego
  analogReference(INTERNAL);
  // Otworzenie portu szeregowego (115200 bodów)
  Serial.begin(115200);
}

void loop() {
  // Wykonywanie funkcji zbierania danych po otrzymaniu sygnału rozpoczęcia START_CMD
  if (isStartSignalReceived) {
    // Odczyt temperatury z LM35 z pinu analogowego
    int reading = analogRead(LM35_DATA);
    // Przeliczenie informacji o napięciu na temperaturę
    float tempC = ((reading * 3.2) - 500) / 10;    
    // Wysłanie temperatury z dokładnością do 6 miejsc po przecinku
    Serial.println(tempC, 6);
  }
  
  delay(1000);
}

// Funkcja obsługi zdarzenia przyjścia informacji przez port szeregowy
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
