/*
 * DDGaoHelp - Test sketches for working with DDGao on forum thread: http://forum.arduino.cc//index.php?topic=194183
 *
 * By Chris "Sembazuru" Elliott, SembazuruCDE (at) GMail.com
 * 2013-10-30
 */

// Add any requred #include lines here for external libraries.

// Define any desired global scope constants and variables.
// (Global constants and variables are usually intialized to zero when defined.)

void setup()
{
  Serial.begin(115200); // Change this to whatever your like running your Serial Monitor at.
  while (!Serial); // Wait for serial port to connect. Needed for Leonardo only.
  delay(1000); // Simply to allow time for the ERW versions of the IDE time to automagically open the Serial Monitor. 1 second chosen arbitrarily.

  unsigned long time;
  time = millis();
  while(millis() - time < 30UL * 60UL * 1000UL){
    int sensorValue = analogRead(A0); //read the analog input on pin 0
    float voltage = sensorValue * (5.00/1023.00);
    Serial.print(voltage,DEC);
    Serial.println(",");
    delay(1000);
  }
}

void loop()
{
  // Put your main code here, to run repeatedly:

}


