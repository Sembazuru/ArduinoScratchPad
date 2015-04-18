/*
 * Chronodot_on_AdafruitTFT - Sample sketch to read the time from a chronodot
 *   and display the time using the Adafruit libraries for ILI9341.
 *   Use the SQW output of the chronodot on an interrupt pin to trigger updating
 *   the time on the display.
 *
 * By Chris "Sembazuru" Elliott, SembazuruCDE (at) GMail.com
 * 2015-04-18
 *
 * Added random positioning for "screen saver" effect. - 2015-03-19
 *
 * Added set clock mode. Taken and modified from the set clock example in the chronodot library which is GPL 2011 by Radek Wierzbicki
 * Set clock mode activated by booting up with D3 held to GND, and uses the Serial Monitor to set the new time. - 2015-04-18
 *
 */

// Add any requred #include lines here for external libraries.
#include <Wire.h>
#include "Chronodot.h"
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

// Define any desired global scope constants and variables.
// (Global constants and variables are usually intialized to zero when defined.)

// create global chronodot instance
Chronodot chronodot = Chronodot();

// remember last square wave state
const byte SQWinterrupt = 0; // Chronodot 1 second SQW has falling edge on start of the second. Use as falling edge interrupt.
const byte SQWpin = 2; // Interrupt 0 is pin 2 on Uno
volatile boolean TimeTriggered = false; // Flag to act on when time is triggered.

// Week Day Name table with padding to 9 characters wide.
const char* weekDayName[] =
{
  "Sunday   ",
  "Monday   ",
  "Tuesday  ",
  "Wednesday",
  "Thursday ",
  "Friday   ",
  "Saturday "
};

// For the Adafruit TFT shield, these are the default.
const byte TFT_DC = 9;
const byte TFT_CS = 10;

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Pin to check at start for clock set mode or regular mode
const byte setClockPin = 3;

void setup()
{
  // Put your setup code here, to run once:
  chronodot.setSQW(1); // Set square wave pin output of 1Hz
  pinMode(SQWpin, INPUT_PULLUP); // Don't forget to set the pinmode of the interrupt pin.
  attachInterrupt(SQWinterrupt, TrigTime, FALLING);

  // Initialize TFT display and clear it to a black screen.
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);

  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));

  chronodot.readTimeDate(); // Update the chronodot object with the current time in the chronodot device.
  updateTime(); // Show the time on the display.

  // Check the state of the set pin, and if low (active) then go to the clock set mode loop instead of the main loop()
  pinMode(setClockPin, INPUT_PULLUP);
  if (!digitalRead(setClockPin))
  {
    setClockMode();
  }
}

void loop()
{
  // Put your main code here, to run repeatedly:
  if (TimeTriggered)
  {
    TimeTriggered = false; // Clear the triggered flag so this if statement won't execute again until an interrupt.
    chronodot.readTimeDate(); // Update the chronodot object with the current time in the chronodot device.
    updateTime(); // Show the time on the display.
  }
}

void TrigTime() // ISR to set the triggered flag on an interrupt.
{
  TimeTriggered = true;
}

void updateTime()
{
  static byte xPos; // Static datatype so this local variable is remembered when this function is re-entered again to blank the characters printed during this iteration.
  static byte yPos; // Static datatype so this local variable is remembered when this function is re-entered again to blank the characters printed during this iteration.
  byte textWidth = 10; // Number of characters wide
  byte textHeight = 3; // Number of lines high
  byte scaler = 2; // Adjust this to adjust the character size.
  byte x = scaler * 6; // Character width (5) plus one separator space
  byte y = scaler * 9; // Character height (8) plus one separator space
  tft.fillRect(xPos, yPos, x * textWidth, y * textHeight, ILI9341_BLACK); // Clear just the time text area

  // Get a new cursor position.
  xPos = random(x, tft.width() - (x * textWidth + x)); // random number between x (1 character margin) and display width - (width of text block plus 1 character margin).
  yPos = random(y, tft.height() - (y * textHeight + y)); // random number between y and display height - (height of text block plus 1 character margin).

  tft.setCursor(xPos, yPos); // First line.
  tft.setTextSize(scaler);
  tft.setTextColor(ILI9341_YELLOW);
  tft.print(chronodot.timeDate.year);
  tft.print('-');
  printPadded(chronodot.timeDate.month);
  tft.print('-');
  printPadded(chronodot.timeDate.day);

  tft.setCursor(xPos, yPos + y); // Second line.
  tft.print(weekDayName[chronodot.timeDate.weekDay - 1]); // Chronodot reports weekDay number as [1...7], but lookup table is indexed [0...6].

  tft.setCursor(xPos, yPos + (y * 2)); // Third line.
  printPadded(chronodot.timeDate.hours);
  tft.print(':');
  printPadded(chronodot.timeDate.minutes);
  tft.print(':');
  printPadded(chronodot.timeDate.seconds);
}

