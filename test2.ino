//This is the current loaded file.
/*
DIP switches on Sabertooth:
DIP 1: OFF && DIP 2: ON  => R/C Control Mode
DIP 3: ON => Power Supply Mode (May be set to OFF with battery)
DIP 4: OFF => Independent Mode (S1 = M1, S2 = M2)
DIP 5: ON => Linear Control
DIP 6: OFF => Microcontroller Mode
*/

// include the Servo library
#include <Servo.h>


#define DELAY 10

uint8_t FORWARD = 1;
uint8_t BACKWARD = 2;
uint8_t LEFT = 3;
uint8_t RIGHT = 4;
uint8_t QUIT = 5;
uint8_t STOP = 6;

Servo FrontLeftMotor;  // create a servo object
Servo FrontRightMotor; // create a servo object
Servo BackLeftMotor;   // create a servo object
Servo BackRightMotor;  // create a servo object

int speed = 1500;
int f_speed = 1500;
int b_speed = 1500;
int acceleration = 2;
int deceleration = 25;
//std::string incoming_data();

void setup()
{
  Serial.begin(9600);         
  FrontLeftMotor.attach(9);   
  FrontRightMotor.attach(10); 
  BackLeftMotor.attach(3);    
  BackRightMotor.attach(4);   
  pinMode(LED_BUILTIN, OUTPUT);

  // Set all motors to neutral speed initially
  FrontLeftMotor.writeMicroseconds(1500);
  FrontRightMotor.writeMicroseconds(1500);
  BackLeftMotor.writeMicroseconds(1500);
  BackRightMotor.writeMicroseconds(1500);
}

void loop()
{
  if (Serial.available() > 0)
  {
    String incoming_data(Serial.readString());
    char t_command = incoming_data.charAt(0);
    incoming_data.remove(0, 1);
    int t_delay = incoming_data.toInt();
    if (t_command == 'f')
    {
      for (speed = 1500; speed < 2000; speed += acceleration)
      {
        FrontLeftMotor.writeMicroseconds(speed);
        FrontRightMotor.writeMicroseconds(speed);
        BackLeftMotor.writeMicroseconds(speed);
        BackRightMotor.writeMicroseconds(speed);
        delay(DELAY);
      }
      delay(t_delay);
      for (speed = 2000; speed > 1500; speed -= deceleration)
      {
        FrontLeftMotor.writeMicroseconds(speed);
        FrontRightMotor.writeMicroseconds(speed);
        BackLeftMotor.writeMicroseconds(speed);
        BackRightMotor.writeMicroseconds(speed);
        delay(DELAY);
      }
      //delay(1000);
    }
    else if (t_command == "b")
    {
      for (speed = 1500; speed > 1000; speed -= acceleration)
      {
        FrontLeftMotor.writeMicroseconds(speed);
        FrontRightMotor.writeMicroseconds(speed);
        BackLeftMotor.writeMicroseconds(speed);
        BackRightMotor.writeMicroseconds(speed);
        delay(DELAY);
      }
      delay(t_delay);
      for (speed = 1000; speed < 1500; speed += deceleration)
      {
        FrontLeftMotor.writeMicroseconds(speed);
        FrontRightMotor.writeMicroseconds(speed);
        BackLeftMotor.writeMicroseconds(speed);
        BackRightMotor.writeMicroseconds(speed);
        delay(DELAY);
      }
      //delay(1000);
    }
    else if (t_command == "r") //changed from left to right just to test
    {
      // CHANGED: Implemented omniwheel strafing RIGHT
      for (speed = 1500; speed < 2000; speed += acceleration)
      {
        FrontLeftMotor.writeMicroseconds(1500 - (speed - 1500));  // reverse
        FrontRightMotor.writeMicroseconds(speed);                 // forward
        BackLeftMotor.writeMicroseconds(speed);                   // forward
        BackRightMotor.writeMicroseconds(1500 - (speed - 1500));  // reverse
        delay(DELAY);
      }
      delay(t_delay);
      for (speed = 2000; speed > 1500; speed -= deceleration)
      {
        FrontLeftMotor.writeMicroseconds(1500 - (speed - 1500));  // reverse
        FrontRightMotor.writeMicroseconds(speed);                 // forward
        BackLeftMotor.writeMicroseconds(speed);                   // forward
        BackRightMotor.writeMicroseconds(1500 - (speed - 1500));  // reverse 
        delay(DELAY);
      }
      //delay(1000);
    }
    else if (t_command == "l") //changed from right to left just to test
    {
      // CHANGED: Implemented omniwheel strafing LEFT
      for (speed = 1500; speed < 2000; speed += acceleration)
      {
        FrontLeftMotor.writeMicroseconds(speed);                   // forward
        FrontRightMotor.writeMicroseconds(1500 - (speed - 1500));  // reverse
        BackLeftMotor.writeMicroseconds(1500 - (speed - 1500));    // reverse
        BackRightMotor.writeMicroseconds(speed);                   // forward
        delay(DELAY);
      }
      delay(t_delay);
      for (speed = 2000; speed > 1500; speed -= deceleration)
      {
        FrontLeftMotor.writeMicroseconds(speed);                   
        FrontRightMotor.writeMicroseconds(1500 - (speed - 1500));  
        BackLeftMotor.writeMicroseconds(1500 - (speed - 1500));    
        BackRightMotor.writeMicroseconds(speed);   
        delay(DELAY);
      }
      //delay(1000);
    }
    else if (t_command == "s")
    {
      FrontLeftMotor.writeMicroseconds(1500);
      FrontRightMotor.writeMicroseconds(1500);
      BackLeftMotor.writeMicroseconds(1500);
      BackRightMotor.writeMicroseconds(1500);
    }
  }
}
