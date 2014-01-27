# Makefile for Dataframe
# Author: Luke de Oliveira (luke.deoliveira@yale.edu)

# --- set directories
BIN          := bin
SRC          := src
INC          := include
LIB          := $(CURDIR)/lib



#  set search path
vpath %.o    $(BIN)
vpath %.cxx  $(SRC) 
vpath %.hh   $(INC) 

# --- set compiler and flags (roll c options and include paths together)


CXX          := g++
CXXFLAGS     := -Wall -fPIC -I$(INC) -g -std=c++11 
LIBRARY      := $(LIB)/libDataframe.so
# LDFLAGS      := -Wl,--no-undefined 

# --- external dirs 

# LIBS += -lyaml-cpp

# ---- define objects
# - not-python 

FRAME_OBJ     := csv_reader.o dataframe.o

# - command line interface
EXE_OBJ      := dataframe_test.o

EXECUTABLE   := test

LIB_OBJ      := $(FRAME_OBJ)
ALLOBJ       := $(EXE_OBJ) $(FRAME_OBJ)
ALLOUTPUT    := $(EXECUTABLE)


all: $(LIBRARY) $(ALLOUTPUT)

lib: $(LIBRARY)

$(EXECUTABLE): $(LIB_OBJ:%=$(BIN)/%) $(EXE_OBJ:%=$(BIN)/%)
	@echo "linking $^ --> $@"
	@$(CXX) -o $@ $^ $(LIBS)


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
	rm -fr $(EXECUTABLE) $(LIB)

rmdep: 
	rm -f $(DEP)/*.d