# CubeSat testing system
This repository contains code for a testing system used for testing CubeSats.
The system is designed to be flexible and distributed. Central to the system is the logging server. To this server, clients can connect. Once connected they can report test outcomes, receive commands and more. 

## Logging Server
The logging server is a node.js based server. It has an API for receiving testing data and registering expected outcomes.

## Exp-Sim client
The experiment client is a node.js program that runs on a computer that is also connected to all experiment simulator PCBs. The client will establish which PCB is which (Since UART device names change and cannot be tied to a device).

## Exp-Sim Arduino code
This code is programmed to the experiment simulators.

## Programming the Bootloader
The microcontroller version used in the experiment simulators, the breakout board, and the sun sensor simulator is the ATMEGA328PB. The Arduino UNO uses the ATMEGA328P. However, this chip was not available for order and so the ATMEGA328PB version was used instead. The ATMEGA328PB has new features compared to the ATMEGA328PB but is backward compatible. To burn the bootloader a regular Arduino UNO was used together with the Arduino ISP sketch that is available in the examples folder of the Arduino IDE. This turns the Arduino into an ISP programmer. The Arduino IDE supports flashing the default UNO bootloader using an Arduino ISP, however; due to the different version of microcontroller, the default procedure will end with errors due to an incorrect device signature caused by the new ATMEGA328PB. 

The following programming method requires that ARVDUDE is installed and that the command is executed in the directory of the custom bootloader on a Linux machine.

	avrdude -C/home/mg/arduino-1.8.5/hardware/tools/avr/etc/avrdude.conf -v -patmega328p -cstk500v1 -P/dev/ttyACM0 -b19200 -Uflash:w:optiboot_atmega328.hex:i -Ulock:w:0x0F:m -F

The -F flag forces AVRDUDE to ignore the incorrect device signature.