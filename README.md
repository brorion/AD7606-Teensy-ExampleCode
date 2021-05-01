# AD7606-Teensy-Library
Library for the AD7606 16-bit ADC for the Teensy 4.1

Currently a WIP

# TODO:
(DONE) Write arduino test code that can operate at full speed

(Partially implemented) Manage data storage (saving all 8 channels at 200kSPS would require 3.2MB per second of data. Even with 8+8 MB of PSRAM, the max amount of data is roughly 4 seconds)

Write library to make things neater

Add more documentation here - pinouts, links to resources, how to modify the code for other configurations

Add better timers that dynamically change to match clock speeds?

Add support for other data modes? (Low priority)

# Documentation
Teensy schematic for pinout to native GPIO label mapping: https://www.pjrc.com/teensy/schematic.html

Alternative teensy schematic: https://github.com/KurtE/TeensyDocuments

i.MX RT datasheet for lookup of GPIO blocks: https://www.pjrc.com/teensy/IMXRT1060RM_rev2.pdf (search GPIO6, GPIO7, etc, or go to page ~305 for GPIO block configurations)

GPIO pin reading for teensy 4.0 instead of 4.1: https://forum.pjrc.com/threads/58377-Reading-multiple-GPIO-pins-on-the-Teensy-4-0-quot-atomically-quot (use this if you want to use the teensy 4.0, which has fewer pins broken out, and doesn't have the full 16 pins for the parallel read-in. You'll need to make a custom address block to read the data, as documented in the forum post.)

NOP delays: https://forum.pjrc.com/threads/42865-A-set-of-scope-tested-100-500-ns-delay-macros

Haven't tried this, but it should be possible to do a similar thing for writing to ports directly: https://forum.pjrc.com/threads/57698-Parallel-IO-is-it-possible

# CHANGELOG
23 Apr 2021: Uploaded initial Arduino test code (AD7606_SlowRead) as a proof of concept for whether the code works

23 Apr 2021: Second Arduino test code (AD7606_FastRead) that can operate at full speed (>200kSPS). 
