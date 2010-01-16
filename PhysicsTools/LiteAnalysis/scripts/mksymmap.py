#!/usr/bin/env python
#------------------------------------------------------------------------------
# File:        mksymmap.py
# Description: Build SymMap file using symlist
# Created:     23-Nov-2004, Harrison B. Prosper
#$Revision: 1.5 $
#------------------------------------------------------------------------------
import os, sys, re
from string import *
from time   import *
from pprint import PrettyPrinter
from boostlib import nameonly
#------------------------------------------------------------------------------
pp        = PrettyPrinter()
base      = os.environ['PYTHON_PROJECTS']
symmap    = 'config/SymMap.py'
libraries = 'config/Libraries'

names     = {}
names['projectfile'] = 'config/Project.xml'
names['base']    = base
names['symmap']  = symmap
names['version'] = '$Revision: 1.5 $'
names['date']    = ctime(time())
names['libs']    = libraries
#-----------------------------------------------------------------------------
if not os.environ.has_key('PYTHON_PROJECTS'):
    print 'PYTHON_PROJECTS not defined!\n'
    sys.exit(0)

# Make sure we're in the right directory

if not os.path.exists('%(projectfile)s' % names):
    print "\nmksymmap.py must be run from a project directory!\n"
    sys.exit(0)

if not os.path.exists(libraries):
    print "\nmksymmap.py config/Libraries file not found!\n"
    sys.exit(0)

# A compare function for ordering strings according to length
def strcmp(x, y):
    if   len(x) < len(y):
        return -1
    elif len(x) > len(y):
        return 1
    else:
        return 0
#-----------------------------------------------------------------------------
print "Read", libraries
records = open(libraries).readlines()
print "containing %s entries" % len(records)
#-----------------------------------------------------------------------------
print "Building %(symmap)s" % names
delim='%(base)s/' % names

env = {}
s = '\\b('
for key, value in os.environ.items():
    if key[-4:] == 'BASE':
        if value != '': env[value]=key
keys = env.keys()

# Order according to string length
keys.sort(strcmp)
keys.reverse()
arch= os.environ['SCRAM_ARCH']

libcount = 0
lastlib = ''
done = {}
SymbolToLibrary = {}

for ifile, file in enumerate(records):
    file = strip(file)
    if find(file,'/gcc/') > -1:   continue
    if find(file,'/qt/') > -1:    continue
    if not os.path.exists(file):
        print "**warning** %s not found\n" % file
        continue

    library, extension = os.path.splitext(file)

    if ifile < len(records)-1:
        nextlibrary, ext = os.path.splitext(records[ifile+1])
    else:
        nextlibrary, ext = ('','')
    if nextlibrary == library: continue
    
    libcount += 1
    print "%4d %s" % (libcount, nameonly(file))
        
    recs = os.popen('symlist.py -i %s' % file).readlines()
    
    for index, record in enumerate(recs):
        for key in keys:
            if key == record[:len(key)]:
                
                record = '$(%s)%s' % (env[key], record[len(key):])
                break

        
        record = replace(record,arch,'$(CMS_SYS)')
        record = strip(record)
        if record == '': continue

        t = split(record)
        path    = t[0]
        library = "%s/%s" % (path,t[1])
        vtype   = t[2]
        symbol  = joinfields(t[3:])
        
        if vtype == 'I': SymbolToLibrary[symbol] = library

# Write out symbol map

print "Saving SymMap..."

out = open(symmap,'w')
out.write(79*"#"+'\n')
out.write("# File:    SymMap\n" % names)
out.write("# Created: %(date)s, mksymmap.py"
          " %(version)s\n" % names)
out.write("#$Revision: 1.5 $\n")
out.write(79*"#"+'\n')
out.write("SymMap =\\\n")
PP = PrettyPrinter(stream=out)
PP.pprint(SymbolToLibrary)
out.write("\n")
os.system('wc -l %s' % symmap)
#-----------------------------------------------------------------------------



