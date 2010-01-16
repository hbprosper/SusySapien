#!/usr/bin/env python
#----------------------------------------------------------------------------
# File: py2html.py
# Description: Convert a Python script to an html document
# Created: 27-May-2001, Harrison B. Prosper
#          05-Dec-2002, HBP add more keywords
#          26-Sep-2003, HBP add more keywords
#$Revision: 1.3 $
#----------------------------------------------------------------------------
# IMPORTS
#----------------------------------------------------------------------------
import os, sys, re
from sets   import Set
from glob   import glob
from string import *
from time   import time, ctime
#----------------------------------------------------------------------------
# COMMAND LINE ARGUMENTS
#----------------------------------------------------------------------------
argv = sys.argv[1:]
argc = len(argv)
if argc < 1:
    print "\nUsage:\n\tpy2html <file> [dir]\n"
    sys.exit(1)
file = argv[0]
if len(argv) == 2:
    directory = argv[1]
else:
    directory = '.'
#----------------------------------------------------------------------------
# CONSTANTS
#----------------------------------------------------------------------------
TOKEN_COLOR   = '"darkmagenta"'
STRING_COLOR  = '"grey"'
COMMENT_COLOR = '"darkred"'
TEXT_COLOR    = '"black"'

FUNCTION_COLOR= '"darkblue"'
METHOD_COLOR  = '"blue"'
EXTFUNCTION_COLOR = '"darkgreen"'


TOKENA  = ["import","from","if","def","while","return",
           "global","for","in","print","not","else","elif","del","lambda",
           "class","break","try","except","pass"]

TOKENB  = ["open","len","filter","map","eval","exec", "xrange", "range",
           "tuple","list","raw_input","enumerate", "del", "dir",
           "__init__","__del__","__str__"]

ALPHANUMERIC = letters+digits
#----------------------------------------------------------------------------
functions = re.compile(r'^ *def *(?P<name>\w+)',re.M)
imports   = re.compile('^ *from +\w+ +import +(?P<name>\w+)', re.M)
classes   = re.compile('^\s*class +(?P<name>\w+)', re.M)
others    = re.compile(r'(?!<\.)\b(?P<name>[a-zA-Z_]\w*)(?=\()', re.M)
methods   = re.compile(r'(?<=\.)\b(?P<name>[a-zA-Z_]\w*)(?=\()', re.M)

tokena    = re.compile(r'\b%s\b' % joinfields(TOKENA,'|'), re.M)
tokenb    = re.compile(r'\b%s\b' % joinfields(TOKENB,'|'), re.M)
#----------------------------------------------------------------------------
# FUNCTIONS
#----------------------------------------------------------------------------
def fatal(s):
    print "**error** %s"
    sys.exit(0)
#----------------------------------------------------------------------------
def color(c,s):
    return '<font color=%s>%s</font>' % (c,s)
#----------------------------------------------------------------------------
def boldcolor(c,s):
    #return color(c,s)
    return '<b><font color=%s>%s</font></b>' % (c,s)
#----------------------------------------------------------------------------
def findquote(s):
    i = find(s, '"')
    if i < 0: i = 999
    j = find(s, "'")
    if j < 0: j = 999
    k = min(i,j)
    if   k == 999:
        return ("",-1)   
    elif k == i:
        return ('"',k)
    else:
        return ("'",k)
#----------------------------------------------------------------------------
def findqstr(s):

    quote, j = findquote(s)
    if j < 0: return ""
    
    # Check if this is a triple quoted string

    if s[j:j+3] == 3*quote:
        quote = 3*quote

    # Find end of quoted string

    k = find(s[j+len(quote):],quote)
    str = s[j:j+k+2*len(quote)]
    return (str,s[j+k+2*len(quote):])
#----------------------------------------------------------------------------
def splitline(line):
    s = splitfields(line,"#")
    record = s[0]
    if len(s) > 1:
        comment = "#"+s[1]
    else:
        comment = ""
    return(record,comment)
#----------------------------------------------------------------------------
def getqstr(str):
        
    qstr = []
    n    = 0
    tmp  = "A"
    while (tmp <> "") and (n < 10):
        n = n + 1
        tmp = findqstr(str)
        if tmp <> "":
            qstr.append(tmp[0])
            str = tmp[1]
    return qstr
