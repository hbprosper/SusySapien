#!/usr/bin/env python
#---------------------------------------------------------------------------
# File:        xml2boost.py
#
# Description: Create boost interface files from the XML files created with
#              header2xml.py. The XML files describe the structure and
#              contents of C++ header files in a format considerably easier
#              to read and understand than that produced by gccxml.
#              xml2boost.py is a valiant attempt to build Boost.Python
#              interface files automatically. The basic method is to use a
#              large number of heuristic rules. 
#
#              xml2boost makes heavy use of recursion, generally regarded as
#              a computSationally inefficient technique. However, recursive 
#              algorithms are extremely powerful and lead to much simpler
#              code. Therefore, since speed is not an issue and given the
#              difficulty of what is being attempted, many of the
#              algorithms used are recursive.
#---------------------------------------------------------------------------
# Created:     17-Dec-2003 Harrison B. Prosper, CERN
# Updated:     08-Mar-2004 HBP Fix a few problems
#              09-Mar-2004 HBP Consolidate export files into a single file
#              15-Mar-2004 HBP
#              22-Mar-2004 HBP Change convertor method
#           04-14-May-2004 HBP at CERN. Numerous fixes
#              21-May-2004 HBP Add registration checks
#              10-Sep-2004 HBP Bug fixes
#              15-Sep-2004 HBP Fix optional argument problem with proxies
#              19-Sep-2004 HBP Fix updateStruct
#              22-Sep-2004 HBP Use TypesDB
#              19-Oct-2004 HBP Improve handling of callbacks
#              18-Nov-2004 HBP Swap src <-> interface
#              19-Nov-2004 HBP Handle templated functions
#              08-Dec-2004 HBP Fix iterator translation bug
#              10-Apr-2005 HBP Complete XML database cache
#              01-May-2005 HBP Adding more comments (at 30,000 feet!)
#              14-May-2005 HBP Add automatic scanning of global typedefs
#              21-May-2005 HBP Fix translation
#                              Change default to no holder class
#              22-May-2005 HBP Go back to previous default!
#              28-May-2005 HBP split xml2boost.py into xml2boost.py and
#                              xml2boostDatabase.py
#              11-Aug-2005 HBP Allow return of refs from maps and vectors
#              02-Aug-2005 HBP Change from lib<headename>.so to
#                              libPy<headername>.so
#              25-Jan-2006 HBP Change Template instantiation code - remove
#                              module name in include
#              10-Feb-2006 HBP Fix strippedTemplate
#              23-Apr-2006 HBP Fix handling of simple references
#              13-May-2006 HBP Change handling of auto-instantiation of
#                              vectors. Create separate wrapper functions.
#              29-Aug-2009 HBP change opaque to opaque_
#$Revision: 1.2 $
#------------ ---------------------------------------------------------------
# TODO
#  1. Tidy up use of context. Wherever possible use class element directly.
#  2. Tidy up wrapper builder.
#  3. Add more comments.
#---------------------------------------------------------------------------
import os, sys, re, posixpath
from time   import *
from string import *
from glob   import glob
from getopt import getopt, GetoptError
from pprint import PrettyPrinter
from elementtree.ElementTree import ElementTree, XML
#---------------------------------------------------------------------------
from header2xml import \
	 findComponents, \
	 findHeader, \
	 findAll,\
	 findFile, \
	 templateDecoration, \
	 getHeaderList, \
	 ocomment, \
	 classType, \
	 namespaceNames,\
	 number, \
	 istemplate, \
	 isspecializedtemplate, \
	 stripDefaultValues, \
	 templatepar, \
	 templateParList, \
	 cleantype, \
	 findClassElements, \
	 extractTemplatedItems, \
	 squeezeblanks, \
	 unfoldTypedef, \
	 TYPEDEFTYPE, \
	 FTYPEDEFTYPE
#===========================================================================
from boostlib  import *
from xml2boostDatabase import *
#===========================================================================
SHOWITER = 0
STANDALONE = 0
exportpresent = re.compile('^void (class|enums|functions)_\w+',re.M)
#===========================================================================
# Scan the XML tree and perform various fixups. For example, replace all
# types by their fully qualified forms. Also flag elements to be pruned
# from the tree.
#===========================================================================
def initializeTree(context, elem, depth=0):

	depth += 1
	if depth >= 50: fatal('lost in woods..! tag: %s' % elem.tag)    
	if depth == 1:

		context['headername'] = DB.headername(elem)
		context['functioninfo'] = {}
		context['namespace'] = []
		context['scope'] = []           # To keep track of scope
		context['classname'] = ''
		context['typemap'] = {}
		context['headerelement'] = elem

		context['template']        = ''
		context['strippedtemplate']= ''
		context['templatepar']     = ''
		context['templateparlist'] = []

		#-------------------------------------------------
		# Reset class depth
		#-------------------------------------------------
		context['classdepth'] = 0  # Keep track of nesting level
		context['classelement'] = None

		#-------------------------------------------------
		# Add non-bound operators to appropriate class
		# before building records
		#-------------------------------------------------
		addOperatorsToClass(elem)

		#-------------------------------------------------
		# Add callbacks defined in edits.py
		#-------------------------------------------------
		addCallbackToHeader(context)

		#-------------------------------------------------
		# Commit additions to in-memory class database
		#-------------------------------------------------        
		DB.commit(elem)

		context['globaltypemap'] = DB.query('globaltypemap')

	#-------------------------------------------------
	# Loop over elements
	#-------------------------------------------------
	for element in elem.getchildren():

		tag = element.tag

		if element.attrib.has_key('name'):
			name = element.attrib['name']
		else:
			name = ''

		if element.text == None:
			text = ""
		else:
			text = strip(element.text)

		context['tag']  = tag
		context['name'] = name
		context['text'] = text

		elemtab  = '  '*(depth-1)
		elemname = name
		context['elemtab']  = elemtab
		context['elemtag']  = element.tag
		context['elemname'] = name
		debug(1,["%(elemtab)s<%(elemtag)s> %(elemname)s" % context],0)

		if   tag == 'namespace':

			if name <> '': context['scope'].append(name)

			initializeTree(context, element, depth)

			if name <> '': context['scope'].pop()

		elif tag == 'section':

			initializeTree(context, element, depth)

		elif tag == 'class':

			if excludeClass(name):
				inform('Exclude class %(name)s' % context)
				element.attrib['name']='%REMOVE'
				continue

			# ----------------------------------
			# IMPORTANT: Increment class depth
			# ----------------------------------
			context['classdepth'] += 1

			# Note parent class

			if context['classdepth'] == 1:
				context['classelement'] = element                
			context['classname'] = name

			#-----------------------------------------------------------
			# Check for structs. If a struct add a default constructor
			# if one does not already exist, and enclose it 
			# within a public section. Also, add a default constructor
			# to a class if needed.
			#-----------------------------------------------------------
			if find(element.attrib['type'],'struct') > -1:
				updateStruct(element)
			else:
				updateClass(element)

			attributes = element.attrib
			if attributes.has_key("template"):
				context['template']        = attributes['template']
				context['strippedtemplate']= attributes['strippedtemplate']
				context['templatepar']     = attributes['templatepar']
				context['templateparlist'] = attributes['templateparlist']
			else:
				context['template']        = ''       # Important to reset!
				context['strippedtemplate']= ''
				context['templatepar']     = ''
				context['templateparlist'] = []

			typemap = DB.query('typemap',name)
			if typemap <> None:
				context['typemap'] = typemap

			# Note: Here we keep track of scope.

			fname = split(element.attrib['fullname'],'::').pop()

			context['scope'].append(fname) # Keep track of class namespace

			initializeTree(context, element, depth)

			context['scope'].pop() # Remember to pop!

			# ----------------------------------
			# IMPORTANT: Decrement class depth
			# ----------------------------------
			context['classdepth'] -= 1

			# If we are not in a class, nullify classelement

			# Reset class name, in case we have moved up from a
			# nested class
			# FIXME - this is a bit of a hack!

			if context['classdepth'] > 0:
				context['classname'] = split(context['scope'][-1],'<')[0]
				name = context['classname']
				typemap = DB.query('typemap',name)
				if typemap <> None:
					context['typemap'] = typemap
			else:

				# IMPORTANT: Reset classelement

				context['classname'] = ''
				context['classelement'] = None

		elif tag in ROUTINES:

			argslist = element.attrib['args']
			retype   = element.attrib['return']
			qual     = element.attrib['qual']

			# Ignore methods with more than MAXARGS arguments

			if type(argslist) == type(''):
				nargs = len(eval(argslist))
			else:
				nargs = len(argslist)

			if nargs >= MAXARGS:
				element.attrib['name']='%REMOVE'
				warning('Too many arguments in %s' % text)
				continue

			# Check name of method. It may be qualified with the
			# name of the class in which it is embedded. If so, remove
			# the namespace.

			fname = '%(classname)s::' % context
			if name[0:len(fname)] == fname:
				name = split(name, '::').pop()
				element.attrib['name'] = name

			# Ignore pure virtual functions

			pureVirtual = find(qual,'=') > -1
			if pureVirtual:
				# Don't forget to remove template function
				element.attrib['name']='%REMOVE'
				continue

			# Be bad. Ignore friends!

			if friendWord.search(retype) <> None:
				element.attrib['name']='%REMOVE'
				continue

			# Replace templated methods by instantiated versions thereof
			# if requested.

			templateMethod = istemplate.match(retype) <> None
			if templateMethod:
				instantiateFunction(elem, element, context)

				# Don't forget to remove the template function
				element.attrib['name']='%REMOVE'
				continue

			if tag == 'function1' or tag == 'function2':

				context['classname'] = '' # Reset

				if excludeFunction(context['headername'], text):

					if includeFunction(context['headername'], text):
						log(context['headername'],
							'Include %s:' % tag + '\n\t\t%s' % text)
					else:
						warning('Exclude function: \n\t\t%s' % text)
						element.attrib['name']='%REMOVE'
						continue

			elif excludeMethod(context['classname'], text):

				if includeMethod(context['classname'], text):
					log(context['headername'],
						'Include %s:' % tag + '\n\t\t%s' % text)
				else:
					warning('Exclude %s:' % tag + '\n\t\t%s' % text)
					element.attrib['name']='%REMOVE'
					continue

			elif tag == 'operator':

				# Get operator name

				m = opname.search(name)
				if m == None:
					warning("%s currently unsupported" % name)
					element.attrib['name']='%REMOVE'
					continue

				# Handle operator[](const int)

				op = m.group('name')
				if op == '[]':

					if type(argslist) == type(""): argslist = eval(argslist)
					nwrap, kdefault, arguments = getArgs(argslist)
					if len(arguments) == 1:
						str = arguments[0]
						str = lower(str)
						if intctype.match(str) == None:
							inform("%s(%s) ** currently unsupported" % \
								   (name,arguments[0]))
							element.attrib['name']='%REMOVE'
							continue

					# Bail if we have void or void* return type

					if voidtype.search(retype)  <> None:
						warning("%s %s(%s) currently unsupported" % \
								(retype, name, arguments[0]))
						element.attrib['name']='%REMOVE'
						continue

			#---------------------------------------------------------
			# Ok, Fix types
			#---------------------------------------------------------

			replaceTypes(element)

			# Finally, translate all types to their underlying
			# types using the typemaps of current class... 

			translateTypes(context, element)

			# ...then analyze arguments

			analyzeReturnAndArguments(context, element)

		elif tag == 'member':

			if excludeMember(context['classname'], text):
				inform('Exclude member: %(text)s' % context)
				element.text ='%REMOVE'
				continue


		#end if        
		if element.tag in ['class','namespace']:
			context['elemtab']  = elemtab
			context['elemtag']  = element.tag
			context['elemname'] = elemname
			debug(1,["%(elemtab)s</%(elemtag)s>" % context,""],0)
#---------------------------------------------------------------------------
# Prune the tree by removing elements that are to be excluded. First prune
# away the immediate children before continuing to prune the rest of the
# tree, recursively.
#---------------------------------------------------------------------------
def pruneTree(elem, depth=0):
	depth += 1
	if depth >= 50:
		fatal('lost in tree...! tag: %s' % elem.tag)

	# First prune current list of children
	active = 1
	while active:
		active = 0
		for element in elem.getchildren():
			if element.attrib.has_key('name'):
				if element.attrib['name'] == '%REMOVE':
					elem.remove(element)
					active = 1 # Ok, try again
					break
			elif element.text == '%REMOVE':
				elem.remove(element)
				active = 1 # Ok, try again
				break

	# Then climb rest of tree
	for element in elem.getchildren():
		pruneTree(element, depth)
#---------------------------------------------------------------------------
# Get publicly inherited base classes. For now, ignore iterator base
# classes.
#---------------------------------------------------------------------------
def getBasenames(context, element):
	baseclasses = element.findall("baseclass")  
	context['bases'] = []
	for baseclass in baseclasses:
		inheritance, basename =\
					 splitfields(baseclass.attrib['name'],' ',1)

		if basename[0:14] == 'std::iterator<':
			inform('ignore base class %s' % basename)
			continue

		inheritance = strip(inheritance)
		basename = strip(basename)
		basename = translateAllTypes(context, basename)
		context['bases'].append((inheritance, basename))

		updateTemplatemap(context, basename)

#---------------------------------------------------------------------------
# Kind of obvious huh!
#---------------------------------------------------------------------------
def tostr(v):
	for i, x in enumerate(v):
		if type(x) == type(0):
			v[i] = '%d' % v[i]
	return v
#---------------------------------------------------------------------------
def printTree(element, out=None, depth=0):
	if not PRINT: return
	if depth == 0:
		out = open('tree.xml','w')
		out.write('\n%s\n' % element.attrib['name'])

	depth += NTAB
	if depth >= 50:
		fatal('lost in tree...! tag: %s' % element.tag)        

	for child in element.getchildren():

		out.write(write("<%s>" % child.tag, depth))
		for (key, value) in child.attrib.items():

			delim = ',\n\t' + (len(key)+2)*' ' 

			if type(value) == type([]):
				if len(value) == 0: continue

				if type(value[0]) == type(""):
					str = write("\t%s: %s" % \
								(key, joinfields(value,delim)),depth)
					out.write(str)
				else:
					recs = []
					for index, v in enumerate(value):
						v = tostr(v)
						recs.append("[%s]" % joinfields(v,','))
					str = write("\t%s: %s" % \
								(key, joinfields(recs,delim)),depth)
					out.write(str)
			elif type(value) == type(""):
				out.write(write("\t%s: %s" % (key, value), depth))

			elif type(value) == type(0):
				out.write(write("\t%s: %d" % (key, value), depth))

			elif type(value) == type(1.0):
				out.write(write("\t%s: %f" % (key, value), depth))

		if child.text <> None:
			text = rstrip(child.text)
			if text <> "":
				text = rstrip(stripBlanklines(text))
				out.write('\ttext:\n'+text)
				out.write('\n')

		printTree(child, out, depth)

	depth -= NTAB;
	out.write(write("</%s>\n" % element.tag, depth))
#===========================================================================
# Climb XML tree recursively and build ordered list of records
#===========================================================================
def processTree(context, elem, depth=0):

	depth += 1
	if depth == 1:
		context['namespace']= []
		context['scope']    = []         # Needed to create full-name
		context['template'] = ''
		context['strippedtemplate'] = '' # Needed in thinWrappers
		context['templatepar'] = ''      # <T, ..>
		context['templateparlist'] = []  # [T, ..]

	elif depth >= 50:
		fatal("lost in tree...go boil your head...I'm bailing!\n\t\t%s" % \
			  elem.tag)

	context['depth'] = depth
	context['tab']   = TAB
	context['inclass'] = 0

	for element in elem.getchildren():

		#-------------------------------------------------------------------
		# Set tag, attributes and name
		#-------------------------------------------------------------------
		# WARNING: name is "volatile". So functions
		# that need it should immediately extract it from "context"
		# and cache it. "name" is volatile in the sense that its
		# value can be changed by a procedure.
		#
		# fullname: fully qualified name
		# name:     last part of fully qualified name
		#-------------------------------------------------------------------
		context['element'] = element
		context['tag'] = element.tag
		attributes = element.attrib                
		if attributes.has_key("name"):
			name = attributes["name"]
		else:
			name = ''
		context['name'] = name

		if name == '%REMOVE': fatal('\%REMOVE found') # Shouldn't happen!

		elemtab  = '  '*(depth-1)
		elemname = name
		context['elemtab']  = elemtab
		context['elemtag']  = element.tag
		context['elemname'] = elemname
		debug(1,["%(elemtab)s<%(elemtag)s> %(elemname)s" % context],0)
		# ---------------------------------------------------------------
		# Process according to tag (that is, group in header2xml jargon)
		# ---------------------------------------------------------------

		if element.tag == "namespace":                

			if name <> '':
				context['namespace'].append(name)
				context['scope'].append(name)

				# Add a using instruction
				using = 'using namespace %s;' % \
						joinfields(context['namespace'],'::')
				unique(context['usingnamespace'], using)


			processTree(context, element, depth)

			if name <> '':
				context['namespace'].pop()
				context['scope'].pop()

		elif element.tag == "class":

			if excludeClass(name):
				warning('Exclude class %(name)s' % context)
				continue

			# ----------------------------------
			# IMPORTANT: Increment class depth
			# ----------------------------------
			context['classdepth'] += 1

			# Keep track of class namespace

			fname = split(element.attrib['fullname'],'::').pop()

			context['scope'].append(fname)

			classRecord(context, element)

			context['scope'].pop()

			# ----------------------------------
			# IMPORTANT: Decrement class depth
			# ----------------------------------
			context['classdepth'] -= 1

		elif element.tag == "enum":

			addDependency(context)
			enumRecord(context, element)

		elif element.tag == "function1" or element.tag == "function2":

			# FIXME
			if find(name, '::') > -1: continue # Hack to ignore inline funcs.

			context['nroutine'] = context['nfunction']
			context['records']  = []
			context['aligntab'] = ''
			methodRecord(context, element, FUNCTION)

			context['nfunction'] = context['nroutine']            
			cpprecord = joinfields(context['records'],'\n')
			if cpprecord <> "":
				addDependency(context)
				context['funrecords'].append(cpprecord)
				debug(1,[cpprecord], 0)

		if element.tag in ['class','namespace']:
			context['elemtab']  = elemtab
			context['elemtag']  = element.tag
			context['elemname'] = elemname
			debug(1,["%(elemtab)s</%(elemtag)s>" % context,""],0)
#===========================================================================
# Build a class record
#===========================================================================
hiddenctor = re.compile(r'(private|protected)defctor')

