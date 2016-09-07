/* PracaMagisterska - czujnik DS18B20
by Zaeus
*/

#define DS18B20_POWER 0       // Cyfrowy Pin 0 zasilania +3,3V
#define DS18B20_DATA 2        // Analogowy Pin A2 odczytu sygnału
#define DS18B20_GROUND 3      // Cyfrowy PIN 3 masy 0V

bool isStartSignalReceived = false; // Flaga otrzymania komendy rozpoczęcia pracy
const String START_CMD = "START_CMD"; // Komenda rozpoczęcia pracy
const String STOP_CMD = "STOP_CMD"; // Komenda zakończenia pracy

void setup()
{
  delay(2000);
  
  // Pin napięcia zasilania +Vcc do DS18B20
  pinMode(DS18B20_POWER, OUTPUT);  
  digitalWrite(DS18B20_POWER, HIGH);
  // Pin masy GND
  pinMode(DS18B20_GROUND, OUTPUT); 
  digitalWrite(DS18B20_GROUND ,LOW);
  
  analogReference(INTERNAL);
  // Otworzenie portu szeregowego (9600 bps)
  Serial.begin(115200);
}

void loop() {
  // Zbieranie danych po otrzymaniu sygnału rozpoczęcia zbierania danych
  if (isStartSignalReceived) {
    // Odczyt temperatury z DS18B20
    int value = analogRead(DS18B20_DATA);
    float tempC = 100 * 3.3 * (float)value / 1024;
    
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
        Serial.println("DS18B20");
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
