#!/usr/bin/env python
#------------------------------------------------------------------------------
# File:        xmlmap.py
# Description: Build map from class name to xml file
# Created:     23-Nov-2004, Harrison B. Prosper
#$Revison: $
#------------------------------------------------------------------------------
import os, sys, re
from time   import *
from string import *
from pprint import PrettyPrinter
from header2xml import nameonly, readXmlFile, findClassElements
#------------------------------------------------------------------------------
version = '1'
pp = PrettyPrinter()
#-----------------------------------------------------------------------------
if not os.environ.has_key('PYTHON_PROJECTS_BASE'):
    print 'PYTHON_PROJECTS_BASE not defined!\n'
    
base = os.environ['PYTHON_PROJECTS_BASE']
xmlfiles = '%s/python/XmlFiles.py'   % base
xmlmap   = '%s/python/XmlMap.py'  % base
names={'base':   base,
       'version':version,
       'date':   ctime(time()),
       'xmlmap': xmlmap}
#-----------------------------------------------------------------------------
print "Read", xmlfiles
if not os.path.exists(xmlfiles):
    print "** Can't find %s" % xmlfiles
    sys.exit(1)
records = open(xmlfiles).readlines()
records.sort()
print "List contains %s entries" % len(records)
#-----------------------------------------------------------------------------
print "Building XmlMap.py..."
delim='%(base)s/' % names

count=0
db = {}
for file in records:
    file = strip(file)
    record = split(file, delim).pop()
    project, record = splitfields(record,'/',1)
    if record[0:4] <> 'src/': continue
    module = replace(split(record[4:-4],'/xml/')[0],'/','.')

    headerelement = readXmlFile(file)
    if headerelement == None:
        print "** Error ** unable to read file\n%s" % file
        continue

    count += 1
    if (count % 1000) == 0: print count

    buffer = []
    classes = findClassElements(headerelement, buffer)
    for element in classes:
        classname = element.attrib['name']
        if not db.has_key('classname'): db[classname] = []
        db[classname].append(file)

# Make database

print "Saving XmlMap..."

out = open(xmlmap,'w')
out.write(79*"#"+'\n')
out.write("# File:    XmlMap\n" % names)
out.write("# Created: %(date)s, xmlmap.py"
          " %(version)s\n" % names)
out.write("#$Revision: 1.1.1.1 $\n")
out.write(79*"#"+'\n')
out.write("XmlMap =\\\n")
PP = PrettyPrinter(stream=out)
PP.pprint(db)
out.write("\n")
#-----------------------------------------------------------------------------



