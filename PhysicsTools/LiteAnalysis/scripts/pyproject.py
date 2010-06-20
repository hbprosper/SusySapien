#!/usr/bin/env python
#==============================================================================
# File:        pyproject.py
#
# Description: Manage Python Projects. This program *must* be run from
#              the project directory, that is, the directory in which the
#              config directory is located. A project area has the following
#              structure:
#
#              ..<project>/config
#                         /src
#                         /src[/package][/module]
#                         /src[/package][/module][/python]
#                         /src[/package][/module][/xml]
#
#               and optionally
#
#                         /src[/package][/module][/src]
#                         /src[/package][/module][/include]
#                         /src[/package][/module][/test]
#               
# Created:     23-Oct-2003 Harrison B. Prosper, CERN
#              02-Dec-2003 HBP, Handle global and local includes
#              09-May-2004 HBP, Add src dir to module level
#              17-Sep-2004 HBP, Minor change to structure
#              19-Nov-2004 HBP, swap meaning of src and interface directories
#                          to be consistent with CMS/SCRAM conventions
#              23-Oct-2005 HBP, Decouple a bit from CMS structure
#              25-May-2006 HBP, Use environment variables in setup(..)
#              25-Sep-2008 HBP, Fix bug in addflags
#$Revision: 1.2 $
#==============================================================================
import os, sys, re, shelve
from boostlib import \
	 nameonly, \
	 expandNames, \
	 readXmlFile, \
	 validateXmlFile, \
	 decodeCallback, \
	 hidepath

from popen2 import popen3
from sets   import Set
from time   import time, ctime, sleep
from glob   import glob
from getopt import getopt, GetoptError
from pprint import PrettyPrinter
from elementtree.ElementTree import ElementTree, XML
from string import *
pp = PrettyPrinter()
#==============================================================================
# Constants
#==============================================================================
VERSION='$Revision: 1.2 $'
VERSION=split(VERSION[:-1]).pop()
USAGE = '''
Usage:
   pyproject setup | config | h2xml | xml2b | template | build
                   | clean  | veryclean | shell  | test
			 [-h]                  Print this
			 [-v]                  Verbose mode: echo all commands
			 [-V]                  Print version number
			 [-T]                  Test mode: echo commands but do nothing
			 [-F]                  Force building of XML or export file
			 [-N]                  No class test during scan of headers
			 [-d<number>]          Debug mode
			 [-t]<target>          Target for build command
			 [-m]<module>          
			 [-e]<Module>          Exclude module
'''

SHORTOPTIONS = 'FVTNvhd:t:m:e:'
LOCALDEBUG = 0
RESET = 1
ADDTO = 0
TARGETS = ['build','check','h2xml','template',
		   'xml2b','clean','veryclean', 'show']
OPTION  = 'shell setup config ' + joinfields(TARGETS)

PYFLAGS    = ['xml2bflags', 'xmlflags', 'compiler','cppflags',
			  'cxxflags','fflags','cflags','ldflags','libs',
			  'withroot',
			  'withmodule']

XMLFLAGS   = ['h2xmlflags']

LIBFLAGS   = ['compiler','cppflags',
			  'cxxflags','fflags','cflags','ldflags','libs']

FORCE   = ""
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
# 'pyver'  :    'python%s' % strip(splitfields(sys.version,' ',1)[0]),
NAMES = {'projectfile':'config/Project.xml',
		 'recursemakefile': '$(PYTHON_PROJECTS)/config/main.mk',
		 'libmakefile':     '$(PYTHON_PROJECTS)/config/library.mk',
		 'xmlmakefile':     '$(PYTHON_PROJECTS)/config/xml.mk',
		 'boostmakefile':   '$(PYTHON_PROJECTS)/config/boost.mk',
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
		 'commonpath' : 'src/ttypes',  
		 'percent': "%",
		 'file'   : "",
		 'mod'    : 'Module',
		 'prb'    : 'Failures',
		 'noc'    : '',
		 'con'    : '',
		 'bad'    : '',
		 'cnt'    : 'OK',
		 'tot'    : 'Total',
		 'max'    : 'Maximum',
		 'space'  : '[ \\t]',
		 'xml'    : 'xml',
		 'wrapperpostfix': '_wrap',    # For swig 
		 'problemsfile': 'problems.txt',
		 'countsfile':   'config/counts.txt',
		 'dbfile':  'config/TypesDB'}

