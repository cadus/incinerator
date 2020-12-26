# CADUS incinerator
Furnace for the incineration of medical waste.

## Project scope
This project contains the firmware & schematic for the control module of the [CADUS](https://www.cadus.org) incinerator.

## Hardware
The control module is based on this ESP32 board: [DEBO JT ESP32 NodeMCU](https://www.reichelt.de/nodemcu-esp32-wifi-und-bluetooth-modul-debo-jt-esp32-p219897.html) similar to [DOIT ESP32 DEVKIT V1](http://elektro.turanis.de/html/prj135/esp32_pinout.png).  
KiCad project covering the schematic of the motherboard carrying the ESP32 is included in the `pcb` folder.

## Build Prerequisites
Install [PlatformIO IDE](https://platformio.org/platformio-ide) or [PlatformIO Core (CLI)](https://docs.platformio.org/en/latest/installation.html#installation-methods)  
PlatformIO will take care of the Arduino framework and third-party libraries.

## Build
PlatformIO IDE: Click "Build"  
PlatformIO CLI:
```
pio run
```

## Install
Connect ESP32 board via USB. Check the serial port assigned to the board, adapt `upload_port` in `platformio.ini`, if necessary.

PlatformIO IDE: Click "Upload"  
PlatformIO CLI:
```
pio run --target upload
```

## Update
Install esptool with `pip3 install --user esptool`.

Connect the processor module via USB and determine the USB serial port. (`/dev/ttyUSB0` in this example)

Update the firmware:
```
python3 -m esptool --port /dev/ttyUSB0 --baud 921600 write_flash 0x10000 firmware.bin
```
