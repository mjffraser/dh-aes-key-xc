cmake_minimum_required(VERSION 3.28)
project(diffie-hellman-key-xc)
set(CMAKE_CXX_STANDARD 17)

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

set(AES_ENCRYPTION_SRC
	src/encrypt/encrypt.cpp
	src/encrypt/keygen.cpp
)

set(ALL_SRC
	${DH_PARAMETER_GEN_SRC}
	${DH_CMDLINE_ARGS_SRC}
	${AES_ENCRYPTION_SRC}
	src/rand/random_util.cpp
	src/main.cpp
	src/dh_params.cpp
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
