# The install-only section is empty for the build tree.
SET(snark_CONFIG_INSTALL_ONLY)

# The "use" file.
SET(snark_USE_FILE ${snark_BINARY_DIR}/CMakeFiles/use_snark.cmake)

# Library directory.
SET(snark_LIBRARY_DIRS_CONFIG ${LIBRARY_OUTPUT_PATH})

# Runtime library directory.
SET(snark_RUNTIME_LIBRARY_DIRS_CONFIG ${LIBRARY_OUTPUT_PATH})

# Binary executable directory.
SET(snark_EXECUTABLE_DIRS_CONFIG ${EXECUTABLE_OUTPUT_PATH})

#-----------------------------------------------------------------------------
# Configure snark-config.cmake for the build tree.

CONFIGURE_FILE( ${snark_SOURCE_DIR}/CMakeFiles/snark-config.cmake.in
                ${snark_BINARY_DIR}/CMakeFiles/snark-config.cmake @ONLY IMMEDIATE )

#-----------------------------------------------------------------------------
# Settings specific to the install tree.

# The "use" file.
SET(snark_USE_FILE \${snark_INSTALL_PREFIX}/${snark_INSTALL_PACKAGE_DIR}/use_snark.cmake)

# Include directories.
SET(snark_INCLUDE_DIRS_CONFIG
  \${snark_INSTALL_PREFIX}/include
  ${snark_EXTERNAL_INCLUDES}
)


# Link directories.
IF(CYGWIN AND BUILD_SHARED_LIBS)
  # In Cygwin programs directly link to the .dll files.
  SET(snark_LIBRARY_DIRS_CONFIG \${snark_INSTALL_PREFIX}/${snark_INSTALL_BIN_DIR})
ELSE(CYGWIN AND BUILD_SHARED_LIBS)
  SET(snark_LIBRARY_DIRS_CONFIG \${snark_INSTALL_PREFIX}/${snark_INSTALL_LIB_DIR})
ENDIF(CYGWIN AND BUILD_SHARED_LIBS)

# Executable and runtime library directories.
IF(WIN32)
  SET(snark_EXECUTABLE_DIRS_CONFIG \${snark_INSTALL_PREFIX}/${snark_INSTALL_BIN_DIR})
  SET(snark_RUNTIME_LIBRARY_DIRS_CONFIG \${snark_INSTALL_PREFIX}/${snark_INSTALL_BIN_DIR})
ELSE(WIN32)
  SET(snark_EXECUTABLE_DIRS_CONFIG \${snark_INSTALL_PREFIX}/${snark_INSTALL_BIN_DIR})
  SET(snark_RUNTIME_LIBRARY_DIRS_CONFIG \${snark_INSTALL_PREFIX}/${snark_INSTALL_LIB_DIR})
ENDIF(WIN32)

IF(WIN32)
  SET(snark_EXE_EXT ".exe")
ENDIF(WIN32)

#-----------------------------------------------------------------------------
# Configure snark-config.cmake for the install tree.

# Construct the proper number of GET_FILENAME_COMPONENT(... PATH)
# calls to compute the installation prefix from snark_DIR.
#STRING(REGEX REPLACE "/" ";" snark_INSTALL_PACKAGE_DIR_COUNT
#  "${snark_INSTALL_PACKAGE_DIR}")
SET(snark_CONFIG_INSTALL_ONLY "
# Compute the installation prefix from snark_DIR.
SET(snark_INSTALL_PREFIX \"\${snark_DIR}/..\")
")
#FOREACH(p ${snark_INSTALL_PACKAGE_DIR_COUNT})
#  SET(snark_CONFIG_INSTALL_ONLY
#    "${snark_CONFIG_INSTALL_ONLY}GET_FILENAME_COMPONENT(snark_INSTALL_PREFIX \"\${snark_INSTALL_PREFIX}\" PATH)\n"
#    )
#ENDFOREACH(p)

# The install tree only has one configuration.
SET(snark_CONFIGURATION_TYPES_CONFIG)

IF(CMAKE_CONFIGURATION_TYPES)
  # There are multiple build configurations.  Configure one
  # QConfig.cmake for each configuration.
  FOREACH(config ${CMAKE_CONFIGURATION_TYPES})
    SET(snark_BUILD_TYPE_CONFIG ${config})
    CONFIGURE_FILE(${snark_SOURCE_DIR}/CMakeFiles/snark-config.cmake.in
                   ${snark_BINARY_DIR}/Utilities/${config}/snark-config.cmake
                   @ONLY IMMEDIATE)
  ENDFOREACH(config)

  # Install the config file corresponding to the build configuration
  # specified when building the install target.  The BUILD_TYPE variable
  # will be set while CMake is processing the install files.
  INSTALL(
    FILES
      "${snark_BINARY_DIR}/Utilities/\${BUILD_TYPE}/snark-config.cmake"
    DESTINATION ${snark_INSTALL_PACKAGE_DIR}
  )

ELSE(CMAKE_CONFIGURATION_TYPES)
  # There is only one build configuration.  Configure one QConfig.cmake.
  SET(snark_BUILD_TYPE_CONFIG ${CMAKE_BUILD_TYPE})
  CONFIGURE_FILE(${snark_SOURCE_DIR}/CMakeFiles/snark-config.cmake.in
                 ${snark_BINARY_DIR}/Utilities/snark-config.cmake @ONLY IMMEDIATE)

  # Setup an install rule for the config file.
  INSTALL(
    FILES
      "${snark_BINARY_DIR}/Utilities/snark-config.cmake"
    DESTINATION ${snark_INSTALL_PACKAGE_DIR}
  )
ENDIF(CMAKE_CONFIGURATION_TYPES)
