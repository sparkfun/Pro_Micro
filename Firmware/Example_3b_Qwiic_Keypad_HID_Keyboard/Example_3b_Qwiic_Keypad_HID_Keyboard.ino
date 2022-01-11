/******************************************************************************
  Example_3b_Qwiic_Keypad_HID_Keyboard.ino
  Written by: Ho Yun "Bobby" Chan
  Date: February 6, 2020
  Development Environment Specifics:
    Arduino IDE 1.8.9

  Description:
    Based on the Jim's Pro Micro "HID Mouse" and Pete' Qwiic Keypad "read button"
    examples, this example outputs keyboard presses associated with the keypad.

  Libraries:
   Keyboard.h (included with Arduino IDE)
   Wire.h (included with Arduino IDE)
   SparkFun_Qwiic_Keypad_Arduino_Library.h (included in the src folder) http://librarymanager/All#SparkFun_keypad

  License:
    This code is released under the MIT License (http://opensource.org/licenses/MIT)

******************************************************************************/

#include <Keyboard.h>
#include <Wire.h>
#include "SparkFun_Qwiic_Keypad_Arduino_Library.h" //Click here to get the library: http://librarymanager/All#SparkFun_keypad
KEYPAD keypad1;                                    //Create instance of this object

void setup() {
  Serial.begin(9600);
  Serial.println("Qwiic KeyPad Example");

  if (keypad1.begin() == false)   // Note, using begin() like this will use default I2C address, 0x4B.
    // You can pass begin() a different address like so: keypad1.begin(Wire, 0x4A).
  {
    Serial.println("Keypad does not appear to be connected. Please check wiring. Freezing...");
    while (1);
  }
  Serial.print("Initialized. Firmware Version: ");
  Serial.println(keypad1.getVersion());
  Serial.println("Press a button: * to do a space. # to go to next line.");

  Keyboard.begin(); //Init keyboard emulation
}

void loop() {
  keypad1.updateFIFO();  // necessary for keypad to pull button from stack to readable register
  char button = keypad1.getButton();

  if (button == -1)
  {
    Serial.println("No keypad detected");
    delay(1000);
  }
  else if (button != 0)
  {
    if (button == '0') {//note that this is a keypad '0' not the button press itself
      Keyboard.write('0');
    }
    else if (button == '1') {
      Keyboard.write('1');
    }
    else if (button == '2') {
      Keyboard.write('2');
    }
    else if (button == '3') {
      Keyboard.write('3');
    }
    else if (button == '4') {
      Keyboard.write('4');
    }
    else if (button == '5') {
      Keyboard.write('5');
    }
    else if (button == '6') {
      Keyboard.write('6');
    }
    else if (button == '7') {
      Keyboard.write('7');
    }
    else if (button == '8') {
      Keyboard.write('8');
    }
    else if (button == '9') {
      Keyboard.write('9');
    }
    else if (button == '#') {
      Keyboard.write('#');
    }
    else if (button == '*') {
      Keyboard.write('*');
    }
  }

  //Do something else. Don't call your Keypad a ton otherwise you'll tie up the I2C bus
  delay(25); //25 is good, more is better
}
