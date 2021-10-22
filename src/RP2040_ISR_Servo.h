/****************************************************************************************************************************
  RP2040_ISR_Servo.h
  For :
  - MBED RP2040-based boards such as Nano_RP2040_Connect, RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040.
  - RP2040-based boards such as RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040 using arduino_pico core
  
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/RP2040_ISR_Servo
  Licensed under MIT license

  Based on SimpleTimer - A timer library for Arduino.
  Author: mromani@ottotecnica.com
  Copyright (c) 2010 OTTOTECNICA Italy

  Based on BlynkTimer.h
  Author: Volodymyr Shymanskyy

  Version: 1.0.1

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      21/08/2021 Initial coding for RP2040 boards using ArduinoCore-mbed or arduino-pico core
  1.0.1   K Hoang      22/10/2021 Fix platform in library.json for PIO
 *****************************************************************************************************************************/

#pragma once

#ifndef RP2040_ISR_Servo_H
#define RP2040_ISR_Servo_H

#if ( defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_RASPBERRY_PI_PICO) || defined(ARDUINO_ADAFRUIT_FEATHER_RP2040) || \
      defined(ARDUINO_GENERIC_RP2040) ) && !defined(ARDUINO_ARCH_MBED)
      
  #define RP2040_ISR_SERVO_VERSION       "RP2040_ISR_Servo v1.0.0"
  
#elif ( defined(ARDUINO_NANO_RP2040_CONNECT) || defined(ARDUINO_RASPBERRY_PI_PICO) || defined(ARDUINO_ADAFRUIT_FEATHER_RP2040) || \
      defined(ARDUINO_GENERIC_RP2040) ) && defined(ARDUINO_ARCH_MBED)
      
  #define RP2040_ISR_SERVO_VERSION       "Mbed RP2040_ISR_Servo v1.0.1"
  
#else      
  #error This code is intended to run on the mbed / non-mbed RP2040 platform! Please check your Tools->Board setting.
#endif

#include <stddef.h>

#include <inttypes.h>

#if defined(ARDUINO)
  #if ARDUINO >= 100
    #include <Arduino.h>
  #else
    #include <WProgram.h>
  #endif
#endif

#include "RP2040_ISR_Servo_Debug.h"

#define RP2040_MAX_PIN           NUM_DIGITAL_PINS
#define RP2040_WRONG_PIN         255

// From Servo.h - Copyright (c) 2009 Michael Margolis.  All right reserved.

#define MIN_PULSE_WIDTH         800       // the shortest pulse sent to a servo  
#define MAX_PULSE_WIDTH         2450      // the longest pulse sent to a servo 
#define DEFAULT_PULSE_WIDTH     1500      // default pulse width when servo is attached
#define REFRESH_INTERVAL        20000     // minumim time to refresh servos in microseconds 

/////////////////////////////////////////////////////

#if defined(ARDUINO_ARCH_MBED)

/////////////////////////////////////////////////////

#include <mbed.h>

#if defined __has_include
#  if __has_include ("pinDefinitions.h")
#    include "pinDefinitions.h"
#  endif
#endif

/////////////////////////////////////////////////////

class ServoImpl 
{
  mbed::DigitalOut   *pin;
  mbed::Timeout      timeout;  // calls a callback once when a timeout expires
  mbed::Ticker       ticker;   // calls a callback repeatedly with a timeout

  public:
  
    ServoImpl(PinName _pin) 
    {
      pin = new mbed::DigitalOut(_pin);
    }

    ~ServoImpl() 
    {
      ticker.detach();
      timeout.detach();
      
      delete pin;
    }

    void start(uint32_t duration_us) 
    {
      duration = duration_us;
      
      // in microsecs
      ticker.attach(mbed::callback(this, &ServoImpl::call), (std::chrono::microseconds) 20000);
    }

    void call() 
    {
      timeout.attach(mbed::callback(this, &ServoImpl::toggle), (std::chrono::microseconds) duration);
      toggle();
    }