void printPadded(int num) // Left-pad single digits with a zero.
{
  if (num < 10)
  {
    tft.print('0');
  }
  tft.print(num);
}

void setClockMode()
{
  // Display notification for setting the clock on the screen
  tft.setCursor(6, 0);
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_ORANGE);
  tft.print(F("Use Serial Monitor to set time & date."));

  // Setup serial port
  Serial.begin(115200); // Change this to whatever your like running your Serial Monitor at.
  while (!Serial); // Wait for serial port to connect. Needed for Leonardo only.
  delay(1000); // Simply to allow time for the ERW versions of the IDE time to automagically open the Serial Monitor. 1 second chosen arbitrarily.

  // Use the following forever loop instead of loop()
  while (1)
  {
    if (Serial.available())
    {
      // try to set the date and time if anything shows up on the serial line
      timeDateElements tE;
      processSyncMessage(tE); // won't actually process anything if the serial buffer doesn't have enough characters for a full message
      if (tE.month > 0) // probably could be any of the timeDate elements that has 0 as an unacceptable value, i.e. month, day, or weekDay.
      {
        chronodot.setTimeDate(tE);
      }
    }
    if (TimeTriggered)
    {
      TimeTriggered = false; // Clear the triggered flag so this if statement won't execute again until an interrupt.
      chronodot.readTimeDate(); // Update the chronodot object with the current time in the chronodot device.
      updateTime(); // Show the time on the display.

      Serial.print(chronodot.timeDate.year);
      Serial.print('-');
      SerialPrintPadded(chronodot.timeDate.month);
      Serial.print('-');
      SerialPrintPadded(chronodot.timeDate.day);
      Serial.print(' ');
      Serial.print(weekDayName[chronodot.timeDate.weekDay - 1]); // Chronodot reports weekDay number as [1...7], but lookup table is indexed [0...6].print(weekDayName[chronodot.timeDate.weekDay - 1]);
      Serial.print(' ');
      SerialPrintPadded(chronodot.timeDate.hours);
      Serial.print(':');
      SerialPrintPadded(chronodot.timeDate.minutes);
      Serial.print(':');
      SerialPrintPadded(chronodot.timeDate.seconds);
      Serial.println(F(" Enter new date and time as 'TYYYYMMDDWhhmmss' where W=1 is Sunday, W=2 is Monday, etc."));
    }
  }
}

void SerialPrintPadded(int num) // Left-pad single digits with a zero.
{
  if (num < 10)
  {
    Serial.print('0');
  }
  Serial.print(num);
}


// get the date and time from serial in the following format
// TYYYYMMDDWhhmmss
// where W is the day of the week with 1=Sunday, 2=Monday, etc.
void processSyncMessage(timeDateElements &tE)
{
  tE.seconds = 0;
  tE.minutes = 0;
  tE.hours   = 0;
  tE.weekDay = 0;
  tE.day     = 0;
  tE.month   = 0;
  tE.year    = 0;

  while (Serial.available() >= 16)
  {
    char c = Serial.read();
    if (c == 'T')
    {
      int d1000, d100, d10, d01;

      d1000 = Serial.read() - '0';
      d100 = Serial.read() - '0';
      d10 = Serial.read() - '0';
      d01 = Serial.read() - '0';
      tE.year = (d1000 * 1000) + (d100 * 100) + (d10 * 10) + d01;

      d10 = Serial.read() - '0';
      d01 = Serial.read() - '0';
      tE.month = (d10 * 10) + d01;

      d10 = Serial.read() - '0';
      d01 = Serial.read() - '0';
      tE.day = (d10 * 10) + d01;

      tE.weekDay = Serial.read() - '0';

      d10 = Serial.read() - '0';
      d01 = Serial.read() - '0';
      tE.hours = (d10 * 10) + d01;

      d10 = Serial.read() - '0';
      d01 = Serial.read() - '0';
      tE.minutes = (d10 * 10) + d01;

      d10 = Serial.read() - '0';
      d01 = Serial.read() - '0';
      tE.seconds = (d10 * 10) + d01;
    }
  }
}

