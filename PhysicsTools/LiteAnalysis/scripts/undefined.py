#!/usr/bin/env python
#------------------------------------------------------------------------------
# File: undefined.py
# Description: List undefined symbols
# Created:     October-2003 Harrison B. Prosper
# $Revision: 1.2 $
#------------------------------------------------------------------------------
import os, sys, re
from glob   import glob
from string import *
from getopt import getopt, GetoptError
#------------------------------------------------------------------------------
# Constants
#------------------------------------------------------------------------------
USAGE = '''
Usage:
 undefined  <error file>
'''
shortOptions = 'lies'
longOptions  = ['help']
undef  = re.compile("(?P<lib>.+(?= undefined ref)) undefined reference to "\
                    "(?P<sym>.*)", re.M)
stripit= re.compile("\(\.gnu.*\):(?= In function)?")
#------------------------------------------------------------------------------
# Functions
#------------------------------------------------------------------------------
def usage():
    print USAGE
    sys.exit(0)
    
def quit(s):
    print "\n**error** %s" %s
    print "\tgoodbye!"
    sys.exit(1)
#------------------------------------------------------------------------------
# List symbols
#------------------------------------------------------------------------------
def main():
    argv = sys.argv[1:]
    if len(argv) < 1:
        files = glob('*.FAILED')
    else:
        files = argv[0]
    
    recs = []
    for file in files:
        if not os.path.exists(file): quit("could not find file %s" % file)

        records = open(file,'r').readlines()
        index = 0
        while index < len(records):
            record = strip(records[index])
            if record == '':
                index += 1
                continue
            
            if find(record, 'undefined ref') < 0:
                index += 1
                if index < len(records):
                    record += record + ' ' + strip(records[index])
                else:
                    continue
                
            m = undef.match(record)
            if m == None:
                index += 1
                continue

            index += 1
            
            t = split(m.group('lib'),'/').pop()
            lib = joinfields(split(t,'.')[:-1],'.')
            #print lib
            
            sym = m.group('sym')[1:-1]
            #print sym
            #print 78*'-'
            rec = strip("%s\t%s" % (lib, sym)) + '\n'
            if not rec in recs: recs.append(rec)
        
    recs.sort()
    open('undefined.txt','w').writelines(recs)
#------------------------------------------------------------------------------
main()

