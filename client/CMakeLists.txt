cmake_minimum_required(VERSION 3.20)

project(office_gang_2 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(raylib REQUIRED)

# Collect client source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../include/*.c"
)

# Build a regular executable for VS Code
add_executable(office_gang_2 ${SOURCES})

# Copy resources beside the regular executable
add_custom_command(TARGET office_gang_2 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:office_gang_2>/resources"
    VERBATIM
)

# Include image assets in the Finder app bundle
set(ASSETS
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/character.png"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/center_world.png"
)

set_source_files_properties(${ASSETS} PROPERTIES
    MACOSX_PACKAGE_LOCATION "Resources"
)

add_executable(office_gang_2_finder MACOSX_BUNDLE
    ${SOURCES}
    ${ASSETS}
)

set_target_properties(office_gang_2_finder PROPERTIES
    MACOSX_BUNDLE_BUNDLE_NAME "Office Gang 2"
)

# Include directories for both targets
foreach(target office_gang_2 office_gang_2_finder)
    target_include_directories(${target} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/../include"
    )

    target_link_libraries(${target} PRIVATE raylib)
endforeach()