classcontext = '''
fullname
nested
classname
ignoredclass
usedstatic
template
templatepar
templateparlist
classtype
pyclassname
proxyclassname
holderclassname
abstract
noncopyable
nondestroyable
nonconstructable
depth
tab
inclass
nmethod
ncallback
ncon
iterable
const_iterable
hassize
'''
classContext = split(classcontext)
#===========================================================================
def classRecord(context, element):

	# NOTE:
	# element is reserved for the input variable. Do not re-define it!

	#--------------------------------------------------        
	# Define current class context
	#--------------------------------------------------
	# Set class depth etc.

	classdepth = context['classdepth']
	nested = classdepth > 1

	name = element.attrib['name']
	classname = name

	# Exclude nested iterator classes. We handle iterators using
	# iteratorRecord

	if classname in ['iterator', 'const_iterator']: return

	# classelement is the parent class, while
	# currentclasselement is, well, the current class. These objects
	# will differ if the current class is a nested class.

	if not nested: context['classelement'] = element
	context['currentclasselement'] = element

	context['modulename']= DB.query('module', name)
	context['classname'] = classname
	context['name']      = name
	context['nested']    = nested
	context['ignoredclass'] = ''
	context['usedstatic'] = [] # For pruning away spurious "staticmethod"

	debug(1,["%(elemtab)s\tclassRecord class: %(name)s, nested %(nested)d" % \
			 context],0)

	#-------------------------------------------------
	# 1. A regular class is a non-template class
	# 2. A specialized class is a fully specialized
	#    template class.
	#
	# NOTE: Nested classes inherit the template
	#       decoration of parent
	#
	#
	# template:        The complete template specification
	#
	# stripedtemplate: The complete template spec., but
	#                  with default values stripped away
	#
	# templatepar:     strippedtemplate without template
	#                  keyword
	# templateparlist: List of template parameters
	#-------------------------------------------------
	attributes = element.attrib
	if not nested:

		if attributes.has_key('template'):
			context['template']        = attributes['template']
			context['strippedtemplate']= attributes['strippedtemplate']
			context['templatepar']     = attributes['templatepar']
			context['templateparlist'] = attributes['templateparlist']
		else:
			context['template'] = ""
			context['strippedtemplate'] = ""
			context['templatepar'] = ""
			context['templateparlist'] = []

	debug(1,['%(elemtab)s\tclassRecord class: %(name)s, '\
			 'templatepar: %(templatepar)s' % context],0)

	if attributes.has_key("type"):
		context['classtype'] = attributes["type"]
		context['type'] = context['classtype']
	else:
		context['classtype'] = ""
		context['type'] = ""

	structType = find(context['classtype'],'struct') > -1

	# Use context to determine class type (of parent)

	abstract, regularClass, specializedClass, \
			  templateClass = classType(context)

	if specializedClass:
		context['template'] = ""
		context['templatepar'] = ""
		context['templateparlist'] = []
		context['specializedtemplateclass'].append(context['classname'])
		inform('ignore specialized template %(classname)s' % context)
		return

	elif templateClass:
		context['templateclass'].append(context['classname'])
		inform('template %(classname)s' % context)
		# We have at least one template class, so put into a
		# header
		context['writeHeader'] = 1

	#--------------------------------------------------
	# If class name is templated create a valid Python
	# identifier. Add class name to template map.
	#--------------------------------------------------
	pyclassname = updateTemplatemap(context, context['classname'])
	context['pyclassname'] = pyclassname

	# Update fields: scopename, scopeprefix

	updateScopename(context)

	debug(1,['scopename<%(scopename)s>' % context])

	context['proxyclassname'] = "__%(pyclassname)s__"  % context
	context['holderclassname'] = "%(pyclassname)sHolder" % context

	context['ncon']    = 0
	context['inclass'] = 1
	context['nmethod'] = 0
	context['ncallback']= 0
	context['iterable']= 0
	context['const_iterable']= 0
	context['hassize'] = 0
	context['getitem'] = ''
	context['setitem'] = ''
	context['cppend']  = []

	#--------------------------------------------------
	# Get constructors and determine whether or not we
	# can construct instances of this class. We cannot
	# construct instances of this class if it has no
	# public constructors.
	#--------------------------------------------------
	ctors = findElements(element, 'constructor')
	numctor = len(ctors)
	nonconstructable = numctor == 0 or \
					   makeNonconstructable(context['classname'])

	#HBP 4-July-2006 remove
	#hiddenctor.search(context['classtype']) != None or \

	#--------------------------------------------------
	# Exclude constructors shadows, but choose the more
	# general of them. (See description below of shadows).
	#--------------------------------------------------
	constructors = []
	conargs = []
	for ctorelem in ctors:

		args = []
		for a in ctorelem.attrib['argsanalysis']:

			arg = a[K_ARG]
			val = a[K_VAL]

			# Update conargs with non-defaulted arguments *only*
			# since, unlike the case for methods, we do not create
			# a wrapper for each version of the constructor.
			if val == '': args.append(arg)                

		# Check for shadows
		index = shadow(context['classname'], args, conargs) 
		if index < 0:
			constructors.append(ctorelem)
		else:
			# Try to decide which constructor to keep.
			# For now, just one heuristic rule:
			# 1. Favor ones without special processing of arguments
			if not 'PyObject*' in args:
				inform("Warning: constructor\n%s\n"\
					   "\t** shadows\n%s\n\t** and WILL replace it" % \
					   (strip(ctorelem.text), strip(constructors[index].text)))
				constructors[index] = ctorelem
			else:
				inform("Warning: constructor\n%s\n"\
					   "\t** shadows\n%s\n\t** but WILL NOT replace it" % \
					   (strip(ctorelem.text), strip(constructors[index].text)))
	#--------------------------------------------------
	# A noncopyable class is an abstract class or one
	# without a public copy constructor. Note: If a
	# base class has a non-public copy constructor
	# then make the derived class noncopyable.
	#--------------------------------------------------
	noncopyable = abstract
	if not noncopyable: noncopyable = makeNoncopyable(context['classname'])
	if not noncopyable: noncopyable = isNoncopyable(context['classname'])

	#--------------------------------------------------
	# A nondestroyable class is one with a private or
	# protected destructor. If the default constructor
	# is private or protected assume class is
	# nonconstructable.
	#--------------------------------------------------
	nondestroyable = isNondestroyable(context['classname'])
	isNonCon = isNonconstructable(context['classname'])
	if nondestroyable: nonconstructable = nonconstructable or isNonCon

	# A class that is nonconstructable is surely also
	# noncopyable!
	noncopyable = noncopyable or nonconstructable

	#--------------------------------------------------
	# Create a class whose instances will hold instances
	# of given class, but do so only if the given class 
	# is neither abstract nor nested nor nonconstructable.
	#
	# This is useful if one wants to overload a virtual
	# C++ method with a method defined in Python. An
	# option to insert selected methods into the holder
	# class is provided, called EnableMethodOverride.
	# Holder classes are also useful because they allow
	# a larger variety of classes to be wrapped,
	# automatically.
	#--------------------------------------------------
	addholder = not (abstract or nested or nonconstructable or
					 excludeHolder(classname))

	# Update class context

	context['abstract'] = abstract
	context['nondestroyable'] = nondestroyable
	context['nonconstructable'] = nonconstructable
	context['noncopyable'] = noncopyable

	# Say what kind of class we have

	if abstract:
		inform('abstract %(classname)s' % context)

	if nonconstructable:
		inform('nonconstructable %(classname)s' % context)

	if nondestroyable:
		inform('nondestroyable %(classname)s' % context)

	if noncopyable:
		inform('noncopyable %(classname)s' % context)

	#--------------------------------------------------        
	# Keep track of classes to be initialized with
	# "class_" construct. Note: nested classes are
	# constructed along with parent.
	#--------------------------------------------------
	if not nested: unique(context['classnames'], context['classname'])

	#-------------------------------------------------
	# Determine if we need a proxy. We need a proxy if
	# class contains constructors with arguments that
	# need special handling, such as pointers to simple
	# C types. But if the class cannot be instantiated
	# then we presume there is no need for a proxy.
	#-------------------------------------------------
	if nonconstructable or abstract:
		addProxy = 0
	else:
		addProxy = checkForProxy(context, constructors)
	context['addproxy'] = addProxy

	#-------------------------------------------------
	# Fill context['bases']
	#-------------------------------------------------
	getBasenames(context, element)

	#-------------------------------------------------
	# Start build of class document
	#-------------------------------------------------
	if context['template'] == '':
		s = 'class %(classname)s\n' % context
	else:
		s = context['template'] + '\nclass %(classname)s\n' % context

	if len(context['bases']) > 0:
		delim = ' : '
		for inheritance, basename in context['bases']:
			s = s + delim + inheritance + " " + basename
			delim = ',\n   '
		s = s + '\n'
	s = s + '{\npublic:'
	element.text = s

	#-------------------------------------------------
	# Start building class record with thin wrapper
	# preamble.
	#-------------------------------------------------
	s = ''
	if nested:
		width = int(MAXWIDTH/1.5)
	else:
		width = MAXWIDTH
	s = s + '//' + width*'-' + '\n'
	s = s + '// Thin wrappers for class: %(fullname)s\n' % context
	s = s + '//' + width*'-' + '\n'
	context['wrprecords'].append(s)

	#-------------------------------------------------
	# Get methods
	#-------------------------------------------------
	publicOnly = not structType
	methods = findElements(element,'method1', publicOnly)+\
			  findElements(element,'method2', publicOnly)

	#-------------------------------------------------
	# Start build of class export routine only if this
	# is not a nested class.
	# Handle both non-templated and templated classes.
	# Note: A stripped template is the template with
	# default arguments stripped away.
	#-------------------------------------------------
	context['classcount'] += 1
	cpprecord = ''
	if not nested:
		cpprecord = cpprecord + '\n'
		if context['template'] <> '':
			cpprecord = cpprecord + context['strippedtemplate'] + '\n'
			cpprecord = cpprecord + \
						'void class_%(pyclassname)s'\
						'(const char* name)\n{\n'
			if addProxy:
				cpprecord = cpprecord + TAB1 + \
							'std::string proxyname = '\
							'std::string("__") + '\
							'std::string(name) + std::string("__");\n\n'
		else:
			cpprecord = cpprecord + \
						'void class_%(pyclassname)s()\n{\n'            
		cpprecord = cpprecord % context

		# Add a check on whether class has been
		# registered with Boost Python.

		s = ''
		s = s + TAB1 + \
			'// Return if this class is already registered\n\n'
		s = s + TAB1 + \
			'if ( boostutil::registered< %(fullname)s >() ) return;\n\n'
		cpprecord = cpprecord + s % context

		#-------------------------------------------------
		# Add record to import base classes from external
		# modules. This is needed because Boost.Python
		# (version 2) requires base classes be registered
		# before the derived class.
		#-------------------------------------------------
		t = importBaseModules(context)
		if t <> '':
			s = ''
			s = s + TAB1 + \
				'// Register base classes for current class'
			s = s + '\n\n'
			s = s + t + '\n\n'
			cpprecord = cpprecord + s

		# If this class is iterable we need to instantiate the
		# appropriate iterator classes.

		scratch = []
		for index, e in enumerate(methods):
			s = iteratorRecord(context, element, e, scratch)
			if s <> '': cpprecord = cpprecord + s % context + '\n'

		scratch = []
		for index, e in enumerate(methods):
			s = rangeRecord(context, element, e, scratch)
			if s <> '': cpprecord = cpprecord + s % context + '\n'

	#end if not nested

	# Add dummy base classes if requested

	t = includeDummyClass(context['classname'])
	if t <> '': cpprecord = cpprecord + t

	#-------------------------------------------------
	# Either instantiate iterators if, needed, or
	# add __iter__, again, if needed.
	#-------------------------------------------------


	# Start scope

	cpprecord = cpprecord + TAB1 + '// Register class\n\n'
	cpprecord = cpprecord + '%(tab)sscope* %(scopeprefix)sscope = new\n\n' % \
				context
	str = '%(tab)s  scope(class_< %(fullname)s' % context    

	# Set alignment tab

	context['aligntab'] = align(str)
	alignment = context['aligntab']

	cpprecord = cpprecord + str

	# Add Holder field

	if addholder:
		cpprecord = cpprecord + ', %(holderclassname)s%(templatepar)s' %\
					context
	#-------------------------------------------------
	# NB: depends contains dependency information
	# that is written to each export function. This
	# information is subsequently used by the routine
	# writeModuleFile that builds <module>.py
	#-------------------------------------------------
	if not nested:
		updateDependencies(context)
		updateotherDependencies(context)

	#-------------------------------------------------    
	# Add base class fields
	#-------------------------------------------------
	if len(context['bases']) > 0:
		baserecord = ',\n%(aligntab)sbases< ' % context
		delim = ''
		for inheritance, basename in context['bases']:

			# Keep only publicly inherited base classes

			if inheritance <> 'public': continue

			# We may need to translate this class name

			basename = translateAllTypes(context, basename)
			baserecord = baserecord + delim + basename
			delim = ', '
		baserecord = baserecord + ' > '
		if find(baserecord, 'bases<  >') < 0:
			cpprecord = cpprecord + baserecord

	# Class could be abstract or have a private
	# or protected copy constructor

	if noncopyable: cpprecord = rstrip(cpprecord) + ', noncopyable'

	# Complete class preamble.

	cpprecord = cpprecord + ' >\n'

	#-------------------------------------------------
	# Add enums
	#-------------------------------------------------
	for e in findElements(element, 'enum'):
		enumRecord(context, e)

	#-------------------------------------------------            
	# Add constructors
	#-------------------------------------------------
	# If we need a proxy class, then omit constructors for
	# the class being shadowed.

	addDocString("class",
				 context, element, '%(classname)s_doc' % context)

	if addProxy:
		if context['template'] <> '':
			cpprecord = cpprecord + \
						'%(aligntab)s(proxyname.c_str(), %(docstring)s,\n'\
						'%(aligntab)s no_init)'
		else:
			cpprecord = cpprecord + \
						'%(aligntab)s("%(proxyclassname)s", %(docstring)s,\n'\
						'%(aligntab)s no_init)'

		context['cpprecords'].append(cpprecord % context)
		debug(1,[cpprecord,''],0)

	else:

		# We don't need a proxy.

		if context['template'] <> '':
			cpprecord = cpprecord + \
						'%(aligntab)s(name, %(docstring)s,\n'\
						'%(aligntab)s no_init)'
		else:
			cpprecord = cpprecord + \
						'%(aligntab)s("%(pyclassname)s", %(docstring)s,\n'\
						'%(aligntab)s no_init)'

		context['cpprecords'].append(cpprecord % context)
		debug(1,[cpprecord,''],0)

		if not (abstract or nonconstructable):
			j = 0
			for i, e in enumerate(constructors):
				context['ncon']= j; j += 1 # Constructor ordinal value
				constructorRecord(context, e)

	#-------------------------------------------------
	# Add methods
	#-------------------------------------------------
	# Keep track of methods so that we can determine
	# shadows.
	# A shadow method is one whose calling sequence
	# cannot be distinguished from that of another,
	# when called from Python. By default, we handle
	# shadows by the simple expedient of appending a
	# number to the name of the method. A slightly
	# better option is to use the AutoRename feature
	# in the edit file.
	#-------------------------------------------------    
	context['hassize'] = 0 # Flag for "size()" method
	addBlankline(context, methods)
	context['routines'] = {}
	context['records'] = []
	for index, e in enumerate(methods):

		name = e.attrib['name']        

		# Check if we have a "size()" method. If so, assume it
		# returns the number of elements of this object.

		if name == "size": context['hassize'] = 1

		context['nroutine'] = context['nmethod']
		methodRecord(context, e)
		context['nmethod'] = context['nroutine']

	context['routines'] = {} # Reset
	pruneStaticRecords(context)
	cpprecord = joinfields(context['records'],'\n')
	if cpprecord <> "":
		context['cpprecords'].append(cpprecord)
		debug(1,[cpprecord], 0)
	#-------------------------------------------------        
	# Add operators
	#-------------------------------------------------
	# Add iterator, if class inherits from an stl container

	if element.attrib.has_key('iterable'):
		if element.attrib['iterable']:
			addIterator(context, element)

	operators = findElements(element, 'operator')
	if nondestroyable:
		if len(operators) > 0:
			warning("Exclude all operators")
	else:
		addBlankline(context, operators)

		for e in operators:
			operatorRecord(context, e)

	#-------------------------------------------------
	# Add members
	#-------------------------------------------------
	members = findElements(element, 'member')
	addBlankline(context, members)
	for e in members:
		memberRecord(context, e)

	#-------------------------------------------------
	# Complete class record
	# 1. Add static method declarations
	# 2. Add enums
	# 3. Add nested classes.
	# 4. Add iterators
	#-------------------------------------------------
	ln = len(context['cpprecords'])
	if context['cpprecords'][ln-1] == '': ln -= 1
	context['cpprecords'][ln-1] = context['cpprecords'][ln-1]+');\n'

	for cpprecord in context['cppend']:
		context['cpprecords'].append(cpprecord)

	debug(1,['%(elemtab)s\tclassRecord class: %(name)s, '\
			 'Look for NESTED clases' % context],0)

	#-------------------------------------------------
	# Important: Save class context before we
	# recursively enter this routine!
	#-------------------------------------------------

	for variable in classContext:
		cmd = "%s = context['%s']" % (variable, variable)
		exec(cmd)

	classes = findElements(element, 'class')
	for e in classes:
		name = split(e.attrib['name'],'::').pop()
		context['nestedname'] = name

		debug(1,['%(elemtab)s\tclassRecord class: %(name)s, '\
				 'nested class: %(nestedname)s' % context],0)

		# NB: Caller is responsible for setting name and namespace fields

		context['name'] = name
		context['namespace'].append(name)

		# ----------------------------------
		# IMPORTANT: Increment class depth
		# ----------------------------------
		context['classdepth'] += 1

		# Keep track of class namespace

		fname = split(e.attrib['fullname'],'::').pop()

		context['scope'].append(fname)

		classRecord(context, e)

		context['scope'].pop()
		context['namespace'].pop()

		# ----------------------------------
		# IMPORTANT: Decrement class depth
		# ----------------------------------
		context['classdepth'] -= 1

	#-------------------------------------------------
	# Restore class context
	#-------------------------------------------------
	for variable in classContext:
		cmd = "context['%s'] = %s" % (variable, variable)
		exec(cmd)    
	updateScopename(context) # Restore scopename and scopeprefix

	# Complete class record

	# Remove empty struct

	if joinfields(context['wrprecords'][-2:],'') == '{};':
		context['wrprecords'] = context['wrprecords'][:-3]

	# Add delete scope instruction

	context['cpprecords'].append("%(tab)sdelete %(scopeprefix)sscope;\n" % \
								 context)

	#-------------------------------------------------
	# Add Holder class if appropriate
	#-------------------------------------------------
	if addholder: holderRecord(context, constructors, methods)

	#-------------------------------------------------        
	# Add proxy class if needed
	#-------------------------------------------------
	if addProxy:
		inform("Add proxy class for %(classname)s" % context)
		context['cpprecords'].append("")
		proxyClass(context, constructors)

	#-------------------------------------------------
	# Complete class record
	#-------------------------------------------------
	if not nested:

		# Add some optional debug info

		cpprecord = ''

		cpprecord += TAB1 + 'if ( BOOSTDEBUG )\n'
		if context['template'] <> '':
			cpprecord += TAB2 + \
						 'std::cout << "  registered " << name << std::endl;'
		else:
			cpprecord += TAB2 + \
						 'std::cout << "  registered %(fullname)s" << '\
						 'std::endl;' % context

		context['cpprecords'].append(cpprecord)

		# Complete class record

		context['cpprecords'].append('}')

#---------------------------------------------------------------------------
# Create a record to register an iterator. But if the iterator belongs to
# the current class, we do not have to register an iterator since one will
# be registered anyway. Note: A class can have an iterator either through
# inheritance or via containment. 
#---------------------------------------------------------------------------
isIteratorType = re.compile('(.+)::(const_iterator|iterator)$')
isRangeType = re.compile('std::pair\<(.+)::(const_iterator|iterator)'\
							   ', (.+)::(const_iterator|iterator)\>')
