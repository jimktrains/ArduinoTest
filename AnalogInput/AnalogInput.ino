const int ledPin = 8;

const int buttonPin = 7;
const int tempPin = A1;
const int potPin = A0;
const int lightPin = A2;
const int tiltPin = A3;

boolean command_complete = false;

String inputString;

const int TEMP = 1;
const int POT = 0;
const int LIGHT = 2;

unsigned long currentMillis = 0;
unsigned long buttonPress = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  Serial.begin(9600);
}


#define delayCheck(lastTime, d) ((currentMillis - (lastTime)) >= (d))

void loop() {
  static unsigned int selection = 0;
  static unsigned int tempValDelta = 0;

  int tempVal = 0;
  int potVal = 0;
  int lightVal = 0;
  int buttonVal = 0;
  int tiltVal = 0;
  
  int selection_val = 0;

  currentMillis = millis();
  
  tempVal = analogRead(tempPin);
  potVal = analogRead(potPin);
  lightVal = analogRead(lightPin);
  tiltVal = analogRead(tiltPin);
  
  buttonVal = digitalRead(buttonPin);
  
  /*
   * I found that since the changes are so small, the blink freq doesn't previably change.
   * I decided to exagerate the change each second instead.
   */
  tempValDelta = updateTempDelta(tempVal, tempValDelta);

  selection = runCommands(selection);
  selection = handleButton(buttonVal, selection);

  selection_val = getSelectionVal(tempValDelta, potVal, lightVal, selection);
  
  blinkCycle(currentMillis, selection_val, selection_val, ledPin);

  printSensorValues(tempVal, tempValDelta, potVal, lightVal, tiltVal, selection);
}

unsigned int runCommands( unsigned int selection) {
  if (command_complete)
  {
    for(int i = 0; i < inputString.length(); i++)
    {
      switch(inputString[i]) {
        case 'U':
          selection = updateSelection(selection, 1);
          break;
        case 'D':
          selection = updateSelection(selection, -1);
          break;
      }
    }
    inputString = "";
    command_complete = false;
  }
  return selection;
}

unsigned int handleButton( int buttonVal, unsigned int selection) {
  static unsigned long buttonPress = 0;
  if (buttonVal == HIGH && delayCheck(buttonPress, 1000) ) {
    buttonPress = currentMillis;
    return updateSelection(selection, 1);
  }
  return selection;
}

unsigned int updateSelection( unsigned int selection, int delta) {
    delta = delta % 3;
    // Appearently % doesn't only return positive values
    if ( delta < 0 ) {
      delta = 3 + delta;
    }

    selection = delta + selection;
    selection = selection % 3;
    Serial.print("Switching to ");
    Serial.println(selection);
    return selection;
}

unsigned int updateTempDelta(unsigned int tempVal, unsigned int tempValDelta) {
  static unsigned long tempCheck = 0;
  static int tempValOld = 0;
  if (delayCheck(tempCheck, 1000)) {
    tempValDelta = abs(tempVal - tempValOld);
    
    tempValOld = tempVal;
    tempCheck = currentMillis;
  }
  return tempValDelta;
}

int getSelectionVal( int tempValDelta, int potVal, int lightVal, int selection) {
  int selection_val;
  switch(selection) {
    case TEMP:
      selection_val = tempValDelta * 100;
      break;
    case POT:
      selection_val = potVal;
      break;
    case LIGHT:
      selection_val = lightVal;
      break;
    default:
      selection_val = 0;
  }
  return selection_val;
}


void blinkCycle(unsigned long currentMillis, unsigned long on_ms, unsigned long off_ms, int ledPin) {
  static unsigned long previousBlink = 0;
  static boolean light_on = false;
  if (light_on) {
    if ( delayCheck(previousBlink, on_ms) ) {
      digitalWrite(ledPin, LOW);
      previousBlink = currentMillis;
      light_on = false;
    }
  }
  else {
    if ( delayCheck(previousBlink, off_ms) ) {
      digitalWrite(ledPin, HIGH);
      previousBlink = currentMillis;
      light_on = true;
    } 
  }
}



void printSensorValues( int tempVal, unsigned int tempValDelta, int potVal, int lightVal, int tiltVal, int selection) {
  static unsigned long print_delay = 0;
  
  if (! delayCheck(print_delay, 1000) ) {
    return;
  }
  print_delay = currentMillis;
  Serial.print("Temp : ");
  Serial.print(tempVal);
  Serial.print(" (");
  Serial.print(tempValDelta);
  Serial.print(")\nPot : ");
  Serial.print(potVal);
  Serial.print("\nLight: ");
  Serial.print(lightVal);
  Serial.print("\nTilt: ");
  Serial.print(tiltVal);
  
  Serial.print("\nSel: ");
  switch(selection) {
    case TEMP:
      Serial.print("Temp\n");
      break;
    case POT:
      Serial.print("Pot\n");
      break;
    case LIGHT:
      Serial.print("Light\n");
      break;
    default:
      Serial.print("Unknown -- ");
      Serial.print(selection);
  }
  Serial.println();
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read(); 
    inputString += inChar;
    if (inChar == '\n') {
      command_complete = true;
    } 
  }
}
