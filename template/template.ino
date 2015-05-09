/*
 * sketchName - Sketch description
 *
 * By Chris "Sembazuru" Elliott, SembazuruCDE (at) GMail.com
 * date
 */

// Add any requred #include lines here for external libraries.

// Define any desired global scope constants and variables.
// (Global constants and variables are usually intialized to zero when defined.)

void setup()
{
  Serial.begin(115200); // Change this to whatever your like running your Serial Monitor at.
  while (!Serial); // Wait for serial port to connect. Needed for Leonardo only.
  delay(1000); // Simply to allow time for the ERW versions of the IDE time to automagically open the Serial Monitor. 1 second chosen arbitrarily.
  Serial.println(F("sketchName.ino"));  // so we know what sketch is running
  Serial.println();

  // Put your setup code here, to run once:

}

void loop()
{
  // Put your main code here, to run repeatedly:

}


