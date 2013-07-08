#!/usr/bin/python

from __future__ import division
import sys
import os

r1=int(sys.argv[1])
r2=int(sys.argv[2])
i_r2=0.0001

v=1.25 * ( 1.0 + r2/r1) + i_r2*r2

print "R1: %d" % r1;
print "R2: %d" % r2;
print "V : %.3f" % v;
