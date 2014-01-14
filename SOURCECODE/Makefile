# Makefile for AGILEPack
# Author: Luke de Oliveira (luke.deoliveira@yale.edu)

# --- set dirs
BIN          := bin
SRC          := src
INC          := include
LIB          := $(CURDIR)/lib

CXX          := g++

YAML_DIR     := $(CURDIR)/yaml-cpp




ifneq ($(MAKECMDGOALS),basic)

YAML_INC     := $(YAML_DIR)/include
YAML_LIB     := $(YAML_DIR)/lib

HACKED_BOOST := $(YAML_DIR)/boost_subset

LIBS         += -L$(YAML_LIB) -Wl,-rpath,$(YAML_LIB)
LIBS         += -lYAMLCPP_INTERNAL

YAML         = yaml_proxy

INFOMSG      := build_yaml
else

YAML_INC     := $(YAML_DIR)/include
HACKED_BOOST := $(YAML_DIR)/boost_subset
LIBS         += -lyaml-cpp

INFOMSG      := dont_build_yaml

endif



# YAML_INC     := $(YAML_DIR)/include



#  set search path
vpath %.o    $(BIN)
vpath %.cxx  $(SRC) 
vpath %.hh   $(INC) 

# --- set compiler and flags (roll c options and include paths together)
DEBUG =  -O0 -g 
PRODUCTION = -O2 -DEIGEN_NO_DEBUG


CXXFLAGS     := -O2 -Wall -fPIC -I$(INC) -g -std=c++11 $(DEBUG) -I$(YAML_INC) -I$(HACKED_BOOST)
# LDFLAGS      := -Wl,--no-undefined 

# fix for ubuntu (that doesn't use bash for /bin/sh)
SHELL        := bash

# --- external dirs 

# LIBS += -lyaml-cpp

# ---- define objects
# - not-python 
LAYER_OBJ    := layer.o autoencoder.o architecture.o

UTIL_OBJ     := activation.o basedefs.o sha1.o

# - command line interface
EXE_OBJ      := main.o


EXECUTABLE   := AGILE

LIB_OBJ      := $(LAYER_OBJ) $(UTIL_OBJ)
ALLOBJ       := $(LAYER_OBJ) $(EXE_OBJ) $(UTIL_OBJ)
ALLOUTPUT    := $(EXECUTABLE) $(LIB)/libAGILE.so


all: $(INFOMSG) $(YAML) $(ALLOUTPUT)
	@echo "+++++++++++++++++++++++++++++++++++++++++++"
	@echo "+++ Complete AGILEPack build sucessful. +++"
	@echo "+++++++++++++++++++++++++++++++++++++++++++"

complete: all

library: $(INFOMSG) $(YAML) $(LIB)/libAGILE.so

cli: $(INFOMSG) $(YAML) $(EXECUTABLE)

yaml: $(YAML)

agile: $(INFOMSG) $(ALLOUTPUT)

$(EXECUTABLE): $(LIB_OBJ:%=$(BIN)/%) $(EXE_OBJ:%=$(BIN)/%)
	@echo "linking $^ --> $@"
	@$(CXX) -o $@ $^ $(LIBS)

$(LIB)/libAGILE.so: $(LIB_OBJ:%=$(BIN)/%)
	@mkdir -p $(LIB)
	@echo "linking $^ --> $@"
	@$(CXX) -o $@ $^ $(LIBS) $(LDFLAGS) -shared

$(YAML):
	@$(MAKE) -C $(YAML_DIR)

dont_build_yaml: 
	@echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
	@echo "+ Assuming YAML C++ exists in a standard search directory. +"
	@echo "+             If this fails, invoke just 'make'.           +"
	@echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"

build_yaml: 
	@echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
	@echo "+   Installing a local version of the YAML C++ library     +"
	@echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"

basic: all

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
ifneq ($(MAKECMDGOALS),purge)
include  $(ALLOBJ:%.o=$(DEP)/%.d)
endif
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
	rm -fr $(BIN) 
	@$(MAKE) -C $(YAML_DIR) clean

purge:
	rm -fr $(CLEANLIST) $(CLEANLIST:%=$(BIN)/%) $(CLEANLIST:%=$(DEP)/%)
	rm -fr $(BIN) 
	rm -fr $(EXECUTABLE) $(LIB)
	@$(MAKE) -C $(YAML_DIR) purge

rmdep: 
	rm -f $(DEP)/*.d