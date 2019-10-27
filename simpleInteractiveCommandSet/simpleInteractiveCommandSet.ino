/*
  Name: simpleInteractiveCommandSet
  Description: A core starting program that includes a simple interactive command set.
      This assumes your project will want to communicate to the Arduino to set parameters and get information.
        (e.g. Get the hardware information like serial number and firmware revision or number of power cycles)
  Author: Tom Clupper
  License: None
  Revision: 10/22/2019 (ver 1.0)

  This is Example#3 from the class found at this link: https://youtu.be/MEm4goe0QIw
*/

// vvvvvvvvvvvvvvvvvvvvvvvvv  Core variables  vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
#include <EEPROM.h>
 
/* Number of times Arduino has been booted */
int NumPowerCycles = 0;

/* Variable used in serial communication */
char InBuff[10];
int BuffIndex = 0;
char EOL = 13;
bool ProcessCommand = false;
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

/* Define variable for the Red LED output */
int RedLEDPin = 11;
int LEDbrightness = 200;                     // A number between 10 and 250
unsigned long LEDinterval = long(120);       // Number of Milliseconds between changes (between 10 and 250)
unsigned long currentMillis = 0;
unsigned long millisFromLastFlicker = 0;     // Number of milliseconds since LED brighness change
bool FlickerMode = false;


/* ============================================= BEGIN SETUP ================================================== */
void setup() {
/* vvvvvvvvvvvvvvvvvvvvvvvvv  Core setup  vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  // initialize the serial port
  Serial.begin(115200);

  // increment the Number of power-on cycles by one
  EEPROM.get(0, NumPowerCycles);
  NumPowerCycles = NumPowerCycles + 1;
  EEPROM.put(0,NumPowerCycles);

  // Flush the serial buffer before we start
  delay(10);
  Serial.flush();
  delay(10);
/* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */
  
  // [Red LED out]
  pinMode(RedLEDPin, OUTPUT);
  digitalWrite(RedLEDPin, LOW);
}
/* ============================================= END SETUP ====================================================== */


/* ============================================= BEGIN LOOP ===================================================== */
void loop() {
  /* vvvvvvvvvvvvvvvvvvvvvvvvv  Core loop code  vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
  CheckforSerialData();
  ProcessCommands();
  /* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */
  
  currentMillis = millis();
  /* NOTE:  millis returns the number of milliseconds passed since the Arduino board began running the current program. 
            This number will overflow (go back to zero), after approximately 50 days.
            A 'long' is 32 bits (4 bytes). Unlike standard longs unsigned longs won’t store negative numbers,
            making their range from 0 to 4,294,967,295 (2^32 - 1). The subtraction will yield the proper result,
            even when currentMillis overflows, or wraps around.  */
  if ( ((currentMillis - millisFromLastFlicker) >= LEDinterval) && FlickerMode) {
   millisFromLastFlicker = currentMillis;    
   analogWrite(RedLEDPin, (random(LEDbrightness)+255-LEDbrightness)); 
  }
}
/* ============================================= END LOOP ======================================================= */


void CheckforSerialData() {
  // See if there is a character ready from the serial port
  if (Serial.available()>0)  {
    InBuff[BuffIndex] = byte(Serial.read());
     if (InBuff[BuffIndex] == EOL)  {
      BuffIndex = 0;
      ProcessCommand = true;
      delay(1);
      Serial.flush();
    } else {
      BuffIndex = BuffIndex + 1;
      if (BuffIndex > 9) BuffIndex = 0;   // Reset the input buffer (This should never happen!!!)
      ProcessCommand = false;
    }
  }  
}
  

void ProcessCommands() {
  if (ProcessCommand)  {
    ProcessCommand = false;
    switch (InBuff[0]) {
      /* vvvvvvvvvvvvvvvvvvvvvvvvv  Core commands  vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
      case 'F': case 'f':   // Flush the serial buffer (This is essentially a "do-nothing" command).
        Serial.flush();
        delay(10);
        break;
        
      case 'I': case 'i':     // Device Information
        // Manufacture,Model,SerialNumber,FirmwareRevision
        Serial.println("Arduino,simpleInteractiveCommandSet,TC001,1.0");
        break;        
        
      case 'P': case 'p':   // Read the number of Power-on cycles stored in EEprom (Use "P0" to reset)
        if (InBuff[1] == '0') {
          NumPowerCycles = 0;
          EEPROM.put(0,NumPowerCycles);
          Serial.println("OK");
        } else {
          Serial.println(NumPowerCycles);          
        }
        break;       
        /* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */
      case 'A': case 'a':     // usage: A (Return the value of analog input pin0)
        /* This return a number from 0 to 2023 (10 bit A/D converter) that is the voltage on Pin 0 */
        Serial.println(analogRead(0));
        break;  

      case 'B': case 'b':     // usage: Bxxx (e.g. B135 sets max random LED brightness level to 135 (B? to inquire))
        if (InBuff[1] == '?')  {
          Serial.println(LEDbrightness);
        } else  {
          LEDbrightness = (InBuff[1]-48)*100+(InBuff[2]-48)*10+(InBuff[3]-48);
          if (LEDbrightness < 10) LEDbrightness = 10;       // At lease 10
          if (LEDbrightness > 250) LEDbrightness = 250;     // No more than 250         
          Serial.println("OK"); 
        }
        break;  
        
      case 'D': case 'd':     // usage: Dxxx (e.g. D200 sets LED flicker delay at 200 msec (D? to inquire))
        if (InBuff[1] == '?')  {
          Serial.println(LEDinterval);
        } else  {
          LEDinterval = long(InBuff[1]-48)*100+(InBuff[2]-48)*10+(InBuff[3]-48);
          if (LEDinterval < 10) LEDinterval = 10;       // At lease 10
          if (LEDinterval > 250) LEDinterval = 250;     // No more than 250         
          Serial.println("OK"); 
        }
        break;  

      case 'L': case 'l':     // usage: Lx (e.g. L toggles LED on and off, L+ sets to flicker mode)
        if (InBuff[1] == '+')  {
          FlickerMode = true;
          Serial.println("FlickerMode");
        } else {
          FlickerMode = false;
          TogglePin(RedLEDPin);      
          Serial.println("OK"); 
        }
        break;  
           
      default:    //  If a non-recognizable command occurs, output the command back to the Serial port
        bool EndofCommand = false;
        Serial.print(":");
        for (int i=0; i <= 9; i++) {
          if (InBuff[i] == EOL) EndofCommand = true;
          if (!EndofCommand)  {
            Serial.print(InBuff[i]);
          }
        }
        Serial.println(":");        
        break;
    }
  }  
}


void TogglePin(int OutputPin) {
  /*  Toggle Digital IO pin and return the value  */
  if (digitalRead(OutputPin) == 1) {
    digitalWrite(OutputPin,LOW);
  } else {
    digitalWrite(OutputPin,HIGH);
  }
}

    
