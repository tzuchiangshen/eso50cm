#project name
PROJECT(TCSGUI)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#introot definition
SET(INTROOT $ENV{INTROOT})
IF(NOT INTROOT)
    SET(CMAKE_INSTALL_PREFIX="/eso50cm/WSSW")
    MESSAGE("INTROOT not defined, CMAKE_INSTALL_PREFIX set to: /eso50cm/WSSW")
ELSE (NOT INTROOT)
    SET(CMAKE_INSTALL_PREFIX ${INTROOT})
    MESSAGE("CMAKE_INSTALL_PREFIX set to: $ENV{INTROOT}")
ENDIF (NOT INTROOT)

# qt4 definition 
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES( 
	/usr/include/Ice
	${QT_INCLUDE_DIR}
	${QT_QTNETWORK_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR} 
	${CMAKE_CURRENT_BINARY_DIR}
)

#define the sources files 
SET(TCSGUI_SRC
	cppContainerServices.cpp
	Observing.cpp
	telescopecli.cpp
	main.cpp
	LCU.cpp
	myRS232.cpp
)

#generate source files from .ui file and append them to the sources list
SET(TCSGUI_UIS telescopecli.ui)
QT4_WRAP_UI(TCSGUI_SRC ${TCSGUI_UIS})

#generate moc files for QObject classes and append them to the sources list
SET(TCSGUI_MOC_SRC
	cppContainerServices.h
	telescopecli.h
) 
QT4_WRAP_CPP(TCSGUI_SRC ${TCSGUI_MOC_SRC})

# create qtcli executable 
ADD_EXECUTABLE(qtcli ${TCSGUI_SRC})
set_property(TARGET qtcli PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin)

# link qt libraries
TARGET_LINK_LIBRARIES(qtcli  
	Ice
	${QT_LIBRARIES} 
	${QT_QTNETWORK_LIBRARIES}
)

#install ../lib/WeatherControl.jar
INSTALL(
   FILES ${PROJECT_SOURCE_DIR}/../bin/qtcli DESTINATION bin 
)


