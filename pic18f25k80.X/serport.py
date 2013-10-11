#!/usr/bin/python
import serial
import time
import math

def main():
    ser = serial.Serial(
        port='/dev/tty.usbserial-A7027BAL',
        baudrate=9600
    )

    clearStrip(ser);
    time.sleep(1)

    data = []
    for i in range(0,105):
        h = i*(360.0/125.0)
        print h
        r, g, b = hsv2rgb(h,1,1)
        print r,g,b
        #r = 10 if i % 3 == 0 else 0
        #g = 10 if i % 3 == 1 else 0
        #b = 10 if i % 3 == 2 else 0
        data += [g,r,b];
    writeBytes(ser,0,data);

    ser.close()             # close port



def a2s(arr):
    return ''.join(chr(b) for b in arr)

def clearStrip(ser):
    data = [0] * (25*3)
    for i in range(5):
        writeBytes(ser,25*i,data);

def writeBytes(ser,loc,data):
    leds_per_transmission = 84;
    if (data.__len__() > leds_per_transmission*3):
        writeBytes(ser,loc,data[:leds_per_transmission*3]);
        writeBytes(ser,loc+leds_per_transmission-1,data[leds_per_transmission*3-1:]);
    else:
        length = data.__len__() + 1;
        data = [length,loc] + data;
        ser.write(a2s(data))

def printAvailable(ser):
    waiting = ser.inWaiting()
    if (waiting == 0): return;
    print ser.read(waiting);


#http://code.activestate.com/recipes/576919-python-rgb-and-hsv-conversion/
def hsv2rgb(h, s, v):
    h = float(h)
    s = float(s)
    v = float(v)
    h60 = h / 60.0
    h60f = math.floor(h60)
    hi = int(h60f) % 6
    f = h60 - h60f
    p = v * (1 - s)
    q = v * (1 - f * s)
    t = v * (1 - (1 - f) * s)
    r, g, b = 0, 0, 0
    if hi == 0: r, g, b = v, t, p
    elif hi == 1: r, g, b = q, v, p
    elif hi == 2: r, g, b = p, v, t
    elif hi == 3: r, g, b = p, q, v
    elif hi == 4: r, g, b = t, p, v
    elif hi == 5: r, g, b = v, p, q
    r, g, b = int(r * 255), int(g * 255), int(b * 255)
    return g/16, r/16, b/16

main()
