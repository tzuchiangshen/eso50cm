CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#project name
PROJECT(ESO50CM)

#introot definition
SET(SWROOT $ENV{SWROOT})
IF(NOT SWROOT)
    SET(CMAKE_INSTALL_PREFIX="/eso50cm/SWROOT")
    MESSAGE("SWROOT not defined, CMAKE_INSTALL_PREFIX set to: /eso50cm/SWROOT")
ELSE (NOT SWROOT)
    SET(CMAKE_INSTALL_PREFIX ${SWROOT})
    MESSAGE("CMAKE_INSTALL_PREFIX set to: $ENV{SWROOT}")
ENDIF (NOT SWROOT)

SET(LCU $ENV{TYPE})
IF(NOT LCU)
# console
ADD_CUSTOM_TARGET(SWROOT ALL
    COMMAND cmake -E echo "Creating SWROOT directory structure ..."
    COMMAND cmake -E make_directory ${SWROOT}
    COMMAND cmake -E echo "${SWROOT} created"
    COMMAND cmake -E make_directory ${SWROOT}/lib
    COMMAND cmake -E echo "${SWROOT}/lib created"
    COMMAND cmake -E make_directory ${SWROOT}/bin
    COMMAND cmake -E echo "${SWROOT}/bin created"
    COMMAND cmake -E make_directory ${SWROOT}/idl
    COMMAND cmake -E echo "${SWROOT}/idl created"
    COMMAND cmake -E make_directory ${SWROOT}/config
    COMMAND cmake -E echo "${SWROOT}/config created"
    COMMAND cmake -E make_directory ${SWROOT}/db
    COMMAND cmake -E echo "${SWROOT}/db created"
    COMMAND cmake -E make_directory ${SWROOT}/db/registry
    COMMAND cmake -E echo "${SWROOT}/db/registry created"
    COMMAND cmake -E make_directory ${SWROOT}/db/nodes
    COMMAND cmake -E echo "${SWROOT}/db/nodes created"
    COMMAND cmake -E make_directory ${SWROOT}/logs
    COMMAND cmake -E echo "${SWROOT}/logs created"
    COMMAND cmake -E make_directory ${SWROOT}/include
    COMMAND cmake -E echo "${SWROOT}/include created"
    COMMAND cmake -E make_directory ${SWROOT}/Sources
    COMMAND cmake -E echo "${SWROOT}/Sources created")
#install modules
ADD_SUBDIRECTORY(Logger/src)
ADD_SUBDIRECTORY(LogPanel/src)
ADD_SUBDIRECTORY(tcs/src)
ADD_SUBDIRECTORY(Observing/src)
ADD_SUBDIRECTORY(TcsGui/src)
ADD_SUBDIRECTORY(HSGui/src)

ELSE(NOT LCU)
# LCU
ADD_CUSTOM_TARGET(SWROOT ALL
    COMMAND cmake -E echo "Creating SWROOT directory structure ..."
    COMMAND cmake -E make_directory ${SWROOT}
    COMMAND cmake -E echo "${SWROOT} created"
    COMMAND cmake -E make_directory ${SWROOT}/lib
    COMMAND cmake -E echo "${SWROOT}/lib created"
    COMMAND cmake -E make_directory ${SWROOT}/bin
    COMMAND cmake -E echo "${SWROOT}/bin created"
    COMMAND cmake -E make_directory ${SWROOT}/idl
    COMMAND cmake -E echo "${SWROOT}/idl created"
    COMMAND cmake -E make_directory ${SWROOT}/config
    COMMAND cmake -E echo "${SWROOT}/config created"
    COMMAND cmake -E make_directory ${SWROOT}/include
    COMMAND cmake -E echo "${SWROOT}/include created"
    COMMAND cmake -E make_directory ${SWROOT}/Sources
    COMMAND cmake -E echo "${SWROOT}/Sources created")
#install modules
ADD_SUBDIRECTORY(Logger/src)
ADD_SUBDIRECTORY(tcs/src)
ENDIF(NOT LCU)




