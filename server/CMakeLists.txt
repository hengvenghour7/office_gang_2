
cmake_minimum_required(VERSION 3.20)

project(office_gang_server C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

add_executable(office_gang_server ${SOURCES})

target_include_directories(office_gang_server PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../include"
)
