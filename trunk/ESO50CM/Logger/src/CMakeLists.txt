CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#project name
PROJECT(Logger)

SET(CMAKE_Java_COMPILER /usr/bin/javac)

#introot definition
SET(INTROOT $ENV{INTROOT})
IF(NOT INTROOT)
    SET(INTROOT $ENV{SWROOT})
    SET(SWROOT $ENV{SWROOT})
    IF(NOT SWROOT)
        SET(CMAKE_INSTALL_PREFIX="/tmp/Logger")
        MESSAGE("Nor INTROOT neither SWROOT defined, CMAKE_INSTALL_PREFIX set to: /tmp/Logger")
    ELSE(NOT SWROOT) 
        SET(CMAKE_INSTALL_PREFIX ${SWROOT})
        MESSAGE("No INTROOT defined. Using SWROOT, CMAKE_INSTALL_PREFIX set to: $ENV{SWROOT}")
    ENDIF(NOT SWROOT)
ELSE(NOT INTROOT)
    SET(CMAKE_INSTALL_PREFIX ${INTROOT})
    MESSAGE("CMAKE_INSTALL_PREFIX set to: $ENV{INTROOT}")
ENDIF(NOT INTROOT)

SET(ARM $ENV{TYPE})

#for generated files from slice
IF(NOT ARM)
    ADD_CUSTOM_COMMAND(
        OUTPUT ${PROJECT_SOURCE_DIR}/LogMessage.cpp ${PROJECT_SOURCE_DIR}/LogMessage.h
        #generate cpp proxies files
        COMMAND slice2cpp --output-dir ${PROJECT_SOURCE_DIR} -I ../idl ${PROJECT_SOURCE_DIR}/../idl/LogMessage.ice
        COMMAND mv ${PROJECT_SOURCE_DIR}/LogMessage.h ${PROJECT_SOURCE_DIR}/../include
        COMMAND ${CMAKE_COMMAND} -E echo " LogMessage.cpp LogMessage.h generated."
    )
    ADD_CUSTOM_COMMAND(
        OUTPUT ${PROJECT_SOURCE_DIR}/LogPublisher.cpp ${PROJECT_SOURCE_DIR}/LogPublisher.h    
        #generate cpp proxies files
        COMMAND slice2cpp --output-dir ${PROJECT_SOURCE_DIR} -I ../idl -I${INTROOT}/idl ${PROJECT_SOURCE_DIR}/../idl/LogPublisher.ice
        COMMAND mv ${PROJECT_SOURCE_DIR}/LogPublisher.h ${PROJECT_SOURCE_DIR}/../include
        COMMAND ${CMAKE_COMMAND} -E echo " LogPublisher.cpp LogPublisher.h generated."
    )
    ADD_CUSTOM_COMMAND(
        OUTPUT ${PROJECT_SOURCE_DIR}/Logger.cpp ${PROJECT_SOURCE_DIR}/Logger.h    
        #generate cpp proxies files
        COMMAND slice2cpp --output-dir ${PROJECT_SOURCE_DIR} -I ../idl ${PROJECT_SOURCE_DIR}/../idl/Logger.ice
        COMMAND mv ${PROJECT_SOURCE_DIR}/Logger.h ${PROJECT_SOURCE_DIR}/../include
        COMMAND ${CMAKE_COMMAND} -E echo " Logger.cpp Logger.h generated."
    )
ELSE(NOT ARM)
    ADD_CUSTOM_COMMAND(
        OUTPUT ${PROJECT_SOURCE_DIR}/Logger.cpp ${PROJECT_SOURCE_DIR}/Logger.h    
        #generate cpp proxies files
        COMMAND slice2cppe --output-dir ${PROJECT_SOURCE_DIR} -I ../idl ${PROJECT_SOURCE_DIR}/../idl/Logger.ice
        COMMAND mv ${PROJECT_SOURCE_DIR}/Logger.h ${PROJECT_SOURCE_DIR}/../include
        COMMAND ${CMAKE_COMMAND} -E echo " Logger.cpp Logger.h generated."
    )
    ADD_CUSTOM_COMMAND(
        OUTPUT ${PROJECT_SOURCE_DIR}/LogPublisher.cpp ${PROJECT_SOURCE_DIR}/LogPublisher.h    
        #generate cpp proxies files
        COMMAND slice2cppe --output-dir ${PROJECT_SOURCE_DIR} -I ../idl -I${INTROOT}/idl ${PROJECT_SOURCE_DIR}/../idl/LogPublisher.ice
        COMMAND mv ${PROJECT_SOURCE_DIR}/LogPublisher.h ${PROJECT_SOURCE_DIR}/../include
        COMMAND ${CMAKE_COMMAND} -E echo " LogPublisher.cpp LogPublisher.h generated."
    )
    ADD_CUSTOM_COMMAND(
        OUTPUT ${PROJECT_SOURCE_DIR}/LogMessage.cpp ${PROJECT_SOURCE_DIR}/LogMessage.h
        #generate cpp proxies files
        COMMAND slice2cppe --output-dir ${PROJECT_SOURCE_DIR} -I ../idl ${PROJECT_SOURCE_DIR}/../idl/LogMessage.ice
        COMMAND mv ${PROJECT_SOURCE_DIR}/LogMessage.h ${PROJECT_SOURCE_DIR}/../include
        COMMAND ${CMAKE_COMMAND} -E echo " LogMessage.cpp LogMessage.h generated."
    )
