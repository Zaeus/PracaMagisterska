/* 
*   Valid for Teensy 3.0 and 3.1, not LC
*/
// Dallas DS18B20 Temperature sensor

#include <ADC.h>

const int readPin = A2; // ADC1
int value2;
ADC *adc = new ADC(); // adc object;

void setup() {

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(readPin, INPUT);
    Serial.begin(9600);
    Serial.println("Begin setup");
    
    ////// ADC1 ////
    adc->setAveraging(32, ADC_1); // set number of averages
    adc->setResolution(12, ADC_1); // set bits of resolution
    adc->setConversionSpeed(ADC_LOW_SPEED, ADC_1); // change the conversion speed
    adc->setSamplingSpeed(ADC_LOW_SPEED, ADC_1); // change the sampling speed
    adc->enableInterrupts(ADC_1);
    adc->analogRead(readPin, ADC_1); // call this to setup everything before the pdb starts
    adc->enableCompare(adc->getMaxValue(ADC_1), 0, ADC_1);

    adc->adc1->stopPDB();
    adc->adc1->startPDB(10); //frequency in Hz
            
    Serial.println("End setup");
}


void loop() {}

void adc1_isr() {
  value2 = (uint16_t)adc->analogReadContinuous(ADC_1); // the unsigned is necessary for 16 bits, otherwise values larger than 3.3/2 V are negative!
  //Serial.print("Value ADC1: ");
  //Serial.println(value2*3.3/adc->getMaxValue(ADC_1), DEC);
  //Serial.print(" Value stopnie!: ");
  Serial.println(value2*3.3/adc->getMaxValue(ADC_1)*100);
}

// pdb interrupt is enabled in case you need it.
void pdb_isr(void) {
  PDB0_SC &=~PDB_SC_PDBIF; // clear interrupt
  //digitalWriteFast(LED_BUILTIN, !digitalReadFast(LED_BUILTIN) );
}
