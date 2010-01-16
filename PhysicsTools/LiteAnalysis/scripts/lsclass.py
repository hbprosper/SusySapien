#!/usr/bin/env python
#------------------------------------------------------------------------------
# File:        lsclass.py
# Description: Exhaustive listing of a CMS class
# Created:     26-Sep-2008, Harrison B. Prosper
# Revised:     14-Aug-2009, HBP - Add ".lis" format
#              10-Dec-2009m HBP - Fix siglist bug
#$Revison:$
#------------------------------------------------------------------------------
import os, sys, re
from sys    import exit
from time   import *
from string import *
from getopt import getopt, GetoptError
from pprint import PrettyPrinter
from boostlib import \
	 nameonly, readXmlFile, findClassElements, stripBlanklines, \
	 templatepar, istemplate, templateParList, findTypedefElements, \
	 baseclassNames, populateDB
#------------------------------------------------------------------------------
REVISION = '$Revision:$'
clname   = re.compile(r'(?<=Class: ).*$',re.M)
methname = re.compile(r'(?<=-o- ).*$',re.M)
methodsig= re.compile(r'Methods from .+$|(?<=-o- ).+$'\
					  '\s+Return: .+$'\
 					  '\s+Arguments: .+$\s+$', re.M)

methodnam= re.compile(r'^.*\s+(?=Return)', re.M)
arguments= re.compile(r'(?<=Arguments: ).*', re.M)
returntyp= re.compile(r'(?<=Return: ).*\s+(?=Arguments)', re.M)

methodname = re.compile(r"^.*\(\) const")
version  = re.compile(r'(?<=CMSSW_).*(?=\/src)')
package  = re.compile(r'(?<=\/src\/).*',re.M)
stripnamespace = re.compile(r'\w+::')
striptemplate  = re.compile(r'\<.*$')
stripvirtual   = re.compile(r'^virtual +')

getsubsystem = re.compile(r'DataFormats.+(?=/interface)')
getheader = re.compile(r'(?<=/interface/).*(?=\.h)')

stripcolon = re.compile(r':')
striptemplatepars = re.compile(r'\<.*\>')
stripconst = re.compile(r'\bconst ')
stripptr   = re.compile(r'\*$')
striparg   = re.compile(r'\bconst |\w+::|\&')
stripptr   = re.compile(r'\*$')
methodname = re.compile(r"^.*\(\) const")
retype     = re.compile(r'float|double|int|unsigned|bool|\bsize_t\b')
simpletype =re.compile(r'void|float|double|int|unsigned|bool|\bsize_t\b|string')
striprtype = re.compile(r'\W+::|\bconst |\*$|&$|Ref$')
striprtypeless = re.compile(r'\bconst |\*$|&$|Ref$')
stripvirtual   = re.compile(r'^virtual +')
ref        = re.compile(r'Ref$')

skipstuff  = re.compile(r'component|iterator|Fwd$')
stripname = re.compile(r'[ ,<>:*&]')

SHORTOPTIONS = 'h:'

T_COLOR = '"black"'
R_COLOR = '"darkmagenta"'
A_COLOR = '"darkgreen"'
B_COLOR = '"darkyellow"'
C_COLOR = '"darkred"'
D_COLOR = '"red"'
V_COLOR = '"green"'
M_COLOR = '"darkblue"'
N_COLOR = '"blue"'
#-----------------------------------------------------------------------------
if not os.environ.has_key('PYTHON_PROJECTS'):
	print 'PYTHON_PROJECTS not defined!\n'
	exit(0)
BASE = os.environ['PYTHON_PROJECTS']
PP = PrettyPrinter()
## try:
## 	from XmlMap import XmlMap
## except:
## 	print "Unable to import XmlMap.py"
## 	sys.exit(0)
#-----------------------------------------------------------------------------
def usage():
	print '''
Usage:
	lsclass.py xmlfile
	'''
	sys.exit(0)
#----------------------------------------------------------------------------
def goodriddance(message):
	print "** error ** %s" % message
	sys.exit(0)
#----------------------------------------------------------------------------
# DEBUG switches
DEBUG2 = 0
#----------------------------------------------------------------------------
def translate(str, typedef):
	for key in typedef.keys():
		oldname = re.compile(r'\b%s\b' % key)
		newname = typedef[key]
		str = oldname.sub(newname, str)
	#str = ref.sub("*", str)
	return str
#----------------------------------------------------------------------------
def printKey(key, XmlMap):
	if XmlMap.has_key(key):
		print "%s:" % key
		PP.pprint(XmlMap[key])
	else:
		print "entry %s not found" % key
	sys.exit(0)
