// Hardware
const byte fuelSensorPin = A0; // Pin the fuel sensor signal is connected to, change to match your reality
const byte ledBargraph[] = // an array of pin numbers to which LEDs are attached
{
  5, 6, 7, 8, 9
};
const byte bargraphCount = sizeof(ledBargraph) / sizeof(ledBargraph[0]); // automagically change the count of LED, and only do the calculation once.
const byte ledIndicator = 13; // even if you don't wire this to an external LED, it should help with troubleshooting to have the on-board LED do something useful.

// Input variables
// None. The input is summed directly to averageSum.

// Output variables
byte bargraphDisplay; // Can get away with byte because the value will never get near the max value of 255

// Processing variables
unsigned long averageStart; // Timer for collecting readings for the average. All timers are unsigned longs because that is the output datatype of millis()
unsigned long displayInterval = 30000; // number of milliseconds between calculate average and display output
unsigned long averageSum = 0; // not sure if we will need this size, but the continuous additions will add up quickly.
unsigned long averageCount = 0; // found an unsigned int here will overflow and give an average too high. Have to use an unsigned long
unsigned int finalAverage; // for calculating the final average into. Probably could instead be a local variable to the display function instead of this global...
unsigned long indicatorStart; // Timer for blinking the indicator led
unsigned long indicatorInterval = 250; // number of milliseconds to keep the indicator led on. Quarter second chosen arbitrarily
boolean indicatorFlag = false; // a flag to help keep track of the indicator status.
const unsigned int sensorHigh = 1024; // High and low ranges for the expected sensor inputs in analog input counts. Modify these here once we know the actual values.
const unsigned int sensorLow = 0;

void setup()
{
  // Set pins for LEDs as digital outputs.
  for (int thisLed = 0; thisLed < bargraphCount; thisLed++)
  {
    pinMode(ledBargraph[thisLed], OUTPUT);
  }
  pinMode(ledIndicator, OUTPUT);
  // Start up the serial port for diagnostic outputs
  Serial.begin(115200); // Change this to whatever your like running your Serial Monitor at.
  while (!Serial); // Wait for serial port to connect. Needed for Leonardo only. I include this for anyone who takes my code and tries to use it on a Leonardo.
  Serial.println(F("Fuel_Gauge_signal_smoothing_with_arduino.ino"));  // so we know what sketch is running
  Serial.println();
  digitalWrite(ledIndicator, HIGH);
  indicatorFlag = true;
  // Start the timer for counting averages
  averageStart = millis();
}

void loop()
{
  readSensor(); // Always want to take a reading
  if(millis() - averageStart > displayInterval) // Check to see if it is time to update the display
  {
    updateDisplay();
  }
  if (indicatorFlag) // check to see if the indicator is on, if not don't waste the clock cycles doing the math with mills()
  {
    if (millis() - indicatorStart > indicatorInterval)
    {
      digitalWrite(ledIndicator, LOW); // LED indicator timed out so turn it off
      indicatorFlag = false;
    }
  }
}

void readSensor()
{
  // Here we need to read the sensor and add the value to the continuing to increasing averageSum variable. Also increment the averageCount variable.
  averageSum = averageSum + analogRead(fuelSensorPin); // Instead of reading analogRead() to a variable (I had defined fuelSensor for this) I'm using it directly.
  // averageSum += analogRead(fuelSensorPin); // Alternate syntax for the proceeding line. This may actually result in a slightly smaller binary, unless the compiler is really good then both would be the same.
  averageCount++; // this is a short cut syntax for averageCount = averageCount + 1;
}

void updateDisplay()
{
  // well we do more than just update the display.
  // We need to
  // * actually calculate the final average
  finalAverage = averageSum / averageCount;
  // * use the map() function to change it to a number of LEDs to light
  bargraphDisplay = map(finalAverage, sensorLow, sensorHigh - ((sensorHigh - sensorLow) / (bargraphCount + 1)), 0, bargraphCount); // Stealing the code from the original sample, using our variables
  // * update the bargraph display

////// See if you can come up with this. It should be basically directly from the original sample, but updated with the variable names we are using.

  // * turn on the indicator LED
  updateDiagnostics();
  digitalWrite(ledIndicator, HIGH);
  // * set the indicatorLED flag True.
  indicatorFlag = true;
  // * set the indicatorStart variable to the current mills()
  indicatorStart = millis();
  // * reset the averageSum and averageCount variables back to zero
  averageSum = 0;
  averageCount = 0;
  // * set the averageStart variable to the current mills()
  averageStart = millis();
}

void updateDiagnostics()
{
  static long updateCount; // Static so this variable is persistent so the value isn't lost each time this function is called.
  updateCount++;
  static long runningCount; // Static so this variable is persistent so the value isn't lost each time this function is called.
  runningCount += averageCount;
  float rpsUpdate = averageCount / ((millis() - averageStart) / 1000.0);
  float rpsStart = runningCount / (millis() / 1000.0);
  Serial.println(F("Average calculations:")); // Using the F() macro is just a habit I've gotten into to not waste valuable SRAM with diagnostic character strings...
  Serial.print(F("\tAccumulated sum of analogRead() = ")); // the \t inside the string is a tab character
  Serial.println(averageSum);
  Serial.print(F("\tNumber of analogRead()s = "));
  Serial.println(averageCount);
  Serial.print(F("\tCalculated average = "));
  Serial.println(finalAverage);
  Serial.println();
  Serial.print(F("Number of LEDs to light up = "));
  Serial.print(bargraphDisplay);
  Serial.print(F(" of "));
  Serial.println(bargraphCount);
  Serial.println();
  Serial.print(F("Update interval in seconds = "));
  Serial.println((displayInterval / 1000.0),3);
  Serial.print(F("Number of reads since start = "));
  Serial.println(updateCount);
  Serial.print(F("Uptime in seconds =\t"));
  Serial.println((millis() / 1000.0),3);
  Serial.print(F("Uptime in minutes =\t"));
  Serial.println((millis() / 60000.0),4);
  Serial.print(F("Uptime in hours =\t"));
  Serial.println((millis() / 3600000.0),5);
  Serial.print(F("Number of reads per second since last update = "));
  Serial.println(rpsUpdate);
  Serial.print(F("Number of reads per second since start = "));
  Serial.println(rpsStart);
  Serial.println();
  Serial.println(F(" -*-*-*-*-*-*-*-*-*-*-*-*-*-"));
  Serial.println();
}


