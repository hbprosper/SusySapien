#!/usr/bin/env python
#==============================================================================
# File:        mkxmlfiles.py
#
# Description: Make xml files from headers specified in Project.xml
#              This program *must* be run from the project directory, that is,
#              the directory in which the config directory is located. A
#              project area has the following structure:
#
#              ..<project>/config
#                         /src
#                         /src[/package][/module][/xml]
#
# Created:     14-Jan-2010 Harrison B. Prosper
#              A stripped down version of pyproject.py
#$Revision:$
#==============================================================================
import os, sys, re
from boostlib import \
	 nameonly, \
	 expandNames, \
	 readXmlFile, \
	 validateXmlFile, \
	 hidepath

from sets   import Set
from time   import time, ctime, sleep
from glob   import glob
from getopt import getopt, GetoptError
from elementtree.ElementTree import ElementTree, XML
from string import *
#==============================================================================
# Constants
#==============================================================================
VERSION='$Revision: $'
VERSION=split(VERSION[:-1]).pop()
USAGE = '''
Usage:
   mkxmlfiles.py setup | h2xml
'''

RESET = 1
ADDTO = 0
TARGETS = ['h2xml']
OPTION  = 'shell setup ' + joinfields(TARGETS)

XMLFLAGS   = ['h2xmlflags']

VERBOSE = ""
TEST    = ""
DEBUG   = ""
SHELL   = ""
LOG     = ""
TARGET  = ""
os.system("python -V >& .python_version")
PYVER   = strip(open(".python_version").read())
TAB     = '  '
day, month, daynum, hour, year = split(ctime(time()))
date = '%2s-%3s-%4s' % (daynum,month,year)

# For print statements

NAMES = {'projectfile':'config/Project.xml',
		 'recursemakefile': '$(PYTHON_PROJECTS)/config/main.mk',
		 'xmlmakefile':     '$(PYTHON_PROJECTS)/config/xml.mk',
		 'tab'    : '    ',
		 'pyver'  :    PYVER,
		 'pyversion': sys.version,
		 'libarea':    '$PYTHON_PROJECTS',
		 'time'   :   ctime(time()),
		 'atime'  :   replace(replace(ctime(time()),' ','-'),':','-'),
		 'date'   :   date,
		 'VERSION':   VERSION,
		 'pwd':       os.environ["PWD"],
		 'package': "",
		 'module' : "",
		 'headerfile': "Headers",
		 'srcdir' : 'src',
		 'tmpdir' : 'tmp',
		 'xmldir' : 'xml',
		 'intdir' : 'include',
		 'percent': "%",
		 'file'   : "",
		 'mod'    : 'Module',
		 'space'  : '[ \\t]',
		 'xml'    : 'xml',
		 'problemsfile': 'problems.txt'}

j = rfind(NAMES['pwd'],'/')
NAMES['projectbase'] = NAMES['pwd'][:j]
NAMES['projectpath'] = NAMES['pwd']
NAMES['prjname'] = split(NAMES['pwd'],'/').pop()
#==============================================================================
# Functions
#==============================================================================
def usage():
	print USAGE

def fatal(s):
	print "\n** Error ** %s\n" % s
	sys.exit(1)
