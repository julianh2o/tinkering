#!/usr/bin/python

color = 0;
numbers = [];
while (color <= 124):
    r = 0
    g = 0
    b = 0
    if (color % 10 == 0): b = 10
    if (color % 30 == 0): g = 10
    r = 10
    g = 10
    b = 10
    numbers += [g,r,b]
    color += 1

out = "char led_buffer[%d] = {" % numbers.__len__()
for n in numbers:
    out += "%d," % n
out = out[:-1]+"};"
print out;
    
