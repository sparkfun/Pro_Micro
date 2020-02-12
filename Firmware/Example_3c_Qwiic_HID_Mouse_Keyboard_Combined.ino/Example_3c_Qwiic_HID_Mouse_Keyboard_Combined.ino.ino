/******************************************************************************
  Example_3c_Qwiic_HID_Mouse_Keyboard_Combined.ino
  Written by: Ho Yun "Bobby" Chan
  Date: February 6, 2020
  Development Environment Specifics:
    Arduino IDE 1.8.9

  Description:
    Based on the following examples:
      - Jim's Pro Micro "HID Mouse/Keyboard"
      - Wes' Qwiic Joystick "basic reading"
      - Pete's Qwiic Keypad "read button"

    This example controls the mouse with the Qwiic Joystick
    and keyboard presses associated with the Qwiic Keypad.
    The serial output is turned off so that Qwiic Pro Micro
    can respond faster.

  Libraries:
   Mouse.h (included with Arduino IDE)
   Keyboard.h (included with Arduino IDE)
   Wire.h (included with Arduino IDE)
   SparkFun_Qwiic_Joystick_Arduino_Library.h (included in the src folder) http://librarymanager/All#SparkFun_joystick
   SparkFun_Qwiic_Keypad_Arduino_Library.h (included in the src folder) http://librarymanager/All#SparkFun_keypad

  License:
    This code is released under the MIT License (http://opensource.org/licenses/MIT)

******************************************************************************/

#include <Mouse.h>
#include <Keyboard.h>
#include <Wire.h>

#include "SparkFun_Qwiic_Joystick_Arduino_Library.h" //Click here to get the library: http://librarymanager/All#SparkFun_joystick
JOYSTICK joystick;                                   //Create instance of this object

#include "SparkFun_Qwiic_Keypad_Arduino_Library.h" //Click here to get the library: http://librarymanager/All#SparkFun_keypad
KEYPAD keypad1;                                    //Create instance of this object

int vertZero, horzZero;       // Stores the initial value of each axis, usually around 512
int vertValue, horzValue;     // Stores current analog output of each axis
const int sensitivity = 150;  // Higher sensitivity value = slower mouse, should be <= about 500
int mouseClickFlag = 0;

//int invertMouse = 1;        //Invert joystick based on orientation
int invertMouse = -1;         //Noninverted joystick based on orientation

//Debug mode, comment one of these lines out using a syntax
//for a single line comment ("//"):
#define DEBUG 0       //0 = HID only
//#define DEBUG 1     //1 = HID with serial output

void setup() {
#if DEBUG
  Serial.begin(9600);
  Serial.println("Qwiic KeyPad Example");
#endif

  if (joystick.begin() == false) {
#if DEBUG
    Serial.println("Joystick does not appear to be connected. Please check wiring. Freezing...");
#endif
    while (1);
  }
  if (keypad1.begin() == false) {  // Note, using begin() like this will use default I2C address, 0x4B.
    // You can pass begin() a different address like so: keypad1.begin(Wire, 0x4A).
#if DEBUG
    Serial.println("Keypad does not appear to be connected. Please check wiring. Freezing...");
#endif
    while (1);
  }

  delay(1000);  // short delay to let outputs settle
  vertZero = joystick.getVertical();  // get the initial values
  horzZero = joystick.getHorizontal();  // Joystick should be in neutral position when reading these

#if DEBUG
  Serial.print("Initialized. Firmware Version: ");
  Serial.println(keypad1.getVersion());
  Serial.println("Press a button: * to do a space. # to go to next line.");
#endif

  Mouse.begin(); //Init mouse emulation
  Keyboard.begin(); //Init keyboard emulation

}//end setup

void loop() {
  keypad1.updateFIFO();  // necessary for keypad to pull button from stack to readable register
  char button = keypad1.getButton();

#if DEBUG
  Serial.print("X: ");
  Serial.print(joystick.getHorizontal());

  Serial.print(" Y: ");
  Serial.print(joystick.getVertical());

  Serial.print(" Button: ");
  Serial.println(joystick.getButton());
#endif

  vertValue = joystick.getVertical() - vertZero; // read vertical offset
  horzValue = joystick.getHorizontal() - horzZero;  // read horizontal offset

  if (vertValue != 0)
    Mouse.move(0, (invertMouse * (vertValue / sensitivity)), 0); // move mouse on y axis
  if (horzValue != 0)
    Mouse.move((invertMouse * (horzValue / sensitivity)), 0, 0); // move mouse on x axis

  if ((joystick.getButton() == 0) && (!mouseClickFlag))  // if the joystick button is pressed
  {
    mouseClickFlag = 1;
    Mouse.press(MOUSE_LEFT);  // click the left button down
  }
  else if ((joystick.getButton()) && (mouseClickFlag)) // if the joystick button is not pressed
  {
    mouseClickFlag = 0;
    Mouse.release(MOUSE_LEFT);  // release the left button
  }

  if (button == -1) {
#if DEBUG
    Serial.println("No keypad detected");
#endif
    delay(1000);
  }

  else if (button != 0) {
#if DEBUG
    Serial.print("Qwiic Keypad Button:  ");
    Serial.println(button);
#endif
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
  //Uncomment this if necessary but since we check the Qwiic Joystick it does not
  // appear to be an issue
  //delay(25); //25 is good, more is better

}//end loop
