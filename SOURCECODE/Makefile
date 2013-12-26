BIN = bin
SRC = src
INC = include

# INSTALLPATH = /usr/local/bin
# HEADERPATH = /usr/local/include


vpath %.o    $(BIN)
vpath %.cxx  $(SRC) 
vpath %.hh    $(INC) 

DEBUG =  -O0 -g 
PRODUCTION = -O2 -DEIGEN_NO_DEBUG

CXX = g++
CXXFLAGS = -std=c++11 -O3 -I$(INC) -fPIC $(DEBUG) -m64 -Wall -Wextra
# LIBS = -L/usr/local/opt/yaml-cpp/lib
LIBS = -lyaml-cpp
LDFLAGS = 
#-L/usr/local/opt/boost/lib

ROOTCFLAGS = 
#$(shell root-config --cflags)
ROOTLIBS = 
#$(shell root-config --libs)
# ROOTLIBS = -L$(shell root-config --libdir) -lTree
ROOTLDFLAGS = 
#$(shell root-config --ldflags)

CXXFLAGS += $(ROOTCFLAGS)
LIBS += $(ROOTLIBS)
LDFLAGS += $(ROOTLDFLAGS)

OBJ = main.o layer.o architecture.o base64.o sha1.o

# HEADER = JetTagger.h

TARGET = AGILE


$(TARGET): $(OBJ:%=$(BIN)/%)
	@echo "Linking the target $@"
	@$(CXX) -o $@ $^ $(LIBS) $(LDFLAGS)

$(BIN)/%.o: %.cxx
	@echo compiling $<
	@mkdir -p $(BIN)
	@$(CXX) -c $(CXXFLAGS) $< -o $@

.PHONY : clean test

CLEANLIST = *~ *.o *.o~

# install:
# 	@echo "Installing $(TARGET) to $(INSTALLPATH)"
# 	@rm -rf $(INSTALLPATH)/$(TARGET)
# 	@ln $(TARGET) $(INSTALLPATH)/$(TARGET)

# header:
# 	@echo "Installing $(HEADER) to $(HEADERPATH)"
# 	@cp $(INC)/$(HEADER) $(HEADERPATH)

clean:
	rm -rf $(CLEANLIST) $(CLEANLIST:%=$(BIN)/%)
	rm -rf $(BIN)

# purge:
# 	rm -rf $(CLEANLIST) $(CLEANLIST:%=$(BIN)/%)
# 	rm -rf $(BIN)
# 	rm -rf $(TARGET) $(INSTALLPATH)/$(TARGET)

# ----- lightweight client example

# APP_EXAMPLE = app-example

# test: $(APP_EXAMPLE)

# $(APP_EXAMPLE): $(APP_EXAMPLE).cxx JetTagger.h
# 	@echo "making lightweight example"
# 	@$(CXX) $< -o $@
# 	@echo "made $(APP_EXAMPLE), run to test!"