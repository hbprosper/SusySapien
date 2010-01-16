#---------------------------------------------------------------------------
# File: boostlib.py
# Description: A collection of boostutil utilities. Most have been culled
#              from either xml2boost.py or header2xml.
# Created: 19-May-2006 Harrison B. Prosper
#$Revision: 1.7 $
#---------------------------------------------------------------------------
import os, sys, re, posixpath, shelve
from string import *
from sets   import Set
from elementtree.ElementTree import ElementTree
from xml.parsers.expat import ExpatError
#---------------------------------------------------------------------------
WEIRD        = "<|<@&@>|>"
TAB          = 4
LT           = '&lt;'
GT           = '&gt;'
AMP          = '&amp;'
QUOT         = '&quot;'
APOS         = '&apos;'
#---------------------------------------------------------------------------
ALPHA  = re.compile(r'[A-Z]')
findlb = re.compile(r'\(',re.M)
findrb = re.compile(r'\)',re.M)
findlt = re.compile(r'\<',re.M)
findgt = re.compile(r'\>',re.M)
#---------------------------------------------------------------------------
# Regular expressions used for tidying up use of "&", "*", "," "!=" "=" 
# "+", "-" and "/"
#---------------------------------------------------------------------------
adjustamp = re.compile('(?P<adjust>[ \t]+[&][ \t]*)',re.M)
adjustast2= re.compile('(?P<adjust>[ \t]+[*][ \t]*[*][ \t]*)',re.M)
adjustast = re.compile('(?P<adjust>[ \t]+[*](?![*]|/)[ \t]*)',re.M)
adjustcom = re.compile('(?P<adjust>[ \t]*[,][ \t]*)',re.M)
adjusteql = re.compile('(?P<adjust>[ \t]+[=][ \t]*)',re.M)
adjustnoteql = re.compile('(?P<adjust>[ \t]+[!][=][ \t]*)',re.M)
adjustplus = re.compile('(?P<adjust>[ \t]+[+][ \t]*)',re.M)
adjustminus= re.compile('(?P<adjust>[ \t]+[-][ \t]*)',re.M)
adjustdiv  = re.compile('(?P<adjust>[ \t]+[/][ \t]*(?![/]))',re.M)

adjustlt  = re.compile('(?P<adjust>[ \t]+<[ \t]*)',re.M)
adjustlpar= re.compile('(?P<adjust>\([ \t]*)',re.M)
adjustrpar= re.compile('(?P<adjust>[ \t]+\))',re.M)
adjustlbra= re.compile('(?P<adjust>\[[ \t]*)',re.M)
adjustrbra= re.compile('(?P<adjust>[ \t]+\])',re.M)

adjustptr = re.compile('(?P<adjust>\*)(?=[A-Za-z]+)',re.M)
adjustref = re.compile('(?P<adjust>\&)(?=[A-Za-z]+)',re.M)

fixstupid = re.compile('(?<=\w)(?P<delim>\*\*|\*|&)(?=\w)',re.M)

# Pick up leading blank lines

leadingblanks = re.compile('(?P<leadingblanks>(^[ \t]*\n)+)',re.M)

callback    = re.compile('(?P<callback>([\w\*\&:_]+\s+)+'\
                         '\(\s*(\w+::)?\s*\*\s*\w+\s*\)\s*\(.*\))')

callback2   = re.compile('(?P<callback>([\w\*\&:_]+\s+)+'\
                           '(\(\s*\*\s*)?'\
                            '(\w+::)?(?P<name>\w+)\s*(\)\s*)?\(.*\))')

callbackName  = re.compile('(?P<name>\(\s*(\w+::)?\s*\*\s*\w+\s*\))')

stripTypedef  = re.compile(r'^\s*\btypedef\s+')
findSpace     = re.compile("\s+",re.M)

# To check for simple types

simpletype= re.compile('(?P<name>\\b'\
                       '(char|short|int|long|float|double|bool)\\b)')
# To extract template parameters

templatepar = re.compile('\w[^<]*<(?P<par>[^>]*)>')

templatecmd  = '(?P<template>template'\
               '\s*(<.*>|([^\{\n]+\n)+)\s*)'
istemplate   = re.compile(templatecmd)

#---------------------------------------------------------------------------
# Given a base class element, return name of base class, minus possible
# template decoration.
#---------------------------------------------------------------------------
getbasename = re.compile(r'(public |private |protected )? *'\
                         '(?P<name>[a-zA-Z].*)')

