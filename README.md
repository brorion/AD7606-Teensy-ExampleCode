# AD7606-Teensy-ExampleCode
Was supposed to be a library for the AD7606 16-bit ADC for the Teensy 4.1, but implementation as such ended at the writing of an example code as it was sufficient for my purposes.

The AD7606 was configured to transfer data in a parallel format, so that was what this example code was written for. For this, direct port manipulation was used to retrieve the states of the digital pins, with the ports configured for the Teensy 4.1. The GPIO ports used are in the i.MX RT datasheet linked below. 

There are two versions of the example code, fastRead and slowRead. slowRead triggers the conversion of the next data after reading in the current set of data, while fastRead triggers the conversion first, and reads the data in an ISR on the rising edge of the busy pin. fastRead can definitely reach the full conversion rate of the AD7606, and possibly slightly faster, but it may be less stable; slowRead only reaches a conversion rate of approx. 198kSPS, but there are fewer things to complicate it. 

# TODO:
(DONE) Write arduino test code that can operate at full speed

(Partially implemented) Manage data storage (saving all 8 channels at 200kSPS would require 3.2MB per second of data. Even with 8+8 MB of PSRAM, the max amount of data is roughly 4 seconds)

Add more documentation here - pinouts, links to resources, how to modify the code for other configurations (see Documentation section below)

# Documentation
Teensy schematic for pinout to native GPIO label mapping: https://www.pjrc.com/teensy/schematic.html

Alternative teensy schematic: https://github.com/KurtE/TeensyDocuments

i.MX RT datasheet for lookup of GPIO blocks: https://www.pjrc.com/teensy/IMXRT1060RM_rev2.pdf (search GPIO6, GPIO7, etc, or go to page ~305 for GPIO block configurations; GPIO1 and GPIO6 share the same pins). For these example codes, the GPIO pins used are GPIO1_IO16 to GPIO1_IO31, corresponding to pins GPIO_AD_B1_00 to GPIO_AD_B1_15 on the teensy schematics above. Pinout used is given further down.

GPIO pin reading for teensy 4.0 instead of 4.1: https://forum.pjrc.com/threads/58377-Reading-multiple-GPIO-pins-on-the-Teensy-4-0-quot-atomically-quot (use this if you want to use the teensy 4.0, which has fewer pins broken out, and doesn't have the full 16 pins for the parallel read-in. You'll need to make a custom address block to read the data, as documented in the forum post.)

NOP delays: https://forum.pjrc.com/threads/42865-A-set-of-scope-tested-100-500-ns-delay-macros

Haven't tried this, but it should be possible to do a similar thing for writing to ports directly: https://forum.pjrc.com/threads/57698-Parallel-IO-is-it-possible

# PINOUT
AD7606 pins : Teensy pins
DB0         : 19
DB1         : 18
DB2         : 14
DB3         : 15
DB4         : 40
DB5         : 41
DB6         : 17
DB7         : 16
DB8         : 22
DB9         : 23
DB10        : 20
DB11        : 21
DB12        : 38
DB13        : 39
DB14        : 26
DB15        : 27
CV          : 24
RD          : 25
BUSY        : 35
CS          : 34
RST         : 36
OS0         : GND
OS1         : GND (change to 3.3V to reduce sampling rate to 50kSPS. For other sampling rates, check the AD7606 datasheet.)
OS2         : GND
RANGE       : 3.3V

# CHANGELOG
23 Apr 2021: Uploaded initial Arduino test code (AD7606_SlowRead) as a proof of concept for whether the code works

23 Apr 2021: Second Arduino test code (AD7606_FastRead) that can operate at full speed (>200kSPS). 
