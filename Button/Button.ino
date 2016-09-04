#include "Blink.h"
#include "BinarySensor.h"

Blink blinker = Blink(8, 5000, 250);
BinarySensor irSensor = BinarySensor(6);


void setup() {
  Serial.begin(9600);
  
}

typedef unsigned long timer;
typedef unsigned long timer_diff;
timer currentMillis = 0;

#define DEBOUND_DELAY 300
#define END_OF_TRAIN_DELAY 1000

void loop(){
  blinker.run();
  irSensor.run();

  boolean has_changed = false;
  currentMillis = millis();
  static timer lastChange = 0;
  static boolean lastState = false;
  static unsigned int carCounter = 0;
   
  int approxReading = irSensor.value();
  
  timer_diff sinceLastChange = currentMillis - lastChange;
  
  if (sinceLastChange  >= DEBOUND_DELAY)
  {
      if (approxReading != lastState)
      {
        lastState = approxReading;
        lastChange = currentMillis;
        has_changed = true;
      }
      
      if (sinceLastChange >= END_OF_TRAIN_DELAY)
      {
 
        if (carCounter != 0)
        {
          carCounter = 0;
          Serial.println("Train Passed");
          blinker.blink(2);
        }
        if (has_changed && lastState == HIGH)
        {
          carCounter = 1;
          Serial.println("New Train!");
          blinker.blink(3);
        }
      }
      else if(has_changed)
      {
        if (lastState == HIGH)
        {
          carCounter++;
          Serial.print("Car ");
          Serial.println(carCounter);
        }
      }
  }
}