    void toggle() 
    {
      *pin = !*pin;
    }

    int32_t duration = -1;
};

/////////////////////////////////////////////////////

#else

/////////////////////////////////////////////////////

#include <hardware/pio.h>

/////////////////////////////////////////////////////

#endif

/////////////////////////////////////////////////////

  // For mbed core
class RP2040_ISR_Servo
{

  public:
    // maximum number of servos
    const static int MAX_SERVOS = 16;

    // constructor
    RP2040_ISR_Servo();

    // destructor
    ~RP2040_ISR_Servo()
    {  
    }

    // Bind servo to the timer and pin, return servoIndex
    int setupServo(uint8_t pin, int min = MIN_PULSE_WIDTH, int max = MAX_PULSE_WIDTH, int value = 0);
    
    // if value is < MIN_PULSE_WIDTH its treated as an angle, otherwise as pulse width in microseconds
    void write(unsigned servoIndex, int value);
    
    // Write pulse width in microseconds
    void writeMicroseconds(unsigned servoIndex, int value);

    // setPosition will set servo to position in degrees
    // by using PWM, turn HIGH 'duration' microseconds within REFRESH_INTERVAL (20000us)
    // returns true on success or -1 on wrong servoIndex
    bool setPosition(unsigned servoIndex, int position);

    // returns last position in degrees if success, or -1 on wrong servoIndex
    int getPosition(unsigned servoIndex);
    
    // setPulseWidth will set servo PWM Pulse Width in microseconds, correcponding to certain position in degrees
    // by using PWM, turn HIGH 'pulseWidth' microseconds within REFRESH_INTERVAL (20000us)
    // min and max for each individual servo are enforced
    // returns true on success or -1 on wrong servoIndex
    bool setPulseWidth(unsigned servoIndex, unsigned int pulseWidth);

    // returns pulseWidth in microsecs (within min/max range) if success, or 0 on wrong servoIndex
    unsigned int getPulseWidth(unsigned servoIndex);

    // destroy the specified servo
    void deleteServo(unsigned servoIndex);

    // returns true if the specified servo is enabled
    bool isEnabled(unsigned servoIndex);

    // enables the specified servo
    bool enable(unsigned servoIndex);

    // disables the specified servo
    bool disable(unsigned servoIndex);

    // enables all servos
    void enableAll();

    // disables all servos
    void disableAll();

    // enables the specified servo if it's currently disabled,
    // and vice-versa
    bool toggle(unsigned servoIndex);

    // returns the number of used servos
    int getNumServos();

    // returns the number of available servos
    int getNumAvailableServos() {
      return MAX_SERVOS - numServos;
    };

  private:

    void init()
    {
      for (int servoIndex = 0; servoIndex < MAX_SERVOS; servoIndex++)
      {
        memset((void*) &servo[servoIndex], 0, sizeof (servo_t));
        servo[servoIndex].enabled  = false;
        // Intentional bad pin
        servo[servoIndex].pin      = RP2040_WRONG_PIN;
      }

      numServos   = 0;
    }

    // find the first available slot
    int findFirstFreeSlot();
    
    uint16_t      _minUs;
    uint16_t      _maxUs;

#if defined(ARDUINO_ARCH_MBED)

    typedef struct
    {
      uint8_t       pin;                  // pin servo connected to
      int           position;             // In degrees
      bool          enabled;              // true if enabled
      
      ServoImpl*    servoImpl;
    } servo_t;
    
#else

    typedef struct
    {
      uint8_t       pin;        // pin servo connected to
      PIO           pio;
      int           smIdx;      // The state machine inside
      int           pgmOffset;  // Where in the SM the code lives
      int           position;   // In degrees
      bool          enabled;    // true if enabled
    } servo_t;

#endif

    servo_t servo[MAX_SERVOS];

    // actual number of servos in use (-1 means uninitialized)
    int numServos;
};

/////////////////////////////////////////////////////

extern RP2040_ISR_Servo RP2040_ISR_Servos;  // create servo object to control up to 16 servos


#endif
