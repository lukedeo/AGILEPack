# makefile for ndhist histogram library
# Author: Dan Guest (dguest@cern.ch)
# Created: Sat Jan 28 13:09:53 CET 2012

# --- set dirs
BIN          := bin
SRC          := src
INC          := include
PYTHON       := python
LIB          := $(CURDIR)/lib

#  set search path
vpath %.o    $(BIN)
vpath %.cxx  $(SRC) 
vpath %.hh   $(INC) 

# --- set compiler and flags (roll c options and include paths together)
CXX          ?= g++
CXXFLAGS     := -O2 -Wall -fPIC -I$(INC) -g -std=c++11
# LDFLAGS      := -Wl,--no-undefined 

# fix for ubuntu (that doesn't use bash for /bin/sh)
SHELL         := bash

# --- external dirs 
# (sometimes hdf is in a werid place, this will work as long as h5ls works)
HDF_PATH      := $(dir $(shell type -p h5ls | xargs dirname))
ifndef HDF_PATH
$(error "couldn't find HDF5 `h5ls` command, HDF5 probably not installed...")
endif
COMMON_LIBS   := $(HDF_PATH)/lib
LIBS          := -L$(COMMON_LIBS) -Wl,-rpath,$(COMMON_LIBS) 
CXXFLAGS      += -I$(HDF_PATH)/include

LIBS += -lhdf5_cpp -lhdf5 

# ---- define objects
# - not-python 
GEN_OBJ     := Histogram.o Binners.o 

# - command line interface
EXE_OBJ      := test.o

ALLOBJ       := $(GEN_OBJ) $(PY_OBJ) $(EXE_OBJ)
ALLOUTPUT    := test $(LIB)/libndhist.so

all: $(ALLOUTPUT) 

test: $(GEN_OBJ:%=$(BIN)/%) $(EXE_OBJ:%=$(BIN)/%)
	@echo "linking $^ --> $@"
	@$(CXX) -o $@ $^ $(LIBS) 

$(LIB)/libndhist.so: $(GEN_OBJ:%=$(BIN)/%)
	@mkdir -p $(LIB)
	@echo "linking $^ --> $@"
	@$(CXX) -o $@ $^ $(LIBS) $(LDFLAGS) -shared

# --------------------------------------------------

# compile rule
$(BIN)/%.o: %.cxx
	@echo compiling $<
	@mkdir -p $(BIN)
	@$(CXX) -c $(CXXFLAGS) $< -o $@

# use auto dependency generation
DEP = $(BIN)

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),rmdep)
include  $(ALLOBJ:%.o=$(DEP)/%.d)
endif
endif

DEPTARGSTR = -MT $(BIN)/$*.o -MT $(DEP)/$*.d
$(DEP)/%.d: %.cxx
	@echo making dependencies for $<
	@mkdir -p $(DEP)
	@$(CXX) -MM -MP $(DEPTARGSTR) $(CXXFLAGS) $< -o $@ 

# clean
.PHONY : clean rmdep
CLEANLIST     = *~ *.o *.o~ *.d core 
clean:
	rm -fr $(CLEANLIST) $(CLEANLIST:%=$(BIN)/%) $(CLEANLIST:%=$(DEP)/%)
	rm -fr $(BIN) $(ALLOUTPUT)

rmdep: 
	rm -f $(DEP)/*.d