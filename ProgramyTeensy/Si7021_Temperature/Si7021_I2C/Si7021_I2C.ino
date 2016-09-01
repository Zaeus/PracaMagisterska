/* PracaMagisterska
by Zaeus
*/

#include "SparkFun_Si7021_Breakout_Library.h"
#include <Wire.h>

int power = A3;
int GND = A2;

Weather sensor;

bool isStartSignalReceived = false; // Flaga otrzymania komendy rozpoczęcia pracy
bool isStopSignalReceived = false; // Flaga otrzymania komendy zakończenia pracy
const String START_CMD = "START_CMD"; // Komenda rozpoczęcia pracy
const String STOP_CMD = "STOP_CMD"; // Komenda zakończenia pracy

void setup() {
  // Otworzenie portu szeregowego (9600 bps)
  Serial.begin(9600);
  pinMode(power, OUTPUT);
  pinMode(GND, OUTPUT);
  digitalWrite(power, HIGH);
  digitalWrite(GND, LOW);
  //Initialize the I2C sensors and ping them
  sensor.begin();
}

void loop() {
  // Wykonywanie funkcji po otrzymaniu sygnału rozpoczęcia i przed otrzymaniem sygnału zakończenia
  if (isStartSignalReceived && !isStopSignalReceived) {
    float tempC = sensor.getTempC();
    
    // Wysłanie temperatury w przypadku pomyślnego uzyskania wartości
    if (isnan(tempC)) {
      Serial.println("Blad odczytu danych z czujnika");
    } 
    else {
      Serial.println(tempC);
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
        Serial.println("Sensor X detection started");
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
