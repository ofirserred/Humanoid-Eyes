#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define LEFT_UPPER_EYELID 0
#define LEFT_LOWER_EYELID 1
#define UP_DOWN_MOVEMENT  2
#define LEFT_RIGHT_MOVEMENT 3
#define RIGHT_UPPER_EYELID 4
#define RIGHT_LOWER_EYELID 5

#define SERVO_MIN 150
#define SERVO_MAX 600
#define SERVO_CENTER ((SERVO_MIN + SERVO_MAX) / 2)  // Calculate the center position
#define EYELID_CLOSED_POS 250  // Position for a slightly closed eyelid
#define EYELID_OPEN_POS 400    // Position for fully open eyelid
#define BLINK_DURATION 200     // Duration of a blink in milliseconds

unsigned long lastBlinkTime = 0;  // Variable to track the last blink time
const unsigned long blinkInterval = 5000;  // Interval for blinking (5 seconds)
unsigned long lastDetectionTime = 0;  // To keep track of the last detection time
unsigned long lastActionTime = 0;  // Time of the last action
String lastGender = "";  // Store the last detected gender
unsigned long actionDelay = 3000;  // 3 seconds delay

void setup() 
{
  pwm.begin();
  pwm.setPWMFreq(60);  // Set frequency to 60 Hz
  Serial.begin(9600);  // Initialize serial communication
  // Center the eyes at the start
  centerEyes();
}

void loop()
 {
  if (Serial.available() > 0) 
  {
    String data = Serial.readStringUntil('\n');
    data.trim();

    int firstComma = data.indexOf(',');
    int secondComma = data.indexOf(',', firstComma + 1);

    if (firstComma > 0 && secondComma > firstComma) {
      String gender = data.substring(0, firstComma);
      int x = data.substring(firstComma + 1, secondComma).toInt();
      int y = data.substring(secondComma + 1).toInt();

      unsigned long currentTime = millis();
      lastDetectionTime = millis();

      // Check if the detected gender is different from the last gender
      if (gender != lastGender) 
      {
        if (currentTime - lastActionTime >= actionDelay)
        {
          // If enough time has passed, react to the new gender
          if (gender == "Man") 
          {
            followMan(x, y);
          } 
          else if (gender == "Woman") 
          {
            lookAtWoman(x, y);
          }

          // Update the last action time and last gender
          lastActionTime = currentTime;
          lastGender = gender;
        }
      } 
      else
      {
        // If the gender is the same as before, act immediately
        if (gender == "Man") 
        {
          followMan(x, y);
        } 
        else if (gender == "Woman")
        {
          lookAtWoman(x, y);
        }
        // Update the last action time
        lastActionTime = currentTime;
      }
    }
  }
  // Check for timeout and center eyes if no detection for 2 seconds
  if (millis() - lastDetectionTime > 2000) 
  {  
    centerEyes();  // Center the eyes
  }
}

void centerEyes() 
{
  pwm.setPWM(LEFT_RIGHT_MOVEMENT, 0, SERVO_CENTER - 32);
  pwm.setPWM(UP_DOWN_MOVEMENT, 0, SERVO_CENTER);
  pwm.setPWM(LEFT_UPPER_EYELID, 0, SERVO_CENTER + 100);
  pwm.setPWM(LEFT_LOWER_EYELID, 0, SERVO_CENTER - 120);
  pwm.setPWM(RIGHT_UPPER_EYELID, 0, SERVO_CENTER - 150);
  pwm.setPWM(RIGHT_LOWER_EYELID, 0, SERVO_CENTER + 100);

  delay(1000);  // Delay to allow servos to reach the center position
}

void followMan(int x, int y) 
{
  int horizontalPosition = map(x, 0, 640, SERVO_MIN, SERVO_MAX);
  int verticalPosition = map(y, 0, 480, SERVO_MAX, SERVO_MIN);

  // Move eyes to follow the man
  pwm.setPWM(LEFT_RIGHT_MOVEMENT, 0, horizontalPosition);
  pwm.setPWM(UP_DOWN_MOVEMENT, 0, verticalPosition);

  // Set eyelids to a slightly closed position
  pwm.setPWM(LEFT_UPPER_EYELID, 0, SERVO_CENTER);
  pwm.setPWM(LEFT_LOWER_EYELID, 0, SERVO_CENTER + 40);
  pwm.setPWM(RIGHT_UPPER_EYELID, 0, SERVO_CENTER);
  pwm.setPWM(RIGHT_LOWER_EYELID, 0, SERVO_CENTER - 60);
}

void lookAtWoman(int x, int y) 
{
  int horizontalPosition = map(x, 0, 640, SERVO_MIN, SERVO_MAX);
  int verticalPosition = map(y, 0, 480, SERVO_MAX, SERVO_MIN);

  pwm.setPWM(LEFT_RIGHT_MOVEMENT, 0, horizontalPosition);
  pwm.setPWM(UP_DOWN_MOVEMENT, 0, verticalPosition);

  // Open the eyelids fully
  pwm.setPWM(LEFT_UPPER_EYELID, 0, SERVO_CENTER + 100);
  pwm.setPWM(LEFT_LOWER_EYELID, 0, SERVO_CENTER - 120);
  pwm.setPWM(RIGHT_UPPER_EYELID, 0, SERVO_CENTER - 150);
  pwm.setPWM(RIGHT_LOWER_EYELID, 0, SERVO_CENTER + 100);

  unsigned long currentTime = millis();
  if (currentTime - lastBlinkTime >= blinkInterval)
  {
    // Activate the blink function
    blink(x,y);
    lastBlinkTime = currentTime;  // Update the last blink time
  }
}

void blink(int x,int y) 
{

  // Close the eyelids briefly
  pwm.setPWM(RIGHT_UPPER_EYELID, 0, SERVO_CENTER + 90);
  pwm.setPWM(RIGHT_LOWER_EYELID, 0, SERVO_CENTER - 90);

  delay(BLINK_DURATION);

  // Open the eyelids again
  pwm.setPWM(RIGHT_UPPER_EYELID, 0, SERVO_CENTER - 150);
  pwm.setPWM(RIGHT_LOWER_EYELID, 0, SERVO_CENTER + 100);
}