#----------------------------------------------------------------------------
def swap(str, old, new):
    lold   = len(old)
    lnew   = len(new)
    newstr = ""
    i = 0
    n = 0

    PRINTME = 0
        
    while i > -1:
        n = n + 1
        if n > 80:
            newstr = newstr + str
            break

        i = find(str, old)
        if PRINTME:
            print "\tSTR( %s )OLD(%s) %d" % (str, old, i)
        
        if i < 0:
            newstr = newstr + str
            break

        # Get character before
        
        if i == 0:
            previousChar = "@"
        else:
            previousChar = str[i-1]
        
        before   = str[:i]      # string before
        str      = str[i+lold:] # rest of string
        nextChar = str[0]       # first character of rest of string

        if PRINTME:
            print "\tPREVIOUS(%s) NEXT(%s)" % (previousChar, nextChar)
            
        if nextChar not in ['(',':']:
            newstr = newstr + before + old # Don't swap

        elif previousChar in ALPHANUMERIC:
            newstr = newstr + before + old # Don't swap

        else:
            newstr = newstr + before + new # Swap

    if PRINTME:
        print "NEW( %s ) %d" % (newstr, i)
        print
    return newstr
#----------------------------------------------------------------------------
def convert2html(record):
    record = replace(record,'&','&amp;')    
    record = replace(record,'<','&lt;')
    record = replace(record,'>','&gt;')
    record = replace(record,'~','&atilde;')
    record = replace(record," ","&nbsp;")
    return record
#----------------------------------------------------------------------------
def accumulateQuotedString(n, records):
    line = records[n]
    quote, i = findquote(line)
    if i >= 0:
        if line[i:i+3] == 3*quote:
            quote = 3*quote            
            j = find(line[i+3:],quote)
            
            if j < 0:                
                # String extends over multiple lines
                line = line + "<br>"

                while n < len(records)-1:
                    n = n + 1
                    s = replace(records[n],"\n","<br>")
                    line = line + s
                    i = find(s, quote)
                    if i >= 0:
                        line = line[:len(line)-4]
                        break
    return (n, line)
#----------------------------------------------------------------------------
# MAIN
#----------------------------------------------------------------------------
if not os.path.exists(file):
    fatal("file %s not found" % file)
    
records = open(file,"r").readlines()
record  = joinfields(records)

# Get imported class or function names

imp = imports.findall(record)
classnames = classes.findall(record)
all = others.findall(record)

# Get function names first

funcnames = functions.findall(record)
funcnames = map(lambda x: x, Set(funcnames) - Set(TOKENA+TOKENB))

methnames = methods.findall(record)
methnames = map(lambda x: x, Set(methnames) - Set(TOKENA+TOKENB))

extnames  =  map(lambda x: x,
                 Set(all) - Set(funcnames) - Set(methnames)-Set(TOKENA+TOKENB))

# Colorize

name, ext = os.path.splitext(file)
filename = "%s/%s.html" % (directory, name)
out = open(filename,"w")
#out.write('<font size=+1>\n')
out.write('<font>\n')
out.write('\t<code>\n\t\t<font color=%s>\n' % TEXT_COLOR)

records = map(lambda x: convert2html(x), records)

n = -1
while n < len(records)-1:
    n = n + 1
    
    n, line = accumulateQuotedString(n, records)
        
    record, comment = splitline(line)

    if record <> "":
        
        # Get list of quoted strings (important to do this first!)

        qstr = getqstr(record)

        # Replace quoted strings with placeholders

        if len(qstr) > 0:
            for i in xrange(len(qstr)):
                record  = replace(record,qstr[i],"@%d@" % i)
        
        # Color A-type tokens
        for token in TOKENA:
            record = re.sub(r'\b%s\b' % token,
                            boldcolor(TOKEN_COLOR, token),
                            record)
            
        # Color B-type tokens
        for token in TOKENB:
            record = re.sub(r'\b%s\b' % token,
                            boldcolor(TOKEN_COLOR, token),
                            record)
        # Color class names
        for name in classnames:
            if find(record, name) > -1:
                record = swap(record, name, boldcolor(TEXT_COLOR, name))

        # Color methods
        for name in methnames:
            if find(record, name) > -1:
                record = swap(record, name, boldcolor(METHOD_COLOR, name))
            
        # Color external functions
        for name in extnames:
            if find(record, name) > -1:
                record = swap(record, name, boldcolor(EXTFUNCTION_COLOR, name))
            
        # Color internal functions
        for name in funcnames:
            if find(record, name) > -1:
                record = swap(record, name, boldcolor(FUNCTION_COLOR, name))

        # Color quoted strings
        if len(qstr) > 0:
            for i in xrange(len(qstr)):
                record = replace(record,"@%d@" % i,
                                 color(STRING_COLOR,qstr[i]))
                
    # Color comment
    if comment <> "":
        comment = color(COMMENT_COLOR,comment)
                
    newline = record + comment + "<br>"
    out.write(newline)

out.write("\t\t</font>\n\t</code>\n</font>\n")
out.close()
