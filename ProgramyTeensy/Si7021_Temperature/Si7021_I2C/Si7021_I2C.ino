/* PracaMagisterska - czujnik Si7021
by Zaeus
*/

#include <Wire.h>
#include <Si7021.h>

// Pin zasilania +3,3V
#define Si7021_POWER 0
// PIN masy 0V
#define Si7021_GROUND 3

// Instancja klasy czujnika z biblioteki Si7021
SI7021 si7021;
// Flaga otrzymania komendy rozpoczęcia pracy
bool isStartSignalReceived = false;   
// Komenda rozpoczęcia pracy
const String START_CMD = "START_CMD"; 
// Komenda zakończenia pracy
const String STOP_CMD = "STOP_CMD";   

void setup() {  
  // Ustawienie pinu 0 na pin wyjściowy - zasilania Vcc
  pinMode(Si7021_POWER, OUTPUT);
  // Ustawienie stanu pinu 0 na napięcie 3,3V
  digitalWrite(Si7021_POWER, HIGH);
  // Ustawienie pinu 3 na pin wyjściowy - zasilania GND
  pinMode(Si7021_GROUND, OUTPUT);
  // Ustawienie stanu pinu 3 na napięcie 0V
  digitalWrite(Si7021_GROUND ,LOW); 
  // Dla Teensy 3.1 pin 18 to SDA (I2C), zaś pin 19 to SCL (I2C) - biblioteka Wire 
  // Otworzenie portu szeregowego (115200 bodów)
  Serial.begin(115200); 
  // Inicjalizacja sensora Si7021 
  si7021.begin();  
  // Rozdzielczość czujnika temperatury ustawiona na 14 bitów
  si7021.setTempRes(14);  
}

// 
void loop() {
  // Zmienna statyczna do kontrolowania podgrzewacza sensora Si7021
  static uint8_t heaterOnOff; 
  
  // Wykonywanie funkcji zbierania danych po otrzymaniu sygnału rozpoczęcia START_CMD
  if (isStartSignalReceived) {
    for(int i = (heaterOnOff ? 20 : 30); i > 0; i--) {
      // Wysłanie temperatury  z 6 miejscami po przecinku
      Serial.println(si7021.readTemp(), 6);
      delay(500);
    }    
    heaterOnOff = !heaterOnOff;
  }
  else {
    delay(500);
  }
}

// Funkcja obsługi zdarzenia przyjścia informacji przez port szeregowy
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
    else if (isStartSignalReceived && Serial.available() > 0) {
      String message = Serial.readString();
      if (STOP_CMD == message) {
        isStartSignalReceived = false;
        return;
      }
    }
  }
}