def extractbaseclassName(e):
    name = e.attrib['name']
    m = getbasename.match(name)
    if m == None: fatal("Can't get name from %s name" % name)
    name = m.group('name')
    return name
#---------------------------------------------------------------------------
# Given template <class X, etc.> extract [X,...]
#---------------------------------------------------------------------------
findtemplatepars = re.compile('(?<=class )(?P<par>\w+)')
def templateParameters(element):
    if element == None: return []
    if type(element) == type(""):
        template = element
    else:
        if not element.attrib.has_key('template'): return []        
        template = element.attrib['template']
    return findtemplatepars.findall(replace(template,'typename','class'))

squeezeEq = re.compile('\s*=\s*')
def templateParList(record):
    m = templatepar.search(record)
    if m == None: fatal("templateParList - "\
                        "can't get template parameters from\n\t" + record)
    template = m.group("par")
    template = squeezeEq.sub('=', template)
    template = replace(replace(template,'class ',''),'typename ','')
    template = map(lambda x: strip(split(x,'=')[0]), split(template,','))
    return template

#---------------------------------------------------------------------------
word = re.compile(r'\b\w+\b')
swords  = re.compile(r'"[^"]*"|\b\w+\b')
swords2 = re.compile(r'\(\s*\*\s*\w+\s*\)|"[^"]*"|\b\w+\b')
#---------------------------------------------------------------------------
def nameonly(s):
    return posixpath.splitext(posixpath.split(s)[1])[0]
#---------------------------------------------------------------------------
def decodeTypedef(record):
	record = strip(stripTypedef.sub("",record))
	i = find(record, '(') # Hack to handle a few crazy typedefs 12/20/09
	if i > 0:
		i -= 1
	else:
		i = rfind(record,' ')
	if record[-1] == ';':
		key  = strip(record[i+1:-1])
	else:
		key  = strip(record[i+1:])
	value= standardizeName(strip(record[:i]))
	return (key, value)

#---------------------------------------------------------------------------
def argOK(field):
    """
    Extract (return type, method name, arguments, qual) of a function
    This code is extremely ugly, but it seems to work most of the time!
    """
    nlb = findlb.findall(field)
    nrb = findrb.findall(field)
    nlt = findlt.findall(field)
    ngt = findgt.findall(field)
    return len(nlb) == len(nrb) and len(nlt) == len(ngt)