#----------------------------------------------------------------------------
def mksignature(retv, name, args):
	#s = [retv, name]+args
	s = [name]+args
	return joinfields(s,'|')
#----------------------------------------------------------------------------
def convert2html(record):
	record = replace(record,'&','&amp;')
	record = replace(record,'<','&lt;')
	record = replace(record,'>','&gt;')
	record = replace(record,'~','&atilde;')
	record = replace(record,"\t","&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;")
	record = replace(record,"\n","<br>")
	record = replace(record," ","&nbsp;")
	return record
#----------------------------------------------------------------------------
def color(c,s):
	return '<font color=%s>%s</font>' % (c,s)
#----------------------------------------------------------------------------
def boldcolor(c,s):
	return '<b><font color=%s>%s</font></b>' % (c,s)
#-----------------------------------------------------------------------------
def printMethods(data, out):
	for cname, fname, hdr in data['class']:
		if not data['method'].has_key(cname): continue

		out.write('Methods from %s\n\n' % fname)
		str = ''
		for name, retv, args, vars, comment in data['method'][cname]:
			str += '\t-o- %s\n' % name
			str += '\t\tReturn: %s\n' % retv
			str += '\t\tArguments: '
			buf  = ''
			for i in range(len(args)):
				str += buf + '%s\t%s\n' % (args[i], vars[i])
				buf  = '\t\t           '
			str += '\n'        
		out.write(str)
#-----------------------------------------------------------------------------
def printHeader(data, out):
	clist   = data['class']
	inclist = data['db']['include'].keys()
	inclist.sort()
	inclist = filter(lambda x: find(x, "DataFormats") > -1, inclist)

	if len(clist) == 0:
		filename = data["filenamestem"]
		print "\t** not processed!", filename
		out.close()
		os.system("rm -rf %s*" % filename)
		sys.exit(0)

	# Write class name and its header file
	
	name, fullname, header = clist[0]
	out.write('Class:\n\t%s\n\n'  % name)
	out.write('Header:\n\t%s\n\n' % header)

	# Write base class names
	
	if len(clist) > 1:
		out.write('BaseClasses:\n')
		for i, (name, fname, hdr) in enumerate(clist[1:]):
			out.write('\t%s\t%s\n' % (fname, hdr))
		out.write("\n")

## 	# Write includes
	
## 	if len(inclist) > 1:
## 		out.write('Includes:\n')
## 		for i, include in enumerate(inclist):
## 			out.write('\t%s\n' % include)
## 		out.write("\n")
	out.write('Version:     CMSSW_%s\n' % data['version'])
	out.write('Created:     %s\tlsclass.py %s\n\n' % \
			  (ctime(time()), REVISION))
#-----------------------------------------------------------------------------
def printTXT(filename, data):
	out = open(filename,'w')
	printHeader(data, out)
	printMethods(data, out)
	out.close()

def printLIS(filename, data):
	out = open(filename,'w')
	#printHeader(data, out)
	if data.has_key('siglist'):
		out.write(joinfields(data['siglist'],'\n') + '\n')
	out.close()	
#-----------------------------------------------------------------------------
def stripclassname(name):
	return stripnamespace.sub("",(striptemplate.sub("",name)))
#-----------------------------------------------------------------------------
def getMethodData(methods, data, typedef):
	for m in methods:
		name = m.attrib['name']        
		args = eval(m.attrib['args'])
		retv = stripvirtual.sub("", m.attrib['return'])
		retv = translate(retv, typedef)

		signature = mksignature(retv, name, args)
			
		# Add method only if this is a new signature
		signature = mksignature(retv, name, args)
		short_sig = stripnamespace.sub("", signature)

		if data['short_signature'].has_key(short_sig): return
		data['short_signature'][short_sig] = 1
		data['signature'][signature] = 1
			
		vars = eval(m.attrib['vars'])
		com  = m.findall('comment')
		comment = ''
		if len(com) > 0:comment = stripBlanklines(com[0].text)
		classname = data['currentclass']

		sig = "%s|%s" % (classname, signature)
		data['siglist'].append(sig)

		if not data['method'].has_key(classname): data['method'][classname]=[]
		data['method'][classname].append((name, retv, args, vars, comment))