stripTypename = re.compile('typename\s+')
#---------------------------------------------------------------------------
# If the identifier after the first typename keyword is not a template
# parameter then strip away the first typename keyword.
#---------------------------------------------------------------------------
def stripFirstTypenameKeyword(context, name):
	t = swords.findall(name)
	if t[0] <> 'typename': return name
	if t[1] in context['templateparlist']: return name    
	return strip(replace(name,'typename','',1))
#---------------------------------------------------------------------------
def getOperators(ops, element, oplist, depth=0):
	depth += 1
	if depth > 5:
		fatal("getOperators - lost in trees")

	for elem in element.getchildren():

		if elem.tag in ['namespace', 'section', 'class']:
			getOperators(ops, elem, oplist, depth)

		elif elem.tag == 'operator':
			name = elem.attrib['name']
			m = opname.search(name)
			if m == None: continue
			op = m.group('name')
			if op in ops: oplist.append((elem, op))
#---------------------------------------------------------------------------
def checkiteratorProtocol(name, itertype):
	iterpp = 0;
	itermm = 0;

	if not DB.classDB.has_key(name): return (iterpp, itermm)

	c = DB.classDB[name]
	operators = []
	ops = ['++','--']
	getOperators(ops, c, operators)

	itype = re.compile('\\b%s\\b' % itertype)
	for o, op in operators:
		retype = o.attrib['return']

		if itype.search(retype) <> None:
			if op == '++': iterpp = 1
			if op == '--': itermm = 1
	return (iterpp, itermm)
#---------------------------------------------------------------------------
def iteratorRecord(context, classelement, element, scratch):

	retype = strip(element.attrib['return'])

	a = isIteratorType.findall(retype)

	if a == []: return ''

	# Register an iterator if class is iterable, either because
	# it inherits from an STL container or because it defines
	# an STL-compliant iterator by containment.

	fullname, itertype = a[0]

	if SHOWITER:
		print "ITERABLE CLASS( %s )( %s )" % (fullname, itertype)

	templated = stripTypename.search(fullname) <> None
	fullname = stripFirstTypenameKeyword(context, fullname)
	strippedfullname  = strip(stripTypename.sub('',fullname))

	t = splitfields(strippedfullname,'<',1)
	if len(t) == 2:
		name, tpars = t
	else:
		name = t[0]
		tpars= ''

	# Strip away possible namespace

	name = split(name, '::').pop()

	# Make sure iterator is bound to a known class.

	if name in ['map', 'list', 'vector']:

		if name == 'map': itertype += '_non_comp'

	elif DB.classDB.has_key(name):

		# Make sure iterator follows STL iterator protocol

		iterpp, itermm = checkiteratorProtocol(name, itertype)

		if not iterpp:
			if SHOWITER:
				print "ITERABLE CLASS( %s ) WRONG PROTOCOL" % fullname
			return ''
		if not itermm:
			if SHOWITER:
				print "ITERABLE CLASS( %s ) WRONG PROTOCOL" % fullname
			return ''

	else:
		if SHOWITER:
			print "ITERABLE CLASS( %s ) ** UNKNOWN **" % name
		return ''

	context['name']     = renameIdentifier(name)
	context['itertype'] = itertype        
	context['itername'] = renameIdentifier(strippedfullname)
	context['iterfullname'] = fullname

	# This class may use reference_type rather than reference...sigh!
	# If so, use correct iterator class.

	context['pfix'] = ''
	reftype = 'reference_type'
	if itertype[0:5] == 'const_': reftype = 'const_' + reftype
	if context['typemap'].has_key(reftype): context['pfix'] = '_rt'

	if templated:
		s = '((std::string("%(name)s")+std::string(name)).c_str());\n'
	else:
		s = '("%(itername)s");\n'
	t = ''
	t = t + TAB1+'boostutil::class_%(itertype)s%(pfix)s< %(iterfullname)s >\n'
	t = t + TAB2 + s
	t = t % context

	if t in scratch: return ''

	scratch.append(t)
	s = ''
	s = s + TAB1 + '// Register %(itertype)s for this class\n\n' % context
	s = s + t
	return s
#---------------------------------------------------------------------------
#---------------------------------------------------------------------------
def rangeRecord(context, classelement, element, scratch):

	retype = strip(element.attrib['return'])    
	a = isRangeType.findall(retype)
	if a == []: return ''

	fullname, itertype, a, b = a[0]
	templated = stripTypename.search(fullname) <> None
	fullname  = stripFirstTypenameKeyword(context, fullname)
	strippedfullname = strip(stripTypename.sub('',fullname))

	t = splitfields(strippedfullname,'<',1)
	if len(t) == 2:
		name, tpars = t
	else:
		name = t[0]
		tpars= ''

	context['name']     = renameIdentifier(name)
	context['itertype'] = itertype
	context['itername'] = renameIdentifier(strippedfullname)
	context['iterfullname'] = fullname

	if useNoinitRange(fullname):
		context['range'] = 'range_no_init'
	else:
		context['range'] = 'range'

	if templated:
		s = '((std::string("%(name)s")+std::string(name)).c_str());\n'
	else:
		s = '("%(itername)s");\n'            
	t = ''
	t = t + TAB1 + \
		'boostutil::class_%(itertype)s_%(range)s< %(iterfullname)s >\n'
	t = t + TAB2 + s
	t = t % context
	if t in scratch: return ''

	scratch.append(t)
	s = ''
	s = s + TAB1 + '// Register an iterator range for this class\n\n'
	s = s + t
	return s
#---------------------------------------------------------------------------
# Write thin wrappers around methods. This is not strictly necessary
# for all methods; however, making thin wrappers for all methods unifies the
# handling of overloaded as well as non-overloaded methods. It also makes
# the export file easier to understand. There is no overhead in this because
# any non-brain dead compiler will automatically "inline" wherever it is
# obvious to do so.
#
# The argsanalysis object contains a detailed analysis of the arguments and
# return types. We attempt to make reasonable guesses about how to handle
# each argument and return.
#---------------------------------------------------------------------------
def fullcallbackName(context, cbname, py=None):
	context['cbname'] = cbname
	if py == None:
		return "%(cbprefix)s%(cbname)s" % context
	else:
		return "%(cbprefix)s%(cbname)s_Py" % context
#---------------------------------------------------------------------------
def thinWrappers(context, element, function=0):

	pynames = [] # List of Python names for routines

	#----------------------------------------------------------    
	# Ignore pure virtual methods
	#----------------------------------------------------------
	pureVirtualMethod = find(element.attrib['qual'],'=') > -1
	if pureVirtualMethod: return pynames

	# Note: attrib and element.attrib are just different names for
	#       the same object
	attrib = element.attrib

	#----------------------------------------------------------
	# Make sure we have a Python name for current routine
	#----------------------------------------------------------
	if not attrib.has_key('pyname'): attrib['pyname'] = attrib['name']
	if attrib['pyname'] == 'print':  attrib['pyname'] = 'pprint'

	classname    = context['classname']
	pyname       = attrib['pyname']    # Python name of routine
	routinename  = attrib['name']      # Actual name of routine to be wrapped
	argslist     = attrib['args']
	argsanalysis = attrib['argsanalysis']
	retype       = attrib['return']
	newretype    = attrib['newreturn']
	staticMethod = attrib['static']

	#----------------------------------------------------------
	# Check if we have been asked to rename routines.
	# If so, update the Python name accordingly.
	#----------------------------------------------------------
	newname = autoRename(classname, routinename, argslist)
	if newname <> routinename:
		pyname = newname
		attrib['pyname'] = newname # Make sure we update element
	else:
		newname = renameMethod(classname, routinename, element.text)
		if newname <> routinename:
			pyname = newname
			attrib['pyname'] = newname # Make sure we update element

	# Keep track of routine signatures to check for shadows
	# A shadow is a routine with the same Python name and signature.

	if not context['routines'].has_key(pyname): context['routines'][pyname]=[]

	#----------------------------------------------------------
	# Ignore routines with unsupported return types
	#----------------------------------------------------------
	if simpletype.search(retype) == None:
		if rfind(retype,'**') > -1:
			context['unsupported'] = retype
			log(classname, 'unsupported return type\n%s' % element.text)
			return pynames

	context['unsupported'] = ''

	# Update context

	INDENT1 = ''
	INDENT2 = '  '
	context['indent1'] = INDENT1
	context['indent2'] = INDENT2
	context['function']= function
	context['staticMethod']= staticMethod
	context['return']      = retype
	context['newreturn']   = newretype
	context['name']        = routinename # actual name of routine
	context['routinename'] = routinename

	#----------------------------------------------------------
	# nwrap    is the number of wrappers to write
	# kdefault is the ordinal value of the first default argument
	#----------------------------------------------------------
	nwrap, kdefault, arguments = getArgs(argslist, routinename)

	#----------------------------------------------------------    
	# Build a wrapper for each version of routine.
	#----------------------------------------------------------
	records = []
	for n in xrange(nwrap):

		# Update number to be appended to wrappername

		context['nroutine'] += 1

		#----------------------------------------------------------
		# Start constructing wrapper signature
		# Determine what to do with returns
		#----------------------------------------------------------

		# Start routine signature

		if function or staticMethod:
			signature = '('
			delim = ''
		else:
			signature = '(%(fullname)s& o'
			delim = ',\n%(localtab)s'  # NB: do not use aligntab!

		sig  = '(' # For standardized signature
		dlim = ''

		#----------------------------------------------------------        
		# Check argument type and make an educated guess
		# about what to do.
		#----------------------------------------------------------        
		args    = [] # To keep track of argument info
		retitem = [] # To keep track of items to be returned in a Python list
		siglist = [] # To accumulate document for signature
		arglist = [] # To check for shadows

		for index, arg in enumerate(arguments[:kdefault+n]):

			typemapped,\
			argtype,\
			oldarg,\
			arg,\
			castarg,\
			val,\
			ttype,  \
			cbname, \
			iotype, \
			iosize, \
			byvalue,\
			var,\
			oldvar = argsanalysis[index]

			byreference = not byvalue

			if argtype == 'unsupported':
				log(classname,
					'%s contains unsupported type %s' % (routinename, arg))
				return []

			#            elif argtype in ['output','simple','simple*'] and \
			#                 iotype in [OUTPUT, INOUT]:

			elif iotype == OUTPUT:

				args.append(argsanalysis[index])

				# Update list of items to be returned by Python list.

				retitem.append((argtype, oldarg, ttype, iotype, var, oldvar))

				#if argtype in ['output','simple','simple*'] and \
				#       iotype in [OUTPUT]:
				continue # Skip signature update!

			elif argtype == 'simple' and byreference and iotype != OUTPUT:

				# Remember to change name of variable to be consistent with
				# those used in buildWrapperBody

				retitem.append((argtype, oldarg, ttype, iotype,
								"q%s" %var, oldvar))
				args.append(argsanalysis[index])

			else:            
				args.append(argsanalysis[index])

			# Update strings
			sig  = sig + dlim + arg

			dlim = ','

			signature = signature + delim + '%s %s' % (arg, var)
			delim = ',\n%(localtab)s'

			# Update arg types

			arglist.append(arg)

		#----------------------------------------------------------
		# Complete signature construction and create a standardized
		# signature to check for shadows.
		#----------------------------------------------------------
		signature = signature + ') '
		sig = sig + ')'
		sig = '%s%s' % (pyname, sig)

		#----------------------------------------------------------        
		# If this method shadows an existing one give
		# it a different name. The default is to add
		# a number at the end of the name equal to the
		# ordinal value of the method in the list of 
		# overloaded methods. A better option is provided
		# by AutoRename. (See edits section.)
		#----------------------------------------------------------

		rindex = shadow(pyname, arglist, context['routines'][pyname])

		# currentpyname is either attrib['pyname']
		# or it is a pyname created to disambiguate a shadowed function

		if rindex > -1:
			count = len(context['routines'][pyname])
			currentpyname = "%s%d" % (routinename, count)

			s = "rename\n" + joinfields(split(element.text),' ')+ \
				"\n\t** to %s" % currentpyname
			log(classname,s)
		else:
			currentpyname = pyname

		# Keep track of Python name and ordinal value of wrapper

		pynames.append((currentpyname, context['nroutine']))

		#----------------------------------------------------------        
		# If we are returning by list, make sure we change the
		# return type to PyObject*
		#----------------------------------------------------------
		returnByList = len(retitem) > 0        
		if returnByList:
			context['newreturn'] = 'PyObject*'
			attrib['newreturn']  = 'PyObject*'

		#----------------------------------------------------------
		# Build wrapper name
		#----------------------------------------------------------        
		context['currentpyname'] = currentpyname

		wrappername = '%(scopeprefix)s%(currentpyname)s_%(nroutine)s' % context
		context['wrappername'] = wrappername
		if context['template'] <> '':
			preamble = context['strippedtemplate'] + '\n'
		else:
			preamble = ''

		preamble = preamble + \
				   '%(indent1)s%(newreturn)s\n%(indent1)s%(wrappername)s' % \
				   context            

		localtab  = (len(INDENT1)+len(wrappername)+1)*' ' # Determine tab
		signature = preamble + signature 
		signature = signature % {'localtab': localtab,
								 'fullname': context['fullname']}

		#-------------------------------------------------
		# Finally, build wrapper body
		#-------------------------------------------------
		context['retitem'] = retitem
		body, retlist = buildWrapperBody(context, element, args)

		# Complete record for this instance of wrapper function
		record  = signature + '\n' + body + '\n'
		records.append(record)

	# NB: Update context

	context['return'] = context['newreturn']
	hpprecord = joinfields(records,'\n')
	if hpprecord <> "":
		if function:
			s = ''
			s = s + '//' + MAXWIDTH*'-' + '\n'
			s = s + '// Function/Procedure\n'% context
			s = s + '//' + MAXWIDTH*'-' + '\n'
			hpprecord = s + hpprecord
		context['wrprecords'].append(hpprecord)
		debug(1,[hpprecord], 0)
	return pynames
