CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#project name
PROJECT(LogPanel)

SET(CMAKE_Java_COMPILER /usr/bin/javac)

#introot definition
SET(INTROOT $ENV{INTROOT})
IF(NOT INTROOT)
    SET(INTROOT $ENV{SWROOT})
    SET(SWROOT $ENV{SWROOT})
    IF(NOT SWROOT)
        SET(CMAKE_INSTALL_PREFIX="/tmp/LogPanel")
        MESSAGE("Nor INTROOT neither SWROOT defined, CMAKE_INSTALL_PREFIX set to: /tmp/LogPanel")
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
#for .h file search
INCLUDE_DIRECTORIES(/usr/lib/include)
INCLUDE_DIRECTORIES(/usr/share/qt4/mkspecs/linux-g++)
INCLUDE_DIRECTORIES(/usr/include/qt4/QtCore)
INCLUDE_DIRECTORIES(/usr/include/qt4/QtGui)
INCLUDE_DIRECTORIES(/usr/include/qt4)
INCLUDE_DIRECTORIES(/eso50cm/SWROOT/include)
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/../include")
INCLUDE_DIRECTORIES("${INTROOT}/include")
INCLUDE_DIRECTORIES("$ENV{SWROOT}/include")
LINK_DIRECTORIES(${INTROOT}/lib)
LINK_DIRECTORIES($ENV{SWROOT}/lib)
LINK_DIRECTORIES(/usr/lib)
LINK_DIRECTORIES(/usr/local/lib)
#for generated files from slice
IF(NOT ARM)
    ADD_CUSTOM_COMMAND(
        OUTPUT ${PROJECT_SOURCE_DIR}/moc_mainwindow.cpp  ${PROJECT_SOURCE_DIR}/ui_mainwindow.h 
        #generate cpp moc and ui files
        COMMAND /usr/bin/moc-qt4 -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4 -I. -I. mainwindow.h -o moc_mainwindow.cpp
        COMMAND /usr/bin/uic-qt4 mainwindow.ui -o ui_mainwindow.h
        COMMAND ${CMAKE_COMMAND} -E echo " moc_mainwindow.cpp and  ui_mainwindow.h generated."
    )
ENDIF(NOT ARM)

IF(NOT ARM)
    ADD_EXECUTABLE(LogPanel main mainwindow moc_mainwindow)
    TARGET_LINK_LIBRARIES(LogPanel LoggerCore Ice IceStorm IceUtil QtGui QtCore pthread)
    set_property(TARGET LogPanel PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin)
ENDIF(NOT ARM)

INSTALL(FILES  ${PROJECT_SOURCE_DIR}/../bin/LogPanel   ${PROJECT_SOURCE_DIR}/../src/logPanel
        DESTINATION ${INTROOT}/bin
        PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
