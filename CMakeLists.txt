cmake_minimum_required(VERSION 3.5)
project(c_plus_plus_parallel_programming)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system thread)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(c_plus_plus_parallel_programming
    main.cpp
    main_thread.cpp
    websocket_server.cpp
)

target_link_libraries(c_plus_plus_parallel_programming
    ${Boost_LIBRARIES}
)
