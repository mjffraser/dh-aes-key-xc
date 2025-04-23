cmake_minimum_required(VERSION 3.10)
project(diffie-hellman-key-xc)
set(CMAKE_CXX_STANDARD 17)

#debug symbols
if(DEFINED DEBUG AND DEBUG STREQUAL "1")
    message(STATUS "WARN: DEBUG SYMBOLS INCLUDED.")
    add_compile_definitions(DEBUG)
else()
    message(STATUS "RELEASE")
endif()
unset(DEBUG CACHE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost CONFIG)
find_package(OpenSSL REQUIRED)


set(SRC
    src/logger.cpp
    src/main.cpp
)

add_executable(dh-key-xc ${SRC})
target_include_directories(dh-key-xc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(Boost_FOUND)
    target_include_directories(dh-key-xc PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(dh-key-xc PRIVATE ${Boost_LIBRARIES} -lboost_random OpenSSL::SSL)
endif()
