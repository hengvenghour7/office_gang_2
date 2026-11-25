
cmake_minimum_required(VERSION 3.20)

project(office_gang_2 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find raylib
find_package(raylib REQUIRED)

# Collect client source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../include/*.c"
)

# Create client executable
add_executable(office_gang_2 ${SOURCES})

set_target_properties(office_gang_2 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# Include header directories
target_include_directories(office_gang_2 PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../include"
)

# Link libraries
target_link_libraries(office_gang_2 PRIVATE
    raylib
)
