cmake_minimum_required(VERSION 3.28)
project(diffie-hellman-key-xc)
set(CMAKE_CXX_STANDARD 17)

#symbols for clang lsp
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost CONFIG)



set(DH_CMDLINE_ARGS_SRC
	src/readArgs/readArgs.cpp
	src/readArgs/internal/general.cpp
	src/readArgs/internal/networking.cpp
)

set(DH_PARAMETER_GEN_SRC
	src/dhParamGen/private.cpp
	src/dhParamGen/public.cpp
	src/dhParamGen/internal/getRandomBytes.cpp
)

set(ALL_SRC
	src/main.cpp
	src/dhParams.cpp
	src/logger.cpp
	${DH_CMDLINE_ARGS_SRC}
)

add_executable(dh-key-xc ${ALL_SRC})
target_include_directories(dh-key-xc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

if(Boost_FOUND)
	target_include_directories(dh-key-xc PRIVATE ${Boost_INCLUDE_DIRS})
	target_link_libraries(dh-key-xc PRIVATE ${Boost_LIBRARIES})
endif()
