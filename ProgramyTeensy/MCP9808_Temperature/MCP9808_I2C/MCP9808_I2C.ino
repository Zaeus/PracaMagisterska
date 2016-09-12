/* PracaMagisterska - czujnik MCP9808
by Zaeus
*/

#include <Wire.h>
#include <Adafruit_MCP9808.h>
 
// Pin zasilania +3,3V
#define MCP9808_POWER 0
// PIN masy 0V
#define MCP9808_GROUND 3

// Instancja klasy czujnika z biblioteki Adafruit_MCP9808
Adafruit_MCP9808 sensorMCP9808 = Adafruit_MCP9808();
// Flaga otrzymania komendy rozpoczęcia pracy
bool isStartSignalReceived = false;   
// Flaga statusu gotowości sensora
bool sensorStatus = false;
// Komenda rozpoczęcia pracy
const String START_CMD = "START_CMD"; 
// Komenda zakończenia pracy
const String STOP_CMD = "STOP_CMD"; 

void setup() {
  // Ustawienie pinu 0 na pin wyjściowy - zasilania Vcc
  pinMode(MCP9808_POWER, OUTPUT);  
  // Ustawienie stanu pinu 0 na napięcie 3,3V
  digitalWrite(MCP9808_POWER, HIGH);
  // Ustawienie pinu 3 na pin wyjściowy - zasilania GND
  pinMode(MCP9808_GROUND, OUTPUT); 
  // Ustawienie stanu pinu 3 na napięcie 0V
  digitalWrite(MCP9808_GROUND ,LOW);
  // Dla Teensy 3.1 pin 18 to SDA (I2C), zaś pin 19 to SCL (I2C) - biblioteka Wire
  
  // Otworzenie portu szeregowego (115200 bodów)
  Serial.begin(115200);
  
  // Inicjalizacja czujnika MCP9808
  sensorStatus = sensorMCP9808.begin();
}

void loop() {  
  // Wykonywanie funkcji zbierania danych po otrzymaniu sygnału rozpoczęcia START_CMD
  if (isStartSignalReceived) {
    // Włączenie czujnika
    sensorMCP9808.shutdown_wake(0);
    // Odczyt temperatury 
    float tempC = sensorMCP9808.readTempC();
    // Wysłanie temperatury z dokładnością do 6 miejsc po przecinku
    Serial.println(tempC, 6);
    delay(250);
    // Wyłączenie czujnika
    sensorMCP9808.shutdown_wake(1);
    delay(500);
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
        Serial.println("MCP9808");
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
