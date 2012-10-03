PROJECT( "graphics" )

FIND_PACKAGE( OpenGL )
FIND_LIBRARY( Qt3D_LIB NAMES Qt3D REQUIRED )
FIND_PACKAGE( Qt4 COMPONENTS QtCore QtGui QtOpenGl QtXml REQUIRED )

INCLUDE( ${QT_USE_FILE} )

# Apple's OpenGL libs are slightly different from most UNIX systems
IF( APPLE )
    include_directories(/usr/X11R6/include/)
    link_directories(/usr/X11R6/lib)
ENDIF( APPLE )

CMAKE_POLICY( SET CMP0012 NEW ) # otherwise it does not recognize ON/OFF values as booleans

SET( dir ${SOURCE_CODE_BASE_DIR}/${PROJECT} )
FILE( GLOB includes ${dir}/*.h )
FILE( GLOB impl_includes ${dir}/impl/*.h )
INSTALL( FILES ${includes} DESTINATION ${CMAKE_INSTALL_PREFIX}/${snark_INSTALL_INCLUDE_DIR}/${PROJECT} )
INSTALL( FILES ${impl_includes} DESTINATION ${CMAKE_INSTALL_PREFIX}/${snark_INSTALL_INCLUDE_DIR}/${PROJECT}/impl )

if( snark_BUILD_APPLICATIONS )
    ADD_SUBDIRECTORY( applications )
endif( snark_BUILD_APPLICATIONS )

ADD_SUBDIRECTORY( qt3d )
