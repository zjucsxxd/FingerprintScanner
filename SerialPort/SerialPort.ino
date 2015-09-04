#include <SoftwareSerial.h>

SoftwareSerial FingerprintSerial(4, 5); // Arduino RX (Fingerprint TX), Arduino TX (Fingerprint RX)

void setup() 
{
  Serial.begin(9600);    
  FingerprintSerial.begin(9600); 
}

byte data = 0; 

void loop() 
{
  while (Serial.available()) 
  {
    data = Serial.read();
    FingerprintSerial.write(data);
  }
  while (FingerprintSerial.available()) 
  {
    data = FingerprintSerial.read();
    Serial.write(data);
  }
}
