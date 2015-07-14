# Makefile for ROOT stuff
# Author: Luke de Oliveira (luke.deoliveira@yale.edu)

# --- set directories
BIN          := bin
SRC          := src
INC          := ../
LIB          := $(CURDIR)/lib

DFRAME_INC   := $(CURDIR)/../dataframe


#  set search path
vpath %.o    $(BIN)
vpath %.cxx  $(SRC) 
vpath %.hh   $(INC)

# --- set compiler and flags (roll c options and include paths together)

CXX          ?= g++
CXXFLAGS     := -Wall -fPIC -I$(INC) -I./ -I../ -g -std=c++11 $(ADDTL_FLG) -I$(DFRAME_INC)

ifeq ($(CXX),clang++)
CXXFLAGS += -stdlib=libc++
endif

ROOTCFLAGS    := $(shell root-config --cflags)
ROOTLIBS      := -L$(shell root-config --libdir)
ROOTLIBS      += -lCore -lTree -lRIO 
#ROOTLIBS      += -lCint		# don't know why we need this...
ROOTLDFLAGS   := $(shell root-config --ldflags)


CXXFLAGS     += $(ROOTCFLAGS)
LDFLAGS      += $(ROOTLDFLAGS)
LIBS         += $(ROOTLIBS)
# LDFLAGS      := -Wl,--no-undefined 

LIBS         += -L$(DFRAME_INC)/lib -ldataframe

LIBRARY = $(LIB)/libROOTinterface.a

# ---- define objects

UTIL_OBJ     := smart_chain.o tree_reader.o vec_tree_reader.o

# - command line interface
EXE_OBJ      := root_test.o

test         := $(TEST_EXEC)

TEST_EXEC   := root_test

LIB_OBJ      := $(UTIL_OBJ)
ALLOBJ       := $(EXE_OBJ) $(UTIL_OBJ)
ALLOUTPUT    := $(LIBRARY)


all: $(ALLOUTPUT)
	@echo "libROOTinterface.a build successful."

root_test: $(LIB_OBJ:%=$(BIN)/%) $(EXE_OBJ:%=$(BIN)/%)
	@echo "linking $^ --> $@"
	@$(CXX) -o $@ $^ $(LIBS) $(LDFLAGS)

$(LIBRARY): $(LIB_OBJ:%=$(BIN)/%)
	@mkdir -p $(LIB)
	@echo "linking objects to --> $@"
	@ar rc $@ $^ && ranlib $@



# --------------------------------------------------


# compile rule
$(BIN)/%.o: %.cxx
	@echo compiling $<
	@mkdir -p $(BIN)
	@$(CXX) -c $(CXXFLAGS) $< -o $@

# use auto dependency generation

DEP = $(BIN)

DEPTARGSTR = -MT $(BIN)/$*.o -MT $(DEP)/$*.d
$(DEP)/%.d: %.cxx
	@echo making dependencies for $<
	@mkdir -p $(DEP)
	@$(CXX) -MM -MP $(DEPTARGSTR) $(CXXFLAGS) $< -o $@ 

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),rmdep)
ifneq ($(MAKECMDGOALS),purge)
include $(ALLOBJ:%.o=$(DEP)/%.d)
endif
endif
endif

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
