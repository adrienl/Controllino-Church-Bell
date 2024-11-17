# Controllino-Church-Bell
Liturgical Bell Ringing Solution for Controllino Mini

Written on Arduino IDE 1.8.19

## TODO

- ✅ Controllino RTC
- ✅ Display Current Date
- ✅ Display Current Time
- ✅ Display Current Timezone
- 🟠 Adjust Time
- ⭕️ Adjust Date
- ✅ Manage Timezone
- ✅ Manage Daylight Saving Time
- ✅ Manage Ringing Schedule
- ✅ Manage Ringing Sequence
- ✅ Display Next Action

## Hardware

- Controllino Mini (https://www.controllino.com/product/controllino-mini/)
- LCD Char Display 16x2 with I2C Module (https://www.gotronic.fr/art-afficheur-lcd-i2c-2x16-caracteres-25650.htm)

## Controllino Software Setup

- Follow the Controllino instructions : https://www.controllino.com/knowledge-base/board-library-setup-in-arduino-ide/
- Set the RTC Switch ON
- Upload the code

## Inputs / Outputs

- Input A0 (12V > Push Button > A0) : Button Decrease Seconds
- Input A1 (12V > Push Button > A1) : Button Increase Seconds
- Input A2 (12V > Push Button > A2) : Button to Ring the bell once
- Output D0 (230V > COM_D0_D1 > D0 > Bell) : Connected to the Bell

## Display

- Pin Header : SCL/SDA/5V/GND

## Class List and files usage

- Controllino-Church-Bell.ino	->	The Main File
- Const.h						->	Global Consts
- Display						->	Manage the display
- RelayManager					->	Manage RelayAction object and trigger Controllino Relays
- RelayAction					->	Set a relay actions pattern
- RTCManager					->	Manage the Controllino RTC Clock
- Timezone						->	Timezone Builder
- DateTimeTool					-> 	Tools for DateTime clas
- DateTime						-> 	DateTime object
- ClockHandler					->	Manage Live Clock And Timezone
- ScheduleWeekDay				->	Weekday Object Builder
- Schedule						->	Schedule Bell Rings
- Event							->	Associate Event Type and schedule to execute the event
