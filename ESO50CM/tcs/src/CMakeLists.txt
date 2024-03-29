CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#project name
PROJECT(LCUCONTROL)
SET(CMAKE_CXX_FLAGS "-lpthread -fpermissive")

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
SET(LCU $ENV{TYPE})
IF(NOT LCU)
ADD_CUSTOM_COMMAND(
    OUTPUT ${PROJECT_SOURCE_DIR}/LCU.cpp ${PROJECT_SOURCE_DIR}/LCU.h
    DEPENDS ${PROJECT_SOURCE_DIR}/../idl/LCU.ice 
    #generate cpp proxies files
    COMMAND ${CMAKE_COMMAND} -E echo " LCU.cpp LCU.h generated."
    COMMAND slice2cpp ${PROJECT_SOURCE_DIR}/../idl/LCU.ice
    #generate python proxies files
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_SOURCE_DIR}/generated"
    COMMAND slice2py -I${INTROOT}/idl  -I${PROJECT_SOURCE_DIR}/../../tcs/idl --output-dir ${PROJECT_SOURCE_DIR}/generated ${PROJECT_SOURCE_DIR}/../idl/LCU.ice
    COMMAND ${CMAKE_COMMAND} -E echo " ICE python proxy generated under src/generated/ dir."
    COMMENT "generating slice files..." 
)
ELSE(NOT LCU)
ADD_CUSTOM_COMMAND(
    OUTPUT ${PROJECT_SOURCE_DIR}/LCU.cpp ${PROJECT_SOURCE_DIR}/LCU.h
    DEPENDS ${PROJECT_SOURCE_DIR}/../idl/LCU.ice 
    #generate cpp proxies files
    COMMAND ${CMAKE_COMMAND} -E echo " LCU.cpp LCU.h generated."
    COMMAND slice2cppe ${PROJECT_SOURCE_DIR}/../idl/LCU.ice
    COMMENT "generating slice files..." 
)
ENDIF(NOT LCU)

IF(LCU)
    ADD_DEFINITIONS(-DARM)
ENDIF(LCU)

ADD_LIBRARY(myLCU SHARED myLCU myTelescope myTClock myTAxis myTMotor myTEncoder myBSemaphore myUtil)
set_property(TARGET myLCU PROPERTY LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../lib)

LINK_DIRECTORIES(${INTROOT}/lib)
IF(NOT LCU)
   ADD_LIBRARY(LCUImpl SHARED LCU LCUImpl LCUGetImpl LCUSetImpl)
   TARGET_LINK_LIBRARIES(LCUImpl myLCU LoggerCore Ice IceUtil)
ELSE(NOT LCU)
   ADD_LIBRARY(LCUImpl SHARED LCU LCUImpl LCUGetImpl LCUSetImpl)
   TARGET_LINK_LIBRARIES(LCUImpl myLCU LoggerCore IceE)
ENDIF(NOT LCU)
set_property(TARGET LCUImpl PROPERTY LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../lib)

#for .h file search
IF(NOT LCU)
   INCLUDE_DIRECTORIES(/usr/include/Ice)
ELSE(NOT LCU)
   INCLUDE_DIRECTORIES(/usr/include/IceE)
ENDIF(NOT LCU)
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/../include")
INCLUDE_DIRECTORIES("${INTROOT}/include")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/../../Logger/include")

#for ../bin/LCUControl executable 
LINK_DIRECTORIES(${INTROOT}/lib)
IF(NOT LCU)
   ADD_EXECUTABLE(LCUControl LCUControl)
   TARGET_LINK_LIBRARIES(LCUControl myLCU LCUImpl LoggerCore Ice IceUtil)
ELSE(NOT LCU)
   ADD_EXECUTABLE(LCUControl LCUControl)
   TARGET_LINK_LIBRARIES(LCUControl myLCU LCUImpl LoggerCore IceE)
ENDIF(NOT LCU)
set_property(TARGET LCUControl PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin)

