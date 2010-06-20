#-----------------------------------------------------------------------
# File: xml.mk
#
# Description: Makefile to build XML files from C++ headers
#  
# Created: 24-Oct-2003	Harrison B. Prosper
#
#                       External macros:
#
#                               h2xmlflags	h2xml flags
#                               verbose         [no]
#
#			Requires:
#                               $PYTHON_PROJECTS/bin/header2xml.py
#
#  Updated: 06-May-2004 HBP
#           17-Oct-2005 HBP make xml only
#-----------------------------------------------------------------------
#$Revision: 1.6 $
#-----------------------------------------------------------------------
# Make sure we have a defined environment

ifndef PYTHON_PROJECTS
$(error PYTHON_PROJECTS undefined)
endif

area	:= $(subst $(PYTHON_PROJECTS)/,,$(shell pwd))
say	:= $(shell echo -e "\n\t\tXML Descriptions - $(area)" >& 2)

ifdef verbose
	AT =
else
	AT = @
endif

ifdef debug
	DEBUG = -d$(debug)
else
	DEBUG =
endif
#-----------------------------------------------------------------------
# COMPILERS/LINKER
#-----------------------------------------------------------------------
H2XML		:= $(PYTHON_PROJECTS)/bin/header2xml.py

# SWITCHES/INCLUDES

H2XMLFLAGS 	:= -S  -I. -Iinclude $(h2xmlflags) $(DEBUG)

# SOURCES

txtfiles:= $(wildcard *.txt)
xmlfiles:= $(txtfiles:.txt=.xml)

# TARGETS AND RULES

h2xml: $(xmlfiles)

$(xmlfiles)	: %.xml : %.txt
	@echo "---> Building XML description $@"
	$(AT)$(H2XML) $(H2XMLFLAGS) `egrep "*" $<` 

clean: xmlclean

xmlclean	:
		$(AT)rm -rf *.xml*

xmlveryclean:
		$(AT)rm -rf *.xml* *.txt*

