cmake_minimum_required(VERSION 3.5)
project(c_plus_plus_parallel_programming)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Boost
find_package(Boost REQUIRED COMPONENTS system thread)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Include Boost and OpenSSL directories
include_directories(${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})

# Add the source files for the executable
add_executable(c_plus_plus_parallel_programming
    main.cpp
    websocket_server.cpp
)

# Link against Boost and OpenSSL libraries
target_link_libraries(c_plus_plus_parallel_programming
    ${Boost_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
)
