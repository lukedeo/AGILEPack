# Makefile for AGILEPack
# Author: Luke de Oliveira (luke.deoliveira@yale.edu)

# --- set dirs
BIN          := bin
SRC          := src
INC          := include
# PYTHON       := python
LIB          := $(CURDIR)/lib

YAML_DIR     := $(CURDIR)/yaml-cpp
YAML_INC     := $(YAML_DIR)/include
YAML_LIB     := $(YAML_DIR)/lib

HACKED_BOOST := $(YAML_DIR)/boost_subset


#  set search path
vpath %.o    $(BIN)
vpath %.cxx  $(SRC) 
vpath %.hh   $(INC) 

# --- set compiler and flags (roll c options and include paths together)
DEBUG =  -O0 -g 
PRODUCTION = -O2 -DEIGEN_NO_DEBUG

CXX          := g++
CXXFLAGS     := -O2 -Wall -fPIC -I$(INC) -g -std=c++11 $(DEBUG) -I$(YAML_INC) -I$(HACKED_BOOST)
# LDFLAGS      := -Wl,--no-undefined 

# fix for ubuntu (that doesn't use bash for /bin/sh)
SHELL         := bash

# --- external dirs 

# LIBS += -lyaml-cpp
LIBS += -L$(YAML_LIB) -Wl,-rpath,$(YAML_LIB)
LIBS += -lYAMLCPP_INTERNAL

# ---- define objects
# - not-python 
LAYER_OBJ     := layer.o autoencoder.o architecture.o

UTIL_OBJ      := activation.o basedefs.o sha1.o

# - command line interface
EXE_OBJ      := main.o


EXECUTABLE   := AGILE

LIB_OBJ      := $(LAYER_OBJ) $(UTIL_OBJ)
ALLOBJ       := $(LAYER_OBJ) $(EXE_OBJ) $(UTIL_OBJ)
ALLOUTPUT    := $(EXECUTABLE) $(LIB)/libAGILE.so


YAML = yaml_proxy

all: $(YAML) $(ALLOUTPUT)
	@echo "+++++++++++++++++++++++++++++++++++++++++++"
	@echo "+++ Complete AGILEPack build sucessful. +++"
	@echo "+++++++++++++++++++++++++++++++++++++++++++"

library: $(YAML) $(LIB)/libAGILE.so

cli: $(YAML) $(EXECUTABLE)

yaml: $(YAML)

agile: $(ALLOUTPUT)

$(EXECUTABLE): $(LIB_OBJ:%=$(BIN)/%) $(EXE_OBJ:%=$(BIN)/%)
	@echo "linking $^ --> $@"
	@$(CXX) -o $@ $^ $(LIBS)

$(LIB)/libAGILE.so: $(LIB_OBJ:%=$(BIN)/%)
	@mkdir -p $(LIB)
	@echo "linking $^ --> $@"
	@$(CXX) -o $@ $^ $(LIBS) $(LDFLAGS) -shared

$(YAML):
	@$(MAKE) -C $(YAML_DIR)

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
	@$(MAKE) -C $(YAML_DIR) clean

purge: clean
	rm -fr $(EXECUTABLE) $(LIB)

rmdep: 
	rm -f $(DEP)/*.d