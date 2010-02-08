#!/usr/bin/env python
#------------------------------------------------------------------------------
# File: updateclass_xml.py
# Created: 11-Dec-2009 HBP, (during CERN visit)
#------------------------------------------------------------------------------
import os, sys, re
from glob import glob
from string import *
from boostlib import *
#------------------------------------------------------------------------------
base = "PhysicsTools/LiteAnalysis/dataformats"
def main():
	cwd = os.path.basename(os.environ['PWD'])
	if cwd != "dataformats":
		print "\t** must be run from dataformats directory"
		sys.exit(0)

	print "\n\tupdating classes_def.xml"
	
	adapters = map(lambda x: '   <class name="%s"/>\n' % nameonly(x),
				   glob("adapters/*.h"))

	os.system("mv ../src/classes_def.xml ../src/classes_def.xml.bak")
	records = open("../src/classes_def.xml.bak").readlines()
	out = open("../src/classes_def.xml","w")
	for record in records:
		if find(record, 'name="a') > -1: continue
		if find(record, '</lcgdict>') > -1: break
		out.write(record)

	for record in adapters:
		out.write(record)
	out.write('</lcgdict>\n')
	out.close()
#------------------------------------------------------------------------------
main()


