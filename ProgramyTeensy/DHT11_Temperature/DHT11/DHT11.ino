/* PracaMagisterska - czujnik DHT11
by Zaeus
*/

#include <dht11.h>

// Pin zasilania +3,3V
#define DHT11_POWER 0
// Pin odczytu sygnału
#define DHT11_DATA 1
// PIN masy 0V
#define DHT11_GROUND 3

// Instancja klasy czujnika z biblioteki DHT11
dht11 dht;
// Flaga otrzymania komendy rozpoczęcia pracy
bool isStartSignalReceived = false;   
// Komenda rozpoczęcia pracy
const String START_CMD = "START_CMD"; 
// Komenda zakończenia pracy
const String STOP_CMD = "STOP_CMD"; 
 
void setup()
{
  // Ustawienie pinu 0 na pin wyjściowy - zasilania Vcc
  pinMode(DHT11_POWER, OUTPUT);  
  // Ustawienie stanu pinu 0 na napięcie 3,3V
  digitalWrite(DHT11_POWER, HIGH);
  // Ustawienie pinu 3 na pin wyjściowy - zasilania GND
  pinMode(DHT11_GROUND, OUTPUT); 
  // Ustawienie stanu pinu 3 na napięcie 0V
  digitalWrite(DHT11_GROUND ,LOW);
  
  // Otworzenie portu szeregowego (115200 bodów)
  Serial.begin(115200);
}
 
void loop()
{
  // Wykonywanie funkcji zbierania danych po otrzymaniu sygnału rozpoczęcia START_CMD
  if (isStartSignalReceived) {
    // Odczyt temperatury z czujnika DHT11
    dht.read(DHT11_DATA);
    float temp = dht.temperature;

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
        Serial.println("DHT11");
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
