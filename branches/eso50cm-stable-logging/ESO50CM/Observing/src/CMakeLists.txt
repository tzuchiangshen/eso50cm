CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#project name
PROJECT(OBSCONTROL)

#introot definition
SET(INTROOT $ENV{INTROOT})
IF(NOT INTROOT)
   SET(INTROOT $ENV{SWROOT})
ENDIF(NOT INTROOT)

IF(NOT INTROOT)
    SET(CMAKE_INSTALL_PREFIX="/eso50cm/SWROOT")
    MESSAGE("INTROOT not defined, CMAKE_INSTALL_PREFIX set to:/eso50cm/SWROOT")
ELSE (NOT INTROOT)
    SET(CMAKE_INSTALL_PREFIX ${INTROOT})
    MESSAGE("CMAKE_INSTALL_PREFIX set to: $ENV{INTROOT}")
ENDIF (NOT INTROOT)

#for generated files from slice
ADD_CUSTOM_COMMAND(
    OUTPUT ${PROJECT_SOURCE_DIR}/Observing.cpp ${PROJECT_SOURCE_DIR}/Observing.h ${PROJECT_SOURCE_DIR}/LCU.cpp ${PROJECT_SOURCE_DIR}/LCU.h
    #DEPENDS ${PROJECT_SOURCE_DIR}/../idl/Observing.ice ${INTROOT}/idl/LCU.ice ${PROJECT_SOURCE_DIR}/../../tcs/idl/LCU.ice  
    #generate cpp proxies files
    COMMAND ${CMAKE_COMMAND} -E echo " Observing.cpp Observing.h generated."
    COMMAND slice2cpp -I${INTROOT}/idl -I${PROJECT_SOURCE_DIR}/../../tcs/idl ${PROJECT_SOURCE_DIR}/../idl/Observing.ice 
    #generate python proxies files
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_SOURCE_DIR}/generated"
    COMMAND slice2py -I${INTROOT}/idl  -I${PROJECT_SOURCE_DIR}/../../tcs/idl --output-dir ${PROJECT_SOURCE_DIR}/generated ${PROJECT_SOURCE_DIR}/../idl/Observing.ice   
    COMMAND ${CMAKE_COMMAND} -E echo " ICE python proxy generated under src/generated/ dir."
    COMMENT "generating slice files..." 
)

#for .h file search
INCLUDE_DIRECTORIES(/usr/include/Ice)
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/../include")
INCLUDE_DIRECTORIES("${INTROOT}/include")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/../../tcs/include")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/../../Logger/include")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/../../tcs/src")   #for generated .h

LINK_DIRECTORIES(${INTROOT}/lib)
LINK_DIRECTORIES("${PROJECT_SOURCE_DIR}/../../Logger/lib")
LINK_DIRECTORIES("${PROJECT_SOURCE_DIR}/../../tcs/lib")
ADD_LIBRARY(ObservingImpl SHARED Observing TelescopeImpl ObservingImpl)
TARGET_LINK_LIBRARIES(ObservingImpl LoggerCore LCUImpl Ice IceUtil)
set_property(TARGET ObservingImpl PROPERTY LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../lib)


#for ../bin/ObsControl executable 
LINK_DIRECTORIES(${INTROOT}/lib)
LINK_DIRECTORIES(/usr/lib)
ADD_EXECUTABLE(ObsControl ObsControl)
TARGET_LINK_LIBRARIES(ObsControl ObservingImpl) 
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
   FILES ${PROJECT_SOURCE_DIR}/../config/Obs-config DESTINATION config
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
  FILES ${PROJECT_SOURCE_DIR}/Client-obs.py DESTINATION bin
)
INSTALL(
  FILES ${PROJECT_SOURCE_DIR}/eso50cm.py 
  DESTINATION bin
  PERMISSIONS OWNER_EXECUTE OWNER_READ 
              GROUP_EXECUTE GROUP_READ 
              WORLD_EXECUTE WORLD_READ 
  RENAME eso50cm)
