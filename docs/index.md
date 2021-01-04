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

## Update via esptool
Install esptool with `pip3 install --user esptool`.

Connect the processor module via USB and determine the USB serial port. (`/dev/ttyUSB0` in this example)

Update the firmware:
```
python3 -m esptool --port /dev/ttyUSB0 --baud 921600 write_flash 0x10000 firmware.bin
```

## Update via flasher
The flasher (distributed with releases) is a small wrapper around `esptool` and can be used for the update instead of calling `esptool` directly.

Using the flasher tool on windows:

* Install CP210x_VCP_Windows driver from https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
* Install Python3 https://www.python.org/downloads/windows/
  (Select "add python to PATH")
* Open terminal in flasher directory
* Enter "pip install esptool pyserial" (only necessary when doing this for the first time)
* Enter "flasher.py"
* Choose serial port (use the one called "Silicon Labs CP210x")
* Hold down the BOOT button on the processor board (right from the USB socket)
* Wait until flasher is finished