#for CXX client of LCUControl
LINK_DIRECTORIES(${INTROOT}/lib)
IF(NOT LCU)
   ADD_EXECUTABLE(ClientCXX Client)
   TARGET_LINK_LIBRARIES(ClientCXX LCUImpl LoggerCore Ice IceUtil)
ELSE(NOT LCU)
   ADD_EXECUTABLE(ClientCXX Client)
   find_package (Threads)
   TARGET_LINK_LIBRARIES(ClientCXX LCUImpl LoggerCore IceE ${CMAKE_THREAD_LIBS_INIT})
ENDIF(NOT LCU)
set_property(TARGET ClientCXX PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin)

#for shm_dbg executable
ADD_EXECUTABLE(shm_dbg semaphore shm_dbg)
set_property(TARGET shm_dbg PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin)

#for telescope61 executable
LINK_DIRECTORIES(${INTROOT}/lib)
ADD_EXECUTABLE(telescope61 telescope61 common rs232 semaphore main)
set_property(TARGET telescope61 PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin)

#for telescope61_sim executable
ADD_EXECUTABLE(telescope61_sim telescope61_sim common rs232 semaphore main_sim)
find_package (Threads)
MESSAGE("CMAKE_THREAD_LIBS_INIT= ${CMAKE_THREAD_LIBS_INIT}")
target_link_libraries (telescope61_sim ${CMAKE_THREAD_LIBS_INIT})
#set_target_properties(telescope61_sim PROPERTIES COMPILE_FLAGS "-lpthread")
set_property(TARGET telescope61_sim PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin)


SET_TARGET_PROPERTIES(LCUControl PROPERTIES LINKER_LANGUAGE CXX)

#delete generated files in make clean 
SET_DIRECTORY_PROPERTIES (
   PROPERTIES 
     ADDITIONAL_MAKE_CLEAN_FILES "LCU.h"
     ADDITIONAL_MAKE_CLEAN_FILES "LCU.cpp"
     ADDITIONAL_MAKE_CLEAN_FILES "${PROJECT_SOURCE_DIR}/generated"
)

#for make install
INSTALL(TARGETS myLCU LCUImpl LCUControl ClientCXX telescope61 telescope61_sim
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

#install configuration files into $INTROOT/config
INSTALL(
   FILES ${PROJECT_SOURCE_DIR}/../config/LCU-config 
         ${PROJECT_SOURCE_DIR}/../config/ESO50cm.conf DESTINATION config 
)

#install .h 
INSTALL(
   DIRECTORY ${PROJECT_SOURCE_DIR}/../include/ DESTINATION include 
   FILES_MATCHING PATTERN "*.h"
)

#install python libraries
IF(NOT LCU)
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
      FILES ${PROJECT_SOURCE_DIR}/DeviceManager.py DESTINATION lib/python/site-packages
   )
   INSTALL(
      FILES ${PROJECT_SOURCE_DIR}/Client-lcu.py DESTINATION bin
   )
   INSTALL(
      FILES ${PROJECT_SOURCE_DIR}/get_position.py DESTINATION bin
      PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE
   )
   INSTALL(
      FILES ${PROJECT_SOURCE_DIR}/offset_telescope.py DESTINATION bin
      PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE
   )
   INSTALL(
      FILES ${PROJECT_SOURCE_DIR}/read_encoders.py DESTINATION bin
      PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE
   )
   INSTALL(
      FILES ${PROJECT_SOURCE_DIR}/read_motors_encoders.py DESTINATION bin
      PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE
   )
   INSTALL(
      FILES ${PROJECT_SOURCE_DIR}/set_tracking.py DESTINATION bin
      PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE
   )
   INSTALL(
      FILES ${PROJECT_SOURCE_DIR}/stop_telescope.py DESTINATION bin
      PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE
   )
   INSTALL(
      FILES ${PROJECT_SOURCE_DIR}/Client-position.py DESTINATION bin
   )
ENDIF(NOT LCU)
