/*
 * Chronodot_on_AdafruitTFT - Sample sketch to read the time from a chronodot
 *   and display the time using the Adafruit libraries for ILI9341.
 *   Use the SQW output of the chronodot on an interrupt pin to trigger updating
 *   the time on the display.
 *
 * By Chris "Sembazuru" Elliott, SembazuruCDE (at) GMail.com
 * 2015-03-18
 *
 * Added random positioning for "screen saver" effect. - 2015-03-19
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
  tft.print(weekDayName[chronodot.timeDate.weekDay - 1]);

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