#---------------------------------------------------------------------------
def decodeMethodName(method):
    DO = 0

    method = adjustlpar.sub(" (",method)
    method = adjustrpar.sub(") ",method)
    method = adjustlbra.sub(" [",method)
    method = adjustrbra.sub("] ",method)

    # Fix some stupidities (see TIterator*GetIteratorOnAlleaves in TTree.h)

    m = fixstupid.search(method)
    if m <> None:
        delim = m.group('delim')
        method= fixstupid.sub(delim+' ', method)
        
    # Change contiguous newlines etc, into a space

    method = findSpace.sub(' ',method)

    # Split up method/function

    j = rfind(method,')')+1
    a = method[:j]
    a = replace(a,' [','[')
    a = replace(a,' <','<')
    t = split(a)    
    qual = replace(strip(method[j:]),';','') # Get rid of possible ";"
    qual = replace(qual,'throwPP','throw()')
    
    # ---------------------------------------
    # Get arguments: I know, I know; this code
    # is hideous!
    #
    # First find start of argument list (...)
    # Look out for functions of the form
    # name<...(...) > method(...)
    # ---------------------------------------
    k = -1
    if DO: print t
    for n, s in enumerate(t):
        
        if s[0] == "(":
            
            if n > 0:
                if find(t[n-1],'operator') < -1: # Do check only for non-ops
                    if find(t[n-1],'<') > 0:
                        if find(t[n-1], '>') < 0:
                            continue
            k = n
            break
    if DO: print k
    
    if k < 0: return ("","","","","")
    
    t[k] = t[k][1:]
    n = len(t)
    t[n-1] = t[n-1][:len(t[n-1])-1]
    args = joinfields(t[k:])
    if args == "": args = 'void'

    if DO: print "args( %s )" % args
    
    # Split at "," making sure we have valid arguments
    
    fields = splitfields(args,',')
    if len(fields) > 1:
        field   = fields[0]
        cfields = fields[1:]
        fields  = []
        for i, s in enumerate(cfields):
            if argOK(field):                
                fields.append(field)
                field = s
            else:
                field = field + "," + s
        fields.append(strip(field))

    if DO: print "FIELDS( %s )" % fields
    
    arglist = []
    varlist = []
    for i, field in enumerate(fields):
        
        field = adjustptr.sub("* ", field) # See for example TGFontTypeComboBox
        field = adjustref.sub("& ", field)

        # WARNING: Changed 30-Oct-2005 *** HBP
        # Use find rather than rfind to avoid problem with
        # arguments such as: const char* training = "Entry$%2==0"
        
        j = find(field,'=') # Check for default parameters
        defaultPar = j > -1
        if defaultPar:
            a = strip(field[:j])
            value = replace(standardizeName(strip(field[j+1:])),' (','(')
        else:
            a = strip(field)

        var = ''

        # Check for callbacks

        if callback.match(a) <> None:

            ca = callbackName.sub('CALLB_',a)
            cretype, cname, carglist, cvarlist, cqual = decodeMethodName(ca)
            cname = replace(callbackName.search(a).group('name'),' ','')
            a = "%s %s(%s) %s" % (cretype,
                                  cname,
                                  joinfields(carglist,', '),
                                  cqual)
        else:
        
            # Try to "guess" which field is the variable and which
            # is the type.
            #
            # Yikes...this is pretty awful code...tidy up later!
            
            b = split(a)
            n = len(b)
            if n == 1:
                a = b[0] # This is easy!
                var = '' # Variable name
            else:
                first = b[0]
                last  = b[-1]
                var = last
                
                # Could have
                #  [const] type1 [type2] [const(&|*)] [varname]

                if last[-1] in ['*','&'] or last == 'const' or \
                       simpletype.match(last) <> None:
                    var = ''
                    pass
                
                elif first == 'const':
 
                    if n == 2:
                        # This must be of the form
                        # const <type>
                        var = ''
                        pass
                    
                    elif simpletype.match(last) <> None:
                        # Last field is a simple type so
                        # assume it is part of the type
                        var = ''
                        pass

                    else:
                        # Last field is not a simple type.
                        # If it ends with either & or * assume it
                        # is part of the type, otherwise assume it is
                        # a name.
                        if last[-1] in  ['*','&']:
                            var = ''
                            pass
                        else:
                            a = joinfields(b[:-1],' ') # Assume last is name
                            var = b[-1]
                            
                elif simpletype.match(last) == None:
                    a = joinfields(b[:-1]) # Assume last is name
                    var = b[-1]

                # Check for array parameter aaa[n]. But note that
                # aaa could be a type!
                if find(last,'[') > -1 and find(last,']') > -1:
                    if a[-1] <> ']':
                        i = find(last,'[')
                        j = rfind(last,']')+1
                        a = a + last[i:j]

                    if DO: print i, "LAST( %s ) -> %s" % (last, a)

        a = strip(a)
        
        # Check for defaulted parameter
        if defaultPar:
            if a == 'char': value = '"%s"' % value[1:-1]
            a = a + '=%s' % value

        #---------------------------
        # Thank goodness we're done!
        #---------------------------
        arglist.append(a)
        varlist.append(var)

    if DO: print "ARGLIST( %s )" % arglist
    
    # This may be an operator; needs special handling
    q = -1
    for n, s in enumerate(t[:q]):
        if find(s,"operator") > -1:
            q = n
            break
    if q < 0:
        name  = t[k-1]
        rtype = joinfields(t[:k-1])
    else:
        name  = joinfields(t[q:k])
        name  = replace(name," [","[")
        name  = replace(name," (","(")
        rtype = joinfields(t[:q])
    return (rtype,name,arglist,varlist,qual)
#---------------------------------------------------------------------------
# Check for callback
#---------------------------------------------------------------------------
findcbname = re.compile('\(\s*(\w+::)?\s*\*\s*(?P<name>[\w_]+)\)\s*\(')
scrunch = re.compile(' +(?=(\*|\[))')
def decodeCallback(record):
    if find(record,'(') > 0 and find(record,')') > 0:
        record = joinfields(split(record),' ')
        record = replace(record,'typedef ','')
        record = replace(record,';','')
        record = standardizeName(record)
        g = findcbname.search(record)
        if g <> None:
            name = g.group('name')
            i = find(record, '(')
            j = rfind(record, '(')
            record = record[:i] + name + record[j:]
        record = scrunch.sub("", record)
        return decodeMethodName(record)
    return None

