# Makefile for libAGILEPack.a and for the main CLI.
# Author: Luke de Oliveira (luke.deoliveira@yale.edu)

# --- set directories so the compiler can find things
BIN           := bin
SRC           := src
INC           := include
LIB           := $(CURDIR)/lib


# --- Directories for subdir make invocations.
ROOT_DIR      := $(CURDIR)/root
AGILE_DIR     := $(CURDIR)/agile
DATAFRAME_DIR := $(CURDIR)/dataframe
YAML_DIR      := $(CURDIR)/yaml-cpp

#  set search paths for automatic file finding.
vpath %.o    $(BIN)
vpath %.cxx  $(SRC) cli-src
vpath %.hh   $(INC) 

# --- set compiler and flags (roll c options and include paths together)

CXX           ?= g++
CXXFLAGS      := -Wall -fPIC -I$(INC) -g -std=c++11
CXXFLAGS      += -I./

ifeq ($(CXX),clang++)
CXXFLAGS += -stdlib=libc++
endif

# --- Take care of AGILEPack stuff with agile-config script

AGILECFLAGS   := $(shell ./agile-config compile --root)
AGILELIBS     := $(shell ./agile-config link --root)

CXXFLAGS      += $(ROOTCFLAGS)
CXXFLAGS      += $(AGILECFLAGS)
LDFLAGS       += $(ROOTLDFLAGS)
LIBS          += $(ROOTLIBS)
LIBS          += $(AGILELIBS)

# --- command line interface and library construction

BINARIES      := model_frame.o neural_net.o
EXE_OBJ       := test_interface.o


EXECUTABLE    := DeepLearnTest

ALLOBJ        := $(EXE_OBJ) $(BINARIES) 


LIBRARIES     := agile_proxy dataframe_proxy root_proxy

LIBRARY       := $(LIB)/libAGILEPack.a

ALLOUTPUT     := $(LIBRARY) $(EXECUTABLE)

all: $(LIBRARY) $(EXECUTABLE)

$(EXECUTABLE):  $(EXE_OBJ:%=$(BIN)/%) 
	@echo "linking $^ --> $@"
	@$(CXX) -o $@ $^ $(LIBS) $(LIBRARY) $(LDFLAGS)


$(LIBRARY): $(LIBRARIES) $(BINARIES:%=$(BIN)/%) 
	@touch $(EXECUTABLE)
	@rm $(EXECUTABLE)
	@mkdir -p $(LIB)
	@echo "linking objects --> $@"
	@ar rc $@ $(shell find $(YAML_DIR) $(ROOT_DIR) $(DATAFRAME_DIR) $(AGILE_DIR) $(BIN)| grep "\.o") && ranlib $@


.NOTPARALLEL: $(EXECUTABLE)

agile_proxy:
	@$(MAKE) -C $(AGILE_DIR)

dataframe_proxy:
	@$(MAKE) -C $(DATAFRAME_DIR) 

root_proxy:
	@$(MAKE) -C $(ROOT_DIR)


# Rules for dependency generation and compilation
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

#purge it!
purge: clean
	rm -rf $(EXECUTABLE) $(LIB)
	@$(MAKE) -C $(AGILE_DIR) purge
	@$(MAKE) -C $(DATAFRAME_DIR)  purge
	@$(MAKE) -C $(ROOT_DIR) purge


rmdep: 
	rm -f $(DEP)/*.d