int micVal = 0;
const int micPin = A2;

void setup(){
      Serial.begin(9600);
}
void loop(){
      micVal = analogRead(micPin);
      micVal = constrain(abs(micVal - 512)-25,0,512);
      Serial.println(micVal, DEC);
}
