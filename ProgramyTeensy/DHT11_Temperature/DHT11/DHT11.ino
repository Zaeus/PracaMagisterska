/* PracaMagisterska
by Zaeus
*/

#include "DHT.h"
 
#define DHTPIN 2          // Numer pinu sygnałowego
#define DHTTYPE DHT11     // Typ czujnika -> DHT11
 
DHT dht(DHTPIN, DHTTYPE); // Definicja czujnika
bool isStartSignalReceived = false; // Flaga otrzymania komendy rozpoczęcia pracy
bool isStopSignalReceived = false; // Flaga otrzymania komendy zakończenia pracy
const String START_CMD = "START_CMD"; // Komenda rozpoczęcia pracy
const String STOP_CMD = "STOP_CMD"; // Komenda zakończenia pracy
 
void setup()
{
  // Otworzenie portu szeregowego (9600 bps)
  Serial.begin(9600);
  // Inicjalizacja czujnika DHT11
  dht.begin();
}
 
void loop()
{
  // Zbieranie danych po otrzymaniu sygnału rozpoczęcia i przed otrzymaniem sygnału zakończenia
  if (isStartSignalReceived && !isStopSignalReceived) {
    // Odczyt temperatury z DHT11
    float temperature = dht.readTemperature();

    // Wysłanie temperatury w przypadku pomyślnego uzyskania wartości
    if (isnan(temperature)) {
      Serial.println("Blad odczytu danych z czujnika");
    } 
    else {
      Serial.println(temperature);
    }
  }
}

// Zdarzenie przyjścia sygnału przez port szeregowy
void serialEvent() {
  while (Serial.available()) {
    if (!isStartSignalReceived && Serial.available() > 0) {
      String message = Serial.readString();
      if (START_CMD == message) {
        isStartSignalReceived = true;
        Serial.println("DHT11");
        Serial.println("Unit [*C]");
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
