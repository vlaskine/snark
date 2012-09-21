SET( PROJECT "qt3d" )
SET( TARGET_NAME rur_graphics_${PROJECT} )
SET( dir ${SOURCE_CODE_BASE_DIR}/graphics/${PROJECT} )

FILE( GLOB source   ${dir}/*.cpp )
FILE( GLOB includes ${dir}/*.h )

QT4_WRAP_CPP( rur_graphics_qt3d_moc ${dir}/view.h OPTIONS -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED )

SOURCE_GROUP( ${TARGET_NAME} FILES ${source} ${includes} ${rur_graphics_qt3d_moc} )

ADD_LIBRARY( ${TARGET_NAME} ${source} ${includes} ${rur_graphics_qt3d_moc} )
SET_TARGET_PROPERTIES( ${TARGET_NAME} PROPERTIES ${rur_LIBRARY_PROPERTIES} )
TARGET_LINK_LIBRARIES( ${TARGET_NAME} ${QT_LIBRARIES} ${Qt3D_LIB} ${rur_ALL_EXTERNAL_LIBRARIES} )

# todo: install library and headers properly
#INSTALL( FILES ${includes} DESTINATION ${CMAKE_INSTALL_PREFIX}/${rur_INSTALL_INCLUDE_DIR}/${PROJECT} )
#INSTALL(
#    TARGETS ${TARGET_NAME}
#    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${rur_INSTALL_BIN_DIR} COMPONENT Runtime     # .exe, .dll
#    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${rur_INSTALL_LIB_DIR} COMPONENT Runtime     # .so, mod.dll
#    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/${rur_INSTALL_LIB_DIR} COMPONENT Development # .a, .lib
#)
