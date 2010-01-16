#!/usr/bin/env python
#------------------------------------------------------------------------------
# File:        mkheadermap.py
# Description: Build HeaderMap.py
# Created:     23-Nov-2004, Harrison B. Prosper
#$Revision: 1.4 $
#------------------------------------------------------------------------------
import os, sys, re
from string import *
from time   import *
from pprint import PrettyPrinter
from boostlib import nameonly
#------------------------------------------------------------------------------
pp = PrettyPrinter()
#-----------------------------------------------------------------------------
if not os.environ.has_key('PYTHON_PROJECTS'):
    print 'PYTHON_PROJECTS not defined!\n'
    sys.exit(0)
    
base = os.environ['PYTHON_PROJECTS']
hdrmap  = '%s/boostutil/config/HeaderMap.py'   % base
names={'base':     base,
       'hdrmap':   hdrmap,
       'version':  '$Revision: 1.4 $',
       'date':     ctime(time())}

# A compare function for ordering strings according to length
def strcmp(x, y):
    if   len(x) < len(y):
        return -1
    elif len(x) > len(y):
        return 1
    else:
        return 0
#-----------------------------------------------------------------------------
argv = sys.argv[1:]
if len(argv) == 0:
    print "Usage:\n\tmkheadermap <header-directories>"
    sys.exit(0)

records = []
for hdir in argv:
    cmd = 'find %s -name "*.h*"' % hdir
    print cmd
    records += map(lambda x: strip(x), os.popen(cmd).readlines())
print "Number of headers: %s" % len(records)
#-----------------------------------------------------------------------------
print "Building %(hdrmap)s" % names
delim='%(base)s/' % names

env = {}
s = '\\b('
for key, value in os.environ.items():
    if key[-4:] == 'BASE':
        env[value]=key
env[os.environ['ROOTSYS']] = 'ROOTSYS'

keys = env.keys()

# Order according to string length
keys.sort(strcmp)
keys.reverse()
arch= os.environ['CMS_SYS']
larch = len(arch)

libcount = 0
lastlib = ''
done = {}
NameToHeader = {}

for ifile, file in enumerate(records):
    file = strip(file)
    if find(file,'/gcc/') > -1:   continue
    if find(file,'/qt/') > -1:    continue
    if not os.path.exists(file):
        print "**warning** %s not found\n" % file
        continue

    name = nameonly(file)
    if ifile % 1000 == 0:
        print ifile, name

    header, extension = os.path.splitext(file)
    for key in keys:
        if key == file[:len(key)]:
            base = env[key]
            hdr  = file[len(key)+1:]
            extra= ''

            if hdr[0:larch] == arch:
                base = '%s/$(CMS_SYS)' % base
                hdr  = hdr[larch+1:]
                
            if hdr[0:4] == 'src/':
                extra = '/src'
                hdr  = hdr[4:]
            elif hdr[0:8] == 'include/':
                extra = '/include'
                hdr  = hdr[8:]
            else:
                continue
            
            record = '%s %s' % (file, hdr)
            break
    else:
        record = ''

    record = strip(record)
    if record == '': continue

    if not NameToHeader.has_key(name): NameToHeader[name] = []
    
    if not record in NameToHeader[name]: NameToHeader[name].append(record)

        
# Write out name map

print "Saving HeaderMap..."

out = open(hdrmap,'w')
out.write(79*"#"+'\n')
out.write("# File:    HeaderMap\n" % names)
out.write("# Created: %(date)s, mkheadermap.py"
          " %(version)s\n" % names)
out.write("#$Revision: 1.4 $\n")
out.write(79*"#"+'\n')
out.write("HeaderMap =\\\n")
PP = PrettyPrinter(stream=out)
PP.pprint(NameToHeader)
out.write("\n")
os.system('wc -l %s' % hdrmap)
#-----------------------------------------------------------------------------



