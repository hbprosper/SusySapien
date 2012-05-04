#---------------------------------------------------------------------------
# File: ReflexLib.py
# Description: A collection of simple Reflex utilities
# Created: 25-Apr-2012 Harrison B. Prosper
#$Revision:$
#---------------------------------------------------------------------------
from ROOT import *
from string import *
import os, sys, re, posixpath
from PhysicsTools.TheNtupleMaker.Lib import fixName, getwords
try:
	from PhysicsTools.TheNtupleMaker.classmap import \
		 ClassToHeaderMap,\
		 ClassToLibraryMap
except:
	print "*** unable to import ClassToHeaderMap"
	print "*** run mkclassmap.py to create classmap.py"
	sys.exit(0)
#------------------------------------------------------------------------------
isfun     = re.compile(r'[0-9]+|bool|short|int|long|float|double'\
					   '|unsigned short|unsigned int|unsigned long')
isFuntype = re.compile(r'[0-9]+|bool|short|int|long|float|double'\
					   '|unsigned short|unsigned int|unsigned long'\
					   '|std::string|char|const char')
isSTL     = re.compile(r'std::(vector|set|map|pair)')
stufftoskip = re.compile(r'operator|__|iterator')
#------------------------------------------------------------------------------
def findHeaders(name):
	hdrs = {}
	pkgs = {}
	# break name into words, but skip fundamental types
	# and the main STL types
	words = getwords.findall(name)
	classlist = []
	for word in words:
		if isfun.match(word) != None:
			continue
		elif isSTL.match(word) != None:
			continue
		else:
			if not word in classlist:
				classlist.append(word)

	# for each word, scan the header map for a header file
	headers = []
	skipme = False
	for cname in classlist:
		# Find associated header
		if ClassToHeaderMap.has_key(cname):
			header = ClassToHeaderMap[cname]
			if type(header) == type([]): header = header[0]
			if not header in headers:
				headers.append(header)
		elif isfun.match(cname) != None:
			continue
	return headers
#----------------------------------------------------------------------------
# Routine to load appropriate library
#----------------------------------------------------------------------------
if not os.environ.has_key("SCRAM_ARCH"):
	print "*** SCRAM_ARCH not defined"
	print "*** need to set up CMS environment"
	sys.exit(0)

SCRAM_ARCH = os.environ["SCRAM_ARCH"]
LOCALLIBAREA = "%s/lib/%s/" % (os.environ["CMSSW_BASE"], SCRAM_ARCH)
LIBAREA = "%s/lib/%s/" % (os.environ["CMSSW_RELEASE_BASE"], SCRAM_ARCH)

LOADED_LIBS = {}

def loadLibrary(name):
	name = fixName(name) # remove unnecessary spaces
	if not ClassToLibraryMap.has_key(name): return
	
	library = ClassToLibraryMap[name]
	if LOADED_LIBS.has_key(library): return
	
	LOADED_LIBS[library] = 0

	found = False
	if   os.path.exists("%s%s.so" % (LOCALLIBAREA, library)):
		found = True
	elif os.path.exists("%s%s.so" % (LIBAREA, library)):
		found = True

	if found:
		if gSystem.Load(library) != 0:
			print "** failed to load %s for %s" % (library, name)
