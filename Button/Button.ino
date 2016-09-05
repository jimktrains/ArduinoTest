#include "Blink.h"
#include "BinarySensor.h"

Blink blinker = Blink(8, 500, 250);
Blink fastBlinker = Blink(8, 100, 50);
unsigned int bounces[2][2] = {{10, 300}, {10, 3000}};
BinarySensor<2> irSensor = BinarySensor<2>(6, bounces);


void setup() {
  Serial.begin(9600);
  Serial.println("Begin");
}

typedef unsigned long timer;
typedef unsigned long timer_diff;
timer currentMillis = 0;

#define DEBOUND_DELAY 100
#define END_OF_TRAIN_DELAY 3000

void loop(){
  blinker.run();
  fastBlinker.run();
  irSensor.run();

  static unsigned int carCount = 0;
   
  BinarySensorState carPresent = irSensor.value(0);
  BinarySensorState trainPresent = irSensor.value(1);

  if (carPresent == BinarySensorState::Rising)
  {
    carCount++;
    fastBlinker.blink(1);
    if (carCount == 1)
    {
      Serial.println("New Train!");
    }
    else
    {
      blinker.blink(carCount);
      Serial.print("Car: ");
      Serial.println(carCount);
    }
  }

  if (trainPresent == BinarySensorState::Falling)
  {
    Serial.println("Train Passed :(");
    carCount = 0;
  }
}
