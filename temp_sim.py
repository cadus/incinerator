#!/usr/bin/env python3
 
import tty, termios
import sys
import serial
import argparse

try:
    from msvcrt import getch  # try to import Windows version
except ImportError:
    def getch():   # define non-Windows version
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        try:
            tty.setraw(sys.stdin.fileno())
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        return ch

class TempSim(object):
    def __init__(self, cmd):
        self.cmd = cmd
        # Start at room temp.
        self.temp = 20

    def mod(self, delta):
        self.temp += delta
        ser.write(f'{self.cmd} {self.temp}\r\n'.encode('utf-8'))
        print(f'\rMain: {temp_main.temp} Aft: {temp_aft.temp}' + ' ' * 10 + '\r', end='')

temp_main = TempSim('TM')
temp_aft = TempSim('TA')

def main():
     while True:
        char = getch()
        if char == 'q' or char == '\x1b':  # x1b is ESC
            exit()
        if char == 'e':
            temp_main.mod(-100)
        if char == 'r':
            temp_main.mod(100)
        if char == 'd':
            temp_main.mod(-10)
        if char == 'f':
            temp_main.mod(10)
        if char == 'u':
            temp_aft.mod(-100)
        if char == 'i':
            temp_aft.mod(100)
        if char == 'j':
            temp_aft.mod(-10)
        if char == 'k':
            temp_aft.mod(10)
        
 
if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Incinerator temperature simulator')
    parser.add_argument('port', help='serial port')
    parser.add_argument('-b', '--baud', help='baud rate', type=int, default=115200)
    args = parser.parse_args()

    ser = serial.Serial(port=args.port, baudrate=args.baud, exclusive=True)
    main()
