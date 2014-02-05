# Makefile for Generic
# Author: Luke de Oliveira (luke.deoliveira@yale.edu)

# --- set directories
BIN          := bin
SRC          := src
INC          := include
# LIB          := $(CURDIR)/lib

ROOT_DIR     := $(CURDIR)/root
ROOT_LIB     := $(ROOT_DIR)/lib
ROOT_INC     := $(ROOT_DIR)/include

AGILE_DIR    := $(CURDIR)/agile
AGILE_LIB    := $(AGILE_DIR)/lib
AGILE_INC    := $(AGILE_DIR)


DATAFRAME_DIR := $(CURDIR)/dataframe
DATAFRAME_LIB := $(DATAFRAME_DIR)/lib
DATAFRAME_INC := $(DATAFRAME_DIR)/include

YAML_DIR      := $(CURDIR)/yaml-cpp
YAML_LIB      := $(YAML_DIR)/lib
YAML_INC      := $(YAML_DIR)

BOOST_INC      := $(YAML_DIR)/boost_subset




#  set search path
vpath %.o    $(BIN)
vpath %.cxx  $(SRC) 
vpath %.hh   $(INC) 

# --- set compiler and flags (roll c options and include paths together)


# --  shit for eigen to be blazingly fast

CXX          ?= g++
CXXFLAGS     := -Wall -fPIC -I$(INC) -g -std=c++11
CXXFLAGS     += -I$(DATAFRAME_INC) 
CXXFLAGS     += -I$(AGILE_INC)
CXXFLAGS     += -I$(ROOT_INC)
CXXFLAGS     += -I$(YAML_INC)
CXXFLAGS     += -I$(BOOST_INC)
CXXFLAGS     += -I./

LIBS         += -L$(DATAFRAME_LIB) -Wl,-rpath,$(DATAFRAME_LIB)
LIBS         += -ldataframe

LIBS         += -L$(AGILE_LIB) -Wl,-rpath,$(AGILE_LIB)
LIBS         += -lagile

LIBS         += -L$(YAML_LIB) -Wl,-rpath,$(YAML_LIB)
LIBS         += -lyamlc++


LIBS         += -L$(ROOT_LIB) -Wl,-rpath,$(ROOT_LIB)
LIBS         += -lROOTinterface
# LDFLAGS      := -Wl,--no-undefined 

# fix for ubuntu (that doesn't use bash for /bin/sh)
SHELL        := bash

ROOTCFLAGS    := $(shell root-config --cflags)
ROOTLIBS      := -L$(shell root-config --libdir)
ROOTLIBS      += -lCore -lTree -lRIO 
ROOTLIBS      += -lCint		# don't know why we need this...
ROOTLDFLAGS   := $(shell root-config --ldflags)


CXXFLAGS     += $(ROOTCFLAGS)
LDFLAGS      += $(ROOTLDFLAGS)
LIBS         += $(ROOTLIBS)

# --- external dirs 

# LIBS += -lyaml-cpp

# ---- define objects
# - not-python 

# - command line interface
BINARIES     := model_frame.o neural_net.o
EXE_OBJ      := test.o



EXECUTABLE   := test

ALLOBJ       := $(EXE_OBJ) $(BINARIES) 


LIBRARIES    := agile_proxy dataframe_proxy root_proxy
ALLOUTPUT    := $(LIBRARIES) $(EXECUTABLE)


all: $(ALLOUTPUT)


$(EXECUTABLE): $(EXE_OBJ:%=$(BIN)/%) $(BINARIES:%=$(BIN)/%)
	@echo "linking $^ --> $@"
	@$(CXX) -o $@ $^ $(LIBS) $(LDFLAGS)

# $(LIB)/libAGILE.so: $(LIB_OBJ:%=$(BIN)/%)
# 	@mkdir -p $(LIB)
# 	@echo "linking $^ --> $@"
# 	@$(CXX) -o $@ $^ $(LIBS) $(LDFLAGS) -shared


agile_proxy:
	@$(MAKE) -C $(AGILE_DIR)

dataframe_proxy:
	@$(MAKE) -C $(DATAFRAME_DIR) 

root_proxy:
	@$(MAKE) -C $(ROOT_DIR)



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

purge:
	rm -fr $(CLEANLIST) $(CLEANLIST:%=$(BIN)/%) $(CLEANLIST:%=$(DEP)/%)
	rm -fr $(BIN) 

rmdep: 
	rm -f $(DEP)/*.d