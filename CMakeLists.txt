cmake_minimum_required(VERSION 3.28)
project(diffie-hellman-key-xc)
set(CMAKE_CXX_STANDARD 17)

#symbols for clang lsp
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#logging source files
set(DH_LOGGING_SRC
	src/logging/logger.cpp
)

set(ALL_SRC
	src/main.cpp
	${DH_LOGGING_SRC}
)

add_executable(dh-key-xc ${ALL_SRC})
target_include_directories(dh-key-xc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