#---------------------------------------------------------------------------
# This is a rather complex routine, which does as its name implies!
# It is in desperate need of a re-write.
#
# We need to determine:
# 1. if we need to perform conversions on arguments *before* calling a
#    routine.
# 2. determine if there are arguments to be returned, either via the return
#    or via the arguments. If so, we need to decide how to return such
#    arguments.
# 3. call routine
# 4. return arguments if needed.
#---------------------------------------------------------------------------
def buildWrapperBody(context, element, args):

	attrib  = element.attrib

	returnByPointer  = attrib['returnByPointer']
	simpleReturnType = attrib['simpleReturnType']
	vectorReturnType = attrib['vectorReturnType']
	retype           = attrib['return']
	newretype        = attrib['newreturn']    
	ttypereturn      = attrib['ttypereturn']        
	staticMethod     = attrib['static']
	routinename      = attrib['name']

	context['name']       = routinename
	context['return']     = retype
	context['newreturn']  = newretype
	context['ttypereturn']= ttypereturn

	INDENT1     = context['indent1']
	INDENT2     = context['indent2']
	function    = context['function']
	retitem     = context['retitem']

	# Guess which are getters and which are setters

	prefix = lower(routinename)
	isaGetter = prefix[0:3] == 'get' or \
				prefix[0:4] == 'read' or \
				prefix[0:6] == 'return'

	isaSetter = prefix[0:3] == 'set' or \
				prefix[0:5] == 'write'

	postbody = '' # Code after call to method

	body = '%s{\n' % INDENT1  # Start of body

	if function or staticMethod:
		context['object'] = context['scopename']
	else:
		context['object'] = 'o.'

	# Check for vector types or pointers to simple types.
	# If present declare a convertor.

	returnByResult = False # to handle returns after call to function
	returnByObject = False
	returnByList   = len(retitem) > 0
	voidreturn     = retype == 'void'    
	trapnegativeReturn = \
					   trapNegativeReturn(context['classname'], routinename) \
					   and returnByPointer

	# Check whether to force a pointer to be zero, when it is negative!
	if trapnegativeReturn:
		returnByResult = True

	#-------------------------------------------------------------
	# Examine each argument and add whatever conversions are
	# necessary to map input arguments to those of the function
	# being wrapped.
	#-------------------------------------------------------------

	# CONVERSIONS

	for typemapped,\
		argtype,\
		oldarg,\
		arg,\
		castarg,\
		val,\
		ttype,\
		cbname,\
		iotype,iosize,byvalue,\
		var, oldvar in args:

		byreference = not byvalue

		# Get rid of const except if we have a pointer

		if ttype[-1] <> '*':
			ttype = strip(replace(ttype, 'const ', '')) # get rid of const
		striparg = stripName(oldarg)

		if argtype == '':
			pass

		elif iotype == OUTPUT:

			# Return arguments in a Python list

			oldarg = replace(oldarg,'&','')
			if pointer.search(oldarg) == None:
				body = body + '%s%s %s;\n' % (INDENT2, oldarg, var)
			else:
				body = body + '%s%s %s = 0;\n' % (INDENT2, oldarg, var)

		elif argtype in ['simple', 'output'] and iotype == OUTPUT:

			oldarg = replace(oldarg,'&','')
			if pointer.search(oldarg) == None:
				body = body + '%s%s %s;\n' % (INDENT2, oldarg, var)
			else:
				body = body + '%s%s %s = 0;\n' % (INDENT2, oldarg, var)

		elif argtype in ['simple'] and byreference and iotype != OUTPUT:

			# We need to first call function, *then* return arguments.
			# Therefore, we should store the return value of the routine
			# in a temporary value, then return arguments before returning
			# from the wrapper.

			m = numctype.search(ttype)
			if m == None: fatal("xml2boost.py - numctype failed on %s" %\
								ttype) 
			atype = m.group()

			if longctype.match(ttype) <> None:
				body = body + '%s%s q%s = extract< %s >(%s);\n' % \
					   (INDENT2, atype, var, atype, var)

			elif intctype.match(ttype) <> None:
				body = body + '%s%s q%s = extract< %s >(%s);\n' % \
					   (INDENT2, atype, var, atype, var)

			elif floatctype.match(ttype) <> None:
				body = body + '%s%s q%s = extract< %s >(%s);\n' % \
					   (INDENT2, atype, var, atype, var)

		elif argtype == 'file*':
			body = body + \
				   '%sFILE* f%s = PyFile_AsFile(%s.ptr());\n' % \
				   (INDENT2, var,var) 

		elif argtype == 'simple**':
			body = body + '%sstatic boostutil::convertor< %s > c%s;\n' % \
				   (INDENT2, ttype, var)
			body = body + '%s%s** pp%s = c%s.to_ppointer(%s);\n' % \
				   (INDENT2, ttype, var, var, var)                

			if isaGetter:
				returnByResult = not voidreturn
				postbody = postbody + \
						   '%sc%s.from_ppointer(%s);\n' % \
						   (INDENT2, var, var)

		elif argtype == 'simple*' and iotype == INPUT:
			body = body + \
				   '%sstatic boostutil::convertor< %s > c%s;\n' % \
				   (INDENT2, ttype, var)
			body = body + '%s%s* p%s = c%s.to_pointer(%s);\n' % \
				   (INDENT2, ttype, var, var, var)

			if isaGetter:
				returnByResult = not voidreturn
				postbody = postbody + \
						   '%sc%s.from_pointer(%s);\n' % \
						   (INDENT2, var,var)

		elif argtype == 'simple*' and iotype == OUTPUT:
			body = body + '%s%s %s[%s];\n' % (INDENT2, ttype, var, iosize)

		elif argtype == 'mapvector':
			body = body + '%sstatic boostutil::convertor< %s > c%s;\n' % \
				   (INDENT2, ttype, var)
			body =body + '%sstd::vector< %s >& v%s = c%s.to_vector(%s);\n'\
				   % (INDENT2, ttype, var, var, var)

			if iotype == INOUT:
				returnByResult = not voidreturn
				postbody = postbody + \
					   '%sc%s.from_vector(%s);\n' % \
					   (INDENT2, var,var)

		elif argtype == 'mapvtype':
			body = body + '%sstatic boostutil::convertor< %s > c%s;\n' % \
				   (INDENT2, ttype,var)
			body = body + \
				   '%sstd::vector< %s >& v%s = c%s.to_vector(%s);\n'\
				   % (INDENT2, ttype, var, var, var)

			if iotype == INOUT:
				returnByResult = not voidreturn
				postbody = postbody + \
					   '%sc%s.from_vector(%s);\n' % \
					   (INDENT2, var,var)
	#-------------------------------------------------
	# Determine how to handle values returned from
	# wrapped routine.
	# Note the order of the following tests.
	#-------------------------------------------------

	retlist = [] # To accumulate documentation on return types

	# Part I
	#--------
	if returnByList:
		context['RETURN'] = ''

	elif voidreturn:
		context['RETURN'] = '%(indent2)s' % context

	elif returnByResult:
		context['RETURN'] = '%(indent2)s%(newreturn)s _result = ' % context
		retlist = [(newretype,"")]

	else:
		context['RETURN'] = '%(indent2)sreturn ' % context

	# Part II
	#--------

	if returnByList:

		if retype in ['void*', 'const void*', 'char*']:
			str = '(%(newreturn)s)%(object)s%(name)s(@' % context
			retlist = [(retype,"")]

		elif simpleReturnType and returnByPointer:

			if ttypereturn == 'const char*'  or \
				   findconst.search(ttypereturn) == None:
				array = 'boostutil::array'
			else:
				array = 'boostutil::array_of_const_types'
			context['array'] = array

			str = '%(array)s< %(ttypereturn)s >' \
				   '((%(ttypereturn)s*)%(object)s%(name)s(@'

			retlist = [('%(array)s< %(ttypereturn)s >' % context,"")]

			unique(context['arraystowrap'], ttypereturn)

		else:
			str = '%(object)s%(name)s(@' % context
			retlist = [(retype,"")]

		if not voidreturn:
			body =  body + '%(indent2)sboost::python::list l;\n'
			body = body + '%(indent2)sl.append(' + str
			if simpleReturnType and returnByPointer:
				enddelim = ')))'
			else:
				enddelim = '))'
		else:
			# If we have just a single object then we don't need
			# a list
			returnByObject = len(retitem) == 1
			returnByList   = not returnByObject
			if returnByList:
				body =  body + '%(indent2)sboost::python::list l;\n'
			body = body + '%(indent2)s' + str
			enddelim = ')'

			retlist = []

	elif retype in ['void*', 'const void*', 'char*']:
		body = body + '%(RETURN)s(%(newreturn)s)%(object)s%(name)s(@'
		enddelim = ')'

		retlist = [(newretype,"")]

	elif retype == 'FILE*':
		body = body + '%(RETURN)sPyFile_FromFile(%(object)s%(name)s(@'
		enddelim = '),NULL,NULL,NULL)'

		retlist = [('file',"")]

	elif simpleReturnType and returnByPointer:

		if ttypereturn == 'const char*'  or \
			   findconst.search(ttypereturn) == None:
			array = 'boostutil::array'
		else:
			array = 'boostutil::array_of_const_types'
		context['array'] = array

		body = body +'%(RETURN)s%(array)s< %(ttypereturn)s >' \
			   '\n    ((%(ttypereturn)s*)%(object)s%(name)s(@'
		enddelim = '))'

		retlist = [('%(array)s< %(ttypereturn)s >' % context,"")]

		unique(context['arraystowrap'], ttypereturn)

	elif vectorReturnType:

		if returnByPointer:
			body = body + '%(RETURN)sboostutil::to_list< %(ttypereturn)s >' \
				   '\n    (*%(object)s%(name)s(@'
			enddelim = '))'
		else:
			body = body + '%(RETURN)sboostutil::to_list< %(ttypereturn)s >' \
				   '\n    (%(object)s%(name)s(@'
			enddelim = '))'

		retlist = [('list< %(ttypereturn)s >' % context,"")]

	else:
		body = body + "%(RETURN)s%(object)s%(name)s(@"
		enddelim = ')'
		retlist = [(newretype,"")]

	#-------------------------------------------------
	# Finish building body. To do so, we must examine
	# each argument and decide what is to be passed to
	# the wrapped routine. For a pointer to a simple
	# type we live dangerously and use boostutil::array.
	#-------------------------------------------------
	body = body % context
	aligntab = align(body, '(@')
	body = replace(body,'(@','(')

	delim = ''
	for typemapped,\
		argtype,oldarg,arg,castarg,\
		val,ttype,cbname,iotype,iosize,byvalue, var, oldvar in args:

		byreference = not byvalue

		if argtype == 'void*':
			body = body + delim + "(%s)%s" % (oldarg, var)            

		elif argtype == 'callback':
			fullcbname = fullcallbackName(context, cbname)
			body = body + delim + '%s_b.callback(%s)' % (fullcbname, var) 

		elif argtype == 'simple' and byreference and iotype != OUTPUT:
			body = body + delim + "q%s" % var

		elif argtype == 'simple*' and iotype == INPUT:
			body = body + delim + "%sp%s" % (castarg, var)

		elif argtype == 'simple*' and iotype == OUTPUT:
			body = body + delim + "%s%s" % (castarg, var)

		elif argtype == 'simple**':
			body = body + delim + "%spp%s" % (castarg, var)

		elif argtype == 'file*':
			body = body + delim + "f%s" % var

		elif argtype == 'mapvector' and iotype != OUTPUT:
			body = body + delim + "v%s" % var

		elif argtype == 'mapvtype' and iotype != OUTPUT:
			body = body + delim + "v%s" % var                

		elif typemapped:
			body = body + delim + '%s(%s)' % (castarg, var)

		else:
			body = body + delim + var

		delim = ',\n%s' % aligntab

	# Finish body and complete record

	body = body + enddelim + ';\n'

	#-------------------------------------------------
	# Now add any additional data to be returned.
	#-------------------------------------------------

	if returnByList:

		for argtype, oldarg, ttype, iotype, var, oldvar in retitem:

			if simpletype.match(lower(oldarg)) <> None:

				# We have a simple C-type

				if oldarg[-1] == '&': oldarg = oldarg[:-1]

				if pointer.search(oldarg) <> None:

					# We have a pointer to a simple C-type

					unique(context['arraystowrap'], ttype)

					if ttype in ['const char*','char*']:

						# We have (const )?char**

						postbody = postbody + \
							   '%sl.append(boostutil::'\
							   'array<%s>(%s));\n' % \
							   (INDENT2, ttype, var)

						retlist.append(('array<%s>' % ttype, oldvar))

					elif ttype in ['const char','char']:

						# We have (const )?char*

						postbody = postbody + \
							   '%sl.append(%s);\n' % \
							   (INDENT2, var)

						retlist.append((ttype, oldvar))

					elif findconst.search(ttype) == None:

						# We have a pointer to a non-const simple C-type

						if argtype in ['simple*','output'] and \
							   iotype == OUTPUT:
							postbody = postbody + \
									   '%sl.append(boostutil::'\
									   'array<%s>(%s));\n' % \
									   (INDENT2, ttype, var)

							retlist.append(('array<%s>' % ttype, oldvar))
						else:
							postbody = postbody + \
									   '%sl.append(%s[0]);\n' % \
									   (INDENT2, var)

							retlist.append((ttype, oldvar))

					elif  argtype in ['simple*','output'] and \
							   iotype == OUTPUT:

						# We have a pointer to a const simple C-type

						postbody = postbody + \
								   '%sl.append(boostutil::'\
								   'array_of_const_types'\
								   '<%s>'\
								   '(%s));\n'% (INDENT2, ttype, var)

						retlist.append(('array_of_const_stypes'\
									   '<%s>' % ttype, oldvar))

					else:
						postbody = postbody + \
								   '%sl.append(*%s);\n' % \
								   (INDENT2, var)
						retlist.append((ttype, oldvar))

				else:

					# We have a simple C-type

					postbody = postbody + \
						   '%sl.append(%s);\n' % (INDENT2, var)
					retlist.append((ttype, oldvar))
			else:

				# We have a non-simple type

				postbody = postbody + \
					   '%sl.append(%s);\n' % (INDENT2, var)
				retlist.append((oldarg, oldvar))

		# IMPORTANT: Increment reference count for returned list,
		# otherwise it will be collected by the garbage man

		postbody = postbody + "%sreturn incref(l.ptr());\n" % INDENT2

	elif returnByObject:

		# We are returning a single item

		argtype, oldarg, ttype, iotype, var, oldvar = retitem[0]
		postbody = postbody + "%sobject ob(%s);\n" % (INDENT2, var)
		postbody = postbody + "%sreturn incref(ob.ptr());\n" % \
				   INDENT2

		retlist.append((ttype, oldvar))

	elif returnByResult and not voidreturn:
		if trapnegativeReturn:
			postbody = postbody + \
					   "%(indent2)sreturn (int)_result < 0 ? "\
					   "(%(newreturn)s)0 : _result;\n" % context
		else:
			postbody = postbody + "%sreturn _result;\n" % INDENT2

	elif voidreturn:
		pass

	body = body + postbody + '%s}' % INDENT1
	body = body % context
	return (body, retlist)
#---------------------------------------------------------------------------
# Build method record. Ignore pure virtual methods and distinguish
# static methods from non-static ones. Also eliminate shadows.
#---------------------------------------------------------------------------
def determineReturnPolicy(context, retype):
	retype = strip(retype)

	opaque_pointer = opaquetype.search(retype) <> None

	if retype == "PyObject*" or \
	   voidtype.match(retype)   <> None or \
	   simpletype.match(retype) <> None or opaque_pointer:
		internal_reference = 0
	else:
		internal_reference = pointertype.search(retype) <> None

	non_const_reference = const_referencetype.search(retype) <> None

	const_reference = const_referencetype.search(retype) <> None

	ref = reference.search(retype) <> None

	# Update context
	context['opaque_pointer']     = opaque_pointer
	context['internal_reference'] = internal_reference
	context['non_const_reference']= non_const_reference
	context['const_reference']    = const_reference
	context['existing_object']    = ref
#---------------------------------------------------------------------------
def returnPolicyRecord(context, record):
	opaque_pointer      = context['opaque_pointer']
	internal_reference  = context['internal_reference']
	const_reference     = context['const_reference']
	non_const_reference = context['non_const_reference']
	existing_object     = context['existing_object']

	# NB: Do opaque test first, then internal reference test!

	if opaque_pointer:
		str = 'return_value_policy<return_opaque_pointer>())'
		if len(record+str) < MAXWIDTH:
			record = record + ', ' + str
		else:
			record = record + ',\n%(aligntab)s     ' % context + str

	elif internal_reference:
		str = 'return_internal_reference<>())'
		if len(record+str) < MAXWIDTH:
			record = record + ', ' + str
		else:
			record = record + ',\n%(aligntab)s     ' % context + str

	elif existing_object:
		str = 'return_value_policy<reference_existing_object>())'
		if len(record+str) < MAXWIDTH:
			record = record + ', ' + str
		else:
			record = record + ',\n%(aligntab)s     ' % context + str

	elif const_reference:
		str = 'return_value_policy<copy_const_reference>())'
		if len(record+str) < MAXWIDTH:
			record = record + ', ' + str
		else:
			record = record + ',\n%(aligntab)s     ' % context + str

	elif non_const_reference:
		str = 'return_value_policy<copy_non_const_reference>())'
		if len(record+str) < MAXWIDTH:
			record = record + ', ' + str
		else:
			record = record + ',\n%(aligntab)s     ' % context + str
	else:
		record = record + ')'
	return record
#---------------------------------------------------------------------------
def methodRecord(context, element, function=0):

	attrib = element.attrib

	#----------------------------------------------------------    
	# Ignore pure virtual methods
	#----------------------------------------------------------
	pureVirtualMethod = find(attrib['qual'],'=') > -1
	if pureVirtualMethod: return 0

	context['name']        = attrib['name']
	context['args']        = attrib['args']
	context['argsanalysis']= attrib['argsanalysis']
	context['return']      = attrib['return']
	context['qual']        = attrib['qual']

	# Update scopename and scopeprefix using current namespace
	# NB: namespace and name fields set by caller

	updateScopename(context)

	retype  = attrib['return']
	staticMethod = attrib['static']

	#----------------------------------------------------------
	# Construct thin wrappers
	# Note: thinWrappers should *not* change setting of
	# context['aligntab']. It is set by the caller to methodRecord
	#----------------------------------------------------------
	pynames = thinWrappers(context, element, function)       

	if len(pynames) > 0:
		retype = context['return']                   # Update return type
	else:
		if context['unsupported'] == '':
			warning("ignoring %(name)s" % context)
		else:            
			warning("ignoring %(name)s; "\
					"uses unsupported type %(unsupported)s" % context)
		return 0

	#----------------------------------------------------------
	# Handle call policies, or "figure out who owns what".
	# The only rigorous way to do this, and even this
	# is a not a sure thing, is to talk to the author 
	# of the code to be wrapped and find out what the 
	# author intended. Since the authors may not be 
	# readily available we're going to make some educated
	# guesses! 
	#----------------------------------------------------------

	# Update context with return policy

	determineReturnPolicy(context, retype)

	# Write a record for each overloaded method

	records = []
	for index, (pyname, nroutine) in enumerate(pynames):

		# Make sure context is up to date

		context['nroutine'] = nroutine  # Important!
		context['pyname'] = pyname

		if function:
			record = TAB1 + 'def("%(pyname)s", %(scopeprefix)s%(pyname)s' % \
					 context
			context['templatepar'] = '' # *** Assume non-templated for now ***

		else:
			record = '%(aligntab)s.def("%(pyname)s"' % context
			str = '%(scopeprefix)s%(pyname)s' % context
			if len(record+str) < MAXWIDTH:
				record = record + ', ' + str
			else:
				record = record + ',\n%(aligntab)s     ' % context + str

		addDocString("method", context, element,
					 '%(scopeprefix)s%(pyname)s_%(nroutine)s_doc' % \
					 context)

		record = record + '_%(nroutine)s%(templatepar)s, %(docstring)s' % \
				 context

		record = returnPolicyRecord(context, record)

		if function: record = record + ';'

		context['records'].append(record)

		if not function:
			if staticMethod:
				record = '%(aligntab)s.staticmethod("%(pyname)s")' % context
				context['records'].append(record)
#---------------------------------------------------------------------------
# Create Holder class
#---------------------------------------------------------------------------
def holderRecord(context, constructors, methods):

	debug(1,['Build Holder for class < %(holderclassname)s >' % context])

	s = ''
	s = s + '//' + MAXWIDTH*'-' + '\n'
	s = s + '// Holder for class: %(fullname)s\n' % context
	s = s + '//' + MAXWIDTH*'-'
	context['wrprecords'].append(s)
	if context['template'] <> '':
		s = context['template'] + '\n'
	else:
		s = ''
	s = s + 'class %(holderclassname)s : public %(fullname)s\n' 
	s = s + '{\n'
	s = s + 'public:'
	s = s % context
	context['wrprecords'].append(s)

	# Note whether or not a constructor contains a "throw()"
	context['throw'] = ''
	context['checkforvalue'] = 1

	for index, e in enumerate(constructors):
		context['args'] = e.attrib['args']
		context['argsanalysis'] = e.attrib['argsanalysis']
		constructorHolder(context)

		# Check for throw()
		if e.attrib['qual'] == 'throw()': context['throw'] = 'throw()'

	# Decide whether we need a copy constructor

	if context['noncopyable']:
		pass
	else:
		arg = 'const %(fullname)s&' % context
		context['args'] = [arg]
		context['argsanalysis'] = K_TOTAL*['']
		context['argsanalysis'][K_TYPEMAPPED]= 0
		context['argsanalysis'][K_OLDARG]    = arg
		context['argsanalysis'][K_ARG]       = arg
		context['argsanalysis'][K_CASTARG]   = '(%s)' % arg[:-1]
		context['argsanalysis'][K_IOTYPE]    = INPUT
		context['argsanalysis'][K_IOSIZE]    = '1'

		context['checkforvalue'] = 0
		constructorHolder(context)         

	# Add destructor

	s = TAB1 + 'virtual ~%(holderclassname)s() %(throw)s {}\n' % context
	context['wrprecords'].append(s)

	# Insert virtual overrides in class if requested

	s = ''
	if insertMethodOverride(context['classname']):

		for e in methods:
			if not e.attrib['virtual']: continue
			context['name']  = e.attrib['name']
			context['return']= isVirtual.sub('',e.attrib['return'])
			context['args']  = e.attrib['args']
			context['argsanalysis'] = e.attrib['argsanalysis']
			context['fullcbname']   = context['name']
			context['fullcbnamepy'] = '_self, "%(name)s"' % context 
			context['call'] = 'call_method'

			s = s + TAB1 + '// This method can be overridden from Python\n'
			s = s + callRecord(context, TAB1) + '\n'

	s = s + 'private:\n'
	s = s + TAB1 + 'PyObject* _self;\n'
	s = s + '};\n'
	context['wrprecords'].append(s)
#---------------------------------------------------------------------------
# Write Holder constructor
#---------------------------------------------------------------------------
def constructorHolder(context):

	argslist = context['args']
	argsanalysis = context['argsanalysis']

	signature1 = TAB1 + '%(holderclassname)s(PyObject* self' % context
	signature2 = TAB2 + ': %(fullname)s(' % context
	aligntab1  = align(signature1)
	aligntab2  = align(signature2)
	delim1 = ',\n%s' % aligntab1
	delim2 = ''
	hpprecord = ''

	for index, arg in enumerate(argslist):

		oldarg, castarg, arg, val = analyzeArgValue(arg,
													context['checkforvalue'])
		var = 'a%d' % index

		# Boost.Python doesn't like xxxx[n] types!
		if array1d.search(oldarg) <> None:
			oldarg = array1d.sub('*', oldarg)

		elif array2d.search(oldarg) <> None:
			oldarg = array2d.sub('**', oldarg)

		elif arg == "...":
			argtype = 'varargs'
			context['addcstdarg'] = 1  # Need to include <cstdarg> header
			oldarg = "va_list"

		elif arg[0:2] == 'CB': # Check for callback
			oldarg = fullcallbackName(context, arg)

		if arg == 'void':
			pass
		else:
			signature1 = signature1 + delim1 + "%s %s%s" % (oldarg, var, val)
			signature2 = signature2 + delim2 + var

		delim1 = ',\n%s' % aligntab1
		delim2 = ',\n%s' % aligntab2

	# Complete signature constructions

	signature1 = signature1 + ')'
	if context['nondestroyable']:
		signature2 = signature2 + '), _self( incref(self) ) {}'
	else:
		signature2 = signature2 + '), _self(self) {}'

	hpprecord = signature1 + '\n' + signature2 + '\n'
	hpprecord = replace(hpprecord, '%','@PERCENT') #Need to escape possible "%"
	hpprecord = hpprecord % context
	hpprecord = replace(hpprecord, '@PERCENT','%') # Go back to "%"
	context['wrprecords'].append(hpprecord)
