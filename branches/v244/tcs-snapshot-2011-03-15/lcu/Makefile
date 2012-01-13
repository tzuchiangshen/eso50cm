# Copyright 2010 Maurizio Baffico <mbaffico@gmail.com>
# Licensed under the GNU GPL.
CXXFLAGS =-Wall -O2
LIBS     =-lm

OBJSRC      = myTCP.cpp \
	      myLCU.cpp \
	      myTelescope.cpp \
	      myTClock.cpp \
	      myTAxis.cpp \
	      myTMotor.cpp \
	      myTEncoder.cpp \
	      myBSemaphore.cpp \
	      myUtil.cpp
OBJECTS     = $(OBJSRC:.cpp=.o)
MODULES_SRC = get_config.cpp \
	      set_config.cpp \
	      sync_to_zenith.cpp \
	      get_position.cpp \
	      set_target.cpp \
	      monitor_position.cpp \
	      offset_telescope.cpp \
	      goto_target.cpp \
	      park_telescope.cpp \
	      save_position.cpp \
	      sync_position.cpp \
	      read_encoders.cpp \
	      set_tracking.cpp \
	      handset_command.cpp
MODULES     = $(MODULES_SRC:.cpp=.so)
MAINOBJ     = myTCP.o
TARGET      = lcu

ROOTDIR=..
OBJDIR=$(ROOTDIR)/obj
BINDIR=$(ROOTDIR)/bin
INCDIR=$(ROOTDIR)/include
SRCDIR=$(ROOTDIR)/src
#LIBSRCDIR=$(ROOTDIR)/libsrc
SODIR=.

VPATH=$(OBJDIR):$(INCDIR):$(LIBSRCDIR)

all: $(TARGET) $(OBJECTS) $(MODULES) 

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -rdynamic -ldl $(LIBS) -I$(INCDIR) $(addprefix $(OBJDIR)/, $(OBJECTS)) -o $(TARGET)  main.cpp

#$(MAINOBJ): $(SRCDIR)/myTCP.cpp
#	$(CXX) -c $(CXXFLAGS) -rdynamic -ldl $(LIBS) -I$(INCDIR) -o $(OBJDIR)/$@ $<
 
$(MODULES): \
%.so: $(SRCDIR)/%.cpp  
	$(CXX) $(CXXFLAGS) -fPIC -shared $(LIBS) -I$(INCDIR) $(addprefix $(OBJDIR)/, $(OBJECTS))  -o $(SODIR)/$@  $<

$(OBJECTS): \
%.o: $(SRCDIR)/%.cpp $(INCDIR)/%.h
	$(CXX) -c $(CXXFLAGS) -I$(INCDIR) -o $(OBJDIR)/$@ $<

clean:
	rm -f $(TARGET) $(SODIR)/*.so $(OBJDIR)/*.o

.PHONY: clean
