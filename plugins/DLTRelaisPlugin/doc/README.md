# DLTRelaisPlugin

DLTRelaisPlugin is used to control Relais for Test Automation.
It is developed for an Arduino SW running on a Wemos Mini D1 HW or Wemos D1 HW and several shields.
It provides an interface to DLT for Logging and Test Automation.

![Image of Wemos D1 Mini Relais Board](https://github.com/alexmucde/dlt-sim/blob/main/plugin/DLTRelaisPlugin/doc/images/WemosD1MiniRelaisBoard.jpg)

![Image of Wemos D1 Relais Board](https://github.com/alexmucde/dlt-sim/blob/main/DLTRelaisPlugin/doc/images/WemosD1RelaisBoard.jpg)

For further information about DLT visit the DLT Viewer project:

https://github.com/GENIVI/dlt-viewer

## Supported Hardware

The following Hardware is currently supported:

* Relais Board: Arduino Wemos Mini D1 + Relais Shields
* Relais Board: Arduino Wemos D1 + Arduino 4 Relais Shields HW-68

## Wemos Mini D1 Hardware

The following parts are needed to setup a Relais board:

* Arduino Wemos Mini D1
  * Wemos D1 Mini [Amazon Germany](https://amzn.to/3thvzYd) [AliExpress](https://s.click.aliexpress.com/e/_AXoYOK)
  * 1-3 Wemos Relais Shields [Amazon Germany](https://amzn.to/3csEJdE) [AliExpress](https://s.click.aliexpress.com/e/_9JeBua)
  * If more than one Relais Shields are used, the pin D1 must be removed and connected to D2 (second Relais) or D6 (third Relais).
  * Triple Base or Dual Base [Amazon Germany](https://amzn.to/3eyI9Ov) [AliExpress Triple Base](https://s.click.aliexpress.com/e/_AXI4VC) [AliExpress Dual Base](https://s.click.aliexpress.com/e/_9In2Z0)
  * Optional: 1-Button Shield [AliExpress](https://s.click.aliexpress.com/e/_9INwTG)

* Arduino Wemos D1
  * Wemos D1 [Amazon Germany](https://amzn.to/3fdFrOu) [AliExpress](https://s.click.aliexpress.com/e/_AMlgFl)
  * Arduino 4 Relais Shield [Amazon Germany](https://amzn.to/3fBYlgY) [AliExpress](https://s.click.aliexpress.com/e/_A4QDGb)

## Arduino Wemos Mini D1 SW

The following Arduino SW is needed [WemosD1MiniRelais3Button.ino](https://github.com/alexmucde/dlt-sim/blob/main/DLTRelaisPlugin/arduino/WemosD1MiniRelais3Button/WemosD1MiniRelais3Button.ino)

The Arduino SW is based on the Wemos Library v0.0.1:

[Wemos Library](https://github.com/alexmucde/WemosLibrary)

Compile, upload and run the SW with the [Arduino IDE](https://www.arduino.cc/en/software).

Clone or copy the Wemos Library into the Arduino Libraries folder before compiling the sketch.

## Arduino Wemos D1 SW 4 Relais Shield

The following Arduino SW is needed [WemosD1Relais4Shield.ino](https://github.com/alexmucde/dlt-sim/blob/main/arduino/DLTRelaisPlugin/WemosD1Relais4Shield/WemosD1Relais4Shield.ino)

The Arduino SW is based on the Wemos Library v0.0.1:

[Wemos Library](https://github.com/alexmucde/WemosLibrary)

Compile, upload and run the SW with the [Arduino IDE](https://www.arduino.cc/en/software).

Clone or copy the Wemos Library into the Arduino Libraries folder before compiling the sketch.

### Features

The Arduino SW provides the following Features:

* Activate and deactivate up to four Relais
* Trigger the Relais for short time (500ms or custom value)
* If a button shield is connected, a short press triggers the Relais 1 connected to pin D1 (only Wemos D1 Mini)
* If a button shield is connected, a long press toggles the Relais 2 connected to pin D2 (only Wemos D1 Mini)

### Protocol

The Wemos D1 Mini and Wemos D1 is connected by a virtual serial device on USB. The serial port settings are 115.200 Baud with 8N1 and no handshake.

A USB driver is needed which can be found here:

https://www.wemos.cc/en/latest/ch340_driver.html

The Arduino SW understands the following commands:

* "R11\n", "R21\n", "R31\n", "R41\n" Activates Relais connected to pin D1, D2, D6 and D7 (Wemos D1 Mini) or D7, D6, D5, D4 (Wemos D1)
* "R10\n", "R20\n", "R30\n", "R40\n" Deactivates Relais connected to pin D1, D2, D6 and D7 (Wemos D1 Mini) or D7, D6, D5, D4 (Wemos D1)
* "R1T\n", "R2T\n", "R3T\n", "R4T\n" Trigger Relais connected to pin D1, D2, D6 and D7 (Wemos D1 Mini) or D7, D6, D5, D4 (Wemos D1) for 500ms
* "R1T<time in ms>\n", "R2T<time in ms>\n", "R3T<time in ms>\n", "R4T<time in ms>\n" Trigger Relais connected to pin D1, D2, D6 and D7 (Wemos D1 Mini) or D7, D6, D5, D4 (Wemos D1) for the defined time in ms

The Arduino board sends the status of Relais, when it was triggered with the button with the same command.

The Arduino board sends every second a watchdog signal "WD\n".

## DLT injection commands

The commands are interpreted independent of the service id.

* \<Relais name\> on/off/trigger
* \<Relais name\> trigger \<time in ms\>

## Donation

If you find this SW useful and you want to donate my work please select one of the following donations:

Paypal Donation:

[![Donations](https://www.paypalobjects.com/en_US/DK/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/donate?hosted_button_id=YBWSNXYWJJP2Q)

Github Sponsors:

[:heart: Sponsor](https://github.com/sponsors/alexmucde)

## Changes

v0.0.1:


## Copyright

Alexander Wenzel <alex@eli2.de>

This code is licensed under Mozilla Public license.