#---------------------------------------------------------------------------
# Build constructor record.
#---------------------------------------------------------------------------
def constructorRecord(context, element):
	context['name']  = element.attrib['name']
	context['args']  = element.attrib['args']
	context['argsanalysis'] = element.attrib['argsanalysis']
	context['return']= element.attrib['return']

	# Update scopename and scopeprefix using current namespace

	updateScopename(context)

	argslist = context['args']
	argsanalysis = context['argsanalysis']

	nwrap, kdefault, arguments = getArgs(argslist)
	if len(arguments) > MAXARGS: return ''

	# Ignore copy constructors

	if len(arguments) == 1:
		arg = arguments[0]
		str = 'const %(classname)s&' % context
		if arg == str:
			warning("Exclude copy constructor")
			return ''

	firstConstructor = context['ncon'] == 0    
	if firstConstructor:
		signature = 'boost::python::init< ' % context
	else:
		signature = '%(aligntab)s.def(boost::python::init< ' % context    

	sig = signature # NB: Need a copy

	delim = ''
	for index, newarg in enumerate(arguments[:kdefault]):

		typemapped, \
		argtype, \
		oldarg, arg, castarg, val, \
		ttype,cbname, \
		iotype, iosize, byvalue, \
		var, oldvar = argsanalysis[index]

		signature = signature + delim + oldarg

		delim = ',\n%(aligntab)s ' % context
		if not firstConstructor: delim += '    '

	if kdefault < len(arguments):
		if len(signature) > len(sig): # Using copy!
			delim = ',\n%(aligntab)s ' % context
			if not firstConstructor: delim += '    '
		else:
			delim = ''

		signature = signature + delim + "boost::python::optional< "
		delim = ''
		for k, newarg in enumerate(arguments[kdefault:]):

			index = k + kdefault # Get correct index!

			# Return analyzed arguments

			typemapped, \
			argtype, \
			oldarg, arg, castarg, val, \
			ttype,cbname,iotype,iosize,byvalue,\
			var, oldvar = argsanalysis[index]

			signature = signature + delim + oldarg

			delim = ',\n%(aligntab)s ' % context
			if not firstConstructor: delim += '    '
		signature = signature + ' > '


	addDocString("ctor",
				 context, element,
				 '%(classname)s_ctor%(ncon)s_doc' % context) 
	signature = signature + ' >(%(docstring)s))' % context

	# If this is the first constructor, replace no_init with 
	# the current constructor declaration.

	if firstConstructor:
		record = context['cpprecords'].pop()
		if find(signature, '::init<  >') >-1:
			newstr = signature
		else:
			newstr =  '\n%(aligntab)s ' % context + signature
		signature = replace(record,'no_init)', newstr)
	debug(1,[signature], 0)
	context['cpprecords'].append(signature)
#---------------------------------------------------------------------------
# Strip away spurious .staticmethod invocations
#---------------------------------------------------------------------------
def pruneStaticRecords(context):
	recs = []
	context['records'].reverse()
	for index, record in enumerate(context['records']):
		if find(record,'.staticmethod') < 0:
			recs.append(record)
			continue
		if record in context['usedstatic']:
			continue
		context['usedstatic'].append(record)
		recs.append(record)
	recs.reverse()
	context['records'] = recs
#---------------------------------------------------------------------------
# Build operator records
#---------------------------------------------------------------------------
def addIterator(context, element):
	if not element.attrib.has_key('iterableclass'): return 
	names = {}
	names['aligntab'] = context['aligntab']
	names['fullname'] = element.attrib['iterableclass']
	s = ''
	s = s + '%(aligntab)s.def("__iter__",'
	s = s + 'boost::python::iterator< %(fullname)s >())'
	s = s % names
	if not s in context['cpprecords']:
		context['cpprecords'].append(s)
		debug(1,[s], 0)
#---------------------------------------------------------------------------
__len__ = '''
%(template)s
int
%(classname)s__len__(%(fullname)s& o) { return %(size)s; }
'''
def add__len__(context, indexOpList):

	if not context.has_key('hassize'): return False
	if not context.has_key('classname'): return False

	# If class exports a size() function we use the generic __len__
	# template provided by boostutil

	if context['hassize']:
		s = '%(aligntab)s.def("__len__", '
		s = s + 'boostutil::__len__< %(fullname)s >)'
	else:

		# Class does not export size() function, so create a specialized
		# __len__ function

		context['size'] = lenFunction(context['classname'])
		if context['size'] == '':
			warning('operator[] index unbounded!')
			context['size']='999999'

		s = __len__ % context
		if not s in context['wrprecords']: context['wrprecords'].append(s)

		s = '%(aligntab)s.def("__len__", '
		s = s + '%(classname)s__len__%(itemtypepar)s)'

	s = s % context
	if not s in indexOpList:
		indexOpList.append(s)
		context['cpprecords'].append(s)
		debug(1,[s], 0)
	return True
#---------------------------------------------------------------------------
__getitem__ = '''
%(template)s
%(itemtype)s
%(classname)s__getitem__(%(fullname)s& o, int i)
{
  if ( (i > -1) && (i < %(classname)s__len__%(itemtypepar)s(o) ) )
	return o[i];
  else
	{
	  PyErr_SetString(PyExc_IndexError, "index out of range");
	  boost::python::throw_error_already_set(); // bail!
	  return o[0];
	}  
}
'''
def add__getitem__(context, indexOpList):
	if not context.has_key('hassize'): return False
	if not context.has_key('classname'): return False

	# If class exports a size() function we use the generic __getitem__
	# template provided by boostutil

	if context['hassize']:
		s = '%(aligntab)s.def("__getitem__", '
		s = s + 'boostutil::__getitem__< %(fullname)s, '\
			'%(itemtype)s >'
	else:
		s = __getitem__ % context
		if not s in context['wrprecords']: context['wrprecords'].append(s)

		s = '%(aligntab)s.def("__getitem__", '
		s = s + '%(classname)s__getitem__%(itemtypepar)s'        

	# Check if this is a pointer type

	isaPointer = pointer.search(context['itemtype']) <> None
	if isaPointer:
		s = s + ',\n%(aligntab)s     '\
			'return_internal_reference<>())'
	else:
		s = s + ')'

	s = s % context
	if not s in indexOpList:
		indexOpList.append(s)
		context['cpprecords'].append(s)
		debug(1,[s], 0)
	return True
#---------------------------------------------------------------------------
__setitem__ = '''
%(template)s
void
%(classname)s__setitem__(%(fullname)s& o, int i, const %(itemtype)s& x)
{
  if ( (i >= 0) && (i < %(classname)s__len__%(itemtypepar)s(o) )) 
	o[i] = x;
  else
	{
	  PyErr_SetString(PyExc_IndexError, "index out of range");
	  boost::python::throw_error_already_set(); // bail!
	}
}
'''
def add__setitem__(context, indexOpList):

	if not context.has_key('hassize'): return False
	if not context.has_key('classname'): return False

	# If class exports a size() function we use the generic __setitem__
	# template provided by boostutil

	if context['hassize']:
		s = '%(aligntab)s.def("__setitem__", '
		s = s + 'boostutil::__setitem__< %(fullname)s, '\
			'%(itemtype)s >)'
	else:        
		s = __setitem__ % context
		if not s in context['wrprecords']: context['wrprecords'].append(s)
		s = '%(aligntab)s.def("__setitem__", '
		s = s + '%(classname)s__setitem__%(itemtypepar)s)'

	s = s % context
	if not s in indexOpList:
		indexOpList.append(s)
		context['cpprecords'].append(s)
		debug(1,[s], 0)        
	return True
#---------------------------------------------------------------------------
# Build operator record
#---------------------------------------------------------------------------
indexOpList = []
def operatorRecord(context, element):

	context['name'] = element.attrib['name']
	context['args'] = element.attrib['args']
	context['argsanalysis'] = element.attrib['argsanalysis']
	context['return']= element.attrib['return']
	context['qual'] = element.attrib['qual']
	#----------------------------------------------------------    
	# Ignore pure virtual methods
	#----------------------------------------------------------
	pureVirtualMethod = find(element.attrib['qual'],'=') > -1
	if pureVirtualMethod: return

	# Update scopename and scopeprefix using current namespace
	# NB: namespace and name, args fields set by caller

	updateScopename(context)

	name     = context['name'];
	argslist = context['args'];
	argsanalysis = context['argsanalysis'];
	retype   = context['return'];
	qual     = context['qual'];

	# Get operator name

	m = opname.search(name)
	if m == None:
		warning("%s currently unsupported" % name)
		return
	op = m.group('name')
	context['oper'] = op

	# Analyze its calling sequence

	nwrap, kdefault, arguments = getArgs(argslist)

	# Update context with return policy

	determineReturnPolicy(context, retype)

	#---------------------------------------------
	# Handle operator[](const int)
	#---------------------------------------------

	if op == '[]':

		# Make sure argument is an "int" type

		if len(arguments) == 1:
			str = arguments[0]
			str = lower(str)
			if intctype.match(str) == None:
				warning("%s (%s) currently unsupported" % (name,arguments[0]))
				return

		# Bail if we have void or void** return type
		if retype in ['void','void**','const void**']:
			warning("%s %s(%s) currently unsupported" % \
					(retype, name, arguments[0]))
			return

		# Check whether to add get and setitem
		const = context['qual'] == 'const'
		abstract = context['abstract']
		ref   = reference.search(retype) <> None

		addsetitem = (not const) and ref

		if abstract:            
			addgetitem = 0
		elif const:
			if ref:
				addgetitem = 1
			else:
				addgetitem = 0
		else:
			addgetitem = 1

		# Remove possible "&" and const from return type
		retype = reference.sub('', retype)
		if retype <> "const char*":
			retype = strip(constWord.sub('',retype))

		#---------------------------------------------
		# We may need to add __len__, __getitem__
		# and/or __setitem__ functions. Handle both
		# templated and non-templated classes.
		#---------------------------------------------

		context['itemtype'] = retype
		if context['template'] != '':
			context['itemtypepar'] = '< %(itemtype)s >' % context
		else:
			context['itemtypepar'] = ''        

		add__len__(context, indexOpList)

		if addgetitem: add__getitem__(context, indexOpList)

		if addsetitem: add__setitem__(context, indexOpList)

		return

	#---------------------------------------------
	# Handle operator++()
	#---------------------------------------------
	elif op == '++':
		if len(arguments) == 0:
			if context['template'] <> '':
				s = context['strippedtemplate'] + '\n'
			else:
				s = ''
			s = s + '%s\n' % retype
			s = s + '%(scopeprefix)snext' % context
			s = s + '(%(fullname)s& o)\n' % context
			s = s + '{\n' 
			s = s + '  return o.operator++();\n'
			s = s + '}\n'
			unique(context['wrprecords'], s)

			addDocString("op",
						 context, element,
						 '%(scopeprefix)snext_doc' % context)

			s = '%(aligntab)s.def("next", %(scopeprefix)snext, %(docstring)s'%\
				context

			if context['template'] <> '': s += context['templatepar']

			s = returnPolicyRecord(context, s)
			unique(context['cpprecords'], s)

			return
	#---------------------------------------------
	# Handle operator--()
	#---------------------------------------------    
	elif op == '--':
		if len(arguments) == 0:
			if context['template'] <> '':
				s = context['strippedtemplate'] + '\n'
			else:
				s = ''
			s = s + '%s\n' % retype
			s = s + '%(scopeprefix)sprevious' % context
			s = s + '(%(fullname)s& o)\n' % context
			s = s + '{\n' 
			s = s + '  return o.operator--();\n'
			s = s + '}\n'
			unique(context['wrprecords'], s)

			addDocString("op",
						 context, element,
						 '%(scopeprefix)sprevious_doc' % context)

			s = '%(aligntab)s.def("previous", %(scopeprefix)sprevious'\
				'%(docstring)s' % \
				context

			if context['template'] <> '': s += context['templatepar']

			s = returnPolicyRecord(context, s)        
			unique(context['cpprecords'], s)
			return
	#---------------------------------------------
	# Handle operatorXX(ZZ)
	#---------------------------------------------
	elif len(arguments) == 1:

		if not op in IGNORE_OPERATORS:
			arg = arguments[0]
			arg = replace(arg,'&', '')

			context['oper'] = op
			context['left'] = 'self'

			# Check argument type. If it contains class name
			# use self <op> self construct, otherwise use other<..>()

			if find(arg, context['classname']) > -1:
				right = 'self'

			elif arg == 'void' or arg == '':
				left  = ''
				right = 'self'

			else:
				m = simpletype.match(lower(arg))
				if m == None:
					right = 'other< %s >()' % arg
				elif arg == 'const char*':
					right = 'other< %s >()' % arg
				elif arg == 'Ssiz_t':            
					right = 'other< %s >()' % arg
				elif len(split(arg)) > 1:
					right = 'other< %s >()' % arg        
				else:
					name = m.group('name')
					right = '%s()' % strip(replace(name,'const ',''))

			context['right']= right
			s = '%(aligntab)s.def(%(left)s %(oper)s %(right)s)' % context
			unique(context['cpprecords'], s)
			debug(1,[s], 0)
		return

	# Ignore operators with anything other than two arguments

	if len(arguments) <> 2: return

	#---------------------------------------------
	# Handle operatorXX(AA, BB)
	#---------------------------------------------    

	# Get left and right arguments of operator method (args 1 and 2!)

	larg, rarg = arguments
	larg = replace(larg,'&', '')
	rarg = replace(rarg,'&', '')

	# Try to get classname etc.

	if context['fullname'] == '':
		t = split(rarg)
		context['classname'] = t[len(t)-1]
		if context['namespace'] <> []:
			str = joinfields(context['namespace'],'::')+'::%(classname)s' % \
				  context 
			context['fullname'] = str
		else:
			context['fullname'] = context['classname']
		context['scopename'] = context['fullname'] + '::'
		context['scopeprefix'] = replace(context['scopename'],'::','_')

	# Handle operator<< (std::ostream&, <classname>&)

	if op == "<<":
		if find(larg, 'ostream') > -1 and \
			   find(rarg, context['classname']) > -1:
			s = ''
			s = s + 'std::string\n'
			s = s + '%(scopeprefix)sstr' % context
			s = s + '(%(fullname)s& o)\n' % context
			s = s + '{\n' 
			s = s + '  std::stringstream s;\n'
			s = s + '  s << o;\n'
			s = s + '  return std::string(s.str());\n'
			s = s + '}\n'
			unique(context['wrprecords'], s)

			s = '%(aligntab)s.def("__str__", %(scopeprefix)sstr)'% context
			#s = '%(aligntab)s.def(str(self))'% context
			unique(context['cpprecords'], s)
		return

	# Handle other operators using boost construct other<...>()

	if find(larg, context['classname']) > -1:
		left = 'self'
	else:
		m = simpletype.match(lower(larg))
		if m == None:
			left = 'other< %s >()' % larg
		elif larg == 'const char*':
			left = 'other< %s >()' % larg
		elif larg == 'Ssiz_t':            
			left = 'other< %s >()' % larg
		elif len(split(larg)) > 1:
			left = 'other< %s >()' % larg
		else:
			name = m.group('name')
			left = '%s()' % strip(replace(name, 'const ', ''))

	if find(rarg, context['classname']) > -1:
		right = 'self'
	else:
		m = simpletype.match(lower(rarg))
		if m == None:
			right = 'other< %s >()' % rarg
		elif rarg == 'const char*':
			right = 'other< %s >()' % rarg
		elif rarg == 'Ssiz_t':            
			right = 'other< %s >()' % rarg
		elif len(split(rarg)) > 1:
			right = 'other< %s >()' % rarg            
		else:
			name = m.group('name')
			right = '%s()' % strip(replace(name,'const ',''))

	context['left'] = left
	context['right']= right

	s = '%(aligntab)s.def(%(left)s %(oper)s %(right)s)' % context
	if not s in context['cpprecords']:
		context['cpprecords'].append(s)
		debug(1,[s], 0)
	return
#---------------------------------------------------------------------------
# Build enum record, either within or outside class scope
#---------------------------------------------------------------------------
def enumRecord(context, element):

	name = element.attrib['name']
	context['name'] = name

	# Update scopename and scopeprefix using current namespace
	# NB: namespace and record fields set by caller

	updateScopename(context)

	if context['name'] == '':
		warning("anonymous enum currently unsupported")
		return

	s = '%(tab)senum_<%(scopename)s%(name)s>("%(name)s")' % context

	fields = findElements(element, 'field')
	if len(fields) > 0:
		values = map(lambda x: x.attrib['name'], fields)
	else:
		values = findAll(enumvalue, element.text)
		values = map(lambda x: split(x[0],'=')[0], values)

	for value in values:
		context['value'] = value
		s1 = '%(tab)s  .value("%(value)s"' % context 
		s2 = '%(scopename)s%(value)s)' % context
		if len(s1+s2) < MAXWIDTH:
			s = s + '\n' + s1 + ', ' + s2
		else:
			s = s + '\n' + s1 + ',\n%(tab)s         ' % context + s2
	s = s + ';\n'
	if context['inclass']:
		context['cppend'].append(s)
	else:
		context['enumrecords'].append(rstrip(s))
#---------------------------------------------------------------------------
# Try to write a plausible callback function that could be used in a method
# that requires one.
#---------------------------------------------------------------------------
def callRecord(context, tab='', breakit=''):

	retype = context['return']

	hstr = tab + '%(return)s\n' % context + tab + '%(fullcbname)s(' % context
	aligntab = align(hstr)
	delim = ''
	args = []
	for index, arg in enumerate(context['args']):

		if arg == 'void':
			var = ''
		else:
			var = ' a%d' % index
		arg = replace(arg,'[]','*')

		args.append((strip(arg), strip(var)))
		hstr = hstr + delim + arg + var
		delim = ',\n%s' % aligntab

	bodystart = ')\n%s{\n' % tab        
	bodyend   = ');\n%s}\n' % tab
	hstr = hstr + bodystart

	context['break'] = breakit
	s = tab
	if retype == "void":
		s = s + '  %(call)s<void>%(break)s(%(fullcbnamepy)s' % context       
	elif retype == "void*":
		s=s +'  return (void*)%(call)s<%(opaque)s*>%(break)s(%(fullcbnamepy)s'\
		   % context
		context['addopaque'] = 1
	else:
		s = s + '  return %(call)s<%(return)s>%(break)s(%(fullcbnamepy)s' % \
			context

	aligntab = align(s,'(',1) # Align at first '('
	s = replace(s, 'ALIGNTAB', aligntab)

	# Decide what to do according to argument type

	delim = ',\n%s' % aligntab

	argstr= '('
	d = ''
	for arg, var in args:
		argstr = argstr + d + arg; d = ', '

		if arg == "void" and len(args) == 1:
			str = ''
			delim = ''

		elif arg == "void*":
			str = 'borrowed((PyObject*)%s)' % var

		elif arg == "const void*":
			str = 'borrowed((PyObject*)%s)' % var

		elif arg == "PyObject*":
			str = 'borrowed(%s)' % var

		elif simpletype.match(lower(arg)) <> None:
			if pointer.search(arg) <> None:
				if arg[-1] == "*":                    
					ttype= arg[:-1]
				elif arg[-2:] == '[]':
					ttype= arg[:-2]

				if ttype == 'const char' or ttype == 'char':
					str = var
				elif ttype == 'const char*' or findconst.search(ttype) == None:
					str = 'boostutil::array< %s >(%s)' % (ttype, var)
					unique(context['arraystowrap'], ttype)
				else:
					str = 'boostutil::array_of_const_types< %s >(%s)' % \
						  (ttype, var)
					unique(context['arraystowrap'], ttype)
			else:
				str = var
		elif constreference.search(arg) <> None:
			str = 'cref(%s)' % var
		elif reference.search(arg) <> None:
			str = 'ref(%s)' % var
		elif pointer.search(arg) <> None:
			str = 'ptr(%s)' % var
		else:
			str = var
		s = s + delim + str
	argstr = argstr + ')'
	s = hstr + s + bodyend
	return s