#-----------------------------------------------------------------------------
# Fill and (optionally) return a map containing project attributes
#-----------------------------------------------------------------------------
def projectAttributes(projectpath, attrib={}):

	returnAttrib = attrib == {}

	# Project Path

	attrib['projectpath'] = projectpath    
	if not os.path.exists("%(projectpath)s" % attrib):
		fatal("project path %s not found\n" % "%(projectpath)s" % attrib)


	# Project File Path    

	attrib['projectfile'] = NAMES['projectfile']
	attrib['projectfilepath'] = '%(projectpath)s/%(projectfile)s' % attrib    
	if not os.path.exists("%(projectfilepath)s" % attrib):
		fatal("project file %(projectfilepath)s not found\n" % attrib)

	# Load Project

	errstr = validateXmlFile("%(projectfilepath)s" % attrib)
	if errstr != '':
		fatal("\n%s" % errstr)

	project = readXmlFile("%(projectfilepath)s" % attrib)
	if project == None:
		fatal("Unable to read project\n"
			  "\t\tin file %(projectfilepath)s" % attrib)

	# Expand variables and re-read project file

	# Project Globals

	globals = project.findall('globals')
	if len(globals) > 0:
		attrib['globals'] = globals[0].attrib
	else:
		attrib['globals'] = {}

	record = expandNames(open("%(projectfilepath)s" % attrib).read(),
						 attrib['globals'])
	prjfile = '%(projectpath)s/config/.expandedProject.xml' % attrib
	open(prjfile,'w').write(record)
	project = readXmlFile(prjfile)
	if project == None:
		fatal("missing entity: project\n"
			  "\t\tin file %(projectfilepath)s" % attrib)

	attrib['project'] = project

	# Project preface(s) and appendix(s) 

	entity('preface', project, attrib, RESET, kind='list')
	entity('appendix', project, attrib, RESET, kind='list')

	# Project Name

	if project.attrib.has_key('name'):
		attrib['projectname'] = project.attrib['name']
	else:
		fatal("missing field: name\n"
			 "\t\tin project %(projectname)s\n"
			 "\t\t\tin file %(projectfilepath)" % attrib)

	# Project Version

	if project.attrib.has_key('version'):
		attrib['projectversion'] = project.attrib['version']
	else:
		attrib['projectversion'] = attrib['projectname']

	# Project Macros

	entity('macros', project, attrib, RESET, kind='map')

	# Project Options

	entity('options', project, attrib, RESET, kind='map')

	# Project Components

	components = project.findall('component')
	if len(components) == 0:
		fatal("project %(projectname)s must contain at least one component"
			  % attrib)

	attrib['components'] = {}
	attrib['componentnames'] = []

	# Loop over project components

	for component in components:

		if component.attrib.has_key('name'):
			componentname = component.attrib['name']
			attrib['componentnames'].append(componentname)
			attrib['components'][componentname] = component
		else:
			fatal("missing field: name\n"
				  "\t\tin component\n"
				  "\t\t\tin project %(projectname)s\n"
				  "\t\t\t\tin file %(projectfilepath)s" % attrib)

	if returnAttrib: return attrib
#-----------------------------------------------------------------------------
# Extract one or more instances of the specified entity
# kind = simple  extract a single simple entity
#      = list    extract a list of simple entities
#      = map     extract a single map entity
#-----------------------------------------------------------------------------
def entity(name, element, attrib, reset=RESET, **keywords):

	# If we are resetting, delete key name if it exists.
	# Note: Only add a key to attrib map if the element
	# contains the specified entity.

	if reset and attrib.has_key(name): del attrib[name]

	# Possible kinds are: simple, list or map

	if keywords.has_key('kind'):
		kind = keywords['kind']
	else:
		kind = 'simple'

	if keywords.has_key('delim'):  # Needed for "ADDTO" single entity
		delim = keywords['delim']
	else:
		delim = ' '

	elems = element.findall(name)
	if len(elems) == 0: return

	if kind == 'simple':
		e = elems[0]
		if e.attrib.has_key('name'):
			if reset or not attrib.has_key(name):
				attrib[name] = e.attrib['name']
			else:
				attrib[name] = attrib[name] + delim + e.attrib['name']
		else:
			fatal("missing field: name\n\t\tin %s\n" % name)

	elif kind == 'list':
		for index, e in enumerate(elems):
			if (reset or not attrib.has_key(name)) and index == 0:
				attrib[name] = []
			if e.attrib.has_key('name'):
				attrib[name].append(e.attrib['name'])
			else:
				fatal("missing field: name\n\t\tin %s\n" % name)

	elif kind == 'map':
		e = elems[0]
		for index, (key, value) in enumerate(e.attrib.items()):
			if (reset or not attrib.has_key(name)) and index == 0:
				attrib[name] = {}
			attrib[name][key] = value
	else:
		fatal('in entity - "kind" not specified for %s' % name)
