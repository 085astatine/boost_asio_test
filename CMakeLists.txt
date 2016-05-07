cmake_minimum_required(VERSION 2.8)

add_definitions(
    -std=c++14
    -Wall
    -Wextra)

message("CXX Compiler ID: " ${CMAKE_CXX_COMPILER_ID})
message("CMake System Name: " ${CMAKE_SYSTEM_NAME})

find_package(Boost REQUIRED system)
message("Boost System Library   : " ${Boost_SYSTEM_LIBRARIES})
message("Boost Include Directory: " ${Boost_INCLUDE_DIR})

include_directories(
    ${Boost_INCLUDE_DIR})

# Tutorial, Timer
add_executable(TimerSync
    timer_sync.cpp)
target_link_libraries(TimerSync
    ${Boost_LIBRARIES})
