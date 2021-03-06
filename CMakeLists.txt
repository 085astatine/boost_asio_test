cmake_minimum_required(VERSION 2.8)

add_definitions(
    -std=c++14
    -Wall
    -Wextra)

message("CXX Compiler ID: " ${CMAKE_CXX_COMPILER_ID})
message("CMake System Name: " ${CMAKE_SYSTEM_NAME})

find_package(Boost REQUIRED system thread)
message("Boost System Library   : " ${Boost_SYSTEM_LIBRARIES})
message("Boost Thread Library   : " ${Boost_THREAD_LIBRARIES})
message("Boost Include Directory: " ${Boost_INCLUDE_DIR})

include_directories(
    ${Boost_INCLUDE_DIR})

# Tutorial, Timer
add_executable(TimerSync
    timer_sync.cpp)
target_link_libraries(TimerSync
    ${Boost_LIBRARIES})
add_executable(TimerAsync
    timer_async.cpp)
target_link_libraries(TimerAsync
    ${Boost_LIBRARIES})
add_executable(TimerBindingArguments
    timer_binding_arguments.cpp)
target_link_libraries(TimerBindingArguments
    ${Boost_LIBRARIES})
add_executable(TimerCallbackClass
    timer_callback_class.cpp)
target_link_libraries(TimerCallbackClass
    ${Boost_LIBRARIES})
add_executable(TimerMultiThread
    timer_multithread.cpp)
target_link_libraries(TimerMultiThread
    ${Boost_LIBRARIES})
# Tutorial, TCP
add_executable(TCPSyncClient
    tcp_sync_client.cpp)
target_link_libraries(TCPSyncClient
    ${Boost_LIBRARIES}
    pthread)
add_executable(TCPSyncServer
    tcp_sync_server.cpp)
target_link_libraries(TCPSyncServer
    ${Boost_LIBRARIES})
add_executable(TCPAsyncServer
    tcp_async_server.cpp)
target_link_libraries(TCPAsyncServer
    ${Boost_LIBRARIES})
# Tutorial, UDP
add_executable(UDPSyncClient
    udp_sync_client.cpp)
target_link_libraries(UDPSyncClient
    ${Boost_LIBRARIES}
    pthread)
add_executable(UDPSyncServer
    udp_sync_server.cpp)
target_link_libraries(UDPSyncServer
    ${Boost_LIBRARIES})
add_executable(UDPAsyncServer
    udp_async_server.cpp)
target_link_libraries(UDPAsyncServer
    ${Boost_LIBRARIES})
