CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#project name
PROJECT(LCUCONTROL)

#introot definition
SET(INTROOT $ENV{INTROOT})
IF(NOT INTROOT)
    SET(CMAKE_INSTALL_PREFIX="/opt/LCUCONTROL")
    MESSAGE("INTROOT not defined, CMAKE_INSTALL_PREFIX set to: /opt/LCUCONTROL")
ELSE (NOT INTROOT)
    SET(CMAKE_INSTALL_PREFIX ${INTROOT})
    MESSAGE("CMAKE_INSTALL_PREFIX set to: $ENV{INTROOT}")
ENDIF (NOT INTROOT)

#for generated files from slice
ADD_CUSTOM_COMMAND(
    OUTPUT ${PROJECT_SOURCE_DIR}/LCU.cpp ${PROJECT_SOURCE_DIR}/LCU.h
    DEPENDS ${PROJECT_SOURCE_DIR}/../idl/LCU.ice 
    #generate cpp proxies files
    COMMAND ${CMAKE_COMMAND} -E echo " LCU.cpp LCU.h generated."
    COMMAND slice2cppe ${PROJECT_SOURCE_DIR}/../idl/LCU.ice
    #generate python proxies files
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_SOURCE_DIR}/generated"
    COMMAND slice2py -I$ENV{INTROOT}/idl  -I${PROJECT_SOURCE_DIR}/../../tcs/idl --output-dir ${PROJECT_SOURCE_DIR}/generated ${PROJECT_SOURCE_DIR}/../idl/LCU.ice
    COMMAND ${CMAKE_COMMAND} -E echo " ICE python proxy generated under src/generated/ dir."
    COMMENT "generating slice files..." 
)

ADD_LIBRARY(myLCU SHARED myLCU myTelescope myTClock myTAxis myTMotor myTEncoder myBSemaphore myUtil)
set_property(TARGET myLCU PROPERTY LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../lib)

ADD_LIBRARY(LCUImpl SHARED LCU LCUImpl LCUGetImpl LCUSetImpl)
TARGET_LINK_LIBRARIES(LCUImpl myLCU IceE IceUtil)
set_property(TARGET LCUImpl PROPERTY LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../lib)

#for .h file search
INCLUDE_DIRECTORIES(/usr/lib/include/IceE)
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/../include")
INCLUDE_DIRECTORIES("$ENV{INTROOT}/include")

#for ../bin/LCUControl executable 
LINK_DIRECTORIES($ENV{INTROOT}/lib)
LINK_DIRECTORIES(/usr/lib)
ADD_EXECUTABLE(LCUControl LCUControl)
TARGET_LINK_LIBRARIES(LCUControl myLCU LCUImpl IceE)
set_property(TARGET LCUControl PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin)

ADD_EXECUTABLE(ClientCXX Client)
TARGET_LINK_LIBRARIES(ClientCXX LCUImpl IceE)
set_property(TARGET ClientCXX PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin)

SET_TARGET_PROPERTIES(LCUControl PROPERTIES LINKER_LANGUAGE CXX)

#delete generated files in make clean 
SET_DIRECTORY_PROPERTIES (
   PROPERTIES 
     ADDITIONAL_MAKE_CLEAN_FILES "LCU.h"
     ADDITIONAL_MAKE_CLEAN_FILES "LCU.cpp"
     ADDITIONAL_MAKE_CLEAN_FILES "${PROJECT_SOURCE_DIR}/generated"
)

#for make install
INSTALL(TARGETS myLCU LCUImpl LCUControl ClientCXX
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

#install *.ice files into $INTROOT/idl
INSTALL(
   FILES ${PROJECT_SOURCE_DIR}/../idl/LCU.ice DESTINATION idl
)

#install generated *.h files into $INTROOT/includes
INSTALL(
   FILES ${PROJECT_SOURCE_DIR}/LCU.h DESTINATION include 
)

#install .h 
INSTALL(
   DIRECTORY ${PROJECT_SOURCE_DIR}/../include/ DESTINATION include 
   FILES_MATCHING PATTERN "*.h"
)

#install python libraries
INSTALL(
   DIRECTORY "${PROJECT_SOURCE_DIR}/generated/" DESTINATION lib/python/site-packages 
   FILES_MATCHING PATTERN "*.py"
)
INSTALL(
   DIRECTORY ${PROJECT_SOURCE_DIR}/generated/OUC DESTINATION lib/python/site-packages
)
INSTALL(
   FILES ${PROJECT_SOURCE_DIR}/AstroUtil.py DESTINATION lib/python/site-packages
)
INSTALL(
   FILES ${PROJECT_SOURCE_DIR}/Client.py DESTINATION bin
)
