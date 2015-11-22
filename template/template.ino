/*
 * sketchName - Sketch description
 *
 * By Chris "Sembazuru" Elliott, SembazuruCDE (at) GMail.com
 * date
 */

#define DEBUG
#include "DebugUtils.h"
// Add any requred #include lines here for external libraries.

// Define any desired global scope constants and variables.
// (Global constants and variables are usually intialized to zero when defined.)

void setup()
{
  Serial.begin(250000); // Change this to whatever your like running your Serial Monitor at.
  while (!Serial); // Wait for serial port to connect. Needed for Leonardo only.
  delay(1000); // Simply to allow time for the ERW versions of the IDE time to automagically open the Serial Monitor. 1 second chosen arbitrarily.
  Serial.print(F("Sketch Name: "));
  Serial.println(F(__FILE__));  // so we know what sketch is running
  Serial.println();
#ifdef DEBUG // If project doens't normally use the serial port, move this ifdef line to before the Serial.begin.
  Serial.println(F("Debug line format-> milliseconds: __PRETTY_FUNCTION__ __FILE__:__LINE__ message"));
#endif
  DEBUG_PRINT(F("Debug is on"));
  
  // Put your setup code here, to run once:

}

void loop()
{
  // Put your main code here, to run repeatedly:

}


