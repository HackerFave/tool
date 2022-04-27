#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "fastrtps" for configuration "Debug"
set_property(TARGET fastrtps APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(fastrtps PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/fastrtpsd-2.5.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/fastrtpsd-2.5.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS fastrtps )
list(APPEND _IMPORT_CHECK_FILES_FOR_fastrtps "${_IMPORT_PREFIX}/lib/fastrtpsd-2.5.lib" "${_IMPORT_PREFIX}/bin/fastrtpsd-2.5.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
