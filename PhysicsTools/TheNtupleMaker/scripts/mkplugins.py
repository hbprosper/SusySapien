#!/usr/bin/env python
#------------------------------------------------------------------------------
# create plugins.cc
# Created: 05-Jan-2010 Harrison B. Prosper
# Updated: 15-Feb-2010 HBP - run mkclasslist.py if needed
#          01-May-2011 HBP - split plugin definitions into separate files
#          18-May-2011 HBP - also create plugins.cc and make it optional
#                            whether to compile with it rather than with the
#                            split files
#          19-May-2011 HBP   no longer modify plugins BuildFile, just create
#                            separate plugin files.
#          31-Mar-2012 HBP   use simplified classes.txt format. change name
#                            to classlist.txt
#$Id: mkplugins.py,v 1.19 2011/05/30 14:37:09 prosper Exp $
#------------------------------------------------------------------------------
import os, sys, re
from string import *
from time import *
from PhysicsTools.TheNtupleMaker.Lib import nameonly
from PhysicsTools.TheNtupleMaker.Lib import cmsswProject
#------------------------------------------------------------------------------
if not os.environ.has_key("CMSSW_BASE"):
	print "\t**you must first set up CMSSW"
	sys.exit(0)
	
PACKAGE, SUBPACKAGE, LOCALBASE, BASE, VERSION = cmsswProject()
cwd = os.path.basename(os.environ['PWD'])
if PACKAGE == None:
	print "\n\t** Must be run from package directory"
	sys.exit(0)

LOCALBASE = "%s/src" % os.environ['CMSSW_BASE']

NPLUGINS  = 12 # split plugins into this number of files

# Load classmap.py

cmd = 'find %s/PhysicsTools/TheNtupleMaker -name "classmap.py"' % LOCALBASE
t = map(strip, os.popen(cmd).readlines())
if len(t) == 0:
	print "\n\t** unable to locate classmap.py"\
		  "\t** try running mkclassmap.py to create it"
	sys.exit(0)

mapfile = t[0]
try:
	execfile(mapfile)
except:
	print "\n\t** unable to load classmap.py"
	sys.exit(0)
#------------------------------------------------------------------------------
getlibs = re.compile(r'(?<=name=").*?(?=")')
PLUGINS_BUILDFILE ='''<use   name="FWCore/FWLite"/>
<use   name="FWCore/PluginManager"/>
<use   name="FWCore/MessageLogger"/>

<use   name="PhysicsTools/TheNtupleMaker"/>
<use   name="PhysicsTools/UtilAlgos"/>
<use   name="PhysicsTools/Utilities"/>
<use   name="PhysicsTools/PatUtils"/>

%(pkgs)s
<use   name="lhapdf"/>
<use   name="f77compiler"/>
<use   name="boost"/>
<use   name="boost_python"/>
<use   name="boost_regex"/>
<use   name="rootminuit"/>
<use   name="rootmath"/>
<use   name="rootrflx"/>

<library   file="TheNtupleMaker.cc"></library>
<library   file="TestMethod.cc"></library>
<library   file="*plugin*.cc" name="bufferplugins"></library>
'''
plugins_deflibs = getlibs.findall(PLUGINS_BUILDFILE)

BUILDFILE = '''<use   name="FWCore/Framework"/>
<use   name="FWCore/PluginManager"/>
<use   name="FWCore/MessageLogger"/>
<use   name="FWCore/ParameterSet"/>

<use   name="PhysicsTools/PatUtils"/>
<use   name="PhysicsTools/UtilAlgos"/>
<use   name="CommonTools/Utils"/>
<use   name="L1Trigger/GlobalTriggerAnalyzer"/>
<use   name="HLTrigger/HLTcore"/>

%(pkgs)s
<use   name="hepmc"/>
<use   name="heppdt"/>
<use   name="clhep"/>
<use   name="lhapdf"/>
<use   name="f77compiler"/>
<use   name="boost_python"/>
<use   name="boost_regex"/>
<use   name="rootminuit"/>
<use   name="rootmath"/>
<use   name="boost"/>
<use   name="rootrflx"/>
<lib   name="Gui"/>
<lib   name="Postscript"/>

<export>
  <lib   name="1"/>
</export>
'''
deflibs = getlibs.findall(BUILDFILE)
#------------------------------------------------------------------------------
isfun     = re.compile(r'[0-9]+|bool|short|int|long|float|double'\
					   '|unsigned short|unsigned int|unsigned long')
isSTL     = re.compile(r'std::(vector|set|map|pair)')
isvector  = re.compile(r'(?<=^std::vector\<).+(?=\>)')
isVector  = re.compile(r'(?<=\wVector\<).+(?=\>)')
getwords  = re.compile(r'[a-z]*[:]*[a-zA-Z0-9]+')
hasnspace = re.compile(r'::')
stripme   = re.compile(r'::|<|>|,| ')
findplugins=re.compile(r'<library file=plugins.cc'\
					   '[^<]+?<flags [^<]+?</library>\s',re.M)

