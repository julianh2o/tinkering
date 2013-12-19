#!/usr/bin/python

import math

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
    return g, r, b
    #return g/16, r/16, b/16

strip_length = 125
numbers = []
for i in range(0,strip_length):
    hue = (float(i)/strip_length) * 360.0
    numbers += hsv2rgb(hue,1,1)
    #numbers += 0,0,0

out = "char led_buffer[DATA_SIZE] = {"
i = 0;
for n in numbers:
    out += "%d," % n
out = out[:-1]+"};"
print out;

print "Length: ",numbers.__len__();
