#include "BinarySensor.h"

BinarySensor::BinarySensor(int pinNumber):
  pinNumber(pinNumber) 
{
  pinMode(pinNumber, INPUT);
  rfir = RingFIR<10, unsigned char>();
}
  
void BinarySensor::run()
{
  unsigned char sensorState = ! digitalRead(pinNumber);
  rfir.add(sensorState);
}

unsigned char BinarySensor::value()
{
    return rfir.value();
}
