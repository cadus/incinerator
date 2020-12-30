#!/usr/bin/env python3

import argparse
import esptool
import serial
import sys

"""
Using the commandline parameters from "pio run -t upload -v"
"""

def esp_update(fwfile, port, baud):
    esptool.main(custom_commandline=[
        "--chip", "esp32",
        "--port", port,
        "--baud", str(baud),
        "--before", "default_reset",
        "--after", "hard_reset",
        "write_flash", "-z", 
        "--flash_mode", "dio", 
        "--flash_freq", "40m", 
        "--flash_size", "detect", 
        "0x1000", "bootloader_dio_40m.bin", 
        "0x8000", "partitions.bin",
        "0xe000", "boot_app0.bin", 
        "0x10000", fwfile
    ])

def port_chooser():
    comports = serial.tools.list_ports.comports()
    n_max = max(len(port.device) for port in comports)
    for n, port in enumerate(comports):
        s = port.device
        s += ' ' * (len(s) - n_max) + ' - ' 
        s += port.product
        print(f'{n}: {s}')
    
    i = input(f'Choose port (0-{len(comports)-1}): ')
    try:
        i = int(i)
    except ValueError:
        i = -1

    if i < 0 or i >= len(comports):
        print('Abort')
        sys.exit(0)

    return comports[i].device

def main():
    parser = argparse.ArgumentParser(
        description='CADUS incinerator flash tool'
    )
    parser.add_argument('-f', '--fwfile',
                        type=str,
                        default='firmware.bin',
                        help='Firmware file'
    )
    parser.add_argument('-p', '--port',
                        type=str,
                        help='Serial port'
    )
    parser.add_argument('-b', '--baud',
                        type=int,
                        default=921600,
                        help='Baud rate'
    )
    args = parser.parse_args()
    
    port = args.port or port_chooser()
    esp_update(args.fwfile, port, args.baud)


if __name__ == '__main__':
    main()
