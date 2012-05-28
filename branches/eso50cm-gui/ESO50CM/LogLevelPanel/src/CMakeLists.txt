project( LogLevelPanel )
cmake_minimum_required( VERSION 2.6 )
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

set ( CMAKE_BUILD_TYPE Release )
add_definitions ( -Wall )
find_package ( Qt4 REQUIRED )
include ( ${QT_USE_FILE} )
include_directories (
	${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
	${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/../include
    ${INTROOT}/include
    $ENV{SWROOT}/include
    ${PROJECT_SOURCE_DIR}/../../Logger/include
	)
LINK_DIRECTORIES(${INTROOT}/lib $ENV{SWROOT}/lib)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../Logger/lib)
set ( LogLevelPanel_HDRS
	
	)

set ( LogLevelPanel_SRCS
	main.cpp
	loglevelpanel.cpp
	)

set ( LogLevelPanel_UIS
	loglevelpanel.ui
	)
QT4_WRAP_UI(UIS ${LogLevelPanel_UIS})

set ( LogLevelPanel_MOCS
	loglevelpanel.h
	)
QT4_WRAP_CPP(MOCS ${LogLevelPanel_MOCS})

add_executable ( LogLevelPanel ${LogLevelPanel_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
target_link_libraries ( LogLevelPanel  ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} LoggerCore Ice)
INSTALL(TARGETS LogLevelPanel
    RUNTIME DESTINATION ${INTROOT}/bin
    LIBRARY DESTINATION ${INTROOT}/lib
    ARCHIVE DESTINATION ${INTROOT}/lib
)
