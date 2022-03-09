/****************************************************************************************************************************
  multiFileProject.ino
  RP2040_MultipleServos.ino
  For :
  - MBED RP2040-based boards such as Nano_RP2040_Connect, RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040.
  - RP2040-based boards such as RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040 using arduino_pico core
  
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/RP2040_ISR_Servo
  Licensed under MIT license
*****************************************************************************************************************************/

// To demo how to include files in multi-file Projects

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

#define RP2040_ISR_SERVO_VERSION_MIN_TARGET      "RP2040_ISR_Servo v1.1.1"
#define RP2040_ISR_SERVO_VERSION_MIN             1001001

#include "multiFileProject.h"

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "RP2040_ISR_Servo.h"

void setup() 
{
  Serial.begin(115200);
  while (!Serial);
  
  Serial.println("\nStart multiFileProject");
  Serial.println(RP2040_ISR_SERVO_VERSION);

#if defined(RP2040_ISR_SERVO_VERSION_MIN)
  if (RP2040_ISR_SERVO_VERSION_INT < RP2040_ISR_SERVO_VERSION_MIN)
  {
    Serial.print("Warning. Must use this example on Version equal or later than : ");
    Serial.println(RP2040_ISR_SERVO_VERSION_MIN_TARGET);
  }
#endif
}

void loop() 
{
  // put your main code here, to run repeatedly:
}