#----------------------------------------------------------------------------
skipmethod = re.compile(r'TClass|TBuffer|TMember|operator|^__')
reftype    = re.compile(r'(?<=edm::Ref\<std::vector\<)(?P<name>.+?)(?=\>,)')
basicstr   = re.compile(r'std::basic_string\<char\>')
vsqueeze   = re.compile(r'(?<=[^>]) +\>')
#----------------------------------------------------------------------------
FINAL   = 1
SCOPED  = 4
def classMethods(classname, db, depth=0):
	loadLibrary(classname)
	
	depth += 1
	if depth > 20:
		print "lost in trees"
		return
	tab = "  " * (depth-1)

	cdb = {'classname': classname,
		   'methods': []}

	thing = Reflex.Type()
	c = thing.ByName(classname)
	n = c.FunctionMemberSize()
	
	for i in xrange(n):
		m = c.FunctionMemberAt(i)
		if not m.IsPublic(): continue
		if not m.IsFunctionMember(): continue
		if m.IsConstructor(): continue
		if m.IsDestructor():  continue

		name  = m.Name()
		f     = m.TypeOf()
		mtype = f.Name(SCOPED)

		# skip some obviously "skipable" stuff
		if stufftoskip.search(mtype) != None: continue
		
		t = split(mtype, '(')

		if len(t) != 2:  continue

		# Get arguments
		args  = strip(t[1])
		args  = replace('(%s' % args, '(void)', '()')

		# Get return type and check for "const"
		rtype = strip(t[0])
		isconst = f.ReturnType().IsConst()
			
		# In C++ there is no overloading across scopes
		# only within scopes
		if db['scopes'].has_key(name):
			# This method is potentially an overload.
			# If we are not in the same scope, however, it cannot
			# overload the existing method
			scope = db['scopes'][name]
			if  scope != classname: continue
		db['scopes'][name] = classname

		signature = name + args

		# Skip setters
		rtype = strip(rtype)
		if rtype in ['void', 'void*']: continue

		# Expand typedefs, but check first for pointers and
		# references
		fullrtype = rtype
		if rtype[-1] in ['*','&']:
			r = thing.ByName(rtype[:-1])
			if r.IsTypedef():
				fullrtype = "%s%s" % (r.Name(SCOPED+FINAL), rtype[-1])
				rtype = fullrtype # Fri Jan 29
		else:
			r = thing.ByName(rtype)
			if r.IsTypedef():
				fullrtype = r.Name(SCOPED+FINAL)			

		if isconst: rtype = "const %s" % rtype
			
		rtype     = strip(basicstr.sub("std::string", rtype))
		fullrtype = strip(basicstr.sub("std::string", fullrtype))
		signature = basicstr.sub("std::string", signature)
		str = "%s  %s" % (rtype, signature)
		if skipmethod.search(str) != None: continue

		m = reftype.findall(str)
		if len(m) != 0:
			for x in m:
				cname = "%sRef" % x
				cmd = re.compile(r"edm::Ref\<.+?,%s\> \>" % x)
				rtype = cmd.sub(cname, rtype)
				signature = cmd.sub(cname, signature)

		# Ok, now added to methods list
		rtype = vsqueeze.sub(">", rtype)
		signature = vsqueeze.sub(">", signature)
		method    = "%32s  %s" % (rtype, signature)

		# Important: make sure we don't have duplicates
		if db['methods'].has_key(method):
			continue
		db['methods'][method] = classname
		cdb['methods'].append((fullrtype, method))

	db['classlist'].append( cdb )

	nb = c.BaseSize()
	for i in xrange(nb):
		b = c.BaseAt(i).ToType()
		basename = b.Name(SCOPED)
		db['baseclassnames'].append(basename)
		classMethods(basename, db, depth)
#----------------------------------------------------------------------------
def classDataMembers(classname, db, depth=0):
	loadLibrary(classname)
	
	depth += 1
	if depth > 20:
		print "lost in trees"
		return
	tab = "  " * (depth-1)

	cdb = {'classname': classname,
		   'datamembers': []}

	thing = Reflex.Type()
	c = thing.ByName(classname)
	n = c.DataMemberSize()

	for i in xrange(n):
		m = c.DataMemberAt(i)
		if not m.IsPublic(): continue

		name  = m.Name()
		dtype = m.TypeOf()
		nametype = dtype.Name(SCOPED+FINAL)
		nametype  = strip(basicstr.sub("std::string", nametype))
		db['scopes'][name] = classname
		signature = name

		# Ok, now added to datamembers list
		nametype = vsqueeze.sub(">", nametype)
		signature= vsqueeze.sub(">", signature)
		member   = "%32s  %s" % (nametype, name)

		if db['datamembers'].has_key(member):
			continue
		db['datamembers'][member] = classname
		cdb['datamembers'].append((nametype, member))
	db['classlist'].append( cdb )

	# scan bases classes
	nb = c.BaseSize()
	for i in xrange(nb):
		b = c.BaseAt(i).ToType()
		basename = b.Name(SCOPED)
		db['baseclassnames'].append(basename)
		classDataMembers(basename, db, depth)
#------------------------------------------------------------------------------
# Get fully qualified name of class
def getFullname(classname):
	loadLibrary(classname)
	t = Reflex.Type.ByName(classname)
	cname = t.FinalType().Name(7)
	if cname != "": classname = cname
	return classname
#------------------------------------------------------------------------------
# Define a container as a class that contains the methods
# size() and operator[](unsigned ? long ?int)
def isContainer(classname):
	loadLibrary(classname)
	
	t = Reflex.Type.ByName(classname)
	t = t.FinalType()
	n = t.FunctionMemberSize()
	print "%s\t%d" % (classname, n)
	
	has_size = False
	has_oper = False
	for i in xrange(n):
		m = t.FunctionMemberAt(i)
		if not m.IsPublic(): continue
		if not m.IsFunctionMember(): continue
		if m.IsConstructor(): continue
		if m.IsDestructor(): continue
		name = m.Name()
		
		if name == "size":
			# check signature
			fname = m.TypeOf().Name(3)
			if isvoid.search(fname) != None:
				has_size = True
		elif name == "operator[]":
			fname = m.TypeOf().Name(3)
			print "%s SIG(%s)" % (name, fname)
			if isint.search(fname) != None:
				has_oper = True
			
		iscon = has_size and has_oper
		if iscon: return True
		
	return False
