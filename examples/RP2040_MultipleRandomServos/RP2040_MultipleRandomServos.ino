/****************************************************************************************************************************
  RP2040_MultipleRandomServos.ino
  For :
  - MBED RP2040-based boards such as Nano_RP2040_Connect, RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040.
  - RP2040-based boards such as RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040 using arduino_pico core
  
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/RP2040_ISR_Servo
  Licensed under MIT license
*****************************************************************************************************************************/

/****************************************************************************************************************************
   From ESP32 Servo Example Using Arduino ESP32 Servo Library
   John K. Bennett
   March, 2017

   Different servos require different pulse widths to vary servo angle, but the range is
   an approximately 500-2500 microsecond pulse every 20ms (50Hz). In general, hobbyist servos
   sweep 180 degrees, so the lowest number in the published range for a particular servo
   represents an angle of 0 degrees, the middle of the range represents 90 degrees, and the top
   of the range represents 180 degrees. So for example, if the range is 1000us to 2000us,
   1000us would equal an angle of 0, 1500us would equal 90 degrees, and 2000us would equal 1800
   degrees.

   - Circuit:
   Servo motors have three wires: power, ground, and signal. The power wire is typically red,
   the ground wire is typically black or brown, and the signal wire is typically yellow,
   orange or white. Since the RP2040 can supply limited current at only 3.3V, and servos draw
   considerable power, we will connect servo power to the VBat pin of the RP2040 (located
   near the USB connector). THIS IS ONLY APPROPRIATE FOR SMALL SERVOS.

   We could also connect servo power to a separate external power source (as long as we connect all of 
   the grounds (RP2040, servo, and external power).
   In this example, we just connect RP2040 ground to servo ground. The servo signal pins
   connect to any available GPIO pins on the RP2040 (in this example, we use pins (D1-D6).

   In this example, we assume four Tower Pro SG90 small servos.
   The published min and max for this servo are 500 and 2400, respectively.
   These values actually drive the servos a little past 0 and 180, so
   if you are particular, adjust the min and max values to match your needs.
   Experimentally, 800 and 2450 are pretty close to 0 and 180.
*****************************************************************************************************************************/

#if ( defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_RASPBERRY_PI_PICO) || defined(ARDUINO_ADAFRUIT_FEATHER_RP2040) || \
      defined(ARDUINO_GENERIC_RP2040) ) && !defined(ARDUINO_ARCH_MBED)
  #if !defined(RP2040_ISR_SERVO_USING_MBED)    
    #define RP2040_ISR_SERVO_USING_MBED     false
  #endif  
  
#elif ( defined(ARDUINO_NANO_RP2040_CONNECT) || defined(ARDUINO_RASPBERRY_PI_PICO) || defined(ARDUINO_ADAFRUIT_FEATHER_RP2040) || \
      defined(ARDUINO_GENERIC_RP2040) ) && defined(ARDUINO_ARCH_MBED)
      
  #if !defined(RP2040_ISR_SERVO_USING_MBED)    
    #define RP2040_ISR_SERVO_USING_MBED     true
  #endif  
  
#else      
  #error This code is intended to run on the mbed / non-mbed RP2040 platform! Please check your Tools->Board setting.
#endif

#define ISR_SERVO_DEBUG             4

// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "RP2040_ISR_Servo.h"

// Published values for SG90 servos; adjust if needed
#define MIN_MICROS        800
#define MAX_MICROS        2450 

#define SERVO_PIN_1       16
#define SERVO_PIN_2       17
#define SERVO_PIN_3       18
#define SERVO_PIN_4       19
#define SERVO_PIN_5       20
#define SERVO_PIN_6       21

typedef struct
{
  int     servoIndex;
  uint8_t servoPin;
} ISR_servo_t;

#define NUM_SERVOS            6

ISR_servo_t ISR_servo[NUM_SERVOS] =
{
  { -1, SERVO_PIN_1 }, { -1, SERVO_PIN_2 }, { -1, SERVO_PIN_3 }, { -1, SERVO_PIN_4 }, { -1, SERVO_PIN_5 }, { -1, SERVO_PIN_6 }
};

void setup()
{
  for (int index = 0; index < NUM_SERVOS; index++)
  {
    pinMode(ISR_servo[index].servoPin, OUTPUT);
    digitalWrite(ISR_servo[index].servoPin, LOW);
  }
  
  Serial.begin(115200);
  while (!Serial);

  delay(200);

#if defined(ARDUINO_ARCH_MBED)
  Serial.print(F("\nStarting RP2040_MultipleRandomServos on Mbed "));
#else
  Serial.print(F("\nStarting RP2040_MultipleRandomServos on "));
#endif

  Serial.println(BOARD_NAME);
  Serial.println(RP2040_ISR_SERVO_VERSION);

  for (int index = 0; index < NUM_SERVOS; index++)
  {
    ISR_servo[index].servoIndex = RP2040_ISR_Servos.setupServo(ISR_servo[index].servoPin, MIN_MICROS, MAX_MICROS);

    if (ISR_servo[index].servoIndex != -1)
    {
      Serial.print(F("Setup OK Servo index = ")); Serial.println(ISR_servo[index].servoIndex);

      RP2040_ISR_Servos.setPosition(ISR_servo[index].servoIndex, 0);
    }
    else
    {
      Serial.print(F("Setup Failed Servo index = ")); Serial.println(ISR_servo[index].servoIndex);
    }
  }
}

void printServoInfo(int indexServo)
{
  Serial.print(F("Servos idx = "));
  Serial.print(indexServo);
  Serial.print(F(", act. pos. (deg) = "));
  Serial.print(RP2040_ISR_Servos.getPosition(ISR_servo[indexServo].servoIndex) );
  Serial.print(F(", pulseWidth (us) = "));
  Serial.println(RP2040_ISR_Servos.getPulseWidth(ISR_servo[indexServo].servoIndex));
}

void loop()
{
  int position;      // position in degrees

  position = 0;
  Serial.println(F("Servos @ 0 degree"));
  
  for (int index = 0; index < NUM_SERVOS; index++)
  {
    RP2040_ISR_Servos.setPosition(ISR_servo[index].servoIndex, position );
    printServoInfo(index);
  }
  // waits 5s between test
  delay(5000);

  position = 90;
  Serial.println(F("Servos @ 90 degree"));
  
  for (int index = 0; index < NUM_SERVOS; index++)
  {
    RP2040_ISR_Servos.setPosition(ISR_servo[index].servoIndex, position );
    printServoInfo(index);
  }
  
  // waits 5s between test
  delay(5000);

  position = 180;
  Serial.println(F("Servos @ 180 degree"));
  
  for (int index = 0; index < NUM_SERVOS; index++)
  {
    RP2040_ISR_Servos.setPosition(ISR_servo[index].servoIndex, position );
    printServoInfo(index);
  }
  
  // waits 5s between test
  delay(5000);

  Serial.println(F("Servos sweeps from 0-180 degress"));
  
  for (position = 0; position <= 180; position += 5)
  {
    // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    for (int index = 0; index < NUM_SERVOS; index++)
    {
      RP2040_ISR_Servos.setPosition(ISR_servo[index].servoIndex, position );
    }
    
    // waits 0.1s for the servo to reach the position
    delay(100);
  }
  
  // waits 5s between test
  delay(5000);
}
