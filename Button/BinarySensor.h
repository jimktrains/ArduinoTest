#ifndef __BINARYSENSOR_H__
#define __BINARYSENSOR_H__

#include "RingFIR.h"
#include "Arduino.h"

class BinarySensor
{
  int pinNumber;
  RingFIR<10, unsigned char> rfir;
  
  public:
    BinarySensor(int pinNumber);
    void run();
    unsigned char value();
};

#endif