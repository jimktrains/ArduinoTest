#ifndef __BLINK__H__
#define __BLINK__H__

#include "Arduino.h"
#include "Task.h"

class Blink : public Task
{
    unsigned int period;
    unsigned int duty_period;
    unsigned int off_period;
    unsigned long next_flip;
    int state;
    int ledPin;
    unsigned int blinks;
    
    
    public:
      Blink(int ledPin, unsigned int period, unsigned int duty_period);

      void blink(unsigned int times);
      void run();
};

#endif
