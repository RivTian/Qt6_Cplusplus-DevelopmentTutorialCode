# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\SharedLibUser_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\SharedLibUser_autogen.dir\\ParseCache.txt"
  "SharedLibUser_autogen"
  )
endif()
