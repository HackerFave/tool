#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "fastdds::fast-discovery-server" for configuration "Debug"
set_property(TARGET fastdds::fast-discovery-server APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(fastdds::fast-discovery-server PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/fast-discovery-serverd-1.0.0.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS fastdds::fast-discovery-server )
list(APPEND _IMPORT_CHECK_FILES_FOR_fastdds::fast-discovery-server "${_IMPORT_PREFIX}/bin/fast-discovery-serverd-1.0.0.exe" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