#---------------------------------------------------------------------------
def stripBlanklines(record):
    """
    Strip away leading and trailng blank lines.
    """
    m = leadingblanks.match(record) # Important: Use match not search!
    if m:
        record = record[m.end():]
    return rstrip(record)
#---------------------------------------------------------------------------
# Tidy up identifiers
#---------------------------------------------------------------------------
def convert2html(record):
    record = replace(record,'&',AMP)    
    record = replace(record,'<',LT)
    record = replace(record,'>',GT)
    record = replace(record,'"',QUOT)
    record = replace(record,'\0', "")
    return record
#---------------------------------------------------------------------------
def convertFromHtml(record):
    record = replace(record, AMP,'&')    
    record = replace(record, LT, '<')
    record = replace(record, GT, '>')
    record = replace(record, QUOT, '"')
    return record
#---------------------------------------------------------------------------
def standardizeName(name):
    # Do more intelligently later!
    name = adjustast2.sub('** ',name) # fix before next line
    name = adjustast.sub('* ',name)
    name = adjustamp.sub('& ',name)
    name = adjustcom.sub(', ',name)
    name = adjusteql.sub('=', name)
    name = adjustplus.sub('+', name)
    name = adjustminus.sub('-', name)
    name = adjustdiv.sub('/', name)
    name = adjustnoteql.sub('!=', name)
    name = adjustlt.sub("<",name)

    name = replace(name,"* )", "*)")
    name = replace(name,"& )", "&)")
    name = replace(name,"* >", "*>")
    name = replace(name,"& >", "&>")
    name = replace(name,"< ",  "<")
    return strip(name)
#---------------------------------------------------------------------------
def standardizeOpName(name):
    i = find(name,'operator')
    if i < 0: return name
    j = find(name[i:],'(') + i
    str = name[i+8:j]
    if word.search(str) == None:
        name = name[:i] + 'operator%s' % strip(str) + name[j:]
    return name
#---------------------------------------------------------------------------
# Read an XML file .... duh!
#---------------------------------------------------------------------------
rowcol = re.compile('line (?P<row>[0-9]+), column (?P<col>[0-9]+)',re.M)
def readXmlFile(filename):
    try:
        return ElementTree(file=filename).getroot()
    except:
        return None
#---------------------------------------------------------------------------
def validateXmlFile(filename):
    try:
        ElementTree(file=filename).getroot()
        return ''
    except ExpatError, error:
        errStr = "\n\tBadly formed XML in file\n\t%s" % filename
        str    = "\t\t%s\n" % error
        errStr = errStr + "\n" + str
        m = rowcol.search(str)
        if m <> None:
            row = m.group('row')
            if row <> None:
                row = atoi(row)
                records = open(filename).readlines()
                startrow = max(0,row-3)
                endrow   = row
                for record in records[startrow:endrow]:
                    errStr = errStr + "\n" + rstrip(record)
                col = m.group('col')
                if col <> None:
                    col = atoi(col)-1
                    errStr = errStr + "\n" + ' '*col + '^'
        return errStr
    except:
        # Fall on sword
        fatal("validateXMLFile - Unknown problem with XML file\n\t%s" % \
              filename)
#---------------------------------------------------------------------------
# Make a key by stripping away qualifiers such as 'const', '*', etc.
#---------------------------------------------------------------------------
makekey = re.compile(r'\bstruct\b|\bconst\b|\bstatic\b|\bvirtual\b|[&]|[*]|')
def stripName(arg, regex=makekey):
    key = split(arg,'=')[0] # In case this comes with a value
    return strip(regex.sub('', key))
