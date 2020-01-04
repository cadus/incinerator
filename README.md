# Firmware for the CADUS incinerator
Furnace for the incineration of medical waste.

## Hardware
ESP32 board: [DEBO JT ESP32 NodeMCU](https://www.reichelt.de/nodemcu-esp32-wifi-und-bluetooth-modul-debo-jt-esp32-p219897.html) similar to [DOIT ESP32 DEVKIT V1](http://elektro.turanis.de/html/prj135/esp32_pinout.png)  
KiCad project covering the schematic is included in the repository.

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

## Project Docs
Project documentation is available on [GitHub pages](https://patrislav1.github.io/incinerator/)