#-----------------------------------------------------------------------------
# Note: The class name must be a fully qualified name
#-----------------------------------------------------------------------------
def getClassData(classname, data, depth=0):
	depth += 1
	if depth > 20:
		print "Lost in trees !"
		return

	db = data['db']
		
	cname = striptemplate.sub("",classname)
	if not db['class'].has_key(cname):
		if DEBUG2 > 0:
			print "\t** class (%s) NOT found" % cname
		return
	include, classxml  = db['class'][cname]

	if not db['include'].has_key(include):
		if DEBUG2 > 0:
			print "\t** include (%s) NOT found" % include
		return
	headerxml, classes = db['include'][include]

	# Get typedefs
	
	if not db['typedef'].has_key(include):
		if DEBUG2 > 0:
			print "\t** typedefs for include (%s) NOT found" % include
		return
	typedef = db['typedef'][include]
	
	#DD
	if DEBUG2 > 0:
		print "\nINCLUDE( %s ) CLASSNAME( %s ) DEPTH(%d)" % \
			  (include, classname, depth)
			
	# Get names of classes from includes

	classnames = {}
	for e in headerxml.findall('include'):
		inc = e.attrib['name'][1:-1]
		if not db['include'].has_key(inc): continue
		hdr, classes = db['include'][inc]
		for c in classes:
			fname = c.attrib['fullname']
			name  = stripclassname(fname)
			classnames[name] = fname
		if DEBUG2 > 0:
			print "  include( %s )" % inc
		
	# Check for templates

	isAtemplate = find(classname, '<') > 0

	if isAtemplate:

		if DEBUG2 > 0: print "\t\t** template class"
		
		# Get template parameters

		t = templateParList(classname)

		# Translate, if possible, using typedefs associated with
		# current class

		for i, x in enumerate(t):
			if typedef.has_key(x):
				t[i] = typedef[x]
		classname = striptemplate.sub("", classname)

		if not data['templateclass'].has_key(classname):
			data['templateclass'][classname] = t

	else:
		# Translate, if possible
		if typedef.has_key(classname):
			classname = typedef[classname]

	# Cache info on current class

	data['currentclass'] = classxml.attrib['fullname']
	vers = version.findall(headerxml.attrib['file'])[0]
	data['version'] = vers
	hdr  = package.findall(headerxml.attrib['file'])[0]

	# Get fully qualified name of current class
	# If this is a template class, replace template parameters
	# with their current values

	name  = classxml.attrib['name']
	fname = classxml.attrib['fullname']
	fullname = fname
	#fullname = translate(fname, typedef)

	if isAtemplate:
		t = templateParList(fname)
		for i, x in enumerate(t):
			oldname = re.compile(r'%s' % x)
			if i >= len(data['templateclass'][classname]): break

			newname = data['templateclass'][classname][i]
			typedef[x] = newname
			fullname = oldname.sub(newname, fname)
	data['class'].append((fname, fullname, hdr))

	# Get methods of current class

	strippedfname = striptemplate.sub("", fname)
	
	getMethodData(db['method'][strippedfname], data, typedef)

	if DEBUG2 > 0:
		PP.pprint(data['siglist'])
		print
		
	# Loop over base classes
	# For each class, determine to which include it belongs
	# by scanning the list of includes associated with the
	# current class.

	bases = baseclassNames(classxml)
	for classname in bases:
		
		if DEBUG2 > 0: print "     base-class( %s )" % classname
		classname = translate(classname, typedef)
		if DEBUG2 > 0: print "       base-class( %s )" % classname

		if not db['class'].has_key(classname):
			name = stripclassname(classname)
			if not classnames.has_key(name):
				print "\t\t** warning - base class name %s not found in map" %\
					  name
				continue

			oldname = re.compile(r'%s' % name)
			if oldname.match(classname) != None:			
				classname = replace(classname, name,
									striptemplate.sub("",classnames[name]))
			
		if DEBUG2 > 0: print "          base-class( %s )" % classname
		getClassData(classname, data, depth)