#---------------------------------------------------------------------------
# Rename a templated identifier to a valid Python identifier
#---------------------------------------------------------------------------
findvector = re.compile(r'\bvector\b')
findlist   = re.compile(r'\blist\b')
findmap    = re.compile(r'\bmap\b')
findarray  = re.compile(r'\barray\b')
findpair   = re.compile(r'\bpair\b')
findset    = re.compile(r'\bset\b')
findconst  = re.compile(r'\bconst\b')
findunsigned  = re.compile(r'\bunsigned\b')
findcontainer=re.compile(r'\b(?P<name>vector|list|map|array|pair|set)\b')
cstring    = re.compile(r'(\bconst )?char *\*')
#---------------------------------------------------------------------------
def renameIdentifier(identifier):
    identifier = cstring.sub("cstring", identifier)
    identifier = replace(identifier,'std::','')
    identifier = replace(identifier,'boostutil::','')
    identifier = replace(identifier,'::','_')
    #identifier = findunsigned.sub('u', identifier)
    i = find(identifier,'<')
    j = rfind(identifier,'>')

    if  i < j and i > -1:
        left   = identifier[:i]
        middle = identifier[i:j+1]
        right  = identifier[j+1:]

        left = stripName(left)        
        left = findvector.sub('v', left)
        left = findlist.sub('l', left)
        left = findmap.sub('m', left)
        left = findarray.sub('a', left)
        left = findpair.sub('p', left)

        middle = findconst.sub('c', middle)
        middle = findvector.sub('v', middle)
        middle = findlist.sub('l', middle)
        middle = findmap.sub('m', middle)
        middle = findarray.sub('a', middle)
        middle = findpair.sub('p', middle)

        middle = replace(middle,'<','')
        middle = replace(middle,'>','')
        middle = replace(middle,',','_')
        middle = replace(middle,'*','ptr')
        middle = replace(middle,'&','ref')

        identifier = left + middle        
        identifier = joinfields(split(identifier),'')
    else:
        m = findcontainer.search(identifier)
        if m <> None:
            identifier = findcontainer.sub(m.group('name')[0], identifier)
    return identifier
#---------------------------------------------------------------------------
def expandpath(str):
    return strip(os.popen("find %s -maxdepth 0 2>/dev/null" % str).read())
#---------------------------------------------------------------------------
def libExists(libdirs, lib):
    path = ''
    for ldir in libdirs:
        fullname = "%s/%s" % (ldir, lib)
        if expandpath(fullname) != '': return True
    return False
#---------------------------------------------------------------------------
striplib = re.compile(r'\.so.*$')
def libDependencies(libpaths, libdirs, lib):
    
    # Get pathname of library

    k = 0
    for i, lpath in enumerate(libpaths):
        k = i
        path = "%s/%s" % (lpath, lib)
        if os.path.exists(path):
            break
        else:
            path = ''
    if path == '': return ([], [])

    # Get lib dependencies

    command = "symlist.py -l %s" % path
    #print command
    records = os.popen(command).readlines()
    ####D
    #print "---> %d" % len(records)
    
    ldflags = [libdirs[k]]
    liblist = []
    for record in records:
        t = split(record)
        if len(t) != 2: continue
        lib, area = t
        if area[0:4] == '/lib': continue
        if area[0:8] == '/usr/lib': continue

        k = 0
        for i, lpath in enumerate(libpaths):
            k = i
            path = "%s/%s" % (lpath, lib)
            if os.path.exists(path):
                break
            else:
                path = ''
        if path == '':
            print "** Warning: %s not along any path given in LibDirs" % lib
            continue

        lib = striplib.sub("", lib[3:])
        
        uniqueList(ldflags, libdirs[k])
        uniqueList(liblist, lib)

    return (ldflags, liblist)
#---------------------------------------------------------------------------
def unique(alist, value):
    if not value in alist: alist.append(value)
def uniqueList(alist, value):
    if not value in alist: alist.append(value)
#---------------------------------------------------------------------------
# Look for files on given list of search paths
#---------------------------------------------------------------------------
def findFile(infile, incs):
    file = strip(os.popen("find %s 2>/dev/null" % infile).readline())    
    if file != "":
        filepath = file
        for includepath in incs:
            tmp = splitfields(filepath, includepath + '/',1)
            if len(tmp) == 2:
                base, file = tmp
            else:
                base = ''
                file = tmp[0]
                
            if base == '': return (filepath, file)
        return (filepath, filepath)

    file = infile
    for includepath in incs:
        filepath = includepath + "/" + file
        filepath = strip(os.popen("find %s 2>/dev/null" % filepath).readline())
        if filepath != "": return (filepath, file)

        filepath = includepath + "/include/" + file
        filepath = strip(os.popen("find %s 2>/dev/null" % filepath).readline())
        if filepath != "": return (filepath, file)

    return ("","")
