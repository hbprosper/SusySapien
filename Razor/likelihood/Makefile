# Build libRooRazor2DBackground.so
# Created 27 Feb. 2013 HBP & SS
# ----------------------------------------------------------------------------
ifndef ROOTSYS
$(error *** Please set up Root)
endif

# Check if user has defined location of RooFit
ifndef ROOFIT

ifndef CMS_PATH
	$(error Please set up CMSSW so that we have access to RooFit)
endif

LCG_PATH	:= $(CMS_PATH)/$(SCRAM_ARCH)/lcg
# use first version we find
ROOFIT 		:= $(word 1, $(shell find $(LCG_PATH)/roofit/*cms* -maxdepth 0))
endif
# ----------------------------------------------------------------------------
NAME		:= RooRazor2DBackground

# get lists of sources
CINTSRCS	:= $(wildcard *_dict.cc)
CCSRCS		:= $(filter-out $(CINTSRCS),$(wildcard Roo*.cc))
CXXSRCS		:= $(wildcard Roo*.cxx)
OTHERSRCS	:= $(filter-out $(CINTSRCS) $(CCSRCS),$(wildcard *.cc))

# list of dictionaries to be created
DICTIONARIES	:= $(CCSRCS:.cc=_dict.cc) $(CXXSRCS:.cxx=_dict.cc)
HEADERS		:= $(CCSRCS:.cc=.h) $(CXXSRCS:.cxx=.h)

# get list of objects
CCOBJS		:= $(CCSRCS:.cc=.o) $(OTHERSRCS:.cc=.o) $(DICTIONARIES:.cc=.o)
CXXOBJS		:= $(CXXSRCS:.cxx=.o)
OBJECTS		:= $(CCOBJS) $(CXXOBJS)

#say := $(shell echo "OTHERSRCS:     $(OTHERSRCS)" >& 2)
#say := $(shell echo "CCOBJS:     $(CCOBJS)" >& 2)
#say := $(shell echo "CXXOBJS:    $(CXXOBJS)" >& 2)
#$(error bye)
# ----------------------------------------------------------------------------
ROOTCINT	:= rootcint
CXX		:= g++
LD		:= g++
CPPFLAGS	:= -I. -I$(ROOFIT)/include
CXXFLAGS	:= -O -Wall -fPIC -g -ansi -Wextra $(shell root-config --cflags)
LDFLAGS		:= -g  -L$(ROOFIT)/lib
# ----------------------------------------------------------------------------
# which operating system?
OS := $(shell uname -s)
ifeq ($(OS),Darwin)
	LDFLAGS += -dynamiclib
else
	LDFLAGS	+= -shared
endif	
LDFLAGS 	+= $(shell root-config --ldflags) $(shell root-config --libs --nonew)
LIBS 		:= -lRooFitCore -lRooFit -lMathMore -lPyROOT -lRooStats

#say := $(shell echo -e "\n" >& 2)
#say := $(shell echo -e "NAME:     $(NAME)\n" >& 2)
#say := $(shell echo -e "CPPFLAGS: $(CPPFLAGS)\n" >& 2)
#say := $(shell echo -e "CXXFLAGS: $(CXXFLAGS)\n" >& 2)
#say := $(shell echo -e "LDFLAGS:  $(LDFLAGS)\n" >& 2)
#say := $(shell echo -e "LIBS:     $(LIBS)\n" >& 2)

# ----------------------------------------------------------------------------
all: lib$(NAME).so

lib$(NAME).so	: $(OBJECTS)
	@echo "\n\t===> Linking shared library $@"
	$(LD) $(LDFLAGS) $(LIBS) -o $@ $^

$(DICTIONARIES)	: %_dict.cc	: %.h
	@echo "\n\t===> Building dictionary $@"
	$(ROOTCINT)	-f $@ -c $(CPPFLAGS) $^

$(CCOBJS)	: %.o	: 	%.cc
	@echo "\n\t==> Compiling $<"
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $<

$(CXXOBJS)	: %.o	: 	%.cxx
	@echo "\n\t==> Compiling $<"
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $<

clean:
	rm -rf *_dict.* *.o 

veryclean:
	rm -rf lib$(NAME).so *_dict.* *.o