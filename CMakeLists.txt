cmake_minimum_required(VERSION 3.28)
project(diffie-hellman-key-xc)
set(CMAKE_CXX_STANDARD 17)

#remove all sensistive debugging messages from release build
if(DEFINED DEBUG AND DEBUG STREQUAL "1")
	message (STATUS "WARNING DEBUG FLAG DETECTED. SENSITIVE INFORMATION COULD BE LEAKED TO THE DISK.")
	add_compile_definitions(SENSITIVE)
else()
	message(STATUS "RELEASE BUILD")
endif()

unset(DEBUG CACHE)

#symbols for clang lsp
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost CONFIG)
find_package(OpenSSL REQUIRED)

set(DH_CMDLINE_ARGS_SRC
	src/read_args/read_args.cpp
	src/read_args/internal/general.cpp
	src/read_args/internal/networking.cpp
)

set(DH_PARAMETER_GEN_SRC
	src/dh_param_gen/internal/vetted_primes.cpp
	src/dh_param_gen/private.cpp
	src/dh_param_gen/public.cpp
)

set(NETWORK_SRC
	src/networking/server.cpp
	src/networking/client.cpp
	src/networking/internal/message_formatting.cpp
	src/networking/internal/socket_util.cpp
	src/networking/internal/encrypt/aes_gcm.cpp
	src/networking/internal/encrypt/keygen.cpp
)

set(ALL_SRC
	${DH_PARAMETER_GEN_SRC}
	${DH_CMDLINE_ARGS_SRC}
	${AES_ENCRYPTION_SRC}
	${NETWORK_SRC}
	src/rand/random_util.cpp
	src/main.cpp
	src/logger.cpp
)

add_executable(dh-key-xc ${ALL_SRC})
target_include_directories(dh-key-xc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

if(Boost_FOUND)
	target_include_directories(dh-key-xc PRIVATE ${Boost_INCLUDE_DIRS})
	target_link_libraries(dh-key-xc PRIVATE ${Boost_LIBRARIES} -lboost_random OpenSSL::SSL)
endif()

