#!/usr/bin/env python
#-----------------------------------------------------------------------------
# File: findheaders.py
# Description: Starting with Headers, scan recursively for others.
# Created: Summer 2003 Harrison B. Prosper (CERN)
#-----------------------------------------------------------------------------
import os, sys, re
from time   import *
from string import *
from sets   import Set
from boostlib import nameonly, validateXmlFile, expandpath
from pprint import PrettyPrinter
#-----------------------------------------------------------------------------
# Functions
#-----------------------------------------------------------------------------
def fatal(s):
    print "**Error** %s" % s
    sys.exit(1)
#-----------------------------------------------------------------------------
# Make sure we have everything we need
#-----------------------------------------------------------------------------
if not os.path.exists("Headers"):
    fatal("can't find file Headers")

pp = PrettyPrinter()

headers = filter(lambda x: x != '' and x[0] != '#',
                 map(lambda x: strip(x), open("Headers").readlines()))

incpaths = sys.argv[1:]
if len(incpaths) == 0:
    fatal("Please specify search paths for headers")
#-----------------------------------------------------------------------------
include = re.compile(r'^# *include *'
                     '(\<|\")(?P<header>[\w_/]+\.h(pp)?)(\>|\")')

forward = re.compile(r'class (?P<name>[\w_]+);')

def findheaders(incpaths, expandedpaths, header, hdrs, depth=0):
    depth += 1
    if depth > 50:
        fatal("lost in tree!")

    command = 'egrep "^(# *include .*\.h|class \w+;)" %s 2>/dev/null' % header

    records = map(lambda x: strip(x), os.popen(command).readlines())
    for record in records:
        name = ''
        m = include.match(record)
        if m != None:
            name = m.group('header')
        else:
            m = forward.match(record)
            if m != None:
                name = m.group('name') + '.h'
        if name == '': continue
        
        for index, path in enumerate(incpaths):
            hdr = '%s/%s' % (path, name)
            hdr = expandpath(hdr)
            if hdr == '':
                command = 'find %s/* '\
                          '-name "*%s*" 2>/dev/null' % \
                          (path, name)                
                hdr = strip(os.popen(command).read())
            if hdr == '': continue
            hdr = "%s %s" % (path, hdr[len(expandedpaths[index])+1:])

            print "\t", hdr
            if hdr in hdrs: continue
            hdrs.append(hdr)
            findheaders(incpaths, hdr, hdrs, depth)
#-----------------------------------------------------------------------------
def main():
    expandedpaths = []
    for path in incpaths:
        expandedpaths.append(expandpath(path))

    hdrs = []
    for hdr in headers:
        basepath = ''
        kindex = -1
        for index, path in enumerate(incpaths):
            expath = expandedpaths[index]
            if hdr[:len(expath)] == expath:
                if len(expath) > len(basepath):
                    basepath = expath
                    kindex   = index

        if kindex < 0: continue        
        hdr = "%s %s" % (incpaths[kindex], hdr[len(basepath)+1:])
        print hdr
        hdrs.append(hdr)
    
    for header in headers:
        findheaders(incpaths, expandedpaths, header, hdrs)
    hdrs.sort()
    record = joinfields(hdrs,'\n')+'\n'
    open("headers",'w').write(record)
#-----------------------------------------------------------------------------
main()