#---------------------------------------------------------------------------
def callbackRecord(context, cbname, fullcbname, retype, argslist, inline):

	retype = stripCtype.sub("",retype)    
	for index, arg in enumerate(argslist):
		arg = stripCtype.sub("",arg)
		argslist[index] = arg

	context['fullcbname']   = fullcbname
	context['callbackinit'].append(fullcbname)
	context['return'] = retype
	context['args'] = argslist
	context['call'] = 'call'

	# Write typedef and buffer records

	h = ''
	t = 'typedef\n%s\n%s(' % (context['return'], fullcbname)
	aligntab = align(t)

	argstr = joinfields(argslist,',\n%s' % aligntab)
	t = t + '%s);\n' % argstr
	h = h + t
	t = 'boostutil::cbbuffer<%(fullcbname)s*> %(fullcbname)s_b;\n\n' % context
	if len(t) > MAXWIDTH:
		t = 'boostutil::cbbuffer<%(fullcbname)s*>\n%(fullcbname)s_b;\n\n' % \
			context
	h = h + t

	# Write callback macros

	h = h + '// Callback macros\n'
	s = '#define %s(_n_)\n' % upper(fullcbname)
	context['fullcbname'] = fullcbname
	t = '%(fullcbname)s_b.pycallback(%(fullcbname)s_##_n_)' % context
	if len(t) > MAXWIDTH:
		t = '%(fullcbname)s_b.\nALIGNTAB' % context
		t = t + 'pycallback(%(fullcbname)s_##_n_)' % context
	context['fullcbnamepy'] = t
	context['fullcbname'] = '%(fullcbname)s_##_n_' % context

	s = s + callRecord(context,'','\n'+TAB2)

	s = replace(s,'\n',' \\\n')[:-2]
	h = h + s

	s = '#define %s_INIT(_n_)\n' % upper(fullcbname)
	context['fullcbname'] = fullcbname
	t = '  %(fullcbname)s_b.push(%(fullcbname)s_##_n_);\n' % context
	if len(t) > MAXWIDTH:
		t = '  %(fullcbname)s_b.' % context 
		t = t + '\n    push(%(fullcbname)s_##_n_);\n' % context
	s = s + t
	s = replace(s,'\n',' \\\n')[:-2]
	h = h + '\n' + s + '\n\n'

	# Instantiate macros

	h = h + '// Instantiate callbacks\n'
	context['fullcbname'] = upper(fullcbname)
	for n in xrange(callbackCount(cbname)):
		context['n'] = n + 1
		h = h + '%(fullcbname)s(%(n)s)\n' % context
	h = h + '\n'

	return h
#---------------------------------------------------------------------------
# Build member record.
# Distinguish between read-only and write-only members.
#---------------------------------------------------------------------------
def memberRecord(context, element):

	# Update scopename and scopeprefix using current namespace
	# NB: namespace and record fields set by caller

	updateScopename(context)

	record = strip(element.text)

	static = find(record,'static ') > -1
	t = split(record)
	name = t[len(t)-1]
	name = strip(name[:len(name)-1])
	if find(name,'[') > -1:
		warning('%s array currently unsupported' % strip(record))
		return

	# Can't handle bit-fields for now, so ignore them

	if isBitField.match(record) <> None:
		inform('bit-field %s currently unsupported' % record)
		return

	context['name'] = name

	if static:
		s = '%(tab)s%(scopeprefix)sscope->attr("%(name)s")='\
			'%(scopename)s%(name)s' % context
	else:
		if readonly.search(record) == None:
			s1 = '%(aligntab)s.def_readwrite("%(name)s",' % context 
		else:
			s1 = '%(aligntab)s.def_readonly("%(name)s",' % context 

		s2 = '&%(scopename)s%(name)s)' % context
		if len(s1+s2) < MAXWIDTH:
			s = s1 + ' ' + s2
		else:
			s = s1 + '\n%(aligntab)s' % context + 15*' ' + s2 

	if static:
		s = s + ';\n'
		context['cppend'].append(s)
		debug(1,['static', s], 0)
	else:
		context['cpprecords'].append(s)
		debug(1,[s], 0)
#---------------------------------------------------------------------------
# Add a record to import each base class module
#---------------------------------------------------------------------------
def shortName(fullname):
	name = fullname
	words = swords.findall(fullname)
	for word in words:        
		# strip away possible namespaces to get short name            
		if DB.namespaceDB.has_key(word):
			wrd = re.compile(r'\b%s::' % word)
			name = wrd.sub('', name)
	return name
def importBaseModules(context):

	# First import classes specified by user

	s = ''
	clist = importClass(context['classname'])
	for classname, sharedlib in clist:
		if classname in context['imports']: continue
		context['imports'].append(classname)
		context['imports'].append(split(classname,'::').pop())
		element = DB.classElement(classname)
		if element != None:
			fullname = splitfields(element.attrib['fullname'],'<',1)[0]
		else:
			fullname = classname
		s = s + TAB1 + \
			'boostutil::import< %s >("%s");\n' % (fullname, sharedlib)

	# Try to import base classes, if possible

	for inheritance, basename in context['bases']:

		if inheritance <> 'public':    continue

		classname = split(basename,'::').pop()
		if classname in context['imports']: continue
		context['imports'].append(classname)

		element = DB.classElement(basename)
		if element != None:
			fullname = splitfields(element.attrib['fullname'],'<',1)[0]
		else:
			fullname = basename

		# If no description for class exists, register a dummy version

		header = DB.headerElement(classname)
		if header == None:
			s = s + TAB1 + \
				'// WARNING: Registering a dummy class %s\n' % classname
			s = s + TAB1 + \
				'class_< %s, boost::noncopyable > ("%s", no_init);\n' \
				% (fullname, classname)
			inform('Registering dummy base class %s' % classname)
		else:
			headername = DB.headername(header)
			if headername <> '':
				if find(fullname,'<') > 0: # Check for template
					s = s + TAB1 + \
						'boostutil::import< %s >("libPy%s");\n' % \
						(fullname, renameIdentifier(shortName(fullname)))
				else:
					s = s + TAB1 + \
						'boostutil::import< %s >("libPy%s");\n' % \
						(fullname, headername)

			elif forceImport(context['classname']):
				headername = renameIdentifier(classname)
				s = s + TAB1 + \
					'boostutil::import< %s >("libPy%s");\n' % \
					(fullname, headername)
				inform('inserted an import for base class %s' % classname)
			else:
				s = s + TAB1 + \
					'// WARNING: No import for class %s' % classname
				inform('No import for base class %s' % classname)
	return rstrip(s)
#---------------------------------------------------------------------------
# Add a record to import each module on which current one depends
#---------------------------------------------------------------------------
def importRecords(context):
	s = ''
	for inheritance, basename in context['bases']:
		if inheritance <> 'public':    continue
		classname = split(basename,'::').pop()
		if classname in context['imports']: continue
		context['imports'].append(classname)

		name = classHeaderName(classname)
		if name == '':
			s = s + TAB1 + \
				'class_< %s, boost::noncopyable > ("%s", no_init);\n' \
				% (basename, classname)
		else:
			modname = classModuleName(name)
			if modname == '': continue
			s = s + TAB1 + 'boostutil::import("libPy%s");\n' % name

	incs = DB.includeDB.values()
	if len(incs) == 0: return rstrip(s)
	s = s + '\n'
	for header in incs:
		if header[0] in ['"','<']:
			name = nameonly(header[1:-1])
		else:
			name = nameonly(header)
		if name == context['headername']: continue
		if name in context['imports']: continue
		context['imports'].append(name)
		s = s + TAB1 + 'boostutil::import("libPy%s");\n' % name
	return rstrip(s)
#---------------------------------------------------------------------------
# Add include records
#---------------------------------------------------------------------------
def includeRecords(preamble):
	global CONTEXT
	if CONTEXT.has_key('headername'):
		headername = CONTEXT['headername']
	else:
		headername = ''
		CONTEXT['headername'] = ''

	incs = DB.includeDB.values()
	incs.sort()

	# Insert headers specified in edits.py

	insertBeforeHeaders(CONTEXT, preamble)

	for header in incs:
		if excludeInclude(headername, header): continue
		if header[0] in ['"','<']:
			include= '#include %s' % header
		else:
			include= '#include "%s"' % header
		# Include include, once only!
		if include not in preamble:
			preamble.append(include)
			debug(1,[include], 0)
#---------------------------------------------------------------------------
# Check if class needs to be shadowed. A class needs to be shadowed if it
# contains at least one constructor with an argument that requires special
# handling, such as pointers to simple types or callbacks. I'm doing this
# because Boost.Python does not provide a mechanism for writing thin
# wrappers around constructors..at least as far as I can tell!
#---------------------------------------------------------------------------
def checkForProxy(context, constructors):
	debug(1,['Check for proxy'])

	for i, e in enumerate(constructors):

		argslist = e.attrib['args']
		for arg in argslist:

			tmp = splitfields(arg,'=',1) # Could be optional argument
			if len(tmp) == 1:
				arg = strip(tmp[0])
				val = ''
			else:
				arg, val = tmp
				arg = strip(arg)
				val = strip(val)

			# Construct possible callback keys
			# Note: it is possible that the callback argument is of the
			#       form <A>::<B> etc.

			callbkey = stripName(arg)
			callbkey2= split(callbkey,'::').pop()

			debug(1,['\targ: %s' % arg],0)

			if arg == 'const char*' or arg == 'char*':
				continue

			# Check if this is an in-line callback argument

			elif callback.match(arg) <> None: return 1

			# Check if this is a callback argument defined by a typedef

			elif DB.callbackDB.has_key(callbkey) or \
				 DB.callbackDB.has_key(callbkey2):
				return 1

			elif arg == "void*":
				return 1

			elif arg == "void* const":
				return 1

			elif simpletype.match(lower(arg)) <> None:
				if pointer.search(arg) <> None: return 1

	debug(1,['NO Proxy NEEDED',''],0)    
	return 0
#---------------------------------------------------------------------------
# Create proxy class
#---------------------------------------------------------------------------
# Check for shadows
pyorobject = re.compile('PyObject\*|object\&', re.M)
#---------------------------------------------------------------------------
def shadow(currentname, currentargs, routineargs):

	# Scan existing methods

	kindex = -1

	for index, args in enumerate(routineargs):

		# The only possibility of a shadow is if the
		# number of arguments current routine is the same
		# as that of an existing one.

		if len(currentargs) != len(args): continue

		# void is really no argument, so ignore it
		if len(args) == 1 and args[0] == 'void': continue

		# Check each argument. Shadowing occurs if each argument pair, formed
		# of the current argument of the current routine and the current
		# argument of the routine to which it is being compared, is identical.
		#
		# Identical means either that each member of the pair is of the same
		# type or that at least one of them is either a PyObject or an object.

		identical = True
		for i, arg in enumerate(args):

			if arg == currentargs[i]: continue
			if pyorobject.match(arg): continue
			if pyorobject.match(currentargs[i]): continue

			identical = False
			break

		# If the argument types match, then we've found a shadow
		# Return the index of the routine of which the current one
		# is a a shadow
		if identical:
			kindex = index
			break

	# Update signature list for current routine name
	# before we exit
	routineargs.append(currentargs) 
	return kindex
#---------------------------------------------------------------------------
#getargs = re.compile('(?<=[0-9])\([^\)]*\)$', re.M)
getargs = re.compile('(?<=[0-9])\(.*(?=\)$)\)$', re.M)
def extractArgs(signature):
	name = joinfields(split(signature))
	m = getargs.search(name)
	if m == None: fatal("extractArgs failed on\n\t%s" % signature)        
	args = m.group()
	return args
#---------------------------------------------------------------------------
def proxyClass(context, constructors):

	debug(1,['Build Proxy class < %(proxyclassname)s >' % context])

	s = ''
	s = s + '//' + MAXWIDTH*'-' + '\n'
	s = s + '// Proxy for class: %(fullname)s\n' % context
	s = s + '//' + MAXWIDTH*'-'
	context['wrprecords'].append(s)
	context['wrprecords'].append('PLACEHOLDER');

	convertorname = '%(classname)sConvertor' % context
	context['convertorname'] = convertorname

	if context['template'] <> '':
		s = context['template'] + '\n'
	else:
		s = ''

	str = 'class %(proxyclassname)s : public %(convertorname)s,\n' % \
		  context
	n = find(str,'public')
	s = s + str
	s = s + n*' ' + 'public %(fullname)s\n'
	s = s + '{\n'
	s = s + 'public:'
	s = s % context
	context['wrprecords'].append(s)

	context['signature1'] = [] # To keep track of constructor signature
	vbuf = []
	for index, e in enumerate(constructors):

		context['name'] = e.attrib['name']
		context['return'] = ''
		sigargs, s1, s2, \
		hpprecord, v, newargs, \
		newargsanalysis = constructorProxy(context, e,index)

		# IMPORTANT: Update constructor arguments
		e.attrib['args'] = newargs
		e.attrib['argsanalysis'] = newargsanalysis 

		context['signature1'].append(s1)
		context['wrprecords'].append(hpprecord)
		debug(1,[hpprecord], 0)   

		vbuf = vbuf + v
	s = TAB1 + 'virtual ~%(proxyclassname)s() {}\n};\n' % context
	context['wrprecords'].append(s)

	#-------------------------------------------------------------------
	# Build struct containing converters.
	# Add dummy template decoration (template<DUMMY>) to ensure all
	# converters are picked when building export_XXXXX.hpp
	# But remember to delete decoration before writing header!
	#-------------------------------------------------------------------
	if context['template'] <> '':
		s = 'template<DUMMY>'
	else:
		s = ''

	if len(vbuf) > 0:
		s = s + 'struct %(classname)sConvertor\n'
		s = s + '{\n'
		for ttype, var in vbuf:
			ttype = strip(replace(ttype, 'const ', '')) # get rid of const
			s = s + '  boostutil::convertor< %s > %s;\n' % (ttype, var)

		s = s + '};\n'
		s = s % context
		pos = context['wrprecords'].index('PLACEHOLDER')
		context['wrprecords'][pos] = s # Replace PLACEHOLDER with proper record
	else:
		pos = context['wrprecords'].index('PLACEHOLDER')
		s = s + 'struct %(classname)sConvertor {};' % context
		context['wrprecords'][pos] = s

	# Build proxy record
	# Start class preamble 

	cpprecord = '%(tab)sscope* %(proxyclassname)s_scope = new\n\n' + \
				'%(tab)s  scope(class_< %(proxyclassname)s%(templatepar)s'
	cpprecord = cpprecord % context
	context['aligntab'] = align(cpprecord) # Set alignment tab
	cpprecord = cpprecord + ',\n%(aligntab)sbases< %(fullname)s >' % context

	# Add noncopyable if base is.
	if context['noncopyable']: cpprecord = cpprecord + ', noncopyable'
	cpprecord = cpprecord + ' >\n'

	if context['template'] <> '':
		cpprecord = cpprecord + '%(aligntab)s(name, no_init)' % context
	else:
		cpprecord = cpprecord + '%(aligntab)s("%(pyclassname)s", no_init)' % \
					context
	context['cpprecords'].append(cpprecord)
	debug(1,[cpprecord,''],0)

	# Add constructors

	j = 0
	for i, e in enumerate(constructors):

		if context['signature1'][j] <> '': 
			context['ncon']= j
			constructorRecord(context, e)
			j += 1

	# Complete proxy class record

	ln = len(context['cpprecords'])
	if context['cpprecords'][ln-1] == '': ln -= 1
	context['cpprecords'][ln-1] = context['cpprecords'][ln-1]+');\n'
	context['cpprecords'].append("%(tab)sdelete %(proxyclassname)s_scope;" \
							   % context)
#---------------------------------------------------------------------------
# Write proxy constructor.
#
# Arguments:
#  context     map containing context
#  count     ordinal value of constructor
#---------------------------------------------------------------------------
def constructorProxy(context, element, count):
	#------------------------------------------------------------------
	# A constructor proxy consists of a constructor together with its
	# initialization of the class being shadowed.
	#------------------------------------------------------------------
	element.attrib['doc'] = []
	argslist = element.attrib['args']
	argsanalysis = element.attrib['argsanalysis']
	oldargs    = argslist
	sigargs    = [] # Keep track of non-defaulted constructor arguments

	signature1 = TAB1 + '%(proxyclassname)s(' % context    # constructor shadow
	signature2 = TAB2 + ': %(fullname)s(' % context  # constructor shadowed
	aligntab1  = align(signature1)
	aligntab2  = align(signature2)
	delim1 = ''
	delim2 = ''
	hpprecord = ''

	cbs  = []    # Keep track of callbacks
	vbuf = []    # Keep track of vector buffers
	newargs = [] # Keep track of modified arguments

	for index, arg in enumerate(argslist):

		# Return analyzed argument

		typemapped, argtype, \
		oldarg,arg,\
		castarg,val,ttype,cbname,iotype,iosize,byvalue, \
		var, oldvar = argsanalysis[index]

		# Update sigargs with non-defaulted arguments *only*
		# since, unlike the case for methods, we do not create
		# a wrapper for each version of the constructor.

		if val == '':
			sigargs.append(arg)

		# Update new arguments
		newargs.append("%s%s" % (arg,val))  # We may have defaulted arguments

		argsanalysis[index][K_OLDARG] = arg # NB: update oldarg

		if argtype == 'callback':
			signature1 = signature1 + delim1 + "%s %s%s" % (arg, var, val)
			cbname = fullcallbackName(context, cbname)
			str = ''
			if val == '': # Check for default callback 
				str = '%s_b.callback(%s)' % (cbname, var)
			else:                
				str = '%s==%s ? %s : %s_b.callback(%s)' % \
					  (var,val[1:],val[1:],cbname, var)

			signature2 = signature2 + delim2 + str
			cbs.append((cbname, var))

		elif argtype == 'void*':
			signature1 = signature1 + delim1 + "%s %s%s" % (arg, var, val)
			signature2 = signature2 + delim2 + "(%s)%s" % (oldarg, var)

		elif argtype == 'simple**':
			signature1 = signature1 + delim1 + "%s %s%s" % (arg, var, val)
			signature2 = signature2 + delim2 + "%sc%s%s.to_ppointer(%s)" % \
						 (castarg, var, count, var)
			vbuf.append((ttype, 'c%s%s' % (var,count)))            

		elif argtype[0:7] == 'simple*':
			signature1 = signature1 + delim1 + "%s %s%s" % (arg, var, val)
			signature2 = signature2 + delim2 + "%sc%s%s.to_pointer(%s)" % \
						 (castarg, var, count, var)
			vbuf.append((ttype, 'c%s%s' % (var,count)))

		elif argtype == 'mapvector' or argtype == 'mapvtype':
			signature1 = signature1 + delim1 + "%s %s%s" % (arg, var, val)
			signature2 = signature2 + delim2 + "c%s%s.to_vector(%s)" % \
						 (var, count, var)
			vbuf.append((ttype, 'c%s%s' % (var,count)))

		elif argtype == 'void':
			pass

		else:
			signature1 = signature1 + delim1 + "%s %s%s" % (arg, var, val)
			signature2 = signature2 + delim2 + var

		delim1 = ',\n%s' % aligntab1
		delim2 = ',\n%s' % aligntab2

	# Complete signature constructions

	signature1 = signature1 + ')'
	signature2 = signature2 + ')'

	# Build constructor body

	body = TAB2 + '{}'
	hpprecord  = signature1 + '\n' + signature2 + '\n' + body + '\n'
	hpprecord = replace(hpprecord, '%','@PERCENT') #Need to escape possible "%"
	hpprecord = hpprecord % context
	hpprecord = replace(hpprecord, '@PERCENT', '%') 
	return (sigargs,
			signature1, signature2, hpprecord, vbuf, newargs, argsanalysis)
