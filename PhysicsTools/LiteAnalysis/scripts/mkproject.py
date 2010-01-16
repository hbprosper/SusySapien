#!/usr/bin/env python
#-----------------------------------------------------------------------------
# File: mkproject.py
# Description: Create Project.xml for specified CMS project
# Created: Summer 2003 Harrison B. Prosper (CERN)
# Updated: 21-May-2006 HBP - make general
#$Revision:$
#-----------------------------------------------------------------------------
import os, sys, re
from time   import *
from string import *
from sets   import Set
from boostlib import nameonly, validateXmlFile, libExists, libDependencies,\
     expandpath
from pprint import PrettyPrinter
#-----------------------------------------------------------------------------
# Functions
#-----------------------------------------------------------------------------
pp = PrettyPrinter()
def fatal(s):
    print "**Error** %s" % s
    sys.exit(1)
#-----------------------------------------------------------------------------
def preamble(name, alist, out, names):
    str = '%(tab)s<' % names + name + ' '
    tab = len(str)*' '
    for x in alist:
        record = str + x
        out.write(record)
        str = '\n\n' + tab
    out.write('/>\n\n')    
#-----------------------------------------------------------------------------
# Convert libdir to makefile syntax
#-----------------------------------------------------------------------------
findenv  = re.compile(r'(?<=\$)[\w\_\-]+\b')
def convertpaths(paths):
    pathlist  = []
    for i, path in enumerate(paths):
        wrds = findenv.findall(path)
        for wrd in wrds:
            path = re.sub(r'\b%s\b' % wrd, "(%s)" % wrd, path)
        pathlist.append(path)
    return pathlist
#-----------------------------------------------------------------------------
# Make sure we have everything we need
#-----------------------------------------------------------------------------
if not os.path.exists("Project.py"):
    fatal("can't find file Project.py")
    
if not os.path.exists("Headers"):
    fatal("can't find file Headers")
#-----------------------------------------------------------------------------
# Required Keywords: project, basepath
#-----------------------------------------------------------------------------
version = ''
cppflags= ''
cxxflags= ''
ldflags = ''

execfile("Project.py")

try:
    project = strip(project)
except:
    fatal("keyword project not found in file Project.py")

try:
    incpaths = strip(incpaths)
except:
    fatal("keyword incpaths not found in file Project.py")

try:
    libpaths = strip(libpaths)
except:
    fatal("keyword libpaths not found in file Project.py")

libpaths+= ' $PYTHON_PROJECTS/boostutil/lib $BOOST_BASE/lib'
libdirs = split(libpaths)
libpaths= map(lambda x: expandpath(x), libdirs)

incdirs = split(incpaths)
incpaths= map(lambda x: expandpath(x), incdirs)

print incdirs

# Find headers

#print "finding headers..."
#str = replace(joinfields(incdirs,' '),'$','\\$')
#command = "findheaders.py %s" % str
#os.system(command)
#if not os.path.exists("headers"):
#    fatal("Unable to find file headers created by findheaders.py")

hdrs = map(lambda x: strip(x), open("Headers").readlines())
headers = []
for i, hdr in enumerate(hdrs):
    basepath = ''
    kindex = -1
    for index, expath in enumerate(incpaths):
        if hdr[:len(expath)] == expath:
            if len(expath) > len(basepath):
                basepath = expath
                kindex   = index
    if kindex < 0: continue        
    hdr = "%s %s" % (incdirs[kindex], hdr[len(basepath)+1:])
    print i,hdr
    headers.append(hdr)
libdirs = convertpaths(libdirs)
incdirs = convertpaths(incdirs)
#-----------------------------------------------------------------------------
# Optional Keywords: version, cppflags, cxxflags
#-----------------------------------------------------------------------------
options = []
incdirs = joinfields(map(lambda x: " -I%s" % x, incdirs))
cppflags+= incdirs
if cppflags != '':
    cppflags =  joinfields(split(cppflags),'\n')
    options.append('cppflags="\n%s"' % cppflags)

if cxxflags != '':
    cxxflags = joinfields(split(cxxflags),'\n')
    options.append('cxxflags="\n%s"' % cxxflags)

if ldflags != '':
    ldflags = joinfields(split(ldflags),'\n')
    options.append('ldflags="\n%s"' % ldflags)

try:
    withroot = joinfields(split(withroot),'\n')
    options.append('withroot="\n%s"' % withroot)
except:
    pass

try:
    withpython = joinfields(split(withpython),'\n')
    options.append('withpython="\n%s"' % withpython)
