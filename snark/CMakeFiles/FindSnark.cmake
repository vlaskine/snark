# Find the snark installation or build tree.
# If snark is not found, snark_FOUND is set to false.
# The following variables are set if snark is found.
#  snark_FOUND         - Set to true when snark is found.
#  snark_USE_FILE      - CMake file to use snark.
#  snark_MAJOR_VERSION - The snark major version number.
#  snark_MINOR_VERSION - The snark minor version number
#                       (odd non-release).
#  snark_BUILD_VERSION - The snark patch level
#                       (meaningless for odd minor).
#  snark_INCLUDE_DIRS  - Include directories for snark
#  snark_LIBRARY_DIRS  - Link directories for snark libraries
# The following cache entries must be set by the user to locate snark:
#  snark_DIR  - The directory where snark was installed
#            eg c:\program files\snark, c:\lib\snark
#            it is the directory containing subdirs (include, bin, lib, CMakeFiles)
#           This can also be the root of the build tree - the dir where the device makefiles/project files exist
#            eg c:\build\snark\

# Construct consitent error messages for use below.
SET(snark_DIR_DESCRIPTION "snark installation directory - this is either the root of the build tree, or the installed location")
SET(snark_DIR_MESSAGE "snark not found.  Set the snark_DIR cmake cache entry to the ${snark_DIR_DESCRIPTION}")

#If the location is not already known, then search.
IF ( NOT snark_DIR )
  # Get the system search path as a list.
  IF(UNIX)
    STRING(REGEX MATCHALL "[^:]+" snark_DIR_SEARCH1 "$ENV{PATH}")
  ELSE(UNIX)
    STRING(REGEX REPLACE "\\\\" "/" snark_DIR_SEARCH1 "$ENV{PATH}")
  ENDIF(UNIX)
  IF ( WIN32 )
    # Assume this path exists.
    SET ( snark_DIR_SEARCH1
      ${snark_DIR_SEARCH1}
      "C:/Program Files/snark"
    )
  ENDIF ( WIN32 )
  STRING(REGEX REPLACE "/;" ";" snark_DIR_SEARCH2 "${snark_DIR_SEARCH1}")

  SET ( snark_DIR_SEARCH "${snark_DIR_SEARCH2}" } )

  #
  # Look for an installation or build tree.
  #
  FIND_PATH(snark_DIR NAMES use_snark.cmake PATH_SUFFIXES ${SUFFIX_FOR_PATH} PATHS

     # Look for an environment variable snark_DIR.
      $ENV{snark_DIR}

      # Look in places relative to the system executable search path.
      ${snark_DIR_SEARCH}

      # Look in standard UNIX install locations.
      /usr/local/CMakeFiles
      /usr/local/lib
      /usr/lib

      # Help the user find it if we cannot.
      DOC "The ${snark_DIR_DESCRIPTION}"
  )
ENDIF ( NOT snark_DIR )
# If snark was found, load the configuration file to get the rest of the
# settings.
IF(snark_DIR)
  # Make sure the snark_config.cmake file exists in the directory provided.
  IF(EXISTS ${snark_DIR}/snark_config.cmake)
    # We found snark.  Load the settings.
    SET(snark_FOUND 1)
    INCLUDE(${snark_DIR}/snark_config.cmake)
  ENDIF(EXISTS ${snark_DIR}/snark_config.cmake)
ELSE(snark_DIR)
  # We did not find snark_.
  SET(snark_FOUND 0)
ENDIF(snark_DIR)

IF ( snark_FOUND )
  IF ( EXISTS ${snark_USE_FILE} )
    INCLUDE(${snark_USE_FILE})
  ELSE ( EXISTS ${snark_USE_FILE} )
    SET(snark_FOUND 0)
  ENDIF ( EXISTS ${snark_USE_FILE} )
ENDIF ( snark_FOUND )

IF( NOT snark_FOUND )
  MESSAGE(FATAL_ERROR ${snark_DIR_MESSAGE})
ENDIF( NOT snark_FOUND )

