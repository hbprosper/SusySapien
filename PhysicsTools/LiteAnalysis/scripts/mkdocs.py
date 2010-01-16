#!/usr/bin/env python
#------------------------------------------------------------------------------
# File: mkdocs.py
# Description: Create documentation of CMS classes.
# Created: 11-Dec-2009 HBP, (during CERN visit)
#------------------------------------------------------------------------------
import os, sys
from string import *
#------------------------------------------------------------------------------
def main():
	cwd = os.path.basename(os.environ['PWD'])
	if cwd != "dataformats":
		print "\t** must be run from dataformats directory"
		sys.exit(0)
		
	# find all xml files starting from given directory
	files = map(strip, os.popen('find ../src/DataFormats -name "*.xml"'))
	for file in files:
		cmd = "lsclass.py %s" % file
		os.system(cmd)

	# Now create index
	os.system("mkindex.py")
#------------------------------------------------------------------------------
main()



