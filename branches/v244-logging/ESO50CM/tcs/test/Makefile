### Configuration.  ####################################################
CC                     = g++
# Default C compiler options
CFLAGS                 = -I../include
# C source files for the server. 
SOURCES                = testEndian.cpp
# Corresponding object files.
OBJECTS                = $(SOURCES:.cpp=.o)
# Target
TARGET		       = testEndian
### Rules.  ############################################################
# Phony targets don’t correspond to files that are built; they’re names
# for conceptual build targets.
.PHONY:		all clean

# Default target: build everything.
all:		testEndian 

# Clean up build products.
clean:
	rm -f $(OBJECTS) $(TARGET) 

# The main server program.  Link with -Wl,-export-dyanamic so
# dynamically loaded modules can bind symbols in the program.  Link in
# libdl, which contains calls for dynamic loading.
$(TARGET):		$(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) 

# All object files in the server depend on server.h.  But use the
# default rule for building object files from source files.
$(OBJECTS): \
%.o:	%.cpp
	$(CC) $(CFLAGS) -c $< 

