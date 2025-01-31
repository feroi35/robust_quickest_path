# Cplex installation folder
CPLEX = /opt/ibm/ILOG/CPLEX_Studio2211/
CPLEXDIR      = $(CPLEX)/cplex/
CONCERTDIR    = $(CPLEX)/concert/

# ---------------------------------------------------------------------
# Compiler selection, code optimization, debug, and warning options
# ---------------------------------------------------------------------
CCFLAGS = -O3 -m64 -Wall -Wno-ignored-attributes -g

# Debug build flags
DEBUGFLAGS = -g -O0

# ---------------------------------------------------------------------
# Link options and libraries (CPLEX)
# ---------------------------------------------------------------------
LIBFORMAT  = static_pic
SYSTEM     = x86-64_linux

CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

CCLNFLAGS =" "
CCLNFLAGSCPLEX = -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(CONCERTLIBDIR) -lconcert -lrt -lpthread -ldl

CONCERTINCDIR = $(CONCERTDIR)/include
CPLEXINCDIR   = $(CPLEXDIR)/include
CCFLAGSCPLEX = $(CCFLAGS) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR) -DIL_STD -I$(INCDIR) #DIL_STD: CPLEX specific macro


# ---------------------------------------------------------------------
# Linking commands
# ---------------------------------------------------------------------
CXX = g++
SRCDIR = src
INCDIR = include
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(SRCS:$(SRCDIR)/%.cpp=$(SRCDIR)/%.o)
OBJS_DEBUG = $(OBJS:$(SRCDIR)/%.o=$(SRCDIR)/%_debug.o)
TARGET = myprogram

# Release build rule for object files
# use DNDEBUG to remove asserts and turn off some clog debug messages
$(SRCDIR)/%.o: $(SRCDIR)/%.cpp $(INCDIR)/%.h
	$(CXX) $(CCFLAGSCPLEX) -DNDEBUG -c $< -o $@

# Debug build rule for object files
$(SRCDIR)/%_debug.o: $(SRCDIR)/%.cpp $(INCDIR)/%.h
	$(CXX) $(CCFLAGSCPLEX) $(DEBUGFLAGS) -c $< -o $@

# Debug build
debug: $(OBJS_DEBUG)
	$(CXX) $(CCFLAGSCPLEX) $(DEBUGFLAGS) -o $(TARGET)_debug $(OBJS_DEBUG) $(CCLNFLAGSCPLEX)

# Release build
release: $(OBJS)
	$(CXX) $(CCFLAGSCPLEX) -DNDEBUG -o $(TARGET) $(OBJS) $(CCLNFLAGSCPLEX)

clean: cleanobj
	rm -f $(OBJS) $(TARGET) $(TARGET)_debug
cleanobj:
	rm -f $(SRCDIR)/*.o