#-----------------------------------------------------------------------------
def writeDoc(classname, data):

	# Make sure we initialize data keys

	data['class'] = []
	data['templateclass'] = {}
	data['signature'] = {}
	data['short_signature'] = {}
	data['method']  = {}
	data['siglist'] = []
	getClassData(classname, data)
	
	if not data.has_key('class'):
		print "\t*** Unable to find class %s" % classname
		sys.exit(0)
	
	# Write in a non-html formats

	txtfile = '%s.txt' % data['filenamestem']
	htmlfile= replace(txtfile,'.txt','.html')
	lisfile = replace(txtfile,'.txt','.lis')

	os.system("mkdir -p txt; mkdir -p html; mkdir -p lis")
	txtfile = "txt/%s"  % txtfile
	htmlfile= "html/%s" % htmlfile
	lisfile = "lis/%s"  % lisfile
	
	printTXT(txtfile, data)

	#-----------------------------------
	# Write html format
	#-----------------------------------
	record = open(txtfile).read()
	allmethods = methname.findall(record)    
	
	record = convert2html(record)

	out = open(htmlfile, 'w')
	Name     = boldcolor(T_COLOR, 'Name:')
	Fullname = boldcolor(T_COLOR, 'Class:')
	Package  = boldcolor(T_COLOR, 'Header:')
	Return   = color(R_COLOR,     'Return:')
	Arguments= color(A_COLOR,     'Arguments')
	Description= color(D_COLOR,   'Description')
	Methods  = boldcolor(M_COLOR, 'Methods')
	Includes = boldcolor(M_COLOR, 'Includes')
	Bases    = boldcolor(B_COLOR, 'BaseClasses')

	cname = data['class'][0][0]
	record = replace(record, 'Class:&nbsp;&nbsp;&nbsp;%s' % cname,
					 '<b><font size=+2>Class:&nbsp;&nbsp;&nbsp;%s</font></b>'\
					 % cname)

	record = replace(record, 'Name:',      Name)
	record = replace(record, 'Class:',     Fullname)
	record = replace(record, 'Header:',    Package)
	record = replace(record, 'Return:',    Return)
	record = replace(record, 'Arguments',  Arguments)
	record = replace(record, 'Description',Description)
	record = replace(record, 'Methods',    Methods)
	record = replace(record, 'Includes',   Includes)
	record = replace(record, 'BaseClasses',Bases)

	for a in allmethods:
		b = '-o-&nbsp;%s' % a
		c = boldcolor(N_COLOR, a)
		m = re.compile(r'%s(?=\<)' % b ,re.M)
		record = m.sub(c, record)

	out.write('<code>\n')
	out.write('\t<font size=+1 color=%s>\n' % T_COLOR)
	out.write(record)
	out.write('\t</font>\n')
	out.write('</code>\n')
	out.close()

	# Extract signatures of simple methods

	DEBUG3 = 0
	signatures = []
	sigs = methodsig.findall(open(txtfile).read())
	ctype = ""
	for sig in sigs:
		if sig[:7] == "Methods":
			ctype = replace(sig, "Methods from ", "")
			continue
		sig = replace(sig,'\t',' ')
		t = split(sig,'\n')
		rtype = strip(replace(t[1], "Return: ",""))
		if rtype == "void": continue

		name  = strip(t[0])
		atype = strip(replace(t[2], "Arguments: ",""))

		# Check for simple arg type
		arg   = striparg.sub("", atype)
		if simpletype.match(arg) == None: continue
		if arg != "void": atype = joinfields(split(atype)[:-1],' ')
		sig   = "%s|%s|%s|%s" % (ctype, rtype, name, atype)

		if DEBUG3 > 0: print "\tsignature( %s )" % sig
		signatures.append(sig)

	data['siglist'] = signatures
	printLIS(lisfile, data)
#-----------------------------------------------------------------------------
def main():        
	try:
		opts, infiles = getopt(sys.argv[1:],SHORTOPTIONS)
	except GetoptError, m:
		print
		print m
		usage()

	# Decode option, value pairs

	editsfile = ''
	for option, value in opts:

		if option == "-h":
			usage()
		elif option == "-e":
			editsfile = value
			
	if len(infiles) == 0: usage()
	xmlfile = infiles[0]

	# Make sure file exists
	if not os.path.exists(xmlfile):
		goodriddance("file %s not found" % xmlfile)
		
	# Construct include file name, assuming that we
	# are starting at a level higher than the directory src/
	if find(xmlfile, 'src/') < 0:
		goodriddance("lsclass.py - directory src/ not found in %s" % xmlfile)
		
	# Extract name of include from xml file
	
	include=replace(replace(split(xmlfile,
								  'src/')[1],'xml/','interface/'),'.xml','.h')
	filenamestem = replace(replace(include, 'interface/',''),'.h','')
	filenamestem = replace(filenamestem,'/','.')

	# Skip some classes
	
	skipstuff = re.compile(r'iterator|component|print|Fwd$')
	cname = nameonly(include)	
	if skipstuff.search(cname) != None:
		#print "\t\t\t*** skip processing: ", filenamestem
		exit(0)
	
	# Extract informatiom
	
	DB = {}
	populateDB(BASE, include, DB)
	
	headerxml, classes = DB['include'][include]
	
	data = {'db':DB, 'filenamestem': filenamestem}
	for classxml in classes:
		classname = classxml.attrib['fullname']
		if skipstuff.search(classname) != None: continue
		
		t = split(filenamestem,'.')
		name = stripname.sub("_", classxml.attrib['name'])
		t.append(name)
		fstem = joinfields(t, '.')		
		data['filenamestem'] = fstem
		print "processing: ", fstem
		writeDoc(classname, data)
#-----------------------------------------------------------------------------
main()