#-----------------------------------------------------------------------------
def fullName(names, name):
		prefix = joinfields(names['namespace'],'::')
		if prefix <> '': prefix = prefix + '::'
		return prefix + name
#-----------------------------------------------------------------------------
# Return all header base paths for given component
#-----------------------------------------------------------------------------
def headerBasepaths(nameregex, project):

	cnames = joinfields(project['components'].keys())
	componentnames = re.findall(nameregex, cnames)

	includestr = ""
	paths = Set()
	for componentname in componentnames:
		component = project['components'][componentname]

		includes = component.findall('headers')
		for include in includes:
			if include.attrib.has_key('name'):
				includename = include.attrib['name']
			else:
				fatal("missing field: name in component.headers in\n"
					  "\tcomponent %s" % componentname)

			if include.attrib.has_key('version'):
				includeversion = include.attrib['version']
			else:
				includeversion = includename

			if include.attrib.has_key('basepath'):
				includepath = include.attrib['basepath']
			else:
				fatal("missing field: basepath in\n"
					  "\tcomponent.headers %s\n"
					  "\t\tcomponent %s" % (includename,componentname))

			if not includepath in paths:
				includestr = includestr + " -I%s" % includepath

		# Check for embedded components

		components = component.findall('component')
		for c in components:
			includestr = includestr + headerBasepaths(c.attrib['name'],
													  project)            
	return includestr

#-----------------------------------------------------------------------------
# Get headers for current component
#-----------------------------------------------------------------------------
def headerList(component, attrib):

	headerlst = []
	includes = component.findall('headers')
	for include in includes:

		if include.attrib.has_key('name'):
			includename = include.attrib['name']
		else:
			fatal("missing field: name in\n"
				  "\tcomponent.headers %(module)s" % attrib)

		if include.attrib.has_key('version'):
			includeversion = include.attrib['version']
		else:
			includeversion = includename

		if include.attrib.has_key('basepath'):
			includepath = include.attrib['basepath']
		else:
			fatal("missing field: basepath in\n"
				  "\tcomponent.headers %(module)s" % attrib)

		headers = include.findall("header")
		if len(headers) == 0:
			if attrib['module'] != 'ttypes':                
				fatal("missing entity: header in\n"
					  "\tcomponent.headers: %(module)s" % attrib)

		for header in headers:
			if header.attrib.has_key('name'):
				if header.attrib.has_key('maxdepth'):
					maxdepth = "-maxdepth %s" % header.attrib['maxdepth']
				else:
					maxdepth = ""

				if header.attrib.has_key('exclude'):
					exclude = header.attrib['exclude']
					exclude = joinfields(map(lambda x: strip(x),
											 split(exclude)),'|')
				else:
					exclude = ""
				headerlst.append((includepath,
								  header.attrib['name'],
								  exclude, maxdepth))
			else:
				fatal("missing field: header name in "
					  "component.headers %(module)s" % attrib)

	# Check for imported components

	imports = component.findall('import')
	for imp in imports:
		if imp.attrib.has_key('component'):
			cname = imp.attrib['component']
			if cname == component.attrib['name']:
			   fatal("circular import of component %s.\n"
					 "\t\tGo boil your head!\n" % cname)

			if attrib['components'].has_key(cname):
				comp = attrib['components'][cname]
				headerlst = headerlst + headerList(comp, attrib)
			else:
				fatal("missing component: %s\n" % cname)
		else:
			fatal("missing field: component\n"
				  "\t\tin import entity\n"
				  "\t\t\tin component %s" % component.attrib['name'])
	return headerlst


def wrsplit(s,delim=' \\',tab=11):
	recs = []
	lstr = len(s)
	return s

