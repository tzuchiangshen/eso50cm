
CLIENT		= client
SERVER		= LCUControl

TARGETS		= $(CLIENT) $(SERVER)

OBJS		= LCU.o

COBJS		= Client.o 
		  


SOBJS		= LCUImpl.o \
		  LCUControl.o \
		  myLCU.o \
		  myTelescope.o \
		  myTClock.o \
		  myTAxis.o \
		  myTMotor.o \
		  myTEncoder.o \
		  myBSemaphore.o \
		  myUtil.o
	

SRCS		= $(OBJS:.o=.cpp) \
		  $(COBJS:.o=.cpp) \
		  $(SOBJS:.o=.cpp)

SLICE_SRCS	= LCU.ice

#include $(top_srcdir)/config/Make.rules

# CPPFLAGS	:= -I. $(CPPFLAGS)

CXX = /usr/bin/c++
CPPFLAGS        := -I../include -I. -I/usr/include
LDFLAGS         := -L/usr/lib -lIceE

$(CLIENT): $(OBJS) $(COBJS)
	rm -f $@
	$(CXX) $(LDFLAGS) -o $@ $(COBJS) $(OBJS)

$(SERVER): $(OBJS) $(SOBJS)
	rm -f $@
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(SOBJS) $(LIBS)

Client.o: Client.cpp
	$(CXX) -c -DICEE_PURE_CLIENT $(CPPFLAGS) $(CXXFLAGS) $<
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

HelloC.o LCU.o: LCU.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<
	$(CXX) -c -o $(CPPFLAGS) $(CXXFLAGS) $<
	$(CXX) -c -DICEE_PURE_CLIENT -o HelloC.o $(CPPFLAGS) $(CXXFLAGS) $<



