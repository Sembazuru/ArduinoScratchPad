/*
 * Test of using a pointer array to directly access different bytes of a multi-byte datatype.
 * Also concurrently show using a union to do the same thing.
 */

void setup()
{
  Serial.begin(115200); // Change this to whatever your like running your Serial Monitor at.
  while (!Serial) { // Wait for serial port to connect. Needed for Leonardo only.
  }
  delay(1000); // Simply to allow time for the ERW versions of the IDE time to automagically open the Serial Monitor. 1 second chosen arbitrarily.
  // put your setup code here, to run once:
  Serial.println(F("Test of using a pointer array and a union to directly access different bytes of a multi-byte datatype."));
  Serial.println();
  Serial.println();

  // Here is the union. Remember that each part of the union access the same memory location.
  // See http://www.cplusplus.com/doc/tutorial/other_data_types/ for reference.
  union optional_name
  {
    byte ub[2];
    unsigned int ui;
    // } integerUnion; // If only one variable uses this union, one can skip the optional name and simply define the variable here as part of the union declaration.
  };
  union optional_name integerUnion; // Or define one or more variables after the union is declared.

  unsigned int integerValue = 0xD5AA; // Both bytes have MSb == 1 to trap accidental sign issues.
  integerUnion.ui = integerValue;

  Serial.print(F("0x")); // using the F() macro is just a habit I've gotten into to not waste valuable SRAM with diagnostic character strings...
  Serial.print(integerValue,HEX); // the HEX keyword converts to the ASCII representation of the hex value
  Serial.println(F(" == the starting integer value in Hex."));

  // Here is the magic for the array of pointers to access the individual bytes of integerValue
  byte* integerPointer; // Setup the pointer variable. Datatype "byte*" means it is a pointer datatype pointing to the location of a "byte" sized variable
  integerPointer = (byte*)&integerValue; // make the value of the pointer the address of the first byte of our integer

  Serial.print(F("0x"));
  Serial.print(integerPointer[0],HEX);
  Serial.println(F(" == the first byte of the integer value in Hex."));

  Serial.print(F("0x"));
  Serial.print(integerPointer[1],HEX);
  Serial.println(F(" == the second byte of the integer value in Hex."));

  Serial.print(F("0x"));
  Serial.print(integerUnion.ub[0],HEX);
  Serial.println(F(" == the first byte of the integer union in Hex."));

  Serial.print(F("0x"));
  Serial.print(integerUnion.ub[1],HEX);
  Serial.println(F(" == the second byte of the integer union in Hex."));


  Serial.println();
  Serial.println(F("Lets change the integer value to 0x4546 using the pointers and the union."));

  integerPointer[0] = 0x46; // Watch this. This order is correct for "little-endian" architectures where the right-most byte is stored first
  integerPointer[1] = 0x45; // See http://en.wikipedia.org/wiki/Endianness for big vs little endian

  integerUnion.ub[0] = integerPointer[0]; // Make the same changes to integerUnion.
  integerUnion.ub[1] = integerPointer[1];

  Serial.print(F("0x"));
  Serial.print(integerValue,HEX);
  Serial.println(F(" == the final integer value in Hex."));

  Serial.print(F("0x"));
  Serial.print(integerUnion.ui,HEX);
  Serial.println(F(" == the final integer union in Hex."));

}

void loop()
{
  // put your main code here, to run repeatedly:

}


