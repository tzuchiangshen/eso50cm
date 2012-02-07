CC       = gcc
CXX      = g++
LINK     = g++
CXXFLAGS =-Wall -O2
LIBS     =-lm -lncurses -lpanel -lpthread


OBJECTS= myClientTCP.o myClientSocket.o myTimer.o degUtil.o myNCurses.o myRS232.o
MAINOBJ= cli.o
TARGET = cli

ROOTDIR=..
OBJDIR=$(ROOTDIR)/obj
BINDIR=$(ROOTDIR)/bin
INCDIR=$(ROOTDIR)/include
SRCDIR=$(ROOTDIR)/src
DOCDIR=$(ROOTDIR)/doc
LIBDIR=$(ROOTDIR)/lib

VPATH=$(OBJDIR):$(BINDIR):$(INCDIR)

$(TARGET): $(OBJECTS) $(MAINOBJ)
	$(LINK) -o $(BINDIR)/$(TARGET) $(addprefix $(OBJDIR)/,$^)  $(LIBS) 

%.o: $(SRCDIR)/%.cpp $(INCDIR)/%.h
	$(CXX) -c $(CXXFLAGS) -I$(INCDIR) -o $(OBJDIR)/$@ $< 

$(MAINOBJ): main.cpp
	$(CXX) -c $(CXXFLAGS) -I$(INCDIR) -o $(OBJDIR)/$@ $<

#doc: $(wildcard *.cpp) $(wildcard *.h)
#	@echo Making doc...
#	@cd $(DOCDIR) && doxygen Doxyfile &> /dev/null
#	@cd $(DOCDIR)/latex && make &> /dev/null

all: dirs $(TARGET) 

dirs:
	if ! test -d $(BINDIR) ; then mkdir $(BINDIR) ; fi
	if ! test -d $(OBJDIR) ; then mkdir $(OBJDIR) ; fi

clean:
	if test -d $(BINDIR) ; then\
		cd $(BINDIR) && rm -f $(TARGET);\
	fi
	if test -d $(OBJDIR) ; then\
		cd $(OBJDIR) && rm -f $(OBJECTS) $(MAINOBJ);\
	fi

distclean: clean
	if test -d $(DOCDIR)/html;  then rm -rf $(DOCDIR)/html; fi
	if test -d $(DOCDIR)/latex; then rm -rf $(DOCDIR)/latex; fi
	if test -d $(BINDIR); then rmdir $(BINDIR); fi
	if test -d $(OBJDIR); then rmdir $(OBJDIR); fi
	
.PHONY: dirs clean