def gmake(wdir, target, names):
	names['verbose']= replace(names['verbose'],'-v','verbose=yes')
	names['target'] = target
	names['wdir']   = wdir
	command='''
	gmake --no-print-directory \
	-C %(bldpath)s/%(wdir)s %(verbose)s %(target)s
	''' % names
	os.system(command)
	

def mkmake(names):
	outfile = "GNUmakefile"
	out = open(outfile,"w")
	out.write("#" + 78*'-' + '\n')
	out.write("# GNUmakefile\n")
	out.write("# Created:     %(time)s with mkxmlfiles.py\n" % names)
	out.write("#" + 78*'-' + '\n')
	out.write("ifndef PYTHON_PROJECTS\n")
	out.write("$(error PYTHON_PROJECTS is undefined)\n")
	out.write("endif\n")
	out.write("#" + 78*'-' + '\n')
	out.write('include %(recursemakefile)s\n' % names)
	out.close()
	
env = re.compile('(?P<name>(?<=\$)\\b\w+\\b)',re.M)
def reformenv(name):
	all = env.findall(name)
	for x in all:
		name = replace(name, x, '(%s)' % x)
	return name

#-----------------------------------------------------------------------------
def compilerLinkerInput(component, attrib, flaglist, reset=RESET):
	# -------------------------------------------------------
	# If component contains the options entity it adds to
	# the global options.
	# -------------------------------------------------------

	# Remove keys if they exist, so that we reset to initial
	# state
	if reset: 
		for flag in flaglist:
			if attrib.has_key(flag):
				del attrib[flag]

	# Options exist so add to existing ones

	options = component.findall('options')
	for option in options:
		for flag in flaglist:
			if option.attrib.has_key(flag):
				if option.attrib.has_key('override') and \
				   lower(option.attrib['override']) <> "no":
					attrib[flag] = joinfields(split(option.attrib[flag]))
				else:
					if not attrib.has_key(flag): attrib[flag] = ''
					attrib[flag] = "%s %s" % \
								   (attrib[flag],
									joinfields(split(options[0].attrib[flag])))

	# Check for imported components

	imports = component.findall('import')
	for imp in imports:
		if imp.attrib.has_key('component'):
			cname = imp.attrib['component']
			if cname == component.attrib['name']:
				fatal("circular import of component %s.\n"
					  "\t\tGo boil your head!\n" % cname)

			if attrib['components'].has_key(cname):
				comp = attrib['components'][cname]
				# NB: Do not reset!
				compilerLinkerInput(comp, attrib, flaglist, ADDTO) 
			else:
				fatal("missing component: %s\n" % cname)
		else:
			fatal("missing field: component\n"
				  "\t\tin import entity\n"
				  "\t\t\tin component %s" % component.attrib['name'])

#-----------------------------------------------------------------------------
# Command processor for pyproject
#-----------------------------------------------------------------------------
class Command:
	'''
	Command processor for mkxmlfiles.py
	'''

	def __init__(self, cmds):
		pass
		
	def close(self):
		pass

	def __del__(self):
		pass

#----------------------------------------------------------------------------
	def addflags(self, names, out, flaglist):

		addline = False
		for index, flag in enumerate(flaglist):

			if names.has_key(flag):
				values= split(names[flag])
				if len(values) == 0: continue

				addline = True

				str = "%-8s:= " % flag

				value = values[0]

				record='%s%s' % (str, reformenv(value))
				lstr  = len(record)
				out.write(record)
				used = [value]
				if len(values) > 1:
					for value in values[1:]:
						if not value in used:
							used.append(value)
							record = ' %s' % reformenv(value)
							if lstr + len(record) > 50:
								record = ' \\\n\t  ' + record
								lstr = len(record)
								out.write(record)
							else:
								lstr += len(record)
								out.write(record)
				out.write('\n')

				if index < len(flaglist)-1: out.write('\n')

		if ( addline): out.write("#" + 78*'-' + '\n')