except:
    pass

try:
    withmodule = joinfields(split(withmodule),'\n')
    options.append('withmodule="\n%s"' % withmodule)
except:
    pass
#-----------------------------------------------------------------------------
TAB = 2*' '
names = {'project':   project,
         'version':   version,
         'tab':       TAB}

#-----------------------------------------------------------------------------
out = open("Project.xml","w")
out.write('<?xml version="1.0"?>\n')
out.write('<!-- '+70*'=' + ' -->\n')
out.write('<!-- Created: %s' % ctime(time())+" by mkproject.py"+22*' '+'-->\n')
out.write('<!-- '+70*'=' + ' -->\n\n')
out.write('<project name="%(project)s"' % names)
if version != '': out.write('\n         version="%(version)s"' % names)
out.write('>\n\n')
#-----------------------------------------------------------------------------
# Write out stuff common to all components
#-----------------------------------------------------------------------------
preamble('options', options, out, names)
#-----------------------------------------------------------------------------
# Write out each component
#-----------------------------------------------------------------------------
NoPackage  = len(sys.argv[1:]) == 0

headers.sort()
modulemap = {}

print "Number of headers:", len(headers)

for header in headers:
    
    if strip(header) == "": continue

    path, hdr = split(header)
    
    # Determine library name
    
    t = split(hdr, '/')
    for x in ['src','include','interface','inc']:
        if x in t: t.remove(x)

    if len(t) == 0: continue
    
    if len(t) == 1:
        package = ''
        module  = project
        lib     = project
    elif len(t) == 2:
        package = ''
        module  = t[0]
        lib     = t[0]
    elif NoPackage:
        package = ''
        module  = t[1]
        lib     = t[1]
    else:
        package = t[0]
        module  = t[1]
        lib     = t[0]+t[1]

    ####D
    #print "PACKAGE(%s) MODULE(%s) LIB(%s)" % (package, module, lib)

    if not modulemap.has_key(module):
        modulemap[module] = {'package': package, 'path': {}, 'lib': lib}
        
    if not modulemap[module]['path'].has_key(path):
        modulemap[module]['path'][path] = []
    modulemap[module]['path'][path].append(hdr)

####D
#pp.pprint(modulemap)

#-----------------------------------------------------------------------------
# Now write out
#-----------------------------------------------------------------------------
for module, value in modulemap.items():
    
    lib = value['lib']
    package = value['package']
    
    names['package']= package
    names['module'] = module
    names['name']   = module
    names['lib']    = lib
    
    print "%s %s\t%s" % (package, module, lib)
    
    out.write('\n')
    out.write('%(tab)s<component name="%(name)s"\n' % names)
    if package != '':
        out.write('%(tab)s           package="%(package)s"\n' % names)
    out.write('%(tab)s           module="%(module)s">\n' % names)

    # Add libraries

    library = "lib%s.so" % lib
    if libExists(libpaths, library):
        ldflags, libs = libDependencies(libpaths, libdirs, library)
        liblist = [lib] + libs

        names['tab'] = 2*TAB
        out.write("\n")
        out.write('%(tab)s<options ldflags="' % names)
        delim = ''
        for area in ldflags:
            print "\t", area
            names['area'] = area
            names['delim'] = delim
            out.write('%(delim)s-L%(area)s' % names)
            delim = '\n%(tab)s                  ' % names
        out.write('"\n\n' % names)
        delim = ''
        out.write('%(tab)s         libs="' % names)
        for lib in liblist:
            print "\t\t", lib
            names['lib'] = lib
            names['delim'] = delim
            out.write('%(delim)s-l%(lib)s' % names)
            delim = '\n%(tab)s               ' % names
        print
        out.write('"/>\n\n' % names)

    # Add headers
    
    for index, (path, headers) in enumerate(value['path'].items()):

        names['name'] = "%s%d" % (module, index)
        names['path'] = path
        names['tab'] = 2*TAB
        out.write('%(tab)s<headers name="%(name)s"\n'
                  '%(tab)s         basepath="%(path)s">\n' % names)

        for header in headers:
            names['header'] = header
            names['tab'] = 3*TAB    
            out.write('%(tab)s<header name="%(header)s"/>\n' % names)

        names['tab'] = 2*TAB
        out.write('%(tab)s</headers>\n\n' % names)

        names['tab'] = TAB
        out.write('%(tab)s</component>\n' % names)
            
out.write('</project>\n')
out.close()
#-----------------------------------------------------------------------------
print validateXmlFile("Project.xml")