j = rfind(NAMES['pwd'],'/')
NAMES['projectbase'] = NAMES['pwd'][:j]
NAMES['projectpath'] = NAMES['pwd']
NAMES['prjname'] = split(NAMES['pwd'],'/').pop()

# Used in extractNames
CLASSTYPE=1
FUNCTYPE=2
ENUMTYPE=3
METHTYPE=4
TYPEDEFTYPE=5
#==============================================================================
# Functions
#==============================================================================
def usage():
	print USAGE

def fatal(s):
	print "\n** Error ** %s\n" % s
	sys.exit(1)

firstword = re.compile(r'^\w+',re.M)
skipdir   = re.compile(r'/(include|interface|copy|config|'\
					   'doc|test|logs|work|tmp|xml|python)$')
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

	# Add component "ttypes"

	str = '''
	<component name="ttypes" module="ttypes">
	   <headers name="ttypes" basepath="">
	   </headers>
	</component>
	'''
	cstr = XML(str)
	components.append(cstr)

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
# Traverse tree and extract names
#-----------------------------------------------------------------------------
striptemplate = re.compile(r'\b(template|class|typename)\b[ ]*',re.M)
def extractNames(names, elem, elist, depth=0):
	depth += 1
	if depth == 1:
		names['namespace'] = []
		names['classspace'] = []
		names['methods'] = {}
		names['typedefs']= {}

	elif depth >= 50:
		fatal("lost in tree...I'm bailing!\n\t\t%s" % element.tag)

	names['depth'] = depth

	for ielement, element in enumerate(elem.getchildren()):

		if element.attrib.has_key("name"):
			name = element.attrib["name"]
			# Strip away possible template parameters
			tmp = firstword.findall(name)
			if len(tmp) == 1: name = tmp[0]
		else:
			name = ''

		names['name'] = name

		if VERBOSE: print "%s<%s> %s" % (depth*TAB, element.tag, name),

		if element.tag == "namespace":
			if VERBOSE: print ""

			names['namespace'].append(name)
			extractNames(names, element, elist, depth)
			names['namespace'].pop()

		elif element.tag == "section":                
			if VERBOSE: print ""
			extractNames(names, element, elist, depth)

		elif element.tag == "class":

			fullname = fullName(names, name)

		   # Could be a template; if so, append template parameters

			if find(element.attrib['type'],'template') > -1:
				tpar = element.attrib['template']
				tpar = striptemplate.sub('',tpar)
				fullname = fullname + tpar
			else:
				tpar = ''

			if VERBOSE: print " --> %s" % fullname

			basecomps = element.findall('baseclass')
			basenames = []
			for basecomp in basecomps:
				basename = basecomp.attrib['name']
				if VERBOSE: print "%s%s" % (depth*'  ', basename)
				basenames.append(basename)
			elist.append((CLASSTYPE,element, name, fullname, basenames))

			# Initialize map for class methods and typedefs
			names['methods'][name] = []
			names['typedefs'][name]= []

			# Look for possible nested classes
			names['classspace'].append(name)
			names['namespace'].append(name+tpar) # Use complete name
			extractNames(names, element, elist, depth)
			names['namespace'].pop()
			names['classspace'].pop()

		elif element.tag in ["method1", "method2","constructor"]:

			classname = names['classspace'][-1]
			if names['methods'].has_key(classname):
				signature = methodFullSignature(element.text)
				if VERBOSE: print " --> %s" % signature                
				names['methods'][classname].append(signature)

		elif element.tag == "function1" or element.tag == "function2":

			fullname = fullName(names, name)
			if VERBOSE: print " --> %s" % fullname

			elist.append((FUNCTYPE,element, name, fullname, None))

		elif element.tag == "enum":
			if name == '': continue

			fullname = fullName(names, name)
			if VERBOSE: print " --> %s" % fullname

			elist.append((ENUMTYPE,element, name, fullname, None))

		elif element.tag == "typedef":

			record = strip(replace(element.text,'typedef ',''))
			t = decodeCallback(record)
			if t <> None:
				r, name, a, v, q = t
				element.text = record
			else:
				t = split(record)
				name = strip(replace(t[-1],';',''))
				element.text = joinfields(t[:-1])

			fullname = fullName(names, name)

			if VERBOSE: print " --> %s" % fullname
			elist.append((TYPEDEFTYPE, element, name, fullname, None))

			if names['classspace'] <> []:
				classname = names['classspace'][-1]
				if names['typedefs'].has_key(classname):
					names['typedefs'][classname].append(fullname)

		else:
			if VERBOSE: print ""

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

