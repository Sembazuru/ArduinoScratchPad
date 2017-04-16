#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display = Adafruit_SSD1306();

#if defined(ESP8266)
  #define BUTTON_A 0
  #define BUTTON_B 16
  #define BUTTON_C 2
  #define LED      0
#elif defined(ARDUINO_STM32F2_FEATHER)
  #define BUTTON_A PA15
  #define BUTTON_B PC7
  #define BUTTON_C PC5
  #define LED PB5
#elif defined(TEENSYDUINO)
  #define BUTTON_A 4
  #define BUTTON_B 3
  #define BUTTON_C 8
  #define LED 13
#else
  #define BUTTON_A 9
  #define BUTTON_B 6
  #define BUTTON_C 5
  #define LED      13
#endif

#if (SSD1306_LCDHEIGHT != 32)
  #error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

long currentMillis; // For millis() based timeout operations. Set to millis() at the beginning every loop() iteration.

void setup()
{
  Serial.begin(115200);

  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only.
    ; // Remove or comment out for standalone (not computer connected) operation on M0. Otherwise the sketch will hang here.
  }

  Serial.println("OLED FeatherWing test");
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32).
  // init done.
  Serial.println("OLED begun");

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  Serial.println("IO test");

  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);

  // text display tests.
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Connecting to SSID\n'adafruit':");
  display.print("connected!");
  display.println("IP: 10.0.1.23");
  display.println("Sending val #0");
  display.setCursor(0, 0);
  display.display(); // actually display all of the above.
}


void loop()
{
  currentMillis = millis();
  if (! digitalRead(BUTTON_A)) buttonAction('A');
  if (! digitalRead(BUTTON_B)) buttonAction('B');
  if (! digitalRead(BUTTON_C)) buttonAction('C');
  delay(10);
  yield();
}

void buttonAction (char bttn)
{
  static char last_bttn; // Remember last button action to filter out switch bouncing and holding.
  // Timeout last_bttn to allow multiple button presses.
  static long last_bttn_press;
  if ( currentMillis - last_bttn_press >= 250 ) // 250ms (one quarter of a second) to let go of the button.
  {
    last_bttn = 0;
  }
  // Helper flags for demo.
  static boolean inverted = false;
  static boolean dimmed = false;
  // Decode which button was pressed and do something based on which button pressed.
  if (!(last_bttn == bttn))
  {
    Serial.print("Button pressed: ");
    Serial.println(bttn);
    switch (bttn)
    {
      case 'A':
        // Toggle display inverting.
        inverted = !inverted;
        if (inverted)
        {
          display.invertDisplay(inverted);
        }
        else
        {
          display.invertDisplay(inverted);
        }
        break;
      case 'B':
        // Reset both display invert and dim.
        inverted = false;
        dimmed = false;
        display.invertDisplay(inverted);
        display.dim(dimmed);
        break;
      case 'C':
        // Toggle display dimming.
        dimmed = !dimmed;
        if (dimmed)
        {
          display.dim(dimmed);
        }
        else
        {
          display.dim(dimmed);
        }
        break;
      default:
        // if nothing else matches, do the default
        // this default case should never happen so throw an error.
        Serial.println("Unknown button. You should never see this message.");
        break;
    }
    last_bttn = bttn;
    last_bttn_press = currentMillis;
  }
}

