/*
 * SerialSoftwareSerial_loopback - This sketch simply connects the hardware Serial device to a SoftwareSerial device
 *  to provide a hardware testbed to check serial communication.
 *
 * By Chris "Sembazuru" Elliott, SembazuruCDE (at) GMail.com
 * 2013-07(JUL)-17
 *
 * Oops. Turns out this is nearly identical to the SoftwareSerialExample provided by the IDE.
 */

// Add any requred #include lines here for external libraries.
#include <SoftwareSerial.h>


// Define any desired global scope constants and variables.
// (Global constants and variables are usually intialized to zero when defined.)
SoftwareSerial sSerial(10, 11); // RX, TX

void setup()
{
  Serial.begin(115200); // Change this to whatever your like running your Serial Monitor at.
  while (!Serial); // Wait for serial port to connect. Needed for Leonardo only.
  delay(1000); // Simply to allow time for the ERW versions of the IDE time to automagically open the Serial Monitor. 1 second chosen arbitrarily.

  // Put your setup code here, to run once:
  sSerial.begin(9600); // Change this to whatever the device attached to SoftwareSerial is configured for.

}

void loop()
{
  // Put your main code here, to run repeatedly:
  if (Serial.available())
  {
    sSerial.write(Serial.read());
  }

  if (sSerial.available())
  {
    Serial.write(sSerial.read());
  }

}


