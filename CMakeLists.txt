cmake_minimum_required(VERSION 3.12)

set(SRC_DIR "./src")

set(PROJECT_NAME "Test_Gen")
project(${PROJECT_NAME})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "./bin")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

# Set C and C++ standards
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

file(GLOB SRC_FILES "${SRC_DIR}/*.c" "${SRC_DIR}/*.cpp")
# Create an executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

#target_include_directories(${PROJECT_NAME} PUBLIC )
# Compiler Flags Debug(-g -O0) Release(-O3)
set(OPT_FLAG -O2 -g -mavx2)
target_compile_options(
	${PROJECT_NAME} PUBLIC ${OPT_FLAG}
	-Wall -Wextra -Wshadow -Wfloat-conversion -Wconversion
)
target_link_libraries(${PROJECT_NAME} PRIVATE "-l:libm.a")