#include <Servo.h>

// Motor command definitions
#define FORWARD 'f'
#define BACKWARD 'b'
#define LEFT 'l'
#define RIGHT 'r'
#define STOP 's'

#define DELAY 10
#define ACCEL_STEP 2
#define DECEL_STEP 25
#define MAX_SPEED 2000
#define NEUTRAL_SPEED 1500
#define MIN_SPEED 1000

// Servo motor objects
Servo FrontLeftMotor;
Servo FrontRightMotor;
Servo BackLeftMotor;
Servo BackRightMotor;

// Movement states
enum MovementPhase { IDLE, ACCELERATE, CRUISE, DECELERATE };
MovementPhase phase = IDLE;

char currentCommand = '\0'; // '\0' means no command
unsigned long cruiseStartTime = 0;
unsigned long cruiseDuration = 0;
int curSpeed = NEUTRAL_SPEED;

void setup() {
  Serial.begin(9600);
  FrontLeftMotor.attach(9);
  FrontRightMotor.attach(10);
  BackLeftMotor.attach(3);
  BackRightMotor.attach(4);

  stopAllMotors();
  Serial.println("System Ready");
}

void loop() {
  // Read and parse incoming command
  if (Serial.available() > 0) {
    String incoming_data = Serial.readStringUntil('\n');
    char t_command = incoming_data.charAt(0);
    incoming_data.remove(0, 1);
    int t_delay = incoming_data.toInt();

    if (t_command == FORWARD || t_command == BACKWARD || t_command == LEFT || t_command == RIGHT) {
      currentCommand = t_command;
      cruiseDuration = t_delay;
      curSpeed = NEUTRAL_SPEED;
      phase = ACCELERATE;
      Serial.print("Command received: ");
      Serial.println(t_command);
    }
    else if (t_command == STOP) {
      currentCommand = '\0';
      phase = IDLE;
      stopAllMotors();
      Serial.println("STOP received");
    }
  }

  // Handle movement logic for all directions
  switch (currentCommand) {
    case FORWARD:
      handleMotion(setForwardMotors);
      break;
    case BACKWARD:
      handleMotion(setBackwardMotors);
      break;
    case LEFT:
      handleMotion(setLeftMotors);
      break;
    case RIGHT:
      handleMotion(setRightMotors);
      break;
  }
}

// Unified movement handler
void handleMotion(void (*setMotors)(int)) {
  switch (phase) {
    case ACCELERATE:
      if (curSpeed < MAX_SPEED) {
        curSpeed += ACCEL_STEP;
        if (curSpeed >= MAX_SPEED) {
          curSpeed = MAX_SPEED;
          phase = CRUISE;
          cruiseStartTime = millis();
          Serial.println("Reached max speed, starting cruise");
        }
        setMotors(curSpeed);
        delay(DELAY);
      }
      break;

    case CRUISE:
      setMotors(MAX_SPEED);
      if (millis() - cruiseStartTime >= cruiseDuration) {
        phase = DECELERATE;
        Serial.println("Cruise complete, starting deceleration");
      }
      break;

    case DECELERATE:
      if (curSpeed > NEUTRAL_SPEED) {
        curSpeed -= DECEL_STEP;
        if (curSpeed <= NEUTRAL_SPEED) {
          curSpeed = NEUTRAL_SPEED;
          phase = IDLE;
          currentCommand = '\0';
          Serial.println("Deceleration complete");
        }
        setMotors(curSpeed);
        delay(DELAY);
      }
      break;

    default:
      break;
  }
}

void stopAllMotors() {
  setAllMotors(NEUTRAL_SPEED);
}

void setAllMotors(int pwm) {
  FrontLeftMotor.writeMicroseconds(pwm);
  FrontRightMotor.writeMicroseconds(pwm);
  BackLeftMotor.writeMicroseconds(pwm);
  BackRightMotor.writeMicroseconds(pwm);
}

// Directional motor controls
void setForwardMotors(int pwm) {
  setAllMotors(pwm);
}

void setBackwardMotors(int pwm) {
  FrontLeftMotor.writeMicroseconds(NEUTRAL_SPEED - (pwm - NEUTRAL_SPEED));
  FrontRightMotor.writeMicroseconds(NEUTRAL_SPEED - (pwm - NEUTRAL_SPEED));
  BackLeftMotor.writeMicroseconds(NEUTRAL_SPEED - (pwm - NEUTRAL_SPEED));
  BackRightMotor.writeMicroseconds(NEUTRAL_SPEED - (pwm - NEUTRAL_SPEED));
}

void setRightMotors(int pwm) {
  FrontLeftMotor.writeMicroseconds(NEUTRAL_SPEED - (pwm - NEUTRAL_SPEED));  // reverse
  FrontRightMotor.writeMicroseconds(pwm);                                   // forward
  BackLeftMotor.writeMicroseconds(pwm);                                     // forward
  BackRightMotor.writeMicroseconds(NEUTRAL_SPEED - (pwm - NEUTRAL_SPEED));  // reverse
}

void setLeftMotors(int pwm) {
  FrontLeftMotor.writeMicroseconds(pwm);                                    // forward
  FrontRightMotor.writeMicroseconds(NEUTRAL_SPEED - (pwm - NEUTRAL_SPEED)); // reverse
  BackLeftMotor.writeMicroseconds(NEUTRAL_SPEED - (pwm - NEUTRAL_SPEED));   // reverse
  BackRightMotor.writeMicroseconds(pwm);                                    // forward
}
