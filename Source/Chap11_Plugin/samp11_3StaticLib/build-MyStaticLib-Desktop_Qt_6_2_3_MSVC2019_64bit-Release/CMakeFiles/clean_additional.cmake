# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\MyStaticLib_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\MyStaticLib_autogen.dir\\ParseCache.txt"
  "MyStaticLib_autogen"
  )
endif()
