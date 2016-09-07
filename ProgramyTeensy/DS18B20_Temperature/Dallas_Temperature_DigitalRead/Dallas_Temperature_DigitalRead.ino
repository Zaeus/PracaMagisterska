/* PracaMagisterska - czujnik DS18B20
by Zaeus
*/

#include <OneWire.h>
#include <DS18B20.h>

#define DS18B20_POWER 0       // Cyfrowy Pin 0 - zasilanie +3,3V
#define DS18B20_DATA 2        // Cyfrow Pin 2 - odczytu sygnału
#define DS18B20_GROUND 3      // Cyfrowy PIN 3 - masa 0V

bool isStartSignalReceived = false; // Flaga otrzymania komendy rozpoczęcia pracy
const String START_CMD = "START_CMD"; // Komenda rozpoczęcia pracy
const String STOP_CMD = "STOP_CMD"; // Komenda zakończenia pracy

byte address[8] = {0x28, 0xFF, 0xDC, 0x18, 0x64, 0x14, 0x1, 0xA1};

OneWire onewire(DS18B20_DATA);
DS18B20 sensors(&onewire);

void setup()
{
  delay(2000);
  
  // Pin napięcia zasilania +Vcc do DS18B20
  pinMode(DS18B20_POWER, OUTPUT);  
  digitalWrite(DS18B20_POWER, HIGH);
  // Pin masy GND
  pinMode(DS18B20_GROUND, OUTPUT); 
  digitalWrite(DS18B20_GROUND ,LOW);
  
  // Otworzenie portu szeregowego (115200 bps)
  Serial.begin(115200);
  
  E(sensors.begin());
  E(sensors.request(address));
}

void loop() {
  // Zbieranie danych po otrzymaniu sygnału rozpoczęcia zbierania danych
  if (isStartSignalReceived) {
    // Odczyt temperatury z DS18B20
    if (sensors.available())
    {
      float temperature = sensors.readTemperature(address);
      TE(temperature);
  
      Serial.println(temperature);
  
      E(sensors.request(address));
    }    
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
