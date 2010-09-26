#!/usr/bin/env python
import os, sys, re
from glob import glob
from string import *

def usage():
		print '''
Usage:
    cvsdel.py file1 [file2...]
		'''
		sys.exit(0)
	
def main():
	argv = sys.argv[1:]
	if len(argv) == 0:
		usage()
	files = joinfields(filter(lambda x: not os.path.isdir(x), argv), ' ')
	print files
	ans = raw_input('Are you sure you want to delete these files? [n]: ')
	if ans in ['n', 'N', '']:
		sys.exit(0)

	cmdcvs = "cvs delete %s" % files
	open('cvsdelete.sh','w').write(cmdcvs+'\n')
	
	cmdrm = "tar cvf cvsdel.tar %s; rm %s" % (files, files)
	os.system(cmdrm)
	print "cvs delete ..."
	os.system(cmdcvs)
	
main()
