# Find the rur installation or build tree.
# If rur is not found, rur_FOUND is set to false.
# The following variables are set if rur is found.
#  rur_FOUND         - Set to true when rur is found.
#  rur_USE_FILE      - CMake file to use rur.
#  rur_MAJOR_VERSION - The rur major version number.
#  rur_MINOR_VERSION - The rur minor version number
#                       (odd non-release).
#  rur_BUILD_VERSION - The rur patch level
#                       (meaningless for odd minor).
#  rur_INCLUDE_DIRS  - Include directories for rur
#  rur_LIBRARY_DIRS  - Link directories for rur libraries
# The following cache entries must be set by the user to locate rur:
#  rur_DIR  - The directory where rur was installed
#            eg c:\program files\rur, c:\lib\rur
#            it is the directory containing subdirs (include, bin, lib, CMakeFiles)
#           This can also be the root of the build tree - the dir where the device makefiles/project files exist
#            eg c:\build\rur\

# Construct consitent error messages for use below.
SET(rur_DIR_DESCRIPTION "rur installation directory - this is either the root of the build tree, or the installed location")
SET(rur_DIR_MESSAGE "rur not found.  Set the rur_DIR cmake cache entry to the ${rur_DIR_DESCRIPTION}")

#If the location is not already known, then search.
IF ( NOT rur_DIR )
  # Get the system search path as a list.
  IF(UNIX)
    STRING(REGEX MATCHALL "[^:]+" rur_DIR_SEARCH1 "$ENV{PATH}")
  ELSE(UNIX)
    STRING(REGEX REPLACE "\\\\" "/" rur_DIR_SEARCH1 "$ENV{PATH}")
  ENDIF(UNIX)
  IF ( WIN32 )
    # Assume this path exists.
    SET ( rur_DIR_SEARCH1
      ${rur_DIR_SEARCH1}
      "C:/Program Files/rur"
    )
  ENDIF ( WIN32 )
  STRING(REGEX REPLACE "/;" ";" rur_DIR_SEARCH2 "${rur_DIR_SEARCH1}")

  SET ( rur_DIR_SEARCH "${rur_DIR_SEARCH2}" } )

  #
  # Look for an installation or build tree.
  #
  FIND_PATH(rur_DIR NAMES use_rur.cmake PATH_SUFFIXES ${SUFFIX_FOR_PATH} PATHS

     # Look for an environment variable rur_DIR.
      $ENV{rur_DIR}

      # Look in places relative to the system executable search path.
      ${rur_DIR_SEARCH}

      # Look in standard UNIX install locations.
      /usr/local/CMakeFiles
      /usr/local/lib
      /usr/lib

      # Help the user find it if we cannot.
      DOC "The ${rur_DIR_DESCRIPTION}"
  )
ENDIF ( NOT rur_DIR )
# If rur was found, load the configuration file to get the rest of the
# settings.
IF(rur_DIR)
  # Make sure the rur_config.cmake file exists in the directory provided.
  IF(EXISTS ${rur_DIR}/rur_config.cmake)
    # We found rur.  Load the settings.
    SET(rur_FOUND 1)
    INCLUDE(${rur_DIR}/rur_config.cmake)
  ENDIF(EXISTS ${rur_DIR}/rur_config.cmake)
ELSE(rur_DIR)
  # We did not find rur_.
  SET(rur_FOUND 0)
ENDIF(rur_DIR)

IF ( rur_FOUND )
  IF ( EXISTS ${rur_USE_FILE} )
    INCLUDE(${rur_USE_FILE})
  ELSE ( EXISTS ${rur_USE_FILE} )
    SET(rur_FOUND 0)
  ENDIF ( EXISTS ${rur_USE_FILE} )
ENDIF ( rur_FOUND )

IF( NOT rur_FOUND )
  MESSAGE(FATAL_ERROR ${rur_DIR_MESSAGE})
ENDIF( NOT rur_FOUND )

