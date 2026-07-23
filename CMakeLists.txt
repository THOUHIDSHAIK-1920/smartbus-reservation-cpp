cmake_minimum_required(VERSION 3.16)
project(SmartBusReservationSystem LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)
file(GLOB_RECURSE SRC_FILES
    "main.cpp"
    "database/*.cpp"
    "models/*.cpp"
    "ui/*.cpp"
)
file(GLOB_RECURSE HEADER_FILES
    "*.h"
    "database/*.h"
    "models/*.h"
    "ui/*.h"
)
add_executable(SmartBusReservationSystem
    ${SRC_FILES}
    ${HEADER_FILES}
)
target_include_directories(
    SmartBusReservationSystem
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(SmartBusReservationSystem PRIVATE Qt6::Widgets Qt6::Sql)
install(TARGETS SmartBusReservationSystem RUNTIME DESTINATION bin)
