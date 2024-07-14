#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ixwebsocket::ixwebsocket" for configuration "Release"
set_property(TARGET ixwebsocket::ixwebsocket APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(ixwebsocket::ixwebsocket PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libixwebsocket.so.0.0.1"
  IMPORTED_SONAME_RELEASE "libixwebsocket.so.0.0.1"
  )

list(APPEND _cmake_import_check_targets ixwebsocket::ixwebsocket )
list(APPEND _cmake_import_check_files_for_ixwebsocket::ixwebsocket "${_IMPORT_PREFIX}/lib/libixwebsocket.so.0.0.1" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
