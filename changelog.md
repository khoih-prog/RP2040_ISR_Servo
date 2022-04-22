# RP2040_ISR_Servo Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/RP2040_ISR_Servo.svg?)](https://www.ardu-badge.com/RP2040_ISR_Servo)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/RP2040_ISR_Servo.svg)](https://github.com/khoih-prog/RP2040_ISR_Servo/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/RP2040_ISR_Servo/blob/main/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/RP2040_ISR_Servo.svg)](http://github.com/khoih-prog/RP2040_ISR_Servo/issues)

---
---

## Table of Contents

* [Changelog](#changelog)
  * [Releases v1.1.2](#releases-v112)
  * [Releases v1.1.1](#releases-v111)
  * [Releases v1.1.0](#releases-v111)
  * [Releases v1.0.1](#releases-v101)
  * [Releases v1.0.0](#releases-v100)
 
---
---

## Changelog

### Releases v1.1.2

1. Permit using servos with different pulse ranges simultaneously. Check [Fix Bug #5 : Permit using servos with different pulse ranges simultaneously #4](https://github.com/khoih-prog/RP2040_ISR_Servo/pull/4)


### Releases v1.1.1

1. Delete redundant `.cpp` file causing compile error

### Releases v1.1.0

1. Convert to `h-only` style
2. Add example [multiFileProject](examples/multiFileProject) to demo for multiple-file project
3. Fix `setPulseWidth()` bug. Check [setPulseWidth - removed wrong map #2](https://github.com/khoih-prog/RP2040_ISR_Servo/pull/2)
4. Optimize code by using passing by `reference` instead of by `value`

### Releases v1.0.1

1. Fix platform in `library.json`
2. Add `changelog.md`

### Releases v1.0.0

1. Basic 16 ISR-based servo controllers using 1 hardware timer for RP2040-based board
2. Support to both [**Arduino-mbed RP2040** core](https://github.com/arduino/ArduinoCore-mbed) and [**Earle Philhower's arduino-pico core**](https://github.com/earlephilhower/arduino-pico)


