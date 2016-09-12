/* PracaMagisterska - czujnik DHT22
by Zaeus
*/

#include "DHT.h"
 
// Pin zasilania +3,3V
#define DHT22_POWER 0
// Pin odczytu sygnału
#define DHT22_DATA 1
// PIN masy 0V
#define DHT22_GROUND 3
// Typ czujnika -> DHT22
#define DHTTYPE DHT22
 
// Instancja klasy czujnika z biblioteki DHT
DHT dht(DHT22_DATA, DHTTYPE, 40);
// Flaga otrzymania komendy rozpoczęcia pracy
bool isStartSignalReceived = false;   
// Komenda rozpoczęcia pracy
const String START_CMD = "START_CMD"; 
// Komenda zakończenia pracy
const String STOP_CMD = "STOP_CMD"; 
 
void setup()
{
  delay(2000);
  
  // Ustawienie pinu 0 na pin wyjściowy - zasilania Vcc
  pinMode(DHT22_POWER, OUTPUT);  
  // Ustawienie stanu pinu 0 na napięcie 3,3V
  digitalWrite(DHT22_POWER, HIGH);
  // Ustawienie pinu 3 na pin wyjściowy - zasilania GND
  pinMode(DHT22_GROUND, OUTPUT); 
  // Ustawienie stanu pinu 3 na napięcie 0V
  digitalWrite(DHT22_GROUND ,LOW);
  
  // Otworzenie portu szeregowego (115200 bodów)
  Serial.begin(115200);
  // Inicjalizacja czujnika DHT22
  dht.begin();
}
  
void loop()
{
  // Wykonywanie funkcji zbierania danych po otrzymaniu sygnału rozpoczęcia START_CMD
  if (isStartSignalReceived) {
    // Odczyt temperatury z DHT22
    float temp = dht.readTemperature();

    // Wysłanie temperatury w przypadku pomyślnego uzyskania wartości
    if (isnan(temp)) {
      Serial.println("Blad odczytu danych z czujnika");
    } 
    else {
      Serial.println(temp, 2);
    }
  }
  
  delay(2000);
}

// Funkcja obsługi zdarzenia przyjścia informacji przez port szeregowy
void serialEvent() {
  while (Serial.available()) {
    if (!isStartSignalReceived && Serial.available() > 0) {
      String message = Serial.readString();
      if (START_CMD == message) {
        isStartSignalReceived = true;
        Serial.println("DHT22");
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
