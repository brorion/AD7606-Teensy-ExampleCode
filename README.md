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

# CHANGELOG
23 Apr 2021: Uploaded initial Arduino test code (AD7606_SlowRead) as a proof of concept for whether the code works

23 Apr 2021: Second Arduino test code (AD7606_FastRead) that can operate at full speed (>200kSPS). 
