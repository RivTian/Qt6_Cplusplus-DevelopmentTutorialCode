# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\StaticLibUser_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\StaticLibUser_autogen.dir\\ParseCache.txt"
  "StaticLibUser_autogen"
  )
endif()