#---------------------------------------------------------------------------
# Find un-bound operators and add them to appropriate class
#---------------------------------------------------------------------------
def findOperators(element, ops, depth=0):
	depth += NTAB
	if depth >= 50:
		fatal("lost in tree...I'm bailing!\n\t\t%s" % element.tag)

	# Look for operators in current scope    

	oplist = element.findall('operator')
	for op in oplist:
		ops.append(op)

	# Check for operators within namespaces

	nlist = element.findall('namespace')
	for namespace in nlist:
		findOperators(namespace, ops, depth)
#---------------------------------------------------------------------------
def addOperatorsToClass(root, depth=0):
	depth += NTAB
	if depth >= 50:
		fatal("lost in tree...I'm bailing!\n\t\t%s" % element.tag)

	debug(2,['Operators'])
	ops = []
	findOperators(root, ops)
	for i, e in enumerate(ops):
		debug(2,[write("< %s >" % e.tag, depth)],0)
		for item in e.attrib.items():
			debug(2,[write("\t%s: %s" % item, depth)],0)

	debug(2,['Look In Classes'])
	classes = []
	findClassElements(root, classes)
	for i, e in enumerate(classes):
		classname = e.attrib['name']
		debug(2,['','\tclass: %s' % classname],0)

		for j, opelem in enumerate(ops):
			opname = opelem.attrib['name']
			args   = opelem.attrib['args']
			arguments = eval(args)
			for arg in arguments:
				arg = replace(arg,'&','')
				arg = replace(arg,'const ','')
				arg = split(strip(arg),'::')
				arg = arg.pop()

				debug(2,['\t\targ:\t%s' % arg],0)
				if arg == classname:
					xelem = XML('<section name="public"></section>')
					xelem.append(opelem)
					e.append(xelem)
					debug(2,['\tADD %s%s to class %s' % (opname,
														 arguments,
														 classname)],0)
#---------------------------------------------------------------------------
def arrayWrappers(context):
	for index, ttype in enumerate(context['arraystowrap']):
		if excludeInstantiationType(ttype): continue
		instantiateTemplate(context, "array", [ttype], True)

def vectorWrappers(context):    
	for index, ttype in enumerate(context['vectorstowrap']):
		if excludeInstantiationType(ttype): continue
		instantiateTemplate(context, "vector", [ttype], True)
#---------------------------------------------------------------------------
# Given a class return the name of the Python module to which it belongs
#---------------------------------------------------------------------------
def classModuleName(classname):
	module = DB.query('module', classname)
	if module <> None:
		return module
	else:
		return ''
#---------------------------------------------------------------------------
# Given a class return the name of the Python project to which it belongs
#---------------------------------------------------------------------------
def classProjectName(classname):
	project = DB.query('project', classname)
	if project <> None:
		return project
	else:
		return ''    
#---------------------------------------------------------------------------
# Given a class return the name of the header file to which it belongs
#---------------------------------------------------------------------------
def classHeaderName(classname):
	h = DB.headerElement(classname)
	if h <> None:
		return DB.headername(h)
	else:
		# This may be an stl vector type
		name = renameIdentifier(classname)
		if stl_vtype.match(name) <> None:
			return name
		else:
			return ''
#---------------------------------------------------------------------------
# Write module.cpp.
# It is important to call the export functions in the correct order, so
# that base classes are registered before derived classes. 
#-----------------------------------------------------------------------------
def writeModuleFile(context, file):

	command = 'grep -h "//@BaseDependency:" %s' % file
	depends = map(lambda x: split(x)[1:], os.popen(command).readlines())
	classToHeader = {}
	for items in depends:
		if len(items) > 1:
			header    = items[0]        
			classname = items[1]
		else:
			header    = items[0]
			classname = items[0]
		classToHeader[classname] = header

	#---------------------------------------------------------------
	# Map each header (which is one-to-one with the export function)
	# to the base classes on which it depends. Some of these base
	# classes are internal to the current module and some not.
	#---------------------------------------------------------------
	exportmap = {}
	for index, items in enumerate(depends):
		if len(items) > 1:
			header    = items[0]        
			classname = items[1]
		else:
			header    = items[0]
			classname = items[0]

		if not exportmap.has_key(header): exportmap[header] = set([])
		if len(items) > 2:
			basenames = items[2:]
		else:
			basenames = []

		for basename in basenames:
			if classToHeader.has_key(basename):
				name = classToHeader[basename]
				# Make sure to avoid self-references
				if name <> header:
					exportmap[header].add(name)

	#---------------------------------------------------------------
	# List exports with no dependencies
	#---------------------------------------------------------------
	exports = []
	headers = map(lambda x: x, exportmap.keys())
	headers.sort()
	all = set(headers)
	for header in headers:        
		if len(exportmap[header]) == 0:
			exports.append(header)            

	#---------------------------------------------------------------
	# Compute remaining AND exportmap[header].
	# If this is the empty set, then current header comes before
	# remaining headers
	#---------------------------------------------------------------
	before = set(exports)
	remaining  = all - before
	headers = map(lambda x: x, remaining)
	headers.sort()

	lheaders = len(headers)
	while lheaders > 0:
		for header in headers:
			if len( remaining.intersection(exportmap[header]) ) == 0:
				exports.append(header)            

		before = set(exports)
		remaining  = all - before
		headers = map(lambda x: x, remaining)
		headers.sort()
		if lheaders == len(headers):
			fatal("dependency sort failed\n%s" % joinfields(headers,'\n\t'))
		lheaders = len(headers)

	if len(exports) <> len(all):
		fatal("dependency sort incomplete")

	command = 'grep -h "//@OtherDependency:" %s' % file
	depends = map(lambda x: split(x)[1:], os.popen(command).readlines())
	other = []
	for items in depends:
		classname = items[1]
		element = DB.headerElement(classname)
		if element == None: continue
		headername = DB.headername(element)
		if headername in exports: continue
		if headername in other: continue
		other.append(headername)

	writeModule(context, exports, exportmap, other)
#---------------------------------------------------------------------------
def writeModule(context, exports, exportmap, other):
	records = []

	# Pick off last field
	module = split(context['module'],'.').pop()
	context['module'] = module
	records.append("#"+MAXWIDTH*"-")
	records.append("# File:    %(module)s.py" % context)
	records.append("# Created: %(date)s, xml2boost.py"
				   " %(version)s" % context)
	records.append("#$Revision: 1.2 $")
	records.append("#"+MAXWIDTH*"-")

	for header in exports:
		record = 'from libPy%s import *' % header
		s = ''
		delim = '\t# '
		for a in exportmap[header]:
			s = s + delim + a
			delim = ' '
		records.append("%s%s" % (record, s))

	if len(other) > 0:
		records.append("#" + MAXWIDTH*'-')

	for header in other:
		record = ''
		record = record + 'try:\n'        
		record = record + '   from libPy%s import *\n' % header
		record = record + 'except: pass'
		records.append(record)

	# Append possible specialized code

	modfile = "%(projectpath)s/config/%(module)s.py" % context
	if os.path.exists( modfile ):
		recs = map(lambda x: rstrip(x), open(modfile).readlines())
		records += recs

	str = joinfields(records,'\n')

	context['mod'] = module
	out = open("%(outdir)s/%(module)s.py" % context,"w")
	out.write(str+'\n')
	out.close()    
#---------------------------------------------------------------------------
def updateDependencies(context):

	# Add base class fields

	depends = strip(context['headername'] + ' ' + context['classname'])
	context['depends'].append(context['classname'])
	cdelim = ' '
	for inheritance, basename in context['bases']:

		# Keep only publicly inherited base classes

		if inheritance <> 'public': continue

		# We may need to translate this class name
		# Drop possible namespace. Assume single depth only
		# FIXME: Make a bit more robust
		basename= splitfields(basename,'::',1).pop()
		basename= translateAllTypes(context, basename)
		name = renameIdentifier(basename)
		context['depends'].append(name)
		depends = depends + cdelim + name

	context['dependencies'].append(depends)
#---------------------------------------------------------------------------
def addDependency(context):
	depends = context['headername']
	if context['headername'] in context['depends']: return
	context['depends'].append(context['headername'])
	context['dependencies'].append(depends)    
#---------------------------------------------------------------------------
def updateotherDependencies(context):
	headers = DB.typeDB.values()
	if len(headers) == 0: return
	for name in headers:
		depends = context['headername'] + ' ' + name
		if name in context['depends']: continue
		context['depends'].append(name)
		context['otherdependencies'].append(depends)
#---------------------------------------------------------------------------
# Instantiate template classes
#   Create an export file unless requested not to if one already exists
#---------------------------------------------------------------------------
def instantiateTemplate(context, templatename, ttypes, pyname='',
						checkIfexists=True):

	# Ignore a few things that we cannot handle at present

	if "void*" in ttypes:
		inform("template type void* unsupported")
		ttypes.remove("void*")

	if len(ttypes) == 0: return

	preamble = []
	preamble.append("//"+MAXWIDTH*"-")
	preamble.append("// Boost.Python Export File: "
					"export_%(exportname)s.cpp")
	preamble.append("// Created: %(date)s, xml2boost.py"
					" %(version)s")
	preamble.append("//$Revision: 1.2 $")
	preamble.append("//"+MAXWIDTH*'-')
	preamble.append("//@BaseDependency: @DEPENDS@")
	preamble.append("//"+MAXWIDTH*'-')

	header1 = joinfields(preamble,'\n')

	context['preamble'] = [] 
	context['preamble'].append('#include "boostpython.hpp"')

	s = '\n'
	s = s + 'namespace\n{\n'
	s = s + '  bool exported_%(exportname)s = false; // True if called\n'
	s = s + '};\n\n'
	s = s + "void export_%(exportname)s()\n"
	s = s + "{\n"
	s = s + '  if ( exported_%(exportname)s )\n'
	s = s + '    {\n'        
	s = s + '      if ( BOOSTDEBUG )\n'
	s = s + '        std::cout << "\\t\\t** Already exported %(exportname)s"'
	s = s + ' << std::endl;\n'
	s = s + '      return;\n'
	s = s + '    }\n'
	s = s + '  else\n'        
	s = s + '    if ( BOOSTDEBUG )\n'
	s = s + '      std::cout << "export %(exportname)s" '\
		'<< std::endl;\n' 
	header2 = s

	# Check if this is an STL template
	# Remove "std::" namespace

	if templatename[0:5] == 'std::': templatename = templatename[5:]
	if templatename in ['vector','map','list','pair','array']:

		# STL template

		shortname  = templatename
		exportname = templatename
		headername = templatename
		namespace  = 'boostutil::'
		context['exportheader'] = ''
	else:

		# Non-STL template

		#-------------------------------------------------
		# Get XML object associated with class templatename
		# as well as XML object associated with the header
		#-------------------------------------------------
		celement, helement = DB.elements(templatename)
		if celement == None:
			inform('No XML description for template %s' % templatename)
			return
		shortname  = celement.attrib['name']
		headername = DB.headername(helement)
		exportname = shortname
		namespace  = ''

		project = celement.attrib['project']
		module  = replace(celement.attrib['module'],'.','/')

		# Try to determine location of header

		headerpath = '%s/python/' % project        
		if not os.path.exists("%s/%s" % (BASE, headerpath)):
			headerpath = '%s/src/%s/python/' % (project, module)
			if not os.path.exists("%s/%s" % (BASE, headerpath)):
				fatal("Cannot find location of file export_%s.hpp" % \
					  headername)

		context['exportheader'] = '#include "%sexport_%s.hpp"' % \
										  (headerpath, headername)
	#----------------------------------------------------------------------
	postfix = ''
	delim = '< '

	p = shortname
	e = exportname
	c = shortname + "@@" # placeholder for postfix

	# Remember to reset the include database

	DB.includeDB = {}

	usingnamespace = []

	SimpleTypesOnly = True

	for ttype in ttypes:

		# Fix stl types without std:: namespace specifier

		m = badstl.search(ttype)
		if m <> None:
			stl = m.group()
			ttype = badstl.sub("std::%s" % stl, ttype)
		context['name'] = ttype

		# Check if a simple C-type

		simplecType = simplectype.match(ttype)   <> None

		if cstringtype.match(ttype) == None:

			constType          = const_type.match(ttype)    <> None
			referenceType      = referencetype.match(ttype) <> None
			pointerType        = pointertype.match(ttype)   <> None
			constreferenceType = const_referencetype.match(ttype) <> None
			constpointerType   = const_pointertype.match(ttype)   <> None

			if templatename in ['vector','map', 'list']:
				if constreferenceType:
					postfix = '_of_const_refs'
				elif referenceType:
					postfix = '_of_refs'                    
				elif constpointerType:
					postfix = '_of_const_pointers'
				elif pointerType:
					postfix = '_of_pointers'
				elif constType:
					postfix = '_of_const_refs'

				if useNoargVector(ttype): postfix += '_noarg'

			elif templatename in ['array']:
				if constpointerType:
					postfix = '_of_const_pointers'
				elif constType:
					postfix = '_of_const_types'
				elif pointerType:
					postfix = '_of_pointers'

		# Get full types

		shorttype = ttype
		words = swords.findall(ttype)

		for word in words:

			if strictlysimplectype.match(word) != None: continue
			if word in ['const', 'unsigned']: continue

			SimpleTypesOnly = False

			# Update include database

			usingnamespace += DB.addInclude(word)

			# strip away possible namespaces to get short name

			if DB.namespaceDB.has_key(word):
				wrd = re.compile(r'\b%s::' % word)
				shorttype = wrd.sub('', shorttype)

		# Get full type

		fulltype = ttype
		if not simplecType:
			element = DB.classElement(ttype)
			if element <> None:
				fulltype = splitfields(element.attrib['fullname'],'<',1)[0]

		p += delim + shorttype
		e += delim + shorttype
		c += delim + fulltype

		delim = ', '

	p += ' >'
	e += ' >'
	c += ' >'

	if pyname == '': pyname = renameIdentifier(p)
	exportname = renameIdentifier(e)
	classfunname = replace(c, '@@', postfix)

	# Set various name fields

	context['pyname']       = pyname
	context['exportname']   = exportname
	context['classfunname'] = classfunname

	# Update ttypes.py

	if not os.path.exists(context['ttypesdir']):
		command = '''
		mkdir -p %(ttypesdir)s
		cd %(ttypesdir)s
		echo "$(PYTHON_PROJECTS)/boostutil/config/boost.mk" > GNUmakefile
		''' % context
		os.system(command)

	if os.path.exists(TEMPLATEIMPORTFILE):
		records = open(TEMPLATEIMPORTFILE,'r').readlines()
	else:
		records = []
	s = 'from libPy%s import *\n' % pyname
	if s not in records: records.append(s)
	records.sort()
	open(TEMPLATEIMPORTFILE,'w').writelines(records)

	# Name of export function

	generictype = templatename in ['vector','map','list','pair','array'] and \
				  SimpleTypesOnly
	if generictype:
		outfile = '%(ctypesdir)s/export_%(exportname)s.cpp' % context
	else:
		outfile = '%(ttypesdir)s/export_%(exportname)s.cpp' % context

	# If requested, proceed only if the export function
	# does not yet exist.

	if checkIfexists:
		if os.path.exists(outfile): return

	# Inform

	print "---> Instantiate %s\tas %s" % (p, pyname)

	#-------------------------------------------------
	# Insert other headers that might be needed for
	# template instantiation. But do not if we have
	# instantiation of vectors of simple types
	#-------------------------------------------------

	if not generictype:

		includeRecords(context['preamble'])

		# Insert export header

		if context['exportheader'] != '':
			context['preamble'].append(context['exportheader'])

	# Insert boostusing

	context['preamble'].append('#include "boostusing.hpp"')

	if not generictype:

		# Insert other "using namespace"

		done = {}
		for name in usingnamespace:
			if not done.has_key(name):
				if len(done) == 0: context['preamble'].append('')
				context['preamble'].append('using namespace %s;' % name)
				done[name]=1

	# Concatenate preamble

	includerecord = joinfields(context['preamble'],'\n')

	header1= replace(header1,"@DEPENDS@", exportname)

	record = header1 + '\n' + includerecord + '\n' + header2 + '\n'
	record = record + \
			 TAB1 + '%sclass_%s("%s");\n' % (namespace, classfunname, pyname)
	record = record + \
			 TAB1 + 'exported_%(exportname)s = true; '\
			 '// Call me once only!\n'
	record = record + '}\n'
	record = record + '\n'
	record = record + 'BOOST_PYTHON_MODULE(libPy%(exportname)s)\n' % context
	record = record + '{\n'
	record = record + TAB1 + 'export_%(exportname)s();\n' % context
	record = record + '}\n'
	record = record % context

	# Write out records to file
	out = open(outfile,"w")
	out.write(record)
	out.close()
#---------------------------------------------------------------------------
# Add docstring for given element
#---------------------------------------------------------------------------
def addDocString(which, context, element, docstring):
	"""
	 Add docstring for given element
	"""
	docfile = "%(headername)s.doc" % context
	c = element.find('comment')
	if c != None:
		comment = c.text
	else:
		comment = ''

	context['docstring'] = docstring

	if which in ['ctor','method','op']:

		comment = comment + '\n'
		comment = comment + '<args>:   %s\n' %  element.attrib['args']
		comment = comment + '<vars>:   %s\n' %  element.attrib['vars']
		comment = comment + '<return>: %s\n' %  element.attrib['return']

	comment = replace(comment, '"','\\"')
	comment = map(lambda x: '"%s\\n"' % x, split(comment,'\n'))
	context['comment'] = joinfields(comment,'\n')

	str = 'std::string %(docstring)s(\n%(comment)s\n);\n\n' % context

	out = open(docfile,"a")
	out.write(str)
	out.close()
	context['docstring'] = context['docstring'] + '.c_str()'
#---------------------------------------------------------------------------
fvoidp = re.compile(r'void *\*',re.M)
fvoids = re.compile(r'\bvoid\b',re.M)
famps  = re.compile(r' &(?=\w)',re.M)
fstar  = re.compile(r' \*(?=\w)',re.M)
fcharpp= re.compile(r'char \*\*|char\*\*',re.M)
fcstr  = re.compile(r'const char\*|const char\*',re.M)
fccol  = re.compile(r'::',re.M)
fconst = re.compile(r'(?<=\)) *\bconst\b *',re.M)
#---------------------------------------------------------------------------
HTMLTAGS = ['b','br','i','td','tr','table','li','p']
def hidehtmltags(comment):
	for t in HTMLTAGS:
		new = '@'+t+'@'
		old = '<'+t+'>'
		comment = replace(comment, old, new)
		new = '@/'+t+'@'
		old = '</'+t+'>'
		comment = replace(comment, old, new)        
	return comment

