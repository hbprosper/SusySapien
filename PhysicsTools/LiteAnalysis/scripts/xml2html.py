#!/usr/bin/env python
#----------------------------------------------------------------------------
# File: xml2html.py.py
# Description: Convert a XML script creatd with header2xml to an html document
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
    print "\nUsage:\n\txml2html <file> [dir]\n"
    sys.exit(1)
file = argv[0]
if len(argv) == 2:
    directory = argv[1]
else:
    directory = '.'
#----------------------------------------------------------------------------
# CONSTANTS
#----------------------------------------------------------------------------
TOKEN_COLOR   = '"purple"'
STRING_COLOR  = '"grey"'
COMMENT_COLOR = '"darkred"'
TEXT_COLOR    = '"black"'

HEADER_COLOR  = '"blue"'
INCLUDE_COLOR = '"grey"'
NAMESPACE_COLOR= '"green"'
CLASS_COLOR   = '"navy"'
BASE_COLOR    = '"blue"'
SECTION_COLOR = '"rose"'
CONSTR_COLOR  = '"purple"'
DESTRU_COLOR  = '"purple"'

FUNCTION_COLOR= '"blue"'
METHOD_COLOR  = '"darkblue"'
ENUM_COLOR    = '"#2099DD"'
TYPEDEF_COLOR = '"#2099DD"'
TYPEMAP_COLOR = '"#2000DD"'
TYPE_COLOR    = '"#2000DD"'
FIELD_COLOR   = '"#0DDDD0"'
CONST_COLOR   = '"#00F00F"'

FIELDS  = ["name","fullname","module","project","type","value","return",
           "args","vars","file", "template", "qual"]

TOKENS  = ["header","include","field", "class","baseclass","enum","comment",
           "constructor","destructor","method1","method2","operator",
           "function1","function2","section","type","typemap"]

ALPHANUMERIC = letters+digits
#----------------------------------------------------------------------------
others    = re.compile(r'(?!<\.)\b(?P<name>[a-zA-Z_]\w*)(?=\()', re.M)
fields    = re.compile(r'\b%s\b' % joinfields(FIELDS,'|'), re.M)
#----------------------------------------------------------------------------
# FUNCTIONS
#----------------------------------------------------------------------------
#----------------------------------------------------------------------------
def color(c,s):
    return '<font color=%s>%s</font>' % (c,s)
#----------------------------------------------------------------------------
def boldcolor(c,s):
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
def swaptoken(token, record, colour, bold=1):
    if find(record,token) > -1:
        if bold:
            c = boldcolor(colour,token)
        else:
            c = color(colour,token)
        record = re.sub(r'((?<=;)|(?<=;/))%s(?=&)' % token, c, record)
    return record
            
def convert2html(record):
    #record = replace(record,'&','&amp;')    
    record = replace(record,'<','&lt;')
    record = replace(record,'>','&gt;')
    #record = replace(record,'~','&atilde;')
    record = replace(record," ","&nbsp;")
    return record
#----------------------------------------------------------------------------
# MAIN
#----------------------------------------------------------------------------
records = open(file,"r").readlines()
record  = joinfields(records)

# Get function names

all = others.findall(record)
extnames  =  map(lambda x: x,
                 Set(all) - Set(FIELDS+TOKENS))

# Colorize

name, ext = os.path.splitext(file)
filename = "%s/%s.htm" % (directory, name)
out = open(filename,"w")
#out.write('<font size=+1>\n')
out.write('<font>\n')
out.write('\t<code>\n\t\t<font color=%s>\n' % TEXT_COLOR)

records = map(lambda x: convert2html(x), records)

commentmode = 0

n = -1
while n < len(records)-1:
    n = n + 1
    
    line = records[n]
    
    # Accumulate triple-quoted strings
    
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

    if find(line,";comment&") > -1 or find(line,";!--") > -1:
        commentmode = 1

    if commentmode:
        comment = line
        
    else:

        record = line
        
        # Get list of quoted strings (important to do this first!)

        qstr = getqstr(record)

        # Replace quoted strings with placeholders

        if len(qstr) > 0:
            for i in xrange(len(qstr)):
                record  = replace(record,qstr[i],"@%d@" % i)
            
        # Color fields
        for token in FIELDS:
            record = re.sub(r'\b%s\b(?=\=)' % token,
                            color(TOKEN_COLOR,token),
                            record)
            
        record = swaptoken('header',     record, HEADER_COLOR)
        record = swaptoken('include',    record, INCLUDE_COLOR)
        record = swaptoken('namespace',  record, NAMESPACE_COLOR)
        record = swaptoken('class',      record, CLASS_COLOR)
        record = swaptoken('baseclass',  record, BASE_COLOR)
        record = swaptoken('section',    record, SECTION_COLOR)
        record = swaptoken('constructor',record, CONSTR_COLOR)
        record = swaptoken('destructor', record, DESTRU_COLOR)
        record = swaptoken('method1',    record, METHOD_COLOR)
        record = swaptoken('method2',    record, METHOD_COLOR)
        record = swaptoken('enum',       record, ENUM_COLOR)
        record = swaptoken('constant',   record, CONST_COLOR)
        record = swaptoken('typedef',    record, TYPEDEF_COLOR)
        record = swaptoken('typemap',    record, TYPEMAP_COLOR)
        record = swaptoken('type',       record, TYPE_COLOR)
        record = swaptoken('field',      record, FIELD_COLOR,0)
        record = swaptoken('operator',   record, FUNCTION_COLOR)
        record = swaptoken('function1',  record, FUNCTION_COLOR)
        record = swaptoken('function2',  record, FUNCTION_COLOR)

        # Color functions
        for name in extnames:
            record = swap(record,name,color(FUNCTION_COLOR,name))

        # Color quoted strings
        if len(qstr) > 0:
            for i in xrange(len(qstr)):
                record = replace(record,"@%d@" % i,
                                 color(STRING_COLOR,qstr[i]))
                
    # Color comment
    
    if commentmode:
        newline = color(COMMENT_COLOR,comment) + "<br>"
    else:
        newline = record + "<br>"

    out.write(newline)
    
    if find(line,";/comment&") > -1 or line[0:3] == "--&":
        commentmode = 0

out.write("\t\t</font>\n\t</code>\n</font>\n")
out.close()
