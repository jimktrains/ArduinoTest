#ifndef __BINARY_SENSOR_READINGS_H__
#define __BINARY_SENSOR_READINGS_H__

#include "Arduino.h"
#include "BinarySensor.h"
#include "RingFIR.h"
#include "Task.h"

enum BinarySensorState {
  Rising,
  Falling,
  High,
  Low
};

template<unsigned char PHASES>
class BinarySensor : public Task {
  unsigned int bounce[PHASES][2];
  BinarySensorState state[PHASES];
  unsigned long lastChange[PHASES];
  RingFIR<10, unsigned char> rfir;
  int sensorPin;
  
  public:

    BinarySensor(int sensorPin, unsigned int bounce[PHASES][2]);
    void run();
    BinarySensorState value(unsigned char phase);
};

// Can't pass arrays by value
template<unsigned char PHASES>
BinarySensor<PHASES>::BinarySensor(int sensorPin, unsigned int bounce[PHASES][2]) :
 sensorPin(sensorPin)
{
  pinMode(sensorPin, INPUT);
  for(unsigned char i = 0; i < PHASES; i++)
  {
    this->bounce[i][0] = bounce[i][0];
    this->bounce[i][1] = bounce[i][1];
    lastChange[i] = 0;
    state[i] == BinarySensorState::Low;
  }
}

template<unsigned char PHASES>
void BinarySensor<PHASES>::run()
{
  unsigned long currentMillis = millis();
  
  auto curSensorVal = digitalRead(sensorPin);
  rfir.add(curSensorVal);
  
  BinarySensorState sensorBinarySensorState = rfir.value() ? BinarySensorState::High : BinarySensorState::Low;

  for(unsigned char i = 0 ; i < PHASES; i++)
  {
    if (state[i] != sensorBinarySensorState)
    {
      lastChange[i] = currentMillis;
    }
    unsigned long timediff = currentMillis - lastChange[i];
    
    switch(state[i])
    {
      case BinarySensorState::Falling:
        state[i] = BinarySensorState::Low;
        break;
      case BinarySensorState::Rising:
        state[i] = BinarySensorState::High;
        break;
      case BinarySensorState::Low:
        if (timediff >= bounce[i][0])
        {
          state[i] = BinarySensorState::Rising;
        }
        break;
      case BinarySensorState::High:
        if (timediff >= bounce[i][1])
        {
          state[i] = BinarySensorState::Falling;
        }
        break;
    }
  }
}

template<unsigned char PHASES>
BinarySensorState BinarySensor<PHASES>::value(unsigned char phase)
{
  return state[phase];
}

#endif
