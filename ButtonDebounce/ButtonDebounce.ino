/*==========================================================================
 • get stable readings on any number of input pins in buttons[] array
 • non blocking, no interrupts
 • typical response in 5-10 ms (low latency)
 • filters random noise to prevent false triggering
 • detects rising and falling edges on normally high or normally low signals
 • use on push buttons, switches, relays or digital inputs

 buttonState:  ______|________|_|_|_|‾‾|‾‾‾‾‾‾‾‾|_|_|_|____________|_|________

 buttonRead:   _______________________|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|________________

 buttonState:  ‾‾‾‾|‾‾‾‾‾‾‾‾‾‾‾‾|‾|‾|‾|__|_________|_|‾‾|‾|‾|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|‾‾‾

 buttonRead:   ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|___________________|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾

 ====================================================================dlloyd*/

// constants
const byte buttons[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11}; // inputs
const byte qty = sizeof(buttons);
const byte led = 13;

// variables
byte buttonState[qty];
byte buttonRead[qty];
unsigned int millisStart;

void setup()
{
  pinMode(led, OUTPUT);
  for (int i = 0; i < qty; i++)
  {
    pinMode(buttons[i], INPUT_PULLUP);
    buttonState[i] = 0xFF;
    buttonRead[i] = 1;
  }
}

void loop()
{
  buttonFilter();
  // your code starts here
  digitalWrite(led, buttonRead[0]); // test stable reading of first button mapped to led
}

void buttonFilter(void)
{
  if (millis() - millisStart >= 2)
  {
    for (int i = 0; i < qty; i++)
    {
      buttonState[i] = (buttonState[i] << 1) | digitalRead(buttons[i]); // shift and read
      if ((buttonState[i] & B11111) == B01111)  // if rising and high for 3 stable reads
      {
        buttonRead[i] = 1;
      }
      if ((buttonState[i] & B11111) == B10000)  // if falling and low for 3 stable reads
      {
        buttonRead[i] = 0;
      }
    }
    millisStart = millis();
  }
}


