#-----------------------------------------------------------------------
# File: test.mk
# Description: GNUmakefile fragment to build an executable
#
# Available  macros
#
#		programs	Program names (without extension).
#				If defined only specified list of
#				programs will be built otherwise all
#				programs in the current directory will
#				be built.
#
#		sources		Other sources to be compiled
#
# Created: Summer 2000,	Harrison B. Prosper
#-----------------------------------------------------------------------
# Notes:
# a := b
# means immediate assignment
# whereas
# a =  b is deferred assignment
#
#-----------------------------------------------------------------------
# 	Make sure we have a defined environment
ifndef PYTHON_PROJECTS
$(error PYTHON_PROJECTS is undefined)
endif
ifndef program
$(error program is undefined)
endif
include $(PYTHON_PROJECTS)/boostutil/config/common.mk
#-----------------------------------------------------------------------
# SOURCES
#-----------------------------------------------------------------------
fsrcs	:= $(wildcard *.f)
csrcs	:= $(wildcard *.cpp)
fobjs	:= $(patsubst %.f,%.o,$(fsrcs))
cobjs	:= $(patsubst %.cpp,%.o,$(csrcs))
objects	:= $(cobjs) $(fobjs)
SAY	:= $(shell echo "Program: $(program)" >& 2)
SAY	:= $(shell echo "objects: $(objects)" >& 2)
#$(error bye)
#-----------------------------------------------------------------------
# RULES
#-----------------------------------------------------------------------
bin:	$(program)

$(program)	: $(objects)
	@echo ""
	@echo "---> Linking $@"
	$(AT)$(LDSHARED) $(LDFLAGS) $(objects) $(LIBS) -o $@
	@echo ""

$(cobjs)	: %.o : %.cpp
	@echo ""
	@echo "---> Compiling $<" 
	$(AT)$(CXX)	$(CXXFLAGS) $(CPPFLAGS) $<

$(fobjs)	: %.o : %.f
	@echo ""
	@echo "---> Compiling $<" 
	$(AT)$(F77) $(F77FLAGS) $(CPPFLAGS) $<

# Define clean up rule
clean   	:
	rm -rf $(objects) $(program)


