# RP2040_ISR_Servo Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/RP2040_ISR_Servo.svg?)](https://www.ardu-badge.com/RP2040_ISR_Servo)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/RP2040_ISR_Servo.svg)](https://github.com/khoih-prog/RP2040_ISR_Servo/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/RP2040_ISR_Servo/blob/main/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/RP2040_ISR_Servo.svg)](http://github.com/khoih-prog/RP2040_ISR_Servo/issues)

<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Donate to my libraries using BuyMeACoffee" style="height: 50px !important;width: 181px !important;" ></a>
<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://img.shields.io/badge/buy%20me%20a%20coffee-donate-orange.svg?logo=buy-me-a-coffee&logoColor=FFDD00" style="height: 20px !important;width: 200px !important;" ></a>

---
---

## Table of Contents

* [Important Change from v1.1.0](#Important-Change-from-v110)
* [Why do we need this RP2040_ISR_Servo library](#why-do-we-need-this-rp2040_isr_servo-library)
  * [Features](#features)
  * [Important Notes about using ISR](#important-notes-about-using-isr)
  * [Currently supported Boards](#currently-supported-boards)
* [Changelog](changelog.md)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [HOWTO Fix `Multiple Definitions` Linker Error](#howto-fix-multiple-definitions-linker-error) 
* [What special in this RP2040_ISR_Servo library](#what-special-in-this-RP2040_ISR_Servo-library)
* [HOWTO Usage](#howto-usage)
* [Examples](#examples)
  * [ 1. RP2040_MultipleRandomServos](examples/RP2040_MultipleRandomServos)
  * [ 2. RP2040_MultipleServos](examples/RP2040_MultipleServos)
  * [ 3. multiFileProject](examples/multiFileProject) **New**
* [Example RP2040_MultipleServos](#example-rp2040_multipleservos)
  * [1. File RP2040_MultipleServos.ino](#1-file-rp2040_multipleservosino)
* [Debug Terminal Output Samples](#debug-terminal-output-samples)
  * [1. RP2040_MultipleRandomServos on Mbed RaspberryPi Pico](#1-rp2040_multiplerandomservos-on-mbed-raspberrypi-pico)
  * [2. RP2040_MultipleRandomServos on RaspberryPi Pico](#2-rp2040_multiplerandomservos-on-raspberrypi-pico)
  * [3. RP2040_MultipleServos on RaspberryPi Pico](#3-rp2040_multipleservos-on-raspberrypi-pico)
* [Debug](#debug)
* [Troubleshooting](#troubleshooting)
* [Issues](#issues)
* [TO DO](#to-do)
* [DONE](#done)
* [Contributions and Thanks](#contributions-and-thanks)
* [Contributing](#contributing)
* [License](#license)
* [Copyright](#copyright)


---
---

### Important Change from v1.1.0

Please have a look at [HOWTO Fix `Multiple Definitions` Linker Error](#howto-fix-multiple-definitions-linker-error)


---
---

### Why do we need this [RP2040_ISR_Servo library](https://github.com/khoih-prog/RP2040_ISR_Servo)

#### Features

Imagine you have a system with a **mission-critical function** controlling a **robot arm** or doing something much more important. You normally use a software timer to poll, or even place the function in loop(). But what if another function is blocking the loop() or setup().

So your function might not be executed, and the result would be disastrous.

You'd prefer to have your function called, no matter what happening with other functions (busy loop, bug, etc.).

The correct choice is to use a **Hardware Timer with Interrupt** to call your function.

These hardware timers, using interrupt, still work even if other functions are blocking. Moreover, they are **much more precise** (certainly depending on clock frequency accuracy) than other software timers using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

Functions using normal software timers, relying on loop() and calling millis(), won't work if the **loop() or setup() is blocked by certain operation**. For example, certain function is blocking while it's connecting to WiFi or some services.

This library enables you to use `1 Hardware Timer` on an RP2040-based board to control up to `16 independent servo motors`.


#### Important Notes about using ISR

1. Inside the attached function, delay() won’t work and the value returned by millis() will not increment. Serial data received while in the function may be lost. You should declare as volatile any variables that you modify within the attached function.

2. Typically global variables are used to pass data between an ISR and the main program. To make sure variables shared between an ISR and the main program are updated correctly, declare them as volatile.

3. Avoid using Serial.print()-related functions inside ISR. Just for temporary debug purpose, but even this also can crash the system any time. Beware.

4. Your functions are now part of **ISR (Interrupt Service Routine)**, and must be `lean / mean`, and follow certain rules. More to read on:

[HOWTO Attach Interrupt](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)


### Currently supported Boards
 
1. RP2040-based boards such as **Nano_RP2040_Connect, RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040**, etc. using [**Arduino-mbed RP2040** core](https://github.com/arduino/ArduinoCore-mbed)
1. RP2040-based boards such as **Nano_RP2040_Connect, RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040**, etc. using [**Earle Philhower's arduino-pico core**](https://github.com/earlephilhower/arduino-pico)
 
---
---

## Prerequisites

1. [`Arduino IDE 1.8.19+` for Arduino](https://github.com/arduino/Arduino). [![GitHub release](https://img.shields.io/github/release/arduino/Arduino.svg)](https://github.com/arduino/Arduino/releases/latest)
2. [`Arduino mbed_rp2040 core 3.0.1+`](https://github.com/arduino/ArduinoCore-mbed) for Arduino (Use Arduino Board Manager) RP2040-based boards, such as **Arduino Nano RP2040 Connect, RASPBERRY_PI_PICO, etc.**. [![GitHub release](https://img.shields.io/github/release/arduino/ArduinoCore-mbed.svg)](https://github.com/arduino/ArduinoCore-mbed/releases/latest)
3. [`Earle Philhower's arduino-pico core v1.13.2+`](https://github.com/earlephilhower/arduino-pico) for RP2040-based boards such as **RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040**, etc. [![GitHub release](https://img.shields.io/github/release/earlephilhower/arduino-pico.svg)](https://github.com/earlephilhower/arduino-pico/releases/latest)

---

## Installation

### Use Arduino Library Manager

The best and easiest way is to use `Arduino Library Manager`. Search for `RP2040_ISR_Servo`, then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/RP2040_ISR_Servo.svg?)](https://www.ardu-badge.com/RP2040_ISR_Servo) for more detailed instructions.

### Manual Install

Another way to install is to:

1. Navigate to [RP2040_ISR_Servo](https://github.com/khoih-prog/RP2040_ISR_Servo) page.
2. Download the latest release `RP2040_ISR_Servo-main.zip`.
3. Extract the zip file to `RP2040_ISR_Servo-main` directory 
4. Copy whole `RP2040_ISR_Servo-main` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**RP2040_ISR_Servo** library](https://registry.platformio.org/libraries/khoih-prog/RP2040_ISR_Servo) by using [Library Manager](https://registry.platformio.org/libraries/khoih-prog/RP2040_ISR_Servo/installation). Search for **RP2040_ISR_Servo** in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)

---
---

### HOWTO Fix `Multiple Definitions` Linker Error

The current library implementation, using `xyz-Impl.h` instead of standard `xyz.cpp`, possibly creates certain `Multiple Definitions` Linker error in certain use cases.

You can include this `.hpp` file

```
// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "RP2040_ISR_Servo.hpp"     //https://github.com/khoih-prog/RP2040_ISR_Servo
```

in many files. But be sure to use the following `.h` file **in just 1 `.h`, `.cpp` or `.ino` file**, which must **not be included in any other file**, to avoid `Multiple Definitions` Linker Error

```
// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "RP2040_ISR_Servo.h"           //https://github.com/khoih-prog/RP2040_ISR_Servo
```

Check the new [**multiFileProject** example](examples/multiFileProject) for a `HOWTO` demo.

Have a look at the discussion in [Different behaviour using the src_cpp or src_h lib #80](https://github.com/khoih-prog/ESPAsync_WiFiManager/discussions/80)


---
---

### What special in this [RP2040_ISR_Servo library](https://github.com/khoih-prog/RP2040_ISR_Servo)

Now these new **16 ISR-based Servo controllers** just use one RP2040 Hardware Timer. The number 16 is just arbitrarily chosen, and depending on application, you can increase that number to 32, 48, etc. without problem.

The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers

Therefore, their executions are not blocked by bad-behaving functions / tasks. This important feature is absolutely necessary for mission-critical tasks. 

The [**RP2040_MultipleServos**](examples/RP2040_MultipleServos) example, which controls 6 servos independently, will demonstrate the nearly perfect accuracy.

Being ISR-based servo controllers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet and Blynk services.

This non-being-blocked important feature is absolutely necessary for mission-critical tasks.

---
---

## HOWTO Usage

How to use:

```
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

#define TIMER_INTERRUPT_DEBUG       4
#define ISR_SERVO_DEBUG             4

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
```

---
---

### Examples: 

 1. [RP2040_MultipleRandomServos](examples/RP2040_MultipleRandomServos) 
 2. [RP2040_MultipleServos](examples/RP2040_MultipleServos)
 3. [multiFileProject](examples/multiFileProject) **New**
 
---

### Example [RP2040_MultipleServos](examples/RP2040_MultipleServos)


#### 1. File [RP2040_MultipleServos.ino](examples/RP2040_MultipleServos/RP2040_MultipleServos.ino)

https://github.com/khoih-prog/RP2040_ISR_Servo/blob/39e160439b9b4e7c5238202379bd042121784f65/examples/RP2040_MultipleServos/RP2040_MultipleServos.ino#L45-L149

---
---

### Debug Terminal Output Samples

### 1. RP2040_MultipleRandomServos on Mbed RaspberryPi Pico


```
Starting RP2040_MultipleRandomServos on Mbed RaspberryPi Pico
Mbed RP2040_ISR_Servo v1.1.2
Setup OK Servo index = 0
Setup OK Servo index = 1
Setup OK Servo index = 2
Setup OK Servo index = 3
Setup OK Servo index = 4
Setup OK Servo index = 5
Servos @ 0 degree
Servos idx = 0, act. pos. (deg) = 800, pulseWidth (us) = 800
Servos idx = 1, act. pos. (deg) = 800, pulseWidth (us) = 800
Servos idx = 2, act. pos. (deg) = 800, pulseWidth (us) = 800
Servos idx = 3, act. pos. (deg) = 800, pulseWidth (us) = 800
Servos idx = 4, act. pos. (deg) = 800, pulseWidth (us) = 800
Servos idx = 5, act. pos. (deg) = 800, pulseWidth (us) = 800
Servos @ 90 degree
Servos idx = 0, act. pos. (deg) = 1625, pulseWidth (us) = 1625
Servos idx = 1, act. pos. (deg) = 1625, pulseWidth (us) = 1625
Servos idx = 2, act. pos. (deg) = 1625, pulseWidth (us) = 1625
Servos idx = 3, act. pos. (deg) = 1625, pulseWidth (us) = 1625
Servos idx = 4, act. pos. (deg) = 1625, pulseWidth (us) = 1625
Servos idx = 5, act. pos. (deg) = 1625, pulseWidth (us) = 1625
Servos @ 180 degree
Servos idx = 0, act. pos. (deg) = 2450, pulseWidth (us) = 2450
Servos idx = 1, act. pos. (deg) = 2450, pulseWidth (us) = 2450
Servos idx = 2, act. pos. (deg) = 2450, pulseWidth (us) = 2450
Servos idx = 3, act. pos. (deg) = 2450, pulseWidth (us) = 2450
Servos idx = 4, act. pos. (deg) = 2450, pulseWidth (us) = 2450
Servos idx = 5, act. pos. (deg) = 2450, pulseWidth (us) = 2450
Servos sweeps from 0-180 degress
Servos @ 0 degree
Servos idx = 0, act. pos. (deg) = 800, pulseWidth (us) = 800
Servos idx = 1, act. pos. (deg) = 800, pulseWidth (us) = 800
Servos idx = 2, act. pos. (deg) = 800, pulseWidth (us) = 800
Servos idx = 3, act. pos. (deg) = 800, pulseWidth (us) = 800
Servos idx = 4, act. pos. (deg) = 800, pulseWidth (us) = 800
Servos idx = 5, act. pos. (deg) = 800, pulseWidth (us) = 800
```

---

### 2. RP2040_MultipleRandomServos on RaspberryPi Pico

```
Starting RP2040_MultipleRandomServos on RaspberryPi Pico
RP2040_ISR_Servo v1.1.2
Setup OK Servo index = 0
Setup OK Servo index = 1
Setup OK Servo index = 2
Setup OK Servo index = 3
Setup OK Servo index = 4
Setup OK Servo index = 5
Servos @ 0 degree
Servos idx = 0, act. pos. (deg) = 800, pulseWidth (us) = 800
Servos idx = 1, act. pos. (deg) = 800, pulseWidth (us) = 800
Servos idx = 2, act. pos. (deg) = 800, pulseWidth (us) = 800
Servos idx = 3, act. pos. (deg) = 800, pulseWidth (us) = 800
Servos idx = 4, act. pos. (deg) = 800, pulseWidth (us) = 800
Servos idx = 5, act. pos. (deg) = 800, pulseWidth (us) = 800
Servos @ 90 degree
Servos idx = 0, act. pos. (deg) = 1625, pulseWidth (us) = 1625
Servos idx = 1, act. pos. (deg) = 1625, pulseWidth (us) = 1625
Servos idx = 2, act. pos. (deg) = 1625, pulseWidth (us) = 1625
Servos idx = 3, act. pos. (deg) = 1625, pulseWidth (us) = 1625
Servos idx = 4, act. pos. (deg) = 1625, pulseWidth (us) = 1625
Servos idx = 5, act. pos. (deg) = 1625, pulseWidth (us) = 1625
Servos @ 180 degree
Servos idx = 0, act. pos. (deg) = 2450, pulseWidth (us) = 2450
Servos idx = 1, act. pos. (deg) = 2450, pulseWidth (us) = 2450
Servos idx = 2, act. pos. (deg) = 2450, pulseWidth (us) = 2450
Servos idx = 3, act. pos. (deg) = 2450, pulseWidth (us) = 2450
Servos idx = 4, act. pos. (deg) = 2450, pulseWidth (us) = 2450
Servos idx = 5, act. pos. (deg) = 2450, pulseWidth (us) = 2450
Servos sweeps from 0-180 degress
Servos @ 0 degree
Servos idx = 0, act. pos. (deg) = 800, pulseWidth (us) = 800
Servos idx = 1, act. pos. (deg) = 800, pulseWidth (us) = 800
Servos idx = 2, act. pos. (deg) = 800, pulseWidth (us) = 800
Servos idx = 3, act. pos. (deg) = 800, pulseWidth (us) = 800
Servos idx = 4, act. pos. (deg) = 800, pulseWidth (us) = 800
Servos idx = 5, act. pos. (deg) = 800, pulseWidth (us) = 800
```

---

### 3. RP2040_MultipleServos on RaspberryPi Pico

```

Starting RP2040_MultipleServos on RASPBERRY_PI_PICO
RP2040_ISR_Servo v1.1.2
Setup OK Servo index = 0
Setup OK Servo index = 1
Setup OK Servo index = 2
Setup OK Servo index = 3
Setup OK Servo index = 4
Setup OK Servo index = 5
```

---
---

### Debug

Debug is enabled by default on Serial.

You can also change the debugging level from 0 to 2. Be careful and using level 2 only for temporary debug purpose only.

```cpp
#define TIMER_INTERRUPT_DEBUG       1
#define ISR_SERVO_DEBUG             1
```

---

### Troubleshooting

If you get compilation errors, more often than not, you may need to install a newer version of the core for Arduino boards.

Sometimes, the library will only work if you update the board core to the latest version because I am using newly added functions.


---
---

### Issues

Submit issues to: [RP2040_ISR_Servo issues](https://github.com/khoih-prog/RP2040_ISR_Servo/issues)

---
---

## TO DO

1. Search for bug and improvement.

---

## DONE

1. Similar features for Arduino (UNO, Mega, etc...), ESP32, ESP8266 and STM32
2. Add functions `getPosition()` and `getPulseWidth()`
3. Optimize the code
4. Add complicated examples
5. Convert to `h-only` style
6. Add example [multiFileProject](examples/multiFileProject) to demo for multiple-file project
7. Optimize code by using passing by `reference` instead of by `value
8. Permit using servos with different pulse ranges simultaneously


---
---

### Contributions and thanks

Many thanks for everyone for bug reporting, new feature suggesting, testing and contributing to the development of this library. Especially to these people who have directly or indirectly contributed to this [RP2040_ISR_Servo library](https://github.com/khoih-prog/RP2040_ISR_Servo)

1. Thanks to [Radek Voltr](https://github.com/RadekVoltr) for the PR [**setPulseWidth - removed wrong map** #2](https://github.com/khoih-prog/RP2040_ISR_Servo/pull/2), leading to new version v1.1.0
2. Thanks to [Samt43](https://github.com/Samt43) for the PR [**Fix Bug #5 : Permit using servos with different pulse ranges simultaneously** #4](https://github.com/khoih-prog/RP2040_ISR_Servo/pull/4), leading to new version v1.1.2

<table>
  <tr>
    <td align="center"><a href="https://github.com/RadekVoltr"><img src="https://github.com/RadekVoltr.png" width="100px;" alt="RadekVoltr"/><br /><sub><b>Radek Voltr</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/Samt43"><img src="https://github.com/Samt43.png" width="100px;" alt="Samt43"/><br /><sub><b>Samt43</b></sub></a><br /></td>
  </tr> 
</table>

---

## Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---
---

### License

- The library is licensed under [MIT](https://github.com/khoih-prog/RP2040_ISR_Servo/blob/main/LICENSE)

---

## Copyright

Copyright 2021- Khoi Hoang
