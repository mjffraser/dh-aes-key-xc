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
add_compile_options(-Wall)

find_package(Boost CONFIG)
find_package(OpenSSL REQUIRED)

set(READ-ARGS
    src/read-args/read-args.cpp
    src/read-args/internal/general.cpp
    src/read-args/internal/networking.cpp
)

set(DH-PARAM-GEN
    src/dh-param-gen/public.cpp
    src/dh-param-gen/private.cpp
    src/dh-param-gen/internal/rfc-3526-primes.cpp
)

set(AES-UTIL
    src/aes/aes-gcm.cpp
    src/aes/aes-keygen.cpp
    src/aes/internal/iv-formatting.cpp
)

set(SRC
    ${READ-ARGS}
    ${DH-PARAM-GEN}
    ${AES-UTIL}

    src/logger.cpp
    src/main.cpp
)

add_executable(dh-key-xc ${SRC})
target_include_directories(dh-key-xc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(Boost_FOUND)
    target_include_directories(dh-key-xc PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(dh-key-xc PRIVATE ${Boost_LIBRARIES} -lboost_random OpenSSL::SSL)
endif()
