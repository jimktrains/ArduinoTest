#include "Blink.h"

Blink::Blink(int ledPin, unsigned int period, unsigned int duty_period) : 
  period(period), duty_period(duty_period), ledPin(ledPin)
{
    next_flip = 0;
    state = LOW;
    blinks = 0;
    off_period = period - duty_period;
    pinMode(ledPin, OUTPUT);
}

void Blink::blink(unsigned int times)
{
  unsigned long ms = millis();
  blinks = times;
  next_flip = ms + duty_period;
}

void Blink::run() 
{
  unsigned long ms = millis();

  if (next_flip <= ms)
  {
    if (state == HIGH)
    {
      Serial.println("Turning On");
      digitalWrite(ledPin, LOW);
      next_flip = ms + off_period;
      state = LOW;
    }
    else
    {
      Serial.println("Turning Off");
      if (blinks > 0)
      {
        digitalWrite(ledPin, HIGH);
        next_flip = ms + off_period;
        state = HIGH;
        blinks -= 1u;
      }
    }
  }
  
  
}
