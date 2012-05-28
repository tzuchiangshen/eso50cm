#project name for hanset 
PROJECT(HSSGUI)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(CMAKE_CXX_FLAGS "-lpthread -fpermissive")

#introot definition
SET(SWROOT $ENV{SWROOT})
IF(NOT SWROOT)
    SET(CMAKE_INSTALL_PREFIX="/eso50cm/WSSW")
    MESSAGE("SWROOT not defined, CMAKE_INSTALL_PREFIX set to: /eso50cm/WSSW")
ELSE (NOT SWROOT)
    SET(CMAKE_INSTALL_PREFIX ${SWROOT})
    MESSAGE("CMAKE_INSTALL_PREFIX set to: $ENV{SWROOT}")
ENDIF (NOT SWROOT)

#for generated files from slice
#ADD_CUSTOM_COMMAND(
#    OUTPUT ${PROJECT_SOURCE_DIR}/Observing.cpp ${PROJECT_SOURCE_DIR}/Observing.h ${PROJECT_SOURCE_DIR}/LCU.cpp ${PROJECT_SOURCE_DIR}/LCU.h
#    DEPENDS ${SWROOT}/idl/Observing.ice ${SWROOT}/idl/LCU.ice
#    #generate cpp proxies files
#    COMMAND ${CMAKE_COMMAND} -E echo " Observing.cpp Observing.h LCU.cpp LCU.h were generated."
#    COMMAND slice2cpp -I${SWROOT}/idl ${SWROOT}/idl/Observing.ice ${SWROOT}/idl/LCU.ice 
#)

# for more cmake QT information see http://qtnode.net/wiki/Qt4_with_cmake
# qt4 definition 
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES( 
    ${PROJECT_SOURCE_DIR} 
    ${PROJECT_SOURCE_DIR}/../include 
    ${SWROOT}/include 
	/usr/include/Ice
	${PROJECT_SOURCE_DIR}/../../Logger/include
    ${PROJECT_SOURCE_DIR}/../../tcs/include 
    #ICE generated file are at src dir: i.e LCU.h
    ${PROJECT_SOURCE_DIR}/../../tcs/src
    ${PROJECT_SOURCE_DIR}/../../Observing/include 
    #ICE generated file are at src dir: i.e Observing.h
    ${PROJECT_SOURCE_DIR}/../../Observing/src
    ${QT_INCLUDE_DIR}
	${QT_QTNETWORK_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
)

#define the sources files 
SET(GUI_SRC
	hsGuiController.cpp
	hscli.cpp
	main.cpp
)

#generate source files from .ui file and append them to the sources list
SET(GUI_UIS hscli.ui)
QT4_WRAP_UI(GUI_SRC ${GUI_UIS})

#generate moc files for QObject classes and append them to the sources list
SET(GUI_MOC_SRC
    ${PROJECT_SOURCE_DIR}/../include/hsGuiController.h
    ${PROJECT_SOURCE_DIR}/../include/hscli.h
) 
QT4_WRAP_CPP(GUI_SRC ${GUI_MOC_SRC})

# create qtcli executable 
LINK_DIRECTORIES(${SWROOT}/lib)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../tcs/lib)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../Observing/lib)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../Logger/lib)
ADD_EXECUTABLE(qths ${GUI_SRC})
set_property(TARGET qths PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin)

# link qt libraries
TARGET_LINK_LIBRARIES(qths  
	Ice
    IceUtil
    LCUImpl
    ObservingImpl
    LoggerCore
	${QT_LIBRARIES} 
	${QT_QTNETWORK_LIBRARIES}
)

#delete generated files in make clean 
#SET_DIRECTORY_PROPERTIES (
#   PROPERTIES 
#   ADDITIONAL_MAKE_CLEAN_FILES "LCU.h"
#   ADDITIONAL_MAKE_CLEAN_FILES "LCU.cpp"
#   ADDITIONAL_MAKE_CLEAN_FILES "Observing.h"
#   ADDITIONAL_MAKE_CLEAN_FILES "Observing.cpp"
#)

#install 

INSTALL(TARGETS qths
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

#install *.h
INSTALL(
   DIRECTORY ${PROJECT_SOURCE_DIR}/../include/ DESTINATION include
   FILES_MATCHING PATTERN "*.h"
)