#-----------------------------------------------------------------------------
	def setup(self, names):

		# Create directories

		s = ''
		for d in names['dirs']:
			s += ' mkdir -p %s;' % d
		names['mkdir'] = s
		
		command = '''
		cd %(projectpath)s
		mkdir -p %(bldpath)s;
		''' % names

		if len(names['headerlist']) > 0:
			command += '''
			cd %(bldpath)s;
			%(mkdir)s
			(''' % names

			pathmap = {}
			for headerpath, regex, exregex, maxdepth in names['headerlist']:
				names['headerpath'] = headerpath

				if not pathmap.has_key(headerpath):
					cmd = 'find %(headerpath)s -maxdepth 0 2>/dev/null' % names
					pathmap[headerpath] = strip( os.popen(cmd).read() )

			names['regex'] = regex
			names['maxdepth'] = maxdepth
			if exregex <> "":
				names['exclude'] = '| egrep -v "%s"' % exregex
			else:
				names['exclude'] = ''

			if names['noclasstest']:
				command = command + \
			'''find %(headerpath)s -regex "%(headerpath)s/%(regex)s" -follow \\
			%(maxdepth)s %(exclude)s;
			''' % names            
			else:
				command = command + 'find %(headerpath)s '
				command = command + '-regex "%(headerpath)s/%(regex)s" '
				command = command + '-exec egrep -l '
				command = command + \
						  '"^%(space)s*(class|struct)%(space)s+[^;]+$" {} \\;'
				command = command + ' ;\n'
				command = command % names
		##end for
		if len(names['headerlist']) > 0:
			command = rstrip(command)
			command = command[:len(command)-1]
			command = command + ') > %(headerfile)s'
			command = command % names

		# Ok! Now execute plan

		os.system(command)
		if len(names['headerlist']) == 0: return

		filename= '%(bldpath)s/%(headerfile)s' % names
		headers = open(filename).readlines()
		for header in headers:
			header = strip(header)
			name = nameonly(header)
			names['name'] = name
			names['header'] = hidepath(header, pathmap)
			txtfile = '%(bldpath)s/%(xmldir)s/%(name)s.txt' % names
			open(txtfile,'w').write('%(header)s\n' % names)

		command = "cd %(bldpath)s; rm -rf %(headerfile)s" % names
		os.system(command)

		# Create makefiles

		self.mkxmlmake(names)
		
	def h2xml(self, names):
		gmake('xml', 'h2xml', names)

		
	def mkxmlmake(self, names):
		outfile = "%(bldpath)s/xml/GNUmakefile" % names
		out = open(outfile,"w")
		out.write("# GNUmakefile to create XML files\n")
		out.write("# Created: %(time)s with mkxmlfiles.py "
				  "%(VERSION)s\n" % names)
		out.write("#$Revision: 1.12 $\n")
		out.write("#" + 78*'-' + '\n')
		out.write("ifndef PYTHON_PROJECTS\n")
		out.write("$(error PYTHON_PROJECTS is undefined)\n")
		out.write("endif\n")
		out.write("#" + 78*'-' + '\n')

		# Setup compiler/linker inputs.

		compilerLinkerInput(names['project'], names, XMLFLAGS, RESET)
		compilerLinkerInput(names['component'], names, XMLFLAGS, ADDTO)

		self.addflags(names, out, XMLFLAGS)
		out.write("include %(xmlmakefile)s\n" % names)
		out.close()                    
 
	def shell(self, names):
		names['shell'] = names['commands'][1]
		command = '''
		cd %(bldpath)s; %(shell)s; echo ""
		''' % names
		if self.activeMode: os.system(command)