# To extract template parameters

templatepar = re.compile('\w[^<]*<(?P<par>[^>]*)>')

templatecmd  = '(?P<template>template'\
                           '\s*(<.*>|([^\{\n]+\n)+)\s*)'
istemplate   = re.compile(templatecmd)
isTemplate   = re.compile(r'(?P<tclass>\w+(::)?\w+)<.*>')
#------------------------------------------------------------------------------
def exclass(x):

	m = isvector.findall(x)
	n = isVector.findall(x)
	if len(m) > 0:
		x = strip(m[0])
		return ("false", x)
	elif len(n) > 0:
		x = strip(n[0])
		return ("false", x)
	else:
		x = strip(x)
		return ("true", x)
#------------------------------------------------------------------------------
# Check if classlist.txt exists
#------------------------------------------------------------------------------
if not os.path.exists("plugins/classlist.txt"):
	print """
	** run mkclasslist.py to create plugins/classlist.txt
	"""
	sys.exit(0)
#------------------------------------------------------------------------------
cnames = map(strip, open("plugins/classlist.txt").readlines())
cnames = map(exclass, cnames)
names  = {'time': ctime(time())}

# Split across several plugin files
npmax = 1 + len(cnames)/NPLUGINS
np = 0
nplugin = 0 # plugin file number

outrecs = []
hdrs  = {}
pkgs  = {}
count = 0
for index, (ctype, name) in enumerate(cnames):

	words = getwords.findall(name)
	classlist = []
	for word in words:
		if isfun.match(word) != None:
			#print "\tFTYPE(%s)" % word
			continue
		elif isSTL.match(word) != None:
			#print "\tSTL(%s)" % word
			continue
		else:
			if not word in classlist:
				classlist.append(word)
				#print "\tCMS(%s)" % word

	headers = []
	for cname in classlist:
		# Find associated header
		if ClassToHeaderMap.has_key(cname):
			header = ClassToHeaderMap[cname]
			if type(header) == type([]): header = header[0]
			if not header in headers:
				headers.append(header)
				# note package and header
				pkg = split(split(header, "/interface")[0], '/src/')[-1]
				pkgs[pkg] = 1
				hdrs[header] = 1
				
	bname = strip(stripme.sub("", name))
	names['buffername'] = bname
	names['classname']  = name
	names['singleton']  = ctype
	names['shortname']  = bname
	shortname = bname

	np += 1
	if np == 1:
		nplugin += 1
		pluginname = "plugins%2.2d" % nplugin
		print "\n=> plugin file plugins/%s.cc" % pluginname 
		out  = open("plugins/%s.cc" % pluginname, "w")
		names['pluginname'] = pluginname
		record = \
'''// -------------------------------------------------------------------------
// File::   %(pluginname)s.cc
// Created: %(time)s by mkplugins.py
// -------------------------------------------------------------------------
#include "PhysicsTools/TheNtupleMaker/interface/Buffer.h"
#include "PhysicsTools/TheNtupleMaker/interface/pluginfactory.h"
// -------------------------------------------------------------------------
'''
		out.write(record % names)
		
	# ===> special processing for Event class
	
	if bname == "edmEvent":
		record = \
'''
#include "PhysicsTools/TheNtupleMaker/interface/BufferEvent.h"
'''
		out.write(record)
	
	count += 1
	print "%5d\t%5d\t%s" % (count, np, name)

	# If there are no headers, don't write out this class
	if len(headers) == 0:
		print "*** no header found for %s" % name
		continue
	# add buffer specific header

	record = '''
typedef Buffer<%(classname)s, %(singleton)s>
%(buffername)s_t;
DEFINE_EDM_PLUGIN(BufferFactory, %(buffername)s_t,
                  "%(buffername)s");
				  ''' % names
	#out.write(record)
	outrecs.append(record)
	
	if np >= npmax or index == len(cnames)-1 :
		np = 0
		headers = hdrs.keys()
		headers.sort()
		header = ""
		for h in headers:
			header += '\n#include "%s"' % h
		out.write(header)
		record = \
'''
// -------------------------------------------------------------------------
'''
		out.write(record)			
		out.writelines(outrecs)
		out.close()
		hdrs = {} # remember to reset
		outrecs = []
		
# Update plugins buildfile

print "\n\t==> Updating BuildFiles"

pkgs = pkgs.keys()
pkgs.sort()

record = ''
for pkg in pkgs:
	if not pkg in plugins_deflibs:
		record += '<use   name="%s"/>\n' % pkg

record = PLUGINS_BUILDFILE % {'pkgs': record}
os.system("cp plugins/BuildFile.xml plugins/BuildFile.xml.backup")
open("plugins/BuildFile.xml", "w").write(record)

record = ''
for pkg in pkgs:
	if not pkg in deflibs:
		record += '<use   name="%s"/>\n' % pkg

record = BUILDFILE % {'pkgs': record}
os.system("cp BuildFile.xml BuildFile.xml.backup")
open("BuildFile.xml", "w").write(record)