#-----------------------------------------------------------------------------
#
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

def wrlog(s):
	global LOG
	if LOG: LOG.write("%s\n" % rstrip(s))
	if VERBOSE <> "": print s

def wrsplit(s,delim=' \\',tab=11):
	recs = []
	lstr = len(s)
	return s

def undefinedReferences(errors):
	undefs = []
	groups = re.findall("(?<=undefined reference to \`).*?(?=\')",errors)
	if len(groups) > 0:
		for name in groups:
			if name <> 'main':
				undefs.append(name)
	return undefs

def gmake(wdir, target, activeMode, names):
	names['verbose']= replace(names['verbose'],'-v','verbose=yes')
	names['target'] = target
	names['wdir']   = wdir
	command='''
	echo " " >> logs/%(target)s-%(atime)s.log
	date     >> logs/%(target)s-%(atime)s.log
	echo "%(bldpath)s/%(wdir)s" >> logs/%(target)s-%(atime)s.log
	gmake --no-print-directory \
	-C %(bldpath)s/%(wdir)s %(verbose)s %(target)s | tee -a \
	logs/%(target)s-%(atime)s.log 
	''' % names
	wrlog(command)
	if activeMode: os.system(command)    

env = re.compile('(?P<name>(?<=\$)\\b\w+\\b)',re.M)
def reformenv(name):
	all = env.findall(name)
	for x in all:
		name = replace(name, x, '(%s)' % x)
	return name

def addEntryToDB(fullname, d, entries):                    
	newentry = 1
	for e in entries:
		if fullname == e['fullname']:
			newentry = 0
			break
	if newentry: entries.append(d)                        
