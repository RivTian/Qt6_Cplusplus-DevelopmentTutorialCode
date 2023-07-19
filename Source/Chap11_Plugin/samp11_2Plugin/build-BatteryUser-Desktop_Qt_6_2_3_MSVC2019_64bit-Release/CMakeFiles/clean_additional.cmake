# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "BatteryUser_autogen"
  "CMakeFiles\\BatteryUser_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\BatteryUser_autogen.dir\\ParseCache.txt"
  )
endif()
