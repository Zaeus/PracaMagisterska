/* PracaMagisterska - czujnik MCP9808
by Zaeus
*/

#include <Wire.h>
#include "Adafruit_MCP9808.h"
 
#define DHT_POWER 0       // Pin zasilania +3,3V
#define DHT_GROUND 3      // PIN masy 0V

Adafruit_MCP9808 sensorMCP9808 = Adafruit_MCP9808();
bool isStartSignalReceived = false; // Flaga otrzymania komendy rozpoczęcia pracy
const String START_CMD = "START_CMD"; // Komenda rozpoczęcia pracy
const String STOP_CMD = "STOP_CMD"; // Komenda zakończenia pracy

bool sensorStatus = false;

void setup() {
  // Pin napięcia zasilania +Vcc do MCP9808
  pinMode(DHT_POWER, OUTPUT);  
  digitalWrite(DHT_POWER, HIGH);
  // Pin masy GND
  pinMode(DHT_GROUND, OUTPUT); 
  digitalWrite(DHT_GROUND ,LOW);
  // Dla Teensy 3.1 pin 18 to SDA (I2C), zaś pin 19 to SCL (I2C) - biblioteka Wire
  
  // Otworzenie portu szeregowego (115200 bps)
  Serial.begin(115200);
  
  // Inicjalizacja sensoru 
  sensorStatus = sensorMCP9808.begin();
}

void loop() {  
  // Wykonywanie funkcji po otrzymaniu sygnału rozpoczęcia i przed otrzymaniem sygnału zakończenia
  if (isStartSignalReceived) {
    // Włączenie czujnika
    sensorMCP9808.shutdown_wake(0);
    
    float tempC = sensorMCP9808.readTempC();
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

// Zdarzenie przyjścia sygnału przez port szeregowy
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
