#!/usr/bin/python
import serial
import time
import math
import sys

def main():
    ser = serial.Serial(
        port='/dev/tty.usbserial-A7027BAL',
        baudrate=115.2*1000
    )

    try:
        while(True):
            sys.stdout.write(ser.read(1))
            sys.stdout.flush()
    except:
        ser.close()


main()
