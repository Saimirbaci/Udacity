cmake_minimum_required(VERSION 2.6)
project(monitor)
set(CMAKE_CXX_STANDARD 17)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(monitor ${SOURCES})

#add_executable(test "src/test.cc" "src/format.cpp" "src/linux_parser.cpp" 
#    "src/system.cpp" "src/process.cpp" "src/processor.cpp" "src/user.cpp")

set_property(TARGET monitor PROPERTY CXX_STANDARD 17)

#set_property(TARGET test PROPERTY CXX_STANDARD 17)
#target_link_libraries(test ${CURSES_LIBRARIES})

target_link_libraries(monitor ${CURSES_LIBRARIES})
# TODO: Run -Werror in CI.
target_compile_options(monitor PRIVATE -Wall -Wextra)
