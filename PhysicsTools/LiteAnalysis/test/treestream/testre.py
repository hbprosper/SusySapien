#!/usr/bin/env python
import re

print "Hello"
bname = re.compile("[+&*?<>=()]|\[[^0-9]+\]")

str = "recoTrack[a-z]+"
print str, bname.findall(str)

str = "recoTrack[3][2]"
print str, bname.findall(str)

str = "reco_Track__HBP.obj()"
print str, bname.findall(str)
