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
# add all sources to be compiled into shared library
SOURCES		:= $(NAME)_dict.cc $(wildcard *.cc)
OBJECTS		:= $(SOURCES:.cc=.o)
# ----------------------------------------------------------------------------
ROOTCINT	:= rootcint
CXX			:= g++
LD			:= g++
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
LDFLAGS += $(shell root-config --ldflags) $(shell root-config --libs --nonew)
LIBS 		:= -lRooFitCore -lRooFit -lMathMore -lPyROOT -lRooStats

# Check 
#say := $(shell echo -e "\n" >& 2)
#say := $(shell echo -e "NAME:     $(NAME)\n" >& 2)
#say := $(shell echo -e "CPPFLAGS: $(CPPFLAGS)\n" >& 2)
#say := $(shell echo -e "CXXFLAGS: $(CXXFLAGS)\n" >& 2)
#say := $(shell echo -e "LDFLAGS:  $(LDFLAGS)\n" >& 2)
#say := $(shell echo -e "LIBS:     $(LIBS)\n" >& 2)

# ----------------------------------------------------------------------------
all: lib$(NAME).so

lib$(NAME).so	: $(OBJECTS)
	@echo -e "\n\t===> Linking shared library $@"
	$(LD) $(LDFLAGS) $(LIBS) -o $@ $^
	@rm -rf $(NAME)_dict.*

$(NAME)_dict.cc	: $(NAME).h
	@echo -e "\n\t===> Building dictionary $@"
	$(ROOTCINT)	-f $@ -c $(CPPFLAGS) $^

$(OBJECTS)	: %.o	: 	%.cc
	@echo -e "\n\t==> Compiling $<"
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $<

clean:
	rm -rf lib$(NAME).so $(NAME)_dict.* $(OBJECTS) 
