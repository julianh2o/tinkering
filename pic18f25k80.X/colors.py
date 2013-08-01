#!/usr/bin/python

color = 0;
numbers = [];
while (color <= 124):
    r = 0
    g = 0
    b = 0
    if (color % 10 == 0): r = 1
    if (color % 30 == 0): g = 1
    r= "RED";
    g = "GREEN";
    b = "BLUE";
    numbers += [g,r,b]
    color += 1

out = "char led_buffer[%d] = {" % numbers.__len__()
for n in numbers:
    out += "%s," % n
out = out[:-1]+"};"
print out;
    
