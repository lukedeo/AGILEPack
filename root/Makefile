# Makefile for ROOT stuff
# Author: Luke de Oliveira (luke.deoliveira@yale.edu)

# --- set directories
BIN          := bin
SRC          := src
INC          := include
LIB          := $(CURDIR)/lib

DFRAME_INC   := $(CURDIR)/../../generic/dataframe/include


#  set search path
vpath %.o    $(BIN)
vpath %.cxx  $(SRC) 
vpath %.hh   $(INC)

# --- set compiler and flags (roll c options and include paths together)

CXX          ?= g++
# CXX          := clang++
CXXFLAGS     := -Wall -fPIC -I$(INC) -g -std=c++11 $(ADDTL_FLG) -I$(DFRAME_INC)

ROOTCFLAGS    := $(shell root-config --cflags)
ROOTLIBS      := -L$(shell root-config --libdir)
ROOTLIBS      += -lCore -lTree -lRIO 
ROOTLIBS      += -lCint		# don't know why we need this...
ROOTLDFLAGS   := $(shell root-config --ldflags)


CXXFLAGS     += $(ROOTCFLAGS)
LDFLAGS      += $(ROOTLDFLAGS)
LIBS         += $(ROOTLIBS)
# LDFLAGS      := -Wl,--no-undefined 

LIBS         += -L$(DFRAME_INC)/../lib -lDataframe

LIBRARY = $(LIB)/libROOTInterface.so

# ---- define objects

UTIL_OBJ     := smart_chain.o tree_reader.o

# - command line interface
EXE_OBJ      := root_test.o


TEST_EXEC   := root_test

LIB_OBJ      := $(UTIL_OBJ)
ALLOBJ       := $(EXE_OBJ) $(UTIL_OBJ)
ALLOUTPUT    := $(TEST_EXEC) $(LIBRARY)


all: $(ALLOUTPUT)

$(TEST_EXEC): $(LIB_OBJ:%=$(BIN)/%) $(EXE_OBJ:%=$(BIN)/%)
	@echo "linking $^ --> $@"
	@$(CXX) -o $@ $^ $(LIBS) $(LDFLAGS)

$(LIBRARY): $(LIB_OBJ:%=$(BIN)/%)
	@mkdir -p $(LIB)
	@echo "linking objects to --> $@"
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
ifneq ($(MAKECMDGOALS),purge)
include  $(ALLOBJ:%.o=$(DEP)/%.d)
endif
endif
endif

DEPTARGSTR = -MT $(BIN)/$*.o -MT $(DEP)/$*.d
$(DEP)/%.d: %.cxx
	@echo making dependencies for $<
	@mkdir -p $(DEP)

# clean
.PHONY : clean rmdep
CLEANLIST     = *~ *.o *.o~ *.d core 

clean:
	rm -fr $(CLEANLIST) $(CLEANLIST:%=$(BIN)/%) $(CLEANLIST:%=$(DEP)/%)
	rm -fr $(BIN) 

purge:
	rm -fr $(CLEANLIST) $(CLEANLIST:%=$(BIN)/%) $(CLEANLIST:%=$(DEP)/%)
	rm -fr $(BIN) 
	rm -fr $(TEST_EXEC) $(LIB)

rmdep: 
	rm -f $(DEP)/*.d
# DO NOT DELETE
