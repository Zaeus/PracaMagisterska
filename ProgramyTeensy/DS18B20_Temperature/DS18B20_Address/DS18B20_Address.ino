#include <OneWire.h>

// Numer pinu cyfrowego do którego podłaczyłęś czujniki
#define DS18B20_POWER 0       // Cyfrowy Pin 0 - zasilanie +3,3V
#define DS18B20_DATA 2        // Cyfrow Pin 2 - odczytu sygnału
#define DS18B20_GROUND 3      // Cyfrowy PIN 3 - masa 0V

bool isStartSignalReceived = false; // Flaga otrzymania komendy rozpoczęcia pracy
const String START_CMD = "START_CMD"; // Komenda rozpoczęcia pracy
const String STOP_CMD = "STOP_CMD"; // Komenda zakończenia pracy

OneWire onewire(DS18B20_DATA);

void setup()
{
  // Pin napięcia zasilania +Vcc do DS18B20
  pinMode(DS18B20_POWER, OUTPUT);  
  digitalWrite(DS18B20_POWER, HIGH);
  // Pin masy GND
  pinMode(DS18B20_GROUND, OUTPUT); 
  digitalWrite(DS18B20_GROUND ,LOW);
  
  // Otworzenie portu szeregowego (9600 bps)
  Serial.begin(9600);
}

void loop()
{
  if (isStartSignalReceived) {
    byte address[8];
  
    onewire.reset_search();
    while(onewire.search(address))
    {
      if (address[0] != 0x28)
        continue;
  
      if (OneWire::crc8(address, 7) != address[7])
      {
        Serial.println(F("Błędny adres, sprawdz polaczenia"));
        break;
      }
  
      for (byte i=0; i<8; i++)
      {
        Serial.print(F("0x"));
        Serial.print(address[i], HEX);
  
        if (i < 7)
          Serial.print(F(", "));
      }
      Serial.println();
    }    
  }
  
  delay(3000);
}

// Zdarzenie przyjścia sygnału przez port szeregowy
void serialEvent() {
  while (Serial.available()) {
    if (!isStartSignalReceived && Serial.available() > 0) {
      String message = Serial.readString();
      if (START_CMD == message) {
        isStartSignalReceived = true;
        Serial.println("DS18B20 address:");
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
