#!/usr/bin/env python
#-----------------------------------------------------------------------------
# Filename: mkmethodlist.py
# Description: For given class, make an exhaustive listing of methods
# Created:  10-Doc-2009 HBP
#-----------------------------------------------------------------------------
import os, sys, re
from sys    import exit
from time   import *
from string import *
from getopt import getopt, GetoptError
from pprint import PrettyPrinter
from PhysicsTools.LiteAnalysis.Lib import nameonly, readMethods

# Add map directory to PYTHONPATH
mapdir = "%s/map" % os.environ["PWD"]
sys.path.append(mapdir)

# Now try to load classmap

try:
	from classmap import ClassToHeaderMap
except:
	os.system("mkdocs.py")
	try:
		from classmap import ClassToHeaderMap
	except:
		print "\n\t**unable to load classmap.py"
		sys.exit(0)
#-----------------------------------------------------------------------------
if not os.environ.has_key('CMSSW_BASE'):
	print 'CMSSW_BASE not defined!\n'
	exit(0)
BASE = os.environ["PWD"]
PP = PrettyPrinter()
def usage():
	print '''
Usage:
    mkmethodlist.py <TXT-file1> [TXT-file2 ...]
				 '''
	sys.exit(0)

SHORTOPTIONS = 'he:'

BLACK = "\x1b[0;30;48m" # Ctrl[attribute;foreground;backgroundm
RED   = "\x1b[0;31;48m" # \x1b 0,1,...   30+color   40+color

retype = re.compile(r'float|double|int|unsigned|bool|\bsize_t\b')
stripnamespace = re.compile(r'\w+::')
stripcolon = re.compile(r':')
striptemplatepars = re.compile(r'\<.*\>')
stripconst = re.compile(r'\bconst ')
striparg   = re.compile(r'\bconst |\w+::|\&')
stripptr   = re.compile(r'\*$')
methodname = re.compile(r"^.*\(.*\)(?= const)")
retype = re.compile(r'float|double|int|unsigned|bool|\bsize_t\b')
simpletype=re.compile(r'void|float|double|int|unsigned|bool|\bsize_t\b|string')
striprtype = re.compile(r'\W+::|\bconst |\*$|&$|Ref$')
striprtypeless = re.compile(r'\bconst |\*$|&$|Ref$')
stripname = re.compile(r'[ ,\<\>:*&]')
skipmethod= re.compile(r'clone')

DEBUG3 = 0
#----------------------------------------------------------------------------
def expandMethod(filename, methlist):

	header, classname, basenames, methodlist = readMethods(filename)

	# Ok now, get simple methods

	for rtype, name, atype, record in methodlist:

		if DEBUG3 > 3: print "\t\texpandMethod RTYPE( %s, %s )" % (rtype, name)
		
		if rtype == "void": continue
		if atype != "void": continue
		if retype.match(rtype) == None: continue
		
		if DEBUG3 > 2: print "\t\texpandMethod RTYPE( %s, %s, %s )" % \
		   (rtype, name, atype)
		
		# We have a simple return type
		methlist.append((rtype, name))
#----------------------------------------------------------------------------
def mkmethodlist(filename):
	
	names = {}
	tab = "//              "

	header, classname, basenames, methodlist = readMethods(filename)
	if DEBUG3 > 0:
		print "\theader: %s\t\n\t\tclassname: %s" % (header, classname)

	# Return if this is a template class

	if find(classname, "<") > -1:
		if DEBUG3 > 0:
			print "\tskipping template class: %s" % classname
		return
	
	print "processing: %s" % classname
	
	# Ok now, process methods

	simplemethods = []
	compoundmethods = []

	for rtype, name, atype, record in methodlist:

		# Take are not to overwrite these variables

		if DEBUG3 > 2:
			print "\n\tRTYPE( %s ) METHOD( %s ) ARG( %s )" % \
				  (rtype, name, atype)

		# Skip some methods
		
		if skipmethod.match(name) != None: continue
		
		if rtype == "void": continue
		arg = striparg.sub("", atype)

		if simpletype.match(arg) == None: continue

		if ( atype == "void" ):
			fname1 = "%s()"   % name
		else:
			fname1 = "%s(%s)" % (name, atype)
			
		if retype.match(rtype) != None:

			method = [fname1, rtype]

			if DEBUG3 > 0:
				print "\t\tSIMPLE( %s )" % method
			
			simplemethods.append([fname1, rtype])

		else:
			
			# expand complex return type

			cname = striprtypeless.sub("", rtype)
			if DEBUG3 > 0:
				print "\t\t\tCOMPLEX"

			# Get header for this class from ClassToHeaderMap

			if not ClassToHeaderMap.has_key(cname):
				if DEBUG3 > 2:
					print "\t\t** header for class %s NOT found" % cname
				continue
			
			headerfile = ClassToHeaderMap[cname]
			filestem = replace(headerfile, 'interface/', '')
			filestem = split(filestem, '.h')[0]
			filestem = replace(filestem, '/', '.')
			cname  = split(cname,'::').pop()
			txtfilename = "%s/txt/%s.%s.txt" % (BASE, filestem, cname)

			if not os.path.exists(txtfilename):
				if DEBUG3 > 0:
					print "\t\t*** file %s NOT found" % txtfilename
				continue

			if DEBUG3 > 0:
				print "\t\t\t\tRETURN TYPE( %s )" % cname
				
			# txt file exists, so proceed 
			methlist = []
			expandMethod(txtfilename, methlist)
			if len(methlist) == 0: continue

			ispointer = rtype[-1] == "*"
			isRef     = rtype[-3:] == "Ref"
			
			for rtype, fname in methlist:

				fname = "%s()" % fname # Assume void for now
				
				method = None
				if ispointer or isRef:
					method = [fname1, "->", fname, rtype]
				else:
					method = [fname1, ".", fname, rtype]
				compoundmethods.append(method)

				if DEBUG3 > 0:
					print "\tCOMPOUND METHOD( %s )" % method
					
	simplemethods.sort()
	simplemethods = map(lambda x: "%12s  %s" % (x[1],x[0]), simplemethods)
	compoundmethods.sort()
	compoundmethods = map(lambda x: "%12s  %s%s%s" % (x[3],x[0],x[1],x[2]),
						  compoundmethods)
		
	# Write a summary document for this class

	os.system("mkdir -p methods")
	
	methods = simplemethods + compoundmethods

	if len(methods) < 1: return
	
	str = '%s\n' % joinfields(methods,'\n')
	classname = stripcolon.sub("", classname)
	headerfilename   = striptemplatepars.sub("", classname)
	open("methods/%s.txt" % headerfilename, 'w').write(str+'\n')
#----------------------------------------------------------------------------
def main():
	argv = sys.argv[1:]
	if len(argv) == 0:
		print """
Usage:
    mkmethodlist.py give txt-filename
	"""
		sys.exit(0)
	for filename in argv:
		mkmethodlist(filename)
main()

