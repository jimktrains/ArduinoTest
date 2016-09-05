#ifndef __BINARY_SENSOR_READINGS_H__
#define __BINARY_SENSOR_READINGS_H__

#include "Arduino.h"
#include "BinarySensor.h"
#include "BinaryRingFIR.h"

/**
 * Current State of the sensor.
 * 
 * Rising is the transition from Low to High.
 * Falling is the transition from High to Low.
 */

enum BinarySensorState {
  Rising,
  Falling,
  High,
  Low
};

/**
 * Represents a sensor that will read from a binary pin.
 * 
 * The Sensor will transition following
 * 
 * Low -> Rising -> High -> Falling -> Low
 * 
 * Rising and Falling will appear for only a single cycle
 * 
 * This Sensor supports setting seperate debounce periods
 * for rising and falling; i.e. the sensor must be stable
 * in a state for a user-defined amount of amount of time
 * before transitioning from Low to Rising or High to 
 * Falling.
 * 
 * The sensor uses a simple Finite Impulse Response filter
 * with a 10-reading history and uniform weights.
 */
template<unsigned char PHASES>
class BinarySensor
{
  unsigned int bounce[PHASES][2];
  BinarySensorState state[PHASES];
  unsigned long lastChange[PHASES];
  BinaryRingFIR<10> rfir;
  int sensorPin;
  
  public:
    /**
     * Constructs the sensor with the given pin and debounce 
     * periods/phases.
     * 
     * ex:
     * unsigned int bounces[2][2] = {{10, 300}, {10, 3000}};
     * BinarySensor<2> irSensor = BinarySensor<2>(6, bounces);
     * 
     * Would create a sensor with 2 phases:
     * 
     * 10ms L->R and 300ms H->F
     * 10ms L->R and 3s H->F
     */
    BinarySensor(int sensorPin, unsigned int bounce[PHASES][2]);

    /**
     * Update the sensor values.
     */
    void run();

    /*
     * Returns the sensor state for the given phase.
     */
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
