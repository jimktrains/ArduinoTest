#ifndef __BLINK__H__
#define __BLINK__H__

#include "Arduino.h"

/**
 * Blinks an LED for a certain length of time
 */
class Blink
{
    unsigned int period;
    unsigned int duty_period;
    unsigned int off_period;
    unsigned long next_flip;
    int state;
    int ledPin;
    unsigned int blinks;
    
    
    public:
      /**
       * Defined the led to blink, the period, and duty period
       */
      Blink(int ledPin, unsigned int period, unsigned int duty_period);

      /**
       * Blinks the LED for a given number of times.
       * 
       * Will begin blinking at the next run() invocation
       */
      void blink(unsigned int times);

      /**
       * Updates the status of the LED
       */
      void run();
};

#endif
