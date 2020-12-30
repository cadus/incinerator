# Using the flasher on windows

* Install CP210x_VCP_Windows driver from https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
* Install Python3 https://www.python.org/downloads/windows/
  (Select "add python to PATH")
* Open terminal in flasher directory
* Enter "pip install esptool pyserial" (only necessary when doing this for the first time)
* Enter "flasher.py"
* Choose serial port (use the one called "Silicon Labs CP210x")
* Hold down the BOOT button on the processor board (right from the USB socket)
* Wait until flasher is finished