ENDIF(NOT ARM)

IF(ARM)
    ADD_DEFINITIONS(-DARM -DICEE_PURE_CLIENT)
ENDIF(ARM)

ADD_LIBRARY(LoggerCore SHARED LoggerHelper LogPublisher Logger LogMessage)
set_property(TARGET LoggerCore PROPERTY LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../lib)
#for .h file search
INCLUDE_DIRECTORIES(/usr/lib/include)
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/../include")
INCLUDE_DIRECTORIES("${INTROOT}/include")
INCLUDE_DIRECTORIES("$ENV{SWROOT}/include")
LINK_DIRECTORIES(${INTROOT}/lib)
LINK_DIRECTORIES($ENV{SWROOT}/lib)
FIND_LIBRARY(MYSQL_LIB NAMES mysqlcppconn )
IF(EXISTS ${MYSQL_LIB})
    MESSAGE("mysql found, using mysql libraries ")
    ADD_DEFINITIONS(-DMYSQL_LIB)
    ADD_LIBRARY(DBStuff dbstuff)
    set_property(TARGET DBStuff PROPERTY LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../lib)
ENDIF(EXISTS ${MYSQL_LIB})

IF(NOT ARM)
    IF(EXISTS ${MYSQL_LIB})
        ADD_EXECUTABLE(loggingService LoggingService LoggerI dbstuff)
        TARGET_LINK_LIBRARIES(loggingService LoggerCore Ice IceStorm IceUtil mysqlcppconn)
    ELSE(EXISTS ${MYSQL_LIB})
        ADD_EXECUTABLE(loggingService LoggingService LoggerDevelI)
        TARGET_LINK_LIBRARIES(loggingService LoggerCore Ice IceStorm IceUtil)
    ENDIF(EXISTS ${MYSQL_LIB})
    ADD_EXECUTABLE(simpleClient SimpleClientI)
    TARGET_LINK_LIBRARIES(simpleClient LoggerCore IceUtil Ice IceStorm)
    set_property(TARGET loggingService PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin)
ENDIF(NOT ARM)

#delete generated files in make clean 
SET_DIRECTORY_PROPERTIES (
   PROPERTIES 
     ADDITIONAL_MAKE_CLEAN_FILES "LogMessage.h"
     ADDITIONAL_MAKE_CLEAN_FILES "LogMessage.cpp"
     ADDITIONAL_MAKE_CLEAN_FILES "Logger.h"
     ADDITIONAL_MAKE_CLEAN_FILES "Logger.cpp"
     ADDITIONAL_MAKE_CLEAN_FILES "LogPublisher.h"
     ADDITIONAL_MAKE_CLEAN_FILES "LogPublisher.cpp"
)

#IF(EXISTS ${MYSQL_LIB})
#    ADD_CUSTOM_COMMAND(
#        OUTPUT ${PROJECT_SOURCE_DIR}/../config/loggingService.config   
#        #generate create loggingService.config as a copy of the lcu or local version of the file
#        COMMAND cp  ${PROJECT_SOURCE_DIR}/../config/loggingService.config.local   ${PROJECT_SOURCE_DIR}/../config/loggingService.config 
#        )
#ELSE(EXISTS ${MYSQL_LIB})
#    ADD_CUSTOM_COMMAND(
#        OUTPUT ${PROJECT_SOURCE_DIR}/../config/loggingService.config   
#        #generate create loggingService.config as a copy of the lcu or local version of the file
#        COMMAND cp  ${PROJECT_SOURCE_DIR}/../config/loggingService.config.lcu   ${PROJECT_SOURCE_DIR}/../config/loggingService.config 
#       )
#ENDIF(EXISTS ${MYSQL_LIB})


#for make install
IF(NOT ARM)
    INSTALL(TARGETS LoggerCore loggingService simpleClient
    RUNTIME DESTINATION ${INTROOT}/bin
    LIBRARY DESTINATION ${INTROOT}/lib
    ARCHIVE DESTINATION ${INTROOT}/lib)
ELSE(NOT ARM)
    INSTALL(TARGETS LoggerCore
    RUNTIME DESTINATION ${INTROOT}/bin
    LIBRARY DESTINATION ${INTROOT}/lib
    ARCHIVE DESTINATION ${INTROOT}/lib)
ENDIF(NOT ARM)

IF(EXISTS ${MYSQL_LIB})
#for make install
    INSTALL(TARGETS DBStuff
    RUNTIME DESTINATION ${INTROOT}/bin
    LIBRARY DESTINATION ${INTROOT}/lib
    ARCHIVE DESTINATION ${INTROOT}/lib
)
ENDIF(EXISTS ${MYSQL_LIB})



INSTALL(
   DIRECTORY ${PROJECT_SOURCE_DIR}/../idl/ DESTINATION ${INTROOT}/idl 
   FILES_MATCHING PATTERN "*.ice"
)

INSTALL(
   DIRECTORY ${PROJECT_SOURCE_DIR}/../include/ DESTINATION ${INTROOT}/include 
   FILES_MATCHING PATTERN "*.h"
)
INSTALL(
   DIRECTORY ${PROJECT_SOURCE_DIR}/../config/ DESTINATION ${INTROOT}/config
   FILES_MATCHING PATTERN "*.config"
)

INSTALL(FILES  ${PROJECT_SOURCE_DIR}/runLoggingService ${PROJECT_SOURCE_DIR}/runSimpleClient
        DESTINATION ${INTROOT}/bin
        PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)


