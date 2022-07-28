# hobart-h600-timer
A timer pcb including 3D-printed parts and code for Hobart H600 mixing machines

## What is this about?
This repository contains everything you need to retrofit a Hobart H600 mixing machine with a timer and to build a new operating panel for the machine. I bought a Hobart H 600 (3-phase)/380V from a bakery, which went bankrupt and refurbished the machine. As the whole machine was dirty especially the operating panels I chose to replace them and add a timer function to the machine. Everything I have done is included here.

## Pictures before
tbd

## Pictures after
tbd

## Operating panel
I chose to redesign the operating panel to make it compliant to current standards. So I needed a switch to turn the machine on, another to turn it off, an emergency button, a rotary dial knob for the timer, as well as a 7-digit display.

The operating panel is only 159mm x 83mm, which is not much space for the elements planned.

You could use the file for the operating panel "Panel-0069.dxf" for laser cutting an aluminium or stainless-steel panel. I chose stainless-steel. On top of the panel I put an emergency-off button Eaton M22-PV/K11, below is a combined on/off button with control LED Eaton M22-DDL-GR-X0/X1/-A/-LED-W/-K10/-K01. The bottom row is divided into space for the 7-segment display (Adafruit 0.56" 4-Digit 7-Segment Display w/I2C Backpack) and a rotary encoder button Alps STEC12E07.

I am currently designing a backplane for the stainless-steel panel to hold the 7-segment display and the rotary encoder. So this is not ready, yet.

## Bill of materials

This is an almost complete table of everything I needed to build the new operating panel and the timer retrofit.

|Element                 |Producer        |Product-No.                             |Webshop                      |
|------------------------|----------------|----------------------------------------|-----------------------------|
|Operating panel         |self/CNC        |Panel-0069.dxf                          |https://mipart.com/          |
|Backplane               |self/3D-Print   |Backplane.stl                           |https://mipart.com/          |
|7-segment display       |Adafruit        |0.56" 4-Digit 7-Segment Display         |https://www.reichelt.de/     |
|Rotary Encoder          |Alps            |STEC12E07                               |https://www.reichelt.de/     |
|PCB                     |self/PCB        |Hobart H600 Timer.fzz                   |https://aisler.net/p/ZWIEMOXD|
|0603 1kohm Resistor     |any             |SMD-0603 1,0K                           |https://www.reichelt.de/     |
|0603 10kohm Resistor    |any             |SMD-0603 10K                            |https://www.reichelt.de/     |
|Transistor, SOT-23      |NXP             |BC 817-40                               |https://www.reichelt.de/     |
|Diode TS 4148           |any             |TS 4148                                 |https://www.reichelt.de/     |
|Relay                   |Weidmueller     |RCI 314012                              |https://www.reichelt.de/     |
|Switching PSU           |Meanwell        |IRM-03-12                               |https://www.reichelt.de/     |
|Arduino Nano every      |Arduino         |ABX00028                                |https://www.reichelt.de/     |
|Terminal connector (2x) |WAGO            |236-403                                 |https://www.reichelt.de/     |
|Terminal block          |Camdenboss      |CTB0502-2                               |https://www.reichelt.de/     |
|Molex connector         |Molex           |26604040                                |https://www.reichelt.de/     |
|Header w/ 2 pins 2.54   |any             |                                        |https://www.reichelt.de/     |
|Header w/ 3 pins 2.54   |any             |                                        |https://www.reichelt.de/     |
|Emergency off button    |Eaton           |216516 - M22-PV/K11                     |https://www.automation24.de/ |
|Double pushbutton       |Eaton           |216700 - M22-DDL-GR-X1/X0               |https://www.automation24.de/ |
|LED element             |Eaton           |216557 - M22-LED-W                      |https://www.automation24.de/ |
|Contact element         |Eaton           |216378 - M22-K01                        |https://www.automation24.de/ |
|Contact element         |Eaton           |216376 - M22-K10                        |https://www.automation24.de/ |
|Mounting adapter        |Eaton           |216374 - M22-A                          |https://www.automation24.de/ |
|PCB Carrier             |self/3D-Print   |Timer-Mainboard-Carrier.stl             |https://mipart.com/          |