def unhidehtmltags(comment):
	for t in HTMLTAGS:
		old = '@'+t+'@'
		new = '<'+t+'>'
		comment = replace(comment, old, new)
		old = '@/'+t+'@'
		new = '</'+t+'>'
		comment = replace(comment, old, new)        
	return comment
#---------------------------------------------------------------------------
findtb = re.compile('^[ \t]*')
def findtab(s):
	m = findtb.match(s)
	if m == None: return ''
	return m.group()
#---------------------------------------------------------------------------
def concatenate(records, splitRecord=1):
	if len(records) == 0: return ''
	recs = []
	for record in records:
		t = split(record,'\n')
		recs = recs + t

	s = ''
	for record in recs:
		record = rstrip(record)
		n = len(record)
		if n <= 79 or not splitRecord or strip(record)[0:2] == '//':
			s = s + record + '\n'
			continue

		if find(record, 'std::cout') > -1 or find(record, 'std::endl') > -1:
			s = s + record + '\n'
			continue

		# Try to split up string in some reasonable way
		# Align relative to "(" or "<" or "tabs"
		aligntab = align(record,'(')
		if aligntab == '':
			aligntab = align(record,'<')
			if aligntab == '':
				aligntab = findtab(record)
		if aligntab == '':
			s = s + record + '\n'
			continue

		n = len(aligntab)
		n = n + find(record[n:],' ')
		s = s + record[:n] + '\n'
		fields = split(record[n:])
		if record[0:2] == '//':
			comment = '//'
		else:
			comment = ''            
		for field in fields:
			field = replace(field,'>>','> >')
			s = s + comment + aligntab + field + '\n'
	return s + '\n'
#---------------------------------------------------------------------------
# Write the export function.
#---------------------------------------------------------------------------
def writeExportFunction(context):
	exprecords = ["",""]
	exprecords.append("void export_%(headername)s()" % context)
	exprecords.append("{")
	s = ''
	s = s + '  if ( exported_%(headername)s )\n'
	s = s + '    {\n'        
	s = s + '      if ( BOOSTDEBUG )\n'
	s = s + '        std::cout << "\\t\\t** Already exported %(headername)s"'
	s = s + ' << std::endl;\n'
	s = s + '      return;\n'
	s = s + '    }\n'
	s = s + '  else\n'        
	s = s + '    if ( BOOSTDEBUG )\n'
	s = s + '      std::cout << "export %(headername)s"'\
		' << std::endl;\n'
	s = s % context
	exprecords.append(s)

	# Add callback initialization

	if len(context['callbackinit']) > 0:
		exprecords.append('  // Initialize callback maps')
		for fullcbname in context['callbackinit']:
			context['fullcbname'] = upper(fullcbname)
			for n in xrange(callbackCount(fullcbname)):
				context['n'] = n + 1
				s = '  %(fullcbname)s_INIT(%(n)s)' % context
				exprecords.append(s)

	# Add enum export

	if len(context['enumrecords']) > 0:
		exprecords.append("  enums_%(headername)s();" % context)

	# Add function export

	if len(context['funrecords']) > 0:
		exprecords.append("  functions_%(headername)s();" % context)

	# Add class exporting record

	if len(context['classnames']) > 0: exprecords.append('')

	for name in context['classnames']:
		name = split(name,'::').pop()
		if not name in context['templateclass']:
			s = ''
			s = s + '  class_%s();' % name                
			exprecords.append(s)

	# Add user code if specified

	s = insertCode(context['headername'],'  ')
	if s <> "": exprecords.append(s)

	# Remember to set exported flag to true so that
	# export function will be called once only

	s = '\n'
	s = s + TAB1 + 'exported_%(headername)s = true; // Call me once only!'
	exprecords.append(s % context)
	exprecords.append("}")

	exprecords.append("\n")
	if STANDALONE:
		exprecords.append("BOOST_PYTHON_MODULE(%(headername)s)" % context)
	else:
		exprecords.append("BOOST_PYTHON_MODULE(libPy%(headername)s)" % context)
	exprecords.append("{")
	exprecords.append("  export_%(headername)s();" % context)
	#exprecords.append(rstrip(importfromRecord(context)))
	exprecords.append("}")
	return exprecords
#---------------------------------------------------------------------------
templatesToIgnore = re.compile(r'(std::)?(list|iterator)')
def writeTemplatemap(context):
	templatemap = context['templatemap']

	# Clean up map
	A = set(context['templateparlist'])    
	for (key, value) in templatemap.items():

		# For maps, make sure value is two-component

		if find(value, 'map') > -1:
			t = templateParList(value)
			if len(t) != 2:
				del templatemap[key]
				continue

		# Skip iterators and lists for now

		if templatesToIgnore.match(value) != None:
			del templatemap[key]
			continue
		B = set(swords.findall(value))        
		if A.intersection(B) != set([]):
			del templatemap[key]

	# Add arrays to be wrapped to templatemap

	for ttype in context['arraystowrap']:
		value = 'array< %s >' % ttype
		key = renameIdentifier(ttype)
		templatemap[key] = value

	keys = templatemap.keys()
	keys.sort()
	if os.path.exists(TEMPLATEFILE):
		records = open(TEMPLATEFILE,'r').readlines()
	else:
		records = []

	for key in keys:
		value = templatemap[key]        
		t = splitfields(value, '<' , 1)
		if len(t) == 2:
			name, str = t
			str = strip(str[:-1])
			s = 'Instantiate("%s", "%s")\n' % (name, str)
			if s not in records: records.append(s)
	if records <> []:
		records.sort()
		records = filter(lambda x: strip(x) != '', records)
		open(TEMPLATEFILE,'w').writelines(records)
	else:
		os.system("rm -rf %s %s" % (TEMPLATEFILE, TEMPLATEIMPORTFILE))
#===========================================================================
# Main Program
#===========================================================================   
def main():

	global DEBUG
	global CONTEXT
	global PRINT
	global WARN
	global SHOW
	global DB
	global LOG
	global INCLUDE
	global MACRO
	global STANDALONE

	# Decode command line

	# Defaults

	CHECK   = 0
	INCLUDE = []
	MACRO   = []
	SILENT  = 0
	CONTEXT['modulename'] = ''

	argv = sys.argv[1:]
	try:
		opts, args = getopt(argv, SHORTOPTIONS, LONGOPTIONS)
	except GetoptError, message:
		fatal(message)

	for option, value in opts:

		if   option == "-d":
			DEBUG = atoi(value)

		elif option == "-h":
			usage()
			sys.exit(0)

		elif option == "-v":
			print VERSION
			sys.exit(0)

		elif option == "-w":
			WARN = 1

		elif option == "-S":
			SHOW = 1

		elif option == "-s":
			STANDALONE = 1            

		elif option == "-I":
			unique(DB.INCLUDE, value)

		elif option == "-D":
			unique(DB.MACRO, value)

		elif option == "-p":
			PRINT = 1            

		elif option == "-c":
			CHECK = 1

		elif option == "-m":
			CONTEXT['modulename'] = value

		elif option == "-M":

			CONTEXT['outdir'] = value

			# Get list of export files for which shared libraries
			# have been created.

			t = glob("export_*.cpp")
			e = map(lambda x: replace(x, ".NOLINK", ".cpp"),
					glob("export_*.NOLINK"))
			e +=  map(lambda x: replace(x, ".FAILED", ".cpp"),
					glob("export_*.FAILED"))
			for x in e:
				if x in t: t.remove(x)                
			if len(t) == 0: sys.exit(0)

			value = joinfields(t, ' ')
			writeModuleFile(CONTEXT, value)
			sys.exit(0)            

		elif option == "--mapvectortolist":
			MapVectorToList(value)

		elif option == "--defineargtype":
			args = split(value)
			if len(args) == 4:
				DefineArgType(args[0], args[1], args[2], args[3])
			else:
				print "Wrong value for --defineargtype"
				usage()
				sys.exit(0)

	# Instantiate templates

	for option, value in opts:
		if   option == "-t":
			for templatename, ttypes, pyname, aClass in TemplateNames:
				# Skip if this pertains to a templated method
				if not aClass: continue
				instantiateTemplate(CONTEXT, templatename, ttypes, pyname)
			sys.exit(0)

	# Get list of XML files to process

	if len(args) == 0:
		usage()
		sys.exit(0)

	filename = args[0]

	# Check if XML file exists.

	if not os.path.exists(filename): fatal("%s <-- NOT FOUND" % filename)

	#-------------------------------------------------
	# Read XML file and get root element
	#-------------------------------------------------
	root = readXmlFile(filename)
	if root == None:
		fatal("unable to read file %s" % filename)

	if not root.attrib.has_key("name"):
		fatal("missing field name\n"
			  "\t\tin header entity of XML file %s" % filename)
	headername = DB.headername(root)

	# Delete old docstring file

	os.system("rm -rf %s.doc" % headername)

	#-------------------------------------------------        
	# Initialize class context
	#-------------------------------------------------
	exports = []

	os.system("mkdir -p %(docdir)s" % CONTEXT)

	CONTEXT['headername'] = headername
	cppfile = "%(srcdir)s/export_%(headername)s.cpp" % CONTEXT
	hppfile = "%(intdir)s/export_%(headername)s.hpp" % CONTEXT
	if CHECK:
		if os.path.exists(cppfile):
			warning("file %s already exists" % cppfile)
			sys.exit(0)

	CONTEXT['include']       = INCLUDE
	CONTEXT['templatemap']   = {}
	CONTEXT['classcount']    = 0
	CONTEXT['doc']           = []        
	CONTEXT['arraystowrap']  = []    # Array classes to be exported
	CONTEXT['vectorstowrap'] = []    # Vector classes to be exported
	CONTEXT['classnames']    = []
	CONTEXT['basenamelist']  = []
	CONTEXT['imports']       = []
	CONTEXT['callbackinit']  = []    # All callback init functions
	CONTEXT['newfullcbnames']= {}    # Map from fullcbname to newfullcbname
	CONTEXT['included'] = []         # Keep track of includes
	CONTEXT['modules']  = [PACKAGE]  # Keep track of modules
	CONTEXT['preamble'] = []
	CONTEXT['preamble'].append("//"+MAXWIDTH*"-")
	CONTEXT['preamble'].append("// Boost.Python Export "\
							   "File: export_%s.cpp" % headername)
	extindex = len(CONTEXT['preamble'])-1 # We may change '.cpp' to '.hpp'

	CONTEXT['preamble'].append("// Created: %(date)s, xml2boost.py"
							   " %(version)s" % \
							   CONTEXT)                          
	CONTEXT['preamble'].append("//$Revision: 1.2 $")
	CONTEXT['preamble'].append("//"+MAXWIDTH*"-")
	CONTEXT['preamble'].append('#include "boostpython.hpp"')

	#-------------------------------------------------        
	# Insert records before any headers if requested
	#-------------------------------------------------
	insertBeforeHeaders(CONTEXT, CONTEXT['preamble'])

	#-------------------------------------------------
	# Continue initialization of context.
	#-------------------------------------------------
	CONTEXT['usingnamespace'] = []        
	CONTEXT['funrecords'] = []  # For functions
	CONTEXT['wrprecords'] = []  # For thin wrappers
	CONTEXT['cpprecords'] = []  # For classes        
	CONTEXT['routines']   = {}
	CONTEXT['enumnames']  = []
	CONTEXT['functioninfo']= {}
	CONTEXT['enumrecords']= []
	CONTEXT['classdepth'] = 0
	CONTEXT['addnull']    = 0
	CONTEXT['addopaque']  = 0
	CONTEXT['ignoredclass'] = ''
	CONTEXT['nfunction']  = 0
	CONTEXT['cbprefix']   = 'CB_' + headername + "_"
	CONTEXT['nfunction']  = 0
	CONTEXT['templatepar']= ''
	CONTEXT['template']   = ''
	CONTEXT['classname']  = ''
	CONTEXT['templateclass'] = []
	CONTEXT['specializedtemplateclass'] = []
	CONTEXT['writeHeader']= 0        

	#-------------------------------------------------
	# Change certain items in tree, such as args from 
	# strings to lists. Also do some book-keeping.
	#-------------------------------------------------
	initializeTree(CONTEXT, root)    
	pruneTree(root)
	printTree(root)
	if PRINT : sys.exit(0)

	#-------------------------------------------------
	# Traverse tree recursively and build records
	#-------------------------------------------------
	debug(1,['Loop Over Records'])
	processTree(CONTEXT, root)        

	#-------------------------------------------------        
	# Write records to export file
	#-------------------------------------------------
	preamble = CONTEXT['preamble']        
	usingnamespace = CONTEXT['usingnamespace']
	callbacks  = DB.callbackDB
	wrprecords = CONTEXT['wrprecords']

	enumrecords= CONTEXT['enumrecords']
	if len(enumrecords) > 0:
		enumrecord = "void enums_%(headername)s()\n{" % CONTEXT
		enumrecords.insert(0, enumrecord)
		enumrecords.append('}')

	funrecords = CONTEXT['funrecords']
	if len(funrecords) > 0:
		funrecord = "void functions_%(headername)s()\n{" % CONTEXT
		funrecords.insert(0, funrecord)
		funrecords.append('}')

	cpprecords = CONTEXT['cpprecords']

	# Check if we need to handle a variable number of arguments

	if CONTEXT.has_key('addcstdarg'):
		preamble.append('#include <cstdarg>')

	#-------------------------------------------------
	# Do this before we includeRecords, in case other
	# includes are added by this function.
	#-------------------------------------------------
	translateCallbackTypes(CONTEXT)

	#-------------------------------------------------
	# Add includes
	#-------------------------------------------------
	includeRecords(preamble)

	#-------------------------------------------------        
	# Insert records after any headers if requested
	#-------------------------------------------------
	insertAfterHeaders(CONTEXT, preamble)

	preamble.append('#include "boostusing.hpp"')
	preamble.append("")
	# Check if doc string file exists
	if os.path.exists("%(headername)s.doc" % CONTEXT):
		preamble.append("// docstrings")
		preamble.append('#include "%(headername)s.doc"' % CONTEXT)
	preamble.append("")
	preamble = preamble + CONTEXT['usingnamespace']
	preamble.append("")

	#-------------------------------------------------        
	# Add callbacks
	#-------------------------------------------------
	callbrecords = []
	cname = callbacks.keys()
	cname.sort()
	for name in cname:
		cbretype, cbargs, inline, skip = callbacks[name]
		if skip: continue

		cbname = fullcallbackName(CONTEXT, name)
		if excludeCallback(CONTEXT['headername'], cbname): continue
		record = callbackRecord(CONTEXT, name,
								cbname, cbretype, cbargs, inline)
		if record <> "":
			s = '//' + MAXWIDTH*'-' + \
				'\n// Callbacks\n'
			s = s + '//' + MAXWIDTH*'-' + '\n'                
			record = s + record
			callbrecords.append(record)

	s = 'namespace\n{\n'
	s = s + TAB1 + \
		'bool exported_%(headername)s = false; // True if called\n' % \
		CONTEXT

	#-------------------------------------------------
	# Add opaque pointer if needed
	#-------------------------------------------------        
	if CONTEXT['addopaque']:
		s = s + '  struct %(opaque)s;' % CONTEXT
		s = s + ' // Opaque type to handle void*\n'

	#-------------------------------------------------
	# Add null object if needed
	#-------------------------------------------------
	if CONTEXT['addnull']:
		s = s + '  object null(0); // For NULL default arguments\n'
	s = s + '};\n'

	if CONTEXT['addopaque']:
		s = s + 'BOOST_PYTHON_OPAQUE_SPECIALIZED_TYPE_ID(%(opaque)s);'\
		'\n' % CONTEXT        
	preamble.append(s)

	#-------------------------------------------------
	# Add export function
	#-------------------------------------------------
	chdrecords = []
	chdrecords.append("//" + MAXWIDTH*'-')
	chdrecords.append("// Export procedures")
	chdrecords.append("//" + MAXWIDTH*'-')

	# Write a header for templated functions, if present

	if CONTEXT['writeHeader']:

		# Construct preamble for header

		name = upper(headername)
		hppstr = ''
		hppstr = hppstr + "#ifndef EXPORT_%s_HPP\n" % name
		hppstr = hppstr + "#define EXPORT_%s_HPP\n" % name        
		preamble[extindex] = replace(preamble[extindex],'.cpp','.hpp')
		hppstr = hppstr + concatenate(preamble)
		hppstr = hppstr + concatenate(callbrecords,0)

		# Initial string for export_<headername>.cpp function

		cppstr = ''
		cppstr = cppstr + "//"+MAXWIDTH*"-" + '\n'
		cppstr = cppstr + "// Boost.Python Export "\
				 "File: export_%s.cpp\n" % headername 
		cppstr = cppstr + "// Created: %(date)s, xml2boost.py" \
				 " %(version)s\n" % CONTEXT

		cppstr = cppstr + "//$Revision: 1.2 $\n"
		cppstr = cppstr + "//"+MAXWIDTH*"-" + '\n'
		cppstr = cppstr + '#include "export_%s.hpp"\n' % headername
		cppstr = cppstr + "//" + MAXWIDTH*'-'
		cppstr = cppstr + joinfields(CONTEXT['dependencies'],
									 '\n//@BaseDependency:  ') + '\n\n'
		cppstr = cppstr + joinfields(CONTEXT['otherdependencies'],
									 '\n//@OtherDependency: ') + '\n\n'
	else:
		cppstr = ''
		cppstr = cppstr + concatenate(preamble)
		cppstr = cppstr + joinfields(CONTEXT['dependencies'],
									 '\n//@BaseDependency:  ') + '\n\n'
		cppstr = cppstr + joinfields(CONTEXT['otherdependencies'],
									 '\n//@OtherDependency: ') + '\n\n'        
		cppstr = cppstr + concatenate(callbrecords,0)

	# Complete export_<headername>.cpp record

	cppstr = cppstr + concatenate(wrprecords)
	cppstr = cppstr + concatenate(chdrecords)
	cppstr = cppstr + concatenate(enumrecords)
	cppstr = cppstr + concatenate(funrecords)
	cppstr = cppstr + concatenate(cpprecords)

	#-----------------------------------------------------------------
	# Before we write out records, place all templated functions and
	# templated classes in header.
	#-----------------------------------------------------------------
	cppstr, hpplist = extractTemplatedItems(cppstr)

	# If no class of enum of function is to be exported don't
	# create an export function

	if exportpresent.search(cppstr) == None:
		a, b = os.path.splitext(cppfile)
		nolinkfile = '%s.NOLINK' % a
		open(nolinkfile,'w').write('')
	else:
		CONTEXT['enumrecords'] = enumrecords
		CONTEXT['funrecords']  = funrecords
		exprecords = writeExportFunction(CONTEXT)
		cppstr = cppstr + concatenate(exprecords)

	out = open(cppfile,"w")
	cppstr = squeezeblanks(cppstr)
	out.write(cppstr)
	out.close()

	if CONTEXT['writeHeader']:
		str = joinfields(hpplist,'\n\n')

		# NB: Remember to remove template<DUMMY> from converters

		str = replace(str, 'template<DUMMY>', '')

		hppstr = hppstr + '\n' + str
		hppstr = hppstr + "\n#endif\n"
		out = open(hppfile,"w")
		hppstr = squeezeblanks(hppstr)
		out.write(hppstr)
		out.close()

	writeTemplatemap(CONTEXT)

	# Close logfile
	LOG.close(headername)
#---------------------------------------------------------------------------
if __name__ == "__main__":
	main()









