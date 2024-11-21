cmake_minimum_required(VERSION 3.28)
project(diffie-hellman-key-xc)
set(CMAKE_CXX_STANDARD 17)

#symbols for clang lsp
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#logging source files
set(DH_LOGGING_SRC
	src/logging/logger.cpp
)

set(DH_CONFIG_SRC
	src/config/getConfig.cpp
	src/config/internal/import.cpp
	src/config/internal/validate.cpp
)

set(ALL_SRC
	src/main.cpp
	src/dhParams.cpp
	${DH_LOGGING_SRC}
	${DH_CONFIG_SRC}
)

add_executable(dh-key-xc ${ALL_SRC})
target_include_directories(dh-key-xc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(dh-key-xc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)

