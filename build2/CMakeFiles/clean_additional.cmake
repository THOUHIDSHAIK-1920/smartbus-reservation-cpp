# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\SmartBusReservationSystem_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\SmartBusReservationSystem_autogen.dir\\ParseCache.txt"
  "SmartBusReservationSystem_autogen"
  )
endif()