#---------------------------------------------------------------------------
# Extract method signature (name + args)
#---------------------------------------------------------------------------
def methodSignature(classname,record):
    methtype,name,args,vars,qual = decodeMethodName(record)
    return "%s::%s(%s)" % (classname,name,joinfields(args,', '))

def methodFullSignature(record):
    methtype,name,args,vars,qual = decodeMethodName(record)
    str = "%s %s(%s) %s" % (methtype,name,joinfields(args,', '),qual)
    return strip(str)
#---------------------------------------------------------------------------
# Expand macros then environment variables then shell commands
#---------------------------------------------------------------------------
envar = re.compile('(?P<env>[$]\w+)',re.M)
shell = re.compile('(?P<shell>[$]\(shell [^\)]+\))',re.M)
def expandNames(record, macros={}, what=1):

    # Expand macros
    
    for key, value in macros.items():
        record = replace(record,'$(%s)' % key, value)

    if what < 2: return record

    # Expand environment variables
    
    tokens = []
    for str,group,start,end in findAll(envar,record):
        env = str[1:]
        if os.environ.has_key(env):
            tokens.append((os.environ[env],start,end))
    if len(tokens) > 0:
        record = splice(record, tokens,'') # Suppress newline

    if what < 3: return record
    
    # Expand shell commands

    tokens = []
    for str,group,start,end in findAll(shell,record):
        command = str[7:len(str)-1]
        stdout, stdin, stderr = popen3(command)            
        errors = strip(stderr.read())
        if len(errors) == 0:
            tokens.append((stdout.read(),start,end))
    if len(tokens) > 0: record = splice(record, tokens,'')# Suppress newline
    return record
#---------------------------------------------------------------------------
def hidepath(file, pathmap=None):
    if pathmap == None:
        pathmap = os.environ
        prefix = '$'
    else:
        prefix = ''
        
    key = ''
    value = ''
    for k, v in pathmap.items():
        if v == '': continue
        if v == '.': continue
        if k == 'PWD': continue
        
        if v == file[:len(v)]:
            if len(v) > len(value):
                key = k
                value = v
    if key == '': return file
    n = len(value)
    return '%s%s%s' % (prefix, key, file[n:])
#---------------------------------------------------------------------------
# Look for classes in current element and recursively within each class
#---------------------------------------------------------------------------
def findClassElements(elem, classes, depth=0):
    if elem == None: return
    
    depth += 1
    if depth >= 20:
        fatal("lost in tree...I'm bailing!\n\t\t%s" % elem.tag)

    # Loop over elements
    
    for element in elem.getchildren():
        if element.tag == 'namespace':
            findClassElements(element, classes, depth)
        elif element.tag == 'section':
            findClassElements(element, classes, depth)
        elif element.tag == 'class':
            classes.append(element)
            findClassElements(element, classes, depth)
    return

#---------------------------------------------------------------------------
# Look for typedefs in current element and recursively
#---------------------------------------------------------------------------
def findTypedefElements(elem, elements, depth=0, namespace=[]):
    depth += 1
    if depth >= 20:
        fatal("lost in tree...I'm bailing!\n\t\t%s" % elem.tag)

    # Loop over elements
    
    for element in elem.getchildren():
        if element.tag == 'namespace':
            if element.attrib.has_key('name'):
                namespace.append(element.attrib['name'])

            findTypedefElements(element, elements, depth, namespace)

            if element.attrib.has_key('name'):
                namespace.pop()
                
        elif element.tag == 'class':
            namespace.append(element.attrib['name'])

            findTypedefElements(element, elements, depth, namespace)

            namespace.pop()
            
        elif element.tag == 'section':

            findTypedefElements(element, elements, depth, namespace)          

        elif element.tag == 'typedef':
            key, value = decodeTypedef(element.text)
            namespace.append(key)
            fullname = joinfields(namespace,'::')
            namespace.pop()
            element.attrib = {'name':key, 'fullname': fullname, 'value':value}
            elements.append(element)
    return
#---------------------------------------------------------------------------
# Look for namespaces
#---------------------------------------------------------------------------
def listNamespaces(elem, names, depth=0, namespace=[]):
    depth += 1
    if depth >= 20:
        fatal("lost in tree...I'm bailing!\n\t\t%s" % elem.tag)

    # Loop over elements
    
    for element in elem.getchildren():
        if element.tag == 'namespace':
            if element.attrib.has_key('name'):                                
                namespace.append(element.attrib['name'])
                str = joinfields(namespace,'::')
                if not str in names: names.append(str)
                
            listNamespaces(element, names, depth, namespace)

            if element.attrib.has_key('name'):
                namespace.pop()
    return
