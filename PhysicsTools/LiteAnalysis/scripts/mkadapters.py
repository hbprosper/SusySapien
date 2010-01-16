#!/usr/bin/env python
#------------------------------------------------------------------------------
# File: mkadapters.py
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
		
	os.system("mkadapter.py lis/*.lis")
	adapters = ['#ifndef ADAPTERS_H\n','#define ADAPTERS_H\n'] \
			   + map(lambda x: '#include "%s/%s"\n' % (base, x),
				   glob("adapters/*.h"))
	adapters.append("#endif\n")

	open("../interface/adapters.h","w").writelines(adapters)

	os.system("updateclasses_def.py")
#------------------------------------------------------------------------------
main()


