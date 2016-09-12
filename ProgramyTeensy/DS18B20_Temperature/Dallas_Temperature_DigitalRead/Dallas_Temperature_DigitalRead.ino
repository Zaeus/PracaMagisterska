/* PracaMagisterska - czujnik DS18B20
by Zaeus
*/

#include <OneWire.h>
#include <DS18B20.h>

// Pin zasilania +3,3V
#define DS18B20_POWER 0
// Pin odczytu sygnału
#define DS18B20_DATA 2
// PIN masy 0V
#define DS18B20_GROUND 3

// Flaga otrzymania komendy rozpoczęcia pracy
bool isStartSignalReceived = false;   
// Komenda rozpoczęcia pracy
const String START_CMD = "START_CMD"; 
// Komenda zakończenia pracy
const String STOP_CMD = "STOP_CMD";

// Tablica adresów czujnika
byte address[8];

// Instancja klasy OneWire z biblioteki OneWire
OneWire onewire(DS18B20_DATA);
// Instancja klasy czujnika z biblioteki DS18B20
DS18B20 sensor(&onewire);

// Funkcja wyszukująca adres czujnika DS18B20
void findDS18B20Address() {
  onewire.reset_search();
  while(onewire.search(address))
  {
    if (address[0] != 0x28){
      continue;
    }
    if (OneWire::crc8(address, 7) != address[7])
    {
      Serial.println(F("Błędny adres, sprawdź połączenia"));
      break;
    }
  }
}

void setup()
{
  // Ustawienie pinu 0 na pin wyjściowy - zasilania Vcc
  pinMode(DS18B20_POWER, OUTPUT);  
  // Ustawienie stanu pinu 0 na napięcie 3,3V
  digitalWrite(DS18B20_POWER, HIGH);
  // Ustawienie pinu 3 na pin wyjściowy - zasilania GND
  pinMode(DS18B20_GROUND, OUTPUT); 
  // Ustawienie stanu pinu 3 na napięcie 0V
  digitalWrite(DS18B20_GROUND ,LOW);
  
  // Otworzenie portu szeregowego (115200 bodów)
  Serial.begin(115200);  

  // Inicjalizacja adresów czujnika DS18B20
  findDS18B20Address();
  // Inicjalizacja czujnika DS18B20
  sensor.begin();
  sensor.request(address);
}

void loop() {
  // Wykonywanie funkcji zbierania danych po otrzymaniu sygnału rozpoczęcia START_CMD
  if (isStartSignalReceived) {
    // Odczyt temperatury z czujnika DS18B20
    if (sensor.available())
    {
      float temp = sensor.readTemperature(address);  
      // Wysłanie otrzymanej wartości z 6 miejscami po przecinku
      Serial.println(temp, 6);
      sensor.request(address);
    }    
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
