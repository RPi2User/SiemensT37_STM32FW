# WIP

This project is the crucial software for my attemp to modernize my
Siemens T37 Teletype into the developed digital world. 

## Hardware

1. STM32
2. ESP8266
3. Siemens T37
4. CurrentLoop Supply
5. Optoisolated bidirectional TTY/TTL Converter

## Software

This project is meant to be the firmware for the STM32 Devboard.
It'll accept default serial Data via it's USB port and converts it
into a Baudot (CCITT-2) serial bitstream.

### Additional Features

I wanted the Siemens T37 to be an IoT Device. So the ESP8266 will
funciton as a WiFi-Bridge for the STM32.

Via a simple `POST / <data>` the ESP8266 will send via 
`RS232 9600N1` to the STM32. The STM32 will send it to the TTY.


