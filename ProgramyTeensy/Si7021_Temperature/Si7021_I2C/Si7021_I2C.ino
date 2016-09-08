/* PracaMagisterska - czujnik Si7021
by Zaeus
*/

#include <Wire.h>
#include <Si7021.h>
 
#define Si7021_POWER 0       // Pin zasilania +3,3V
#define Si7021_GROUND 3      // PIN masy 0V

SI7021 si7021;
bool isStartSignalReceived = false; // Flaga otrzymania komendy rozpoczęcia pracy
const String START_CMD = "START_CMD"; // Komenda rozpoczęcia pracy
const String STOP_CMD = "STOP_CMD"; // Komenda zakończenia pracy

void setup() {
  // Pin napięcia zasilania +Vcc do Si7021
  pinMode(Si7021_POWER, OUTPUT);  
  digitalWrite(Si7021_POWER, HIGH);
  // Pin masy GND
  pinMode(Si7021_GROUND, OUTPUT); 
  digitalWrite(Si7021_GROUND ,LOW);
  // Dla Teensy 3.1 pin 18 to SDA (I2C), zaś pin 19 to SCL (I2C) - biblioteka Wire
  
  // Otworzenie portu szeregowego (115200 bps)
  Serial.begin(115200);
  
  // Inicjalizacja sensoru Si7021 i ustawienie liczby bitów
  si7021.begin();
  si7021.setTempRes(14); // Rozdzielczość temperatury - 14 bitów
  // Dostępne rozdzielczości:
  // 14-bit Temp <-> 12-bit Humidity
  // 13-bit Temp <-> 10-bit Humidity
  // 12-bit Temp <->  8-bit Humidity
  // 11-bit Temp <-> 11-bit Humidity
}

void loop() {
  // Utworzenie zmiennej statycznej do kontrolowania podgrzewacza
  static uint8_t heaterOnOff; // Create static variable for heater control
  
  // Wykonywanie funkcji po otrzymaniu sygnału rozpoczęcia i przed otrzymaniem sygnału zakończenia
  if (isStartSignalReceived) {
    for(int i = (heaterOnOff ? 20 : 30); i > 0; i--) {
      Serial.println(si7021.readTemp(), 6);
      delay(500);
    }
    
    heaterOnOff = !heaterOnOff;
  }
  else {
    delay(500);
  }
}

// Zdarzenie przyjścia sygnału przez port szeregowy
void serialEvent() {
  while (Serial.available()) {
    if (!isStartSignalReceived && Serial.available() > 0) {
      String message = Serial.readString();
      if (START_CMD == message) {
        isStartSignalReceived = true;
        Serial.println("Si7021");
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