#==============================================================================
# Main Program
#==============================================================================
def main():

	# Make sure we're in the right directory

	if not os.path.exists('%(projectfile)s' % NAMES):
		print "\nmkxmlfiles.py must be run from a project directory!\n"
		sys.exit(1)

	if len(sys.argv[1:]) == 0:
		print USAGE
		sys.exit(0)
		
	NAMES['commands'] = sys.argv[1:]
	
	# Get project attributes. Project Path is working directory

	projectAttributes(NAMES['projectpath'], NAMES)

	# Get commands and determine which components to process

	COMPLIST = NAMES['componentnames']


	# ------------------------------------------------------------------
	# Display splash screen
	# ------------------------------------------------------------------
	print ""
	print "Python Project"
	print "\tName:    %(projectname)s"    % NAMES
	if NAMES['projectversion'] <> NAMES['projectname']:
		print "\tVersion: %(projectversion)s" % NAMES
	print "\tBase:    %(projectbase)s"    % NAMES
	print "\tPath:    %(projectpath)s"    % NAMES
	print "\tPython:  %(pyver)s"   % NAMES
	print 79*"-"
	print "\tCommands"
	print NAMES['commands']
	print 79*"-"
	print ""

	# ------------------------------------------------------------------
	# Loop over components and perform specified actions
	# ------------------------------------------------------------------

	command = Command(NAMES)

	if 'setup' == NAMES['commands'][0]: mkmake(NAMES)

	project = NAMES['project']

	# Start loop
	#-----------
	for index, componentname in enumerate(COMPLIST):

		if not NAMES['components'].has_key(componentname):
			fatal("component %s not found" % componentname)

		NAMES['count']   = index+1
		NAMES['componentname'] = componentname

		component = NAMES['components'][componentname]
		NAMES['component'] = component

		if component.attrib.has_key('package'):
			package = component.attrib['package']
		else:
			package = ""
		NAMES['package'] = package

		if component.attrib.has_key('module'):
			module = component.attrib['module']
		else:
			fatal("module for component %s not found" % componentname)

		NAMES['module'] = module

		# Get sub-directories for component. By default we create
		# xml
		NAMES['dirs'] = ['xml']
		if component.attrib.has_key('dirs'):
			NAMES['dirs'] += split(component.attrib['dirs'])

		# Check for null module

		# ------------------------------------------------------------------
		# Construct build path, that is, the directory in which the build
		# occurs. This is the directory in which the xml directory is
		# located.
		# ------------------------------------------------------------------

		# All paths relative to the project area

		paths = ["src"]
		if package <> "": paths.append(package)
		if module <> "": paths.append(module)
		NAMES['bldpath'] = joinfields(paths,'/')

		str = "%(count)5d\tProcessing: %(bldpath)s" % NAMES
		if VERBOSE == "": print str

		# Get possible prefaces for component

		entity('preface', project,   NAMES, RESET, kind='list')
		entity('preface', component, NAMES, ADDTO, kind='list')

		# Get possible appendices for component

		entity('appendix', project,   NAMES, RESET, kind='list')
		entity('appendix', component, NAMES, ADDTO, kind='list')

		# Get possible macros for component

		entity('macros', project,   NAMES, RESET, kind='map')
		entity('macros', component, NAMES, ADDTO, kind='map')

		# Get list of headers for current component

		headers = headerList(component, NAMES)
		if len(headers) == 0:
			fatal('missing headers or components\n'
				  '\t\tin component %s' % componentname)
			NAMES['include'] = []
			NAMES['headerlist'] = []
		else:
			NAMES['include'] = headerBasepaths(componentname, NAMES)
			NAMES['headerlist'] = headers

		if LOCALDEBUG:
			print "\n\tComponent %(componentname)s Attributes\n" % NAMES
			print pp.pprint(NAMES)
			print ""

		# Okay, let's get to work!

		if 'shell' == NAMES['commands'][0]:
			command.shell(NAMES)

		else:
			for cmd in NAMES['commands']:
				cmd = 'command.%s(NAMES)' % cmd
				eval(cmd)

	command.close()
	if LOG: LOG.close()
# ------------------------------------------------------------------    
main()