#-----------------------------------------------------------------------------
# Command processor for pyproject
#-----------------------------------------------------------------------------
class Command:
	'''
	Command processor for pyproject
	'''

	def __init__(self, cmds):
		testMode = cmds['test']

		self.activeMode = not testMode
		self.sumcnt = 0
		self.sumnoc = 0
		self.sumcon = 0
		self.sumbad = 0
		self.sumtot = 0
		self.summax = 0
		self.opened = 0

		# Open XML database

		if not testMode:
			if 'update' in cmds['commands']:
				try:
					print "Open XML database"
					self.xmlDB = XMLdb()
					self.opened= 1
				except:
					fatal("can't open Python XML database")

	def close(self):
		if self.opened:
			print "Close XML database"
			self.xmlDB.close()
		pass

	def __del__(self):
		pass

	def setup(self, names):
		s = ''
		for d in names['dirs']:
			s += ' mkdir -p %s;' % d
		names['mkdir'] = s

		command = '''
		cd %(projectpath)s
		mkdir -p %(libpath)s;
		mkdir -p %(bldpath)s;
		mkdir -p %(commonpath)s/python;
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

		wrlog(command)

		if self.activeMode:
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

	def veryclean(self, names):
		gmake('src','clean', self.activeMode, names)
		gmake('xml','veryclean', self.activeMode, names)

	def clean(self, names):
		gmake('src','clean', self.activeMode, names)
		gmake('xml','clean', self.activeMode, names)
		gmake('python','clean', self.activeMode, names)

	def h2xml(self, names):
		gmake('xml', 'h2xml', self.activeMode, names)

	def xml2b(self, names):
		gmake('python', 'xml2b', self.activeMode, names)

	def template(self, names):
		gmake('python', 'template', self.activeMode, names)        

	def check(self, names):
		gmake('python', 'check', self.activeMode, names)

	def build(self, names):
		names['verbose'] = replace(names['verbose'],'-v','verbose=yes')
		gmake('python', 'build', self.activeMode, names)

	def mkmake(self, names, wdir, subdir, depth=0):
		depth += 1
		if depth > 1: return

		outfile = wdir + "/GNUmakefile"

		out = open(outfile,"w")
		out.write("#" + 78*'-' + '\n')
		out.write("# GNUmakefile:\n")
		path = replace(wdir, names['projectbase']+'/','')
		out.write("# \t%s\n" % path)
		out.write("# Created:     %(time)s with pyproject.py"
				  " %(VERSION)s\n" % names)
		out.write("#$Revision: 1.2 $\n")
		out.write("#" + 78*'-' + '\n')
		out.write("ifndef PYTHON_PROJECTS\n")
		out.write("$(error PYTHON_PROJECTS is undefined)\n")
		out.write("endif\n")
		out.write("#" + 78*'-' + '\n')
		out.write('include %(recursemakefile)s\n' % names)
		out.close()

		for d in subdir:
			d = wdir + '/' + d
			t = filter(lambda x: os.path.isdir(x), glob("%s/*" % d))
			t = filter(lambda x: skipdir.search(x) == None, t)
			if len(t) > 0:
				sdir = map(lambda x: split(x,'/').pop(), t)
			else:
				sdir = []

			# Make sure we exclude src, include and test directories

			sdir = filter(lambda x: x not in ['src','include','test'], sdir)
			self.mkmake(names, d, sdir, depth)

	def mkxmlmake(self, names):
		outfile = "%(bldpath)s/xml/GNUmakefile" % names
		out = open(outfile,"w")
		out.write("# GNUmakefile to create XML files\n")
		out.write("# Created: %(time)s with pyproject.py "
				  "%(VERSION)s\n" % names)
		out.write("#$Revision: 1.2 $\n")
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

	def mklibmake(self, names):
		outfile = "%(bldpath)s/src/GNUmakefile" % names
		out = open(outfile,"w")
		out.write("# GNUmakefile to create lib%(module)s library\n" % names)
		out.write("# Created: %(time)s with pyproject.py "
				  "%(VERSION)s\n" % names)
		out.write("#$Revision: 1.2 $\n")
		out.write("#" + 78*'-' + '\n')
		out.write("ifndef PYTHON_PROJECTS\n")
		out.write("$(error PYTHON_PROJECTS is undefined)\n")
		out.write("endif\n")
		out.write("#" + 78*'-' + '\n')
		# Setup compiler/linker inputs.

		compilerLinkerInput(names['project'], names, LIBFLAGS, RESET)
		compilerLinkerInput(names['component'], names, LIBFLAGS, ADDTO)

		self.addflags(names, out, LIBFLAGS)
		out.write("include %(libmakefile)s\n" % names)
		out.close()                    

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

	def config(self, names):

		if 'xml' in names['dirs']: self.mkxmlmake(names)

		# Setup compiler/linker inputs.

		compilerLinkerInput(names['project'], names, PYFLAGS, RESET)
		compilerLinkerInput(names['component'], names, PYFLAGS, ADDTO)

		wrlog('\t\tCompiler Input')
		for flag in PYFLAGS:
			if names.has_key(flag):
				wrlog('\t\t\t%s:\t%s' % (flag,names[flag]))

		if self.activeMode:

			# Write GNUmakefile

			outfile = "%(bldpath)s/python/GNUmakefile" % names
			out = open(outfile,"w")
			out.write("# GNUmakefile to create Python modules\n")
			out.write("# Created: %(time)s with pyproject.py "
					  "%(VERSION)s\n" % names)
			out.write("#$Revision: 1.2 $\n")
			out.write("#" + 78*'-' + '\n')
			out.write("ifndef PYTHON_PROJECTS\n")
			out.write("$(error PYTHON_PROJECTS is undefined)\n")
			out.write("endif\n")
			out.write("#" + 78*'-' + '\n')
			if names.has_key('macros'):
				keys = names['macros'].keys()
				keys.sort()
				for key in keys:
					value = names['macros'][key]
					out.write("%s\t:= %s\n" % (key, value))
				out.write("#" + 78*'-' + '\n')

			self.addflags(names, out, PYFLAGS)

			out.write("include %(boostmakefile)s\n" % names)
			out.close()            

	def test(self, names):
		command = '''
		echo " " >> logs/test-%(atime)s.log
		date     >> logs/test-%(atime)s.log
		cd %(bldpath)s;
		echo "from %(projectname)s.%(module)s import *" > test.py;
		python test.py >2 %(projectpath)s/logs/test-%(atime)s.log;
		echo ""
		''' % names
		status = os.system(command)

	def countbegin(self, names):
		self.out = open("%(countsfile)s" % names, "w")
		s = "%(mod)-46s%(prb)9s %(cnt)7s %(tot)7s %(max)7s" % NAMES
		self.out.write("Project: %(projectpath)s\n" % NAMES)
		self.out.write("Created: %(time)s\n"  % NAMES)
		self.out.write(79*'-' + '\n')
		self.out.write("%s\n" % s)
		s = "%-46s %2s %2s %2s" % ("","N","C","I")
		self.out.write("%s\n" % s)
		self.out.write(79*'-' + '\n')

		self.sumcnt = 0
		self.sumnoc = 0
		self.sumcon = 0
		self.sumbad = 0
		self.sumtot = 0
		self.summax = 0

	def count(self, names):
		command = '('
		for headerpath, regex, exregex, maxdepth in names['headerlist']:
			names['headerpath'] = headerpath
			names['regex']      = regex
			if exregex <> "":
				names['exclude'] = '| egrep -v "%s"' % exregex
			else:
				names['exclude'] = ''
			names['maxdepth']   = maxdepth

			command = command + \
			'''find %(headerpath)s -regex "%(headerpath)s/%(regex)s"\
			-exec egrep -l "^%(space)s*class%(space)s+[^;]+$" {} \;\
			%(maxdepth)s %(exclude)s;
			''' % names
		command = rstrip(command)
		command = command[:len(command)-1]
		command = command + ') | wc -l'
		wrlog(command)

		if self.activeMode:
			maxcnt = map(lambda x: atoi(strip(x)),
						 os.popen(command).readlines())[0]
		command = '''
		cd %(bldpath)s        
		ls -1 %(xmldir)s/*.xml | wc -l''' % names
		if os.path.exists("%(bldpath)s/%(problemsfile)s" % names):
			command = command + '''
			grep NOC %(problemsfile)s | wc -l;
			grep CON %(problemsfile)s | wc -l;
			grep BAD %(problemsfile)s | wc -l;
			''' % names            
		wrlog(command)

		if self.activeMode:
			records = map(lambda x: atoi(strip(x)),
						  os.popen(command).readlines())
			if len(records) == 4:
				cnt, noc, con, bad = records
			else:
				cnt = records[0]
				noc = con = bad = 0

			tot = sum(records)
			names['cnt'] = cnt
			names['noc'] = noc
			names['con'] = con
			names['bad'] = bad
			names['tot'] = tot
			names['max'] = maxcnt
			s = "%(bldpath)-46s" % names
			s = s + " %(noc)2d %(con)2d %(bad)2d %(cnt)7d" % names
			s = s + " %(tot)7d %(max)7d" % names
			self.out.write("%s\n" % s)

			self.sumcnt += cnt
			self.sumnoc += noc
			self.sumcon += con
			self.sumbad += bad
			self.sumtot += tot
			self.summax += maxcnt

	def countend(self, names):
		if self.activeMode:
			names['module'] = ''
			names['cnt'] = self.sumcnt
			names['noc'] = self.sumnoc
			names['con'] = self.sumcon
			names['bad'] = self.sumbad
			names['tot'] = self.sumtot
			names['max'] = self.summax
			s = 79*'-' + '\n'
			s = s + "%(module)46s" % names
			s = s+" %(noc)2d %(con)2d %(bad)2d %(cnt)7d %(tot)7d %(max)7d" % \
				names
			self.out.write("%s\n" % s)
			self.out.close()

	def shell(self, names):
		names['shell'] = names['commands'][1]
		command = '''
		cd %(bldpath)s; %(shell)s; echo ""
		''' % names
		wrlog(command)
		if self.activeMode: os.system(command)

	def list(self, names):
		command= '''
		find src/* -name "Classes" -exec egrep "*" {} \; \
		> config/Classes'''
		wrlog(command)
		if self.activeMode: os.system(command)        

	def mkdb(self, names):

		import cPickle

		#---------------------------------------------------------
		# make types database
		#---------------------------------------------------------

		db = {'ClassDB':{},
			  'FunctionDB':{},
			  'EnumDB':{},
			  'TypedefDB':{}}

		wrlog("building database")

		print "Scanning xml files.."        
		cmd = 'find %(projectpath)s/src/* -name "*.xml"' % names
		filelist = map(lambda x: strip(x),os.popen(cmd).readlines())

		print "Building database.."
		for index, xmlfilepath in enumerate(filelist):

			headercomponent = readXmlFile(xmlfilepath)
			if headercomponent == None: fatal("Can't read %s" % xmlfilepath)

			xmlfile = split(xmlfilepath,'/src/').pop()
			if VERBOSE: print "\n<%s>\t%d" % (xmlfile,index+1)

			# Get full module name

			module = names['prjname'] + '.' + \
					 replace(split(xmlfile,'/xml/')[0],'/','.')

			# Get header relative and absolute paths

			header = headercomponent.attrib['name']
			try:
				headerfilepath = headercomponent.attrib['file']
			except:
				fatal('field "file" not found in %s' % xmlfile)

			# Get list of names from component

			namelist = []
			extractNames(names, headercomponent, namelist)

			# Get list of forwarded classes

			forwcomps = headercomponent.findall('forward')
			forwards = []
			for forwcomp in forwcomps:
				forward = forwcomp.attrib['name']
				forwards.append(forward)

			# Sort according to component type

			for ctype, component, name, fullname, basenames in namelist:

				if ctype == CLASSTYPE:

					classtype = component.attrib['type']

					basenames = map(lambda x:
									strip(splitfields(x,' ',1)[1]), basenames)

					if names['methods'].has_key(name):
						methods = names['methods'][name]
					else:
						methods = []

					if names['typedefs'].has_key(name):
						typedefs = names['typedefs'][name]
					else:
						typedefs = []

					if not db['ClassDB'].has_key(name):
						db['ClassDB'][name] = []

					d = {'xmlfile':   xmlfilepath,
						 'headerfile':headerfilepath,
						 'header':    header,
						 'module':    module,
						 'fullname':  fullname,
						 'bases':     basenames,
						 'methods':   methods,
						 'forwards':  forwards,
						 'typedefs':  typedefs,
						 'type':      classtype}

					addEntryToDB(fullname, d, db['ClassDB'][name])

				elif ctype == ENUMTYPE:

					if not db['EnumDB'].has_key(name):
						db['EnumDB'][name] = []

					d = {'xmlfile':   xmlfilepath,
						 'headerfile':headerfilepath,
						 'header':    header,
						 'module':    module,
						 'fullname':  fullname}

					addEntryToDB(fullname, d, db['EnumDB'][name])

				elif ctype == FUNCTYPE:

					if not db['FunctionDB'].has_key(name):
						db['FunctionDB'][name] = []

					d = {'xmlfile':   xmlfilepath,
						 'headerfile':headerfilepath,
						 'header':    header,
						 'module':    module,
						 'fullname':  fullname}

					addEntryToDB(fullname, d, db['FunctionDB'][name])

				elif ctype == TYPEDEFTYPE:

					if not db['TypedefDB'].has_key(name):
						db['TypedefDB'][name] = []

					d = {'xmlfile':   xmlfilepath,
						 'headerfile':headerfilepath,
						 'header':    header,
						 'module':    module,
						 'fullname':  fullname,
						 'type':      component.text}

					addEntryToDB(fullname, d, db['TypedefDB'][name])

		# Make database

		print "Saving database.."
		out = open('config/.TypesDB','w')
		cPickle.dump(db,out)
		out.close()

		file = open('config/.TypesDB','r')
		db   = cPickle.load(file)
		out = open('config/TypesDB','w')
		out.write(79*"#"+'\n')
		out.write("# File:    TypesDB\n" % names)
		out.write("# Created: %(date)s, pyproject.py"
				  " %(VERSION)s\n" % names)
		out.write("#$Revision: 1.2 $\n")
		out.write(79*"#"+'\n')
		PP = PrettyPrinter(stream=out)
		out.write("ClassDB =\\\n")
		PP.pprint(db['ClassDB'])
		out.write("\n")

		out.write("EnumDB =\\\n")
		PP.pprint(db['EnumDB'])
		out.write("\n")

		out.write("FunctionDB =\\\n")
		PP.pprint(db['FunctionDB'])
		out.write("\n")

		out.write("TypedefDB =\\\n")
		PP.pprint(db['TypedefDB'])
		out.write("\n")
		out.close()
#==============================================================================
# Main Program
#==============================================================================
def main():

	global LOG
	global FORCE
	global VERBOSE
	global TEST
	global DEBUG
	global SHELL
	global TARGET

	# Make sure we're in the right directory

	if not os.path.exists('%(projectfile)s' % NAMES):
		print "\npyproject must be run from a project directory!\n"
		sys.exit(1)

	# ------------------------------------------------------------------
	# Decode command line
	# ------------------------------------------------------------------

	coms = sys.argv[1:]
	opts = []
	for index, arg in enumerate(coms):
		if arg[0] == '-':
			opts = coms[index:]
			coms = coms[:index]
			break
	cmds = []
	for cmd in coms:
		coms = re.findall(r'\b%s\w*\b' % cmd, OPTION)
		if len(coms) == 0:
			if len(cmds) > 0 and (cmds[0] == "build" or cmds[0] == "shell"):
				cmds.append(cmd)
			else:
				fatal('command "%s" not recognized' % cmd)
		elif len(coms) > 1:
			fatal('command "%s" is ambiguous' % cmd)
		else:
			cmds.append(coms[0])

	argv = opts + cmds

	NAMES['commands'] = cmds
	NAMES['noclasstest'] = 1 # No class

	try:
		opts, args = getopt(argv, SHORTOPTIONS)
		if len(args)==0:
			usage()
			sys.exit(1)   
	except GetoptError, message:
		fatal("%s" % message)

	compnames = []
	for option, value in opts:
		if   option == "-h":
			usage()
			sys.exit(0)

		elif option == "-V":
			print VERSION
			sys.exit(0)

		elif option == "-F":
			FORCE = option

		elif option == "-N":
			NAMES['noclasstest'] = 1

		elif option == "-v":
			VERBOSE = option

		elif option == "-T":
			TEST=1
			VERBOSE="-v"

		elif option == "-d":
			DEBUG = "-d%s" % value

		elif option == "-m":
			compnames.append(value)

	NAMES['test']    = TEST
	NAMES['force']   = FORCE
	NAMES['verbose'] = VERBOSE
	NAMES['debug']   = DEBUG

	# ------------------------------------------------------------------
	# Get project attributes. Project Path is working directory
	# ------------------------------------------------------------------
	projectAttributes(NAMES['projectpath'], NAMES)
	if LOCALDEBUG:
		print "\n\tProject Attributes\n"
		print pp.pprint(NAMES)

	# ------------------------------------------------------------------
	# Get commands and determine which components to process
	# ------------------------------------------------------------------ 

	if len(compnames) == 0:
		COMPLIST = NAMES['componentnames']
	else:
		COMPLIST = compnames

	# Open log file

	if not os.path.exists("logs"): os.system("mkdir -p logs")
	if NAMES['commands'][0] == 'shell':
		logfile = 'logs/shell.log'
	else:
		logfile = "logs/%s.log" % joinfields(NAMES['commands'],'_')

	#LOG = open(logfile,'w')

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
	if TEST:
		print "\tComponents to Process"
		pp.pprint(COMPLIST)
		print "\n\t Test Mode"    
	print ""

# ------------------------------------------------------------------
# Loop over components and perform specified actions
# ------------------------------------------------------------------

	command = Command(NAMES)

	if  'mkmake' == NAMES['commands'][0]:
		command.mkmake(NAMES, NAMES['projectpath'], ['src'])
		sys.exit(0)

	elif 'list' in NAMES['commands']:
		command.list(NAMES)
		sys.exit(0)

	if 'config' == NAMES['commands'][0]:
		command.mkmake(NAMES, NAMES['projectpath'], ['src'])

	if 'count' in NAMES['commands']: command.countbegin(NAMES)

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
		# doc, xml and python
		if componentname == 'ttypes':
			NAMES['dirs'] = ['doc', 'python']
		else:
			NAMES['dirs'] = ['doc', 'xml', 'python']
		if component.attrib.has_key('dirs'):
			NAMES['dirs'] += split(component.attrib['dirs'])

		# ------------------------------------------------------------------
		# Construct python package path, that is, the path within which the
		# shared object will be placed.
		# ------------------------------------------------------------------
		prjname = project.attrib['name']
		NAMES['prjname'] = prjname
		NAMES['libpath'] = "%(libarea)s/%(prjname)s/lib/python" % NAMES

		# Check for null module

		# ------------------------------------------------------------------
		# Construct build path, that is, the directory in which the build
		# occurs. This is the directory in which the xml, interface, patch,
		# tmp, src and test sub-directories are located.
		# ------------------------------------------------------------------

		# All paths relative to the project area

		paths = ["src"]
		if package <> "": paths.append(package)
		if module <> "": paths.append(module)
		NAMES['bldpath'] = joinfields(paths,'/')

		str = "%(count)5d\tProcessing: %(bldpath)s" % NAMES
		if VERBOSE == "": print str
		wrlog("\n%s" % str)

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
			if componentname != 'ttypes':
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

		if 'build' == NAMES['commands'][0]:
			command.build(NAMES)

		elif 'shell' == NAMES['commands'][0]:
			command.shell(NAMES)

		else:
			for cmd in NAMES['commands']:
				eval('command.%s(NAMES)' % cmd)
	# End for

	if 'count' in NAMES['commands']: command.countend(NAMES)

	command.close()
	if LOG: LOG.close()
# ------------------------------------------------------------------    
if __name__ == "__main__":
	print "\tpyproject %(VERSION)s\n" % NAMES
	main()


