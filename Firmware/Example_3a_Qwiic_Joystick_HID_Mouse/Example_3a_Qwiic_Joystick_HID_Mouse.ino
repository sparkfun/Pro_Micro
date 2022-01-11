/******************************************************************************
  Example_3a_Qwiic_Joystick_HID_Mouse.ino
  Written by: Ho Yun "Bobby" Chan
  Date: January 13, 2020
  Development Environment Specifics:
    Arduino IDE 1.8.9

  Description:
    Based on the Jim's Pro Micro "HID Mouse" and Wes' Qwiic Joystick "basic reading"
    examples, this example moves your computer's mouse based on the joystick's
    position. Pressing down on the joystick's will enable a mouse's left click.
    The left click will relese as soon as you stop pressing down on the joystick.

  Libraries:
    Mouse.h (included with Arduino IDE)
    Wire.h (included with Arduino IDE)
    SparkFun_Qwiic_Joystick_Arduino_Library.h (included in the src folder) http://librarymanager/All#SparkFun_joystick

  License:
    This code is released under the MIT License (http://opensource.org/licenses/MIT)

  ******************************************************************************/

#include <Mouse.h>
#include <Wire.h>

#include "SparkFun_Qwiic_Joystick_Arduino_Library.h" //Click here to get the library: http://librarymanager/All#SparkFun_joystick
JOYSTICK joystick;                                   //Create instance of this object

int vertZero, horzZero;       // Stores the initial value of each axis, usually around 512
int vertValue, horzValue;     // Stores current analog output of each axis
const int sensitivity = 200;  // Higher sensitivity value = slower mouse, should be <= about 500
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
  Serial.println("Example 3: HID Mouse w/ Qwiic Joystick");
#endif

  if (joystick.begin() == false)
  {
#if DEBUG
    Serial.println("Joystick does not appear to be connected. Please check wiring. Freezing...");
#endif
    while (1);
  }

  delay(1000);  // short delay to let outputs settle
  vertZero = joystick.getVertical();  // get the initial values
  horzZero = joystick.getHorizontal();  // Joystick should be in neutral position when reading these

  Mouse.begin(); //Init mouse emulation
}

void loop() {
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

  //delay(200); //remove "//" on this line if you need a small delay
}
