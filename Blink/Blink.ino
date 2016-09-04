/*
  Blink
 Turns on an LED on for one second, then off for one second, repeatedly.
 
 This example code is in the public domain.
 */
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);     
}

// the loop routine runs over and over again forever:
unsigned char count = 0;
unsigned int delay_val = 0;
boolean flip = false;
void loop() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay_val = count * 100;
  if(flip) {
    delay_val = 1000 - delay_val;
  }
  delay(delay_val);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000 - delay_val);               // wait for a second
  count += 1;
  count = count % 10;
  if (count == 0) {
    flip = !flip;
  }
}
  