#---------------------------------------------------------------------------
def baseclassNames(element):
    bases = element.findall('baseclass')
    nlist = []
    for e in bases:
        name = extractbaseclassName(e)
        nlist.append(name)
    return nlist
#---------------------------------------------------------------------------
DEBUG1 = 0
stripincdir    = re.compile(r'interface/|include/')
striptemplate  = re.compile(r'\<.*$')
def extractHeaders(BASE, include, db, depth=0):

	if db['include'].has_key(include): return
	
	# consider only DataFormat includes

	if find(include, 'DataFormats') < 0: return
	
	depth += 1
	if depth > 50:
		print "extractHeaders is lost amongst the trees...woe is me!"
		return
	tab = (depth-1)*'  '


	##D
	if DEBUG1 > 0: print "%sINCLUDE( %s )" % (tab, include)
	
	# construct xml filename from that of include

	xmlstr = stripincdir.sub("xml/",split(include, ".")[0])
	xmlfile= "%s/src/%s.xml" % (BASE, xmlstr) 

	# get associated xml object

	if not os.path.exists(xmlfile):
		##D
		if DEBUG1 > 0: print "\t\tNOT found"
		return

    ##D
	if DEBUG1 > 0: print "%s  XMLFILE( %s )" % (tab, xmlfile)

	# xml file exists - get xml object
	
	headerxml = readXmlFile(xmlfile)
	if headerxml == None:
		print "\t\t** can't read xml file: %s" % xmlfile
		return

	classes = []
	findClassElements(headerxml, classes)
	for e in classes:
		fname  = e.attrib['fullname']
		fname = striptemplate.sub("", fname)
		if db['class'].has_key(fname): continue
		
		if DEBUG1 > 0: print "%s    CLASS( %s )" % (tab, fname)
		db['class'][fname] = (include, e)

		# Get methods of current class

		if not db['method'].has_key(fname): db['method'][fname] = []

		sections = e.findall('section')
		for s in sections:
			if s.attrib.has_key('name'):
				if s.attrib['name'] in ['private','protected']: continue

			# Get methods
			meths = s.findall("method1") + s.findall("method2")
			for m in meths: db['method'][fname].append(m)

	# Get typedefs
	
	if not db['typedef'].has_key(include): db['typedef'][include] = {}
	telem = []
	findTypedefElements(headerxml, telem)
	for e in telem:
		name  = e.attrib['name']
		if db['typedef'][include].has_key(name): continue
		value = e.attrib['value']
		db['typedef'][include][name] = value
		if DEBUG1 > 0:
			print "%s      TYPEDEF( %s -> %s )" % (tab, name, value)

	db['include'][include] = (headerxml, classes)
	
	# Recurse!
	for e in headerxml.findall('include'):
		include = e.attrib['name'][1:-1]
		if db['include'].has_key(include): continue
		if find(include, 'DataFormats') < 0: continue
		
		extractHeaders(BASE, include, db, depth)		
#---------------------------------------------------------------------------
def populateDB(BASE, include, db):
	if DEBUG1 > 0:
		print "\nBEGIN( populateDB ) %s" % include


	# Make sure xml database is initialized

	if not db.has_key('headerlist'): db['headerlist'] = []
	if not db.has_key('include'): db['include'] = {}
	if not db.has_key('class'):   db['class']   = {}
	if not db.has_key('typedef'): db['typedef'] = {}
	if not db.has_key('method'):  db['method']  = {}
		
	extractHeaders(BASE, include, db)

	# Loop over headers and get typedefs for each

	includes = db['include'].keys()	
	for include in includes:

		headerxml, classes = db['include'][include]

		# Expand typedefs by including those from
		# headers within current header
		
		inclist = headerxml.findall('include')
		for incxml in inclist:
			inc = incxml.attrib['name'][1:-1]
			if not db['include'].has_key(inc): continue
			hdr, cls = db['include'][inc]
			telem = []
			findTypedefElements(hdr, telem)
			for e in telem:
				name  = e.attrib['name']
				if db['typedef'][include].has_key(name): continue
				value = e.attrib['value']
				db['typedef'][include][name] = value
			
