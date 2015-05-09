/*
 * Test how Serial.write handles larger than byte datatypes
 */

byte oneByte = 0x41; // ASCII "A"
word twoByte = 0x4243; // ASCII "BC"
byte threeByteArray[] = { // ASCII "D", "E", "F"
  0x44, 0x45, 0x46};

char oneChar = 0x47; // ASCII "G"
int twoChar = 0x4849; // ASCII "HI"
char threeCharArray[] = { // ASCII "J", "K", "L"
  0x4A, 0x4B, 0x4C}; // Syntactically the same as 'char threeCharArray[] = "JKL"'

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) { // Wait for serial port to connect. Needed for Leonardo only.
  }
  delay(1000); // Simply to allow time for the ERW versions of the IDE time to automagically open the Serial Monitor. 1 second chosen arbitrarily.
  Serial.println(F("TestingSerial_write.ino"));  // so we know what sketch is running
  Serial.println();
  Serial.println(F("Using Serial.write()"));
  Serial.println(Serial.write(oneByte)); // Wanted output "A1", on UNO got "A1".
  Serial.println(Serial.write(twoByte)); // Wanted output "BC2", on UNO got "C1". Seems Serial.write will only output the LSB of a word (and presumably larger)
  Serial.println(Serial.write(threeByteArray,3)); // Wanted output "DEF3", on UNO got "DEF3".
  Serial.println(Serial.write(oneChar)); // Wanted output "G1", on UNO got "G1".
  Serial.println(Serial.write(twoChar)); // Wanted output "HI2", on UNO got "I1". Seems Serial.write will only output the LSB of an int (and presumably larger)
  Serial.println(Serial.write(threeCharArray)); // Wanted output "JKL3", on UNO got "JKL3".

  Serial.println(F("Using Serial.print()"));
  Serial.println(Serial.print(oneByte)); // Wanted output "A1", on UNO got "652". Serial.print converts the byte datatype value 0x41 to ASCII encoded decimal 65.
  Serial.println(Serial.print(twoByte)); // Wanted output "BC2", on UNO got "169635" Serial.print converts the word datatype value 0x4243 to ASCII encoded decimal 16963.
  //  Serial.println(Serial.print(threeByteArray)); // Wanted output "DEF3", compiler chokes on array with "call of overloaded 'print(byte [3])' is ambiguous" so commented out
  Serial.println(Serial.print(oneChar)); // Wanted output "G1", on UNO got "G1". Serial.print sees the datatype char and handles as an ASCII character.
  Serial.println(Serial.print(twoChar)); // Wanted output "HI2", on UNO got "185055". Serial.print converts the int datatype value 0x4849 to ASCII encoded decimal 18505.
  Serial.println(Serial.print(threeCharArray)); // Wanted output "JKL3", on UNO got "JKL3". Serial.print sees the datatype char* and handles as a ASCII characters.

}

void loop()
{
  // put your main code here, to run repeatedly:

}



