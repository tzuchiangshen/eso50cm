CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#project name
PROJECT(OBSCONTROL)

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
    OUTPUT ${PROJECT_SOURCE_DIR}/Observing.cpp ${PROJECT_SOURCE_DIR}/Observing.h
    DEPENDS ${PROJECT_SOURCE_DIR}/../idl/Observing.ice 
    #generate cpp proxies files
    COMMAND ${CMAKE_COMMAND} -E echo " Observing.cpp Observing.h generated."
    COMMAND slice2cpp ${PROJECT_SOURCE_DIR}/../idl/Observing.ice $ENV{INTROOT}/idl/LCU.ice -I$ENV{ICE_PATH}
    #generate python proxies files
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_SOURCE_DIR}/generated"
    COMMAND slice2py -I$ENV{INTROOT}/idl  -I${PROJECT_SOURCE_DIR}/../idl --output-dir ${PROJECT_SOURCE_DIR}/generated ${PROJECT_SOURCE_DIR}/../idl/Observing.ice
    COMMAND ${CMAKE_COMMAND} -E echo " ICE python proxy generated under src/generated/ dir."
    COMMENT "generating slice files..." 
)

ADD_LIBRARY(ObservingImpl SHARED Observing TelescopeImpl ObservingImpl)
TARGET_LINK_LIBRARIES(ObservingImpl Ice)
set_property(TARGET ObservingImpl PROPERTY LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../lib)

#for .h file search
INCLUDE_DIRECTORIES(/usr/lib/include/Ice)
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/../include")

#for ../bin/ObsControl executable 
LINK_DIRECTORIES($ENV{INTROOT}/lib)
LINK_DIRECTORIES(/usr/lib)
ADD_EXECUTABLE(ObsControl ObsControl)
TARGET_LINK_LIBRARIES(ObsControl ObservingImpl LCUImpl)
set_property(TARGET ObsControl PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin)


SET_TARGET_PROPERTIES(ObsControl PROPERTIES LINKER_LANGUAGE CXX)

#delete generated files in make clean 
SET_DIRECTORY_PROPERTIES (
   PROPERTIES 
     ADDITIONAL_MAKE_CLEAN_FILES "LCU.h"
     ADDITIONAL_MAKE_CLEAN_FILES "LCU.cpp"
     ADDITIONAL_MAKE_CLEAN_FILES "Observing.h"
     ADDITIONAL_MAKE_CLEAN_FILES "Observing.cpp"
     ADDITIONAL_MAKE_CLEAN_FILES "${PROJECT_SOURCE_DIR}/generated"
)

#for make install
INSTALL(TARGETS ObservingImpl ObsControl
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

#install *.ice files into $INTROOT/idl
INSTALL(
   FILES ${PROJECT_SOURCE_DIR}/../idl/Observing.ice DESTINATION idl
)

#install generated *.h files into $INTROOT/includes
INSTALL(
   FILES ${PROJECT_SOURCE_DIR}/Observing.h DESTINATION include 
)

#install configuration files into $INTROOT/config
INSTALL(
   FILES ${PROJECT_SOURCE_DIR}/../config/config DESTINATION config
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
  FILES ${PROJECT_SOURCE_DIR}/Client.py DESTINATION bin
)

