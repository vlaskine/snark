# The install-only section is empty for the build tree.
SET(rur_CONFIG_INSTALL_ONLY)

# The "use" file.
SET(rur_USE_FILE ${rur_BINARY_DIR}/CMakeFiles/use_rur.cmake)

# Library directory.
SET(rur_LIBRARY_DIRS_CONFIG ${LIBRARY_OUTPUT_PATH})

# Runtime library directory.
SET(rur_RUNTIME_LIBRARY_DIRS_CONFIG ${LIBRARY_OUTPUT_PATH})

# Binary executable directory.
SET(rur_EXECUTABLE_DIRS_CONFIG ${EXECUTABLE_OUTPUT_PATH})

#-----------------------------------------------------------------------------
# Configure rur-config.cmake for the build tree.

CONFIGURE_FILE( ${rur_SOURCE_DIR}/CMakeFiles/rur-config.cmake.in
                ${rur_BINARY_DIR}/CMakeFiles/rur-config.cmake @ONLY IMMEDIATE )

#-----------------------------------------------------------------------------
# Settings specific to the install tree.

# The "use" file.
SET(rur_USE_FILE \${rur_INSTALL_PREFIX}/${rur_INSTALL_PACKAGE_DIR}/use_rur.cmake)

# Include directories.
SET(rur_INCLUDE_DIRS_CONFIG
  \${rur_INSTALL_PREFIX}/include
  ${rur_EXTERNAL_INCLUDES}
)


# Link directories.
IF(CYGWIN AND BUILD_SHARED_LIBS)
  # In Cygwin programs directly link to the .dll files.
  SET(rur_LIBRARY_DIRS_CONFIG \${rur_INSTALL_PREFIX}/${rur_INSTALL_BIN_DIR})
ELSE(CYGWIN AND BUILD_SHARED_LIBS)
  SET(rur_LIBRARY_DIRS_CONFIG \${rur_INSTALL_PREFIX}/${rur_INSTALL_LIB_DIR})
ENDIF(CYGWIN AND BUILD_SHARED_LIBS)

# Executable and runtime library directories.
IF(WIN32)
  SET(rur_EXECUTABLE_DIRS_CONFIG \${rur_INSTALL_PREFIX}/${rur_INSTALL_BIN_DIR})
  SET(rur_RUNTIME_LIBRARY_DIRS_CONFIG \${rur_INSTALL_PREFIX}/${rur_INSTALL_BIN_DIR})
ELSE(WIN32)
  SET(rur_EXECUTABLE_DIRS_CONFIG \${rur_INSTALL_PREFIX}/${rur_INSTALL_BIN_DIR})
  SET(rur_RUNTIME_LIBRARY_DIRS_CONFIG \${rur_INSTALL_PREFIX}/${rur_INSTALL_LIB_DIR})
ENDIF(WIN32)

IF(WIN32)
  SET(rur_EXE_EXT ".exe")
ENDIF(WIN32)

#-----------------------------------------------------------------------------
# Configure rur-config.cmake for the install tree.

# Construct the proper number of GET_FILENAME_COMPONENT(... PATH)
# calls to compute the installation prefix from rur_DIR.
#STRING(REGEX REPLACE "/" ";" rur_INSTALL_PACKAGE_DIR_COUNT
#  "${rur_INSTALL_PACKAGE_DIR}")
SET(rur_CONFIG_INSTALL_ONLY "
# Compute the installation prefix from rur_DIR.
SET(rur_INSTALL_PREFIX \"\${rur_DIR}/..\")
")
#FOREACH(p ${rur_INSTALL_PACKAGE_DIR_COUNT})
#  SET(rur_CONFIG_INSTALL_ONLY
#    "${rur_CONFIG_INSTALL_ONLY}GET_FILENAME_COMPONENT(rur_INSTALL_PREFIX \"\${rur_INSTALL_PREFIX}\" PATH)\n"
#    )
#ENDFOREACH(p)

# The install tree only has one configuration.
SET(rur_CONFIGURATION_TYPES_CONFIG)

IF(CMAKE_CONFIGURATION_TYPES)
  # There are multiple build configurations.  Configure one
  # QConfig.cmake for each configuration.
  FOREACH(config ${CMAKE_CONFIGURATION_TYPES})
    SET(rur_BUILD_TYPE_CONFIG ${config})
    CONFIGURE_FILE(${rur_SOURCE_DIR}/CMakeFiles/rur-config.cmake.in
                   ${rur_BINARY_DIR}/Utilities/${config}/rur-config.cmake
                   @ONLY IMMEDIATE)
  ENDFOREACH(config)

  # Install the config file corresponding to the build configuration
  # specified when building the install target.  The BUILD_TYPE variable
  # will be set while CMake is processing the install files.
  INSTALL(
    FILES
      "${rur_BINARY_DIR}/Utilities/\${BUILD_TYPE}/rur-config.cmake"
    DESTINATION ${rur_INSTALL_PACKAGE_DIR}
  )

ELSE(CMAKE_CONFIGURATION_TYPES)
  # There is only one build configuration.  Configure one QConfig.cmake.
  SET(rur_BUILD_TYPE_CONFIG ${CMAKE_BUILD_TYPE})
  CONFIGURE_FILE(${rur_SOURCE_DIR}/CMakeFiles/rur-config.cmake.in
                 ${rur_BINARY_DIR}/Utilities/rur-config.cmake @ONLY IMMEDIATE)

  # Setup an install rule for the config file.
  INSTALL(
    FILES
      "${rur_BINARY_DIR}/Utilities/rur-config.cmake"
    DESTINATION ${rur_INSTALL_PACKAGE_DIR}
  )
ENDIF(CMAKE_CONFIGURATION_TYPES)
