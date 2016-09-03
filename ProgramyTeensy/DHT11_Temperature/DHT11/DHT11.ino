/* PracaMagisterska - czujnik DHT11
by Zaeus
*/

#include <dht11.h>  // Biblioteka DHT11 z http://playground.arduino.cc/main/DHT11Lib

#define DHT_POWER 0       // Pin zasilania +3,3V
#define DHT_DATA 1        // Pin odczytu sygnału
#define DHT_GROUND 3      // PIN masy 0V

dht11 dht; // Definicja czujnika
bool isStartSignalReceived = false; // Flaga otrzymania komendy rozpoczęcia pracy
const String START_CMD = "START_CMD"; // Komenda rozpoczęcia pracy
const String STOP_CMD = "STOP_CMD"; // Komenda zakończenia pracy
 
void setup()
{
  delay(2000);
  
  // Pin napięcia zasilania +Vcc do DHT11
  pinMode(DHT_POWER, OUTPUT);  
  digitalWrite(DHT_POWER, HIGH);
  // Pin masy GND
  pinMode(DHT_GROUND, OUTPUT); 
  digitalWrite(DHT_GROUND ,LOW);
  
  // Otworzenie portu szeregowego (9600 bps)
  Serial.begin(9600);
}
 
void loop()
{
  // Zbieranie danych po otrzymaniu sygnału rozpoczęcia zbierania danych
  if (isStartSignalReceived) {
    // Odczyt temperatury z DHT11
    dht.read(DHT_DATA);
    float temp = dht.temperature;

    // Wysłanie temperatury w przypadku pomyślnego uzyskania wartości
    if (isnan(temp)) {
      Serial.println("Blad odczytu danych z czujnika");
    } 
    else {
      Serial.println(temp);
    }
  }
  
  delay(2000);
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
  
    if (isStartSignalReceived && Serial.available() > 0) {
      String message = Serial.readString();
      if (STOP_CMD == message) {
        isStartSignalReceived = false;
        return;
      }
    }
  }
}
