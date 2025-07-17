cmake_minimum_required(VERSION 3.16)
project(PegQt VERSION 1.0 LANGUAGES CXX)

set(CMAKE_OSX_DEPLOYMENT_TARGET "15.5" CACHE STRING "Minimum macOS deployment version")
set(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
set(MACOSX_BUNDLE_GUI_IDENTIFIER com.diamond.${PROJECT_NAME})
set(MACOSX_BUNDLE_LONG_VERSION_STRING "1.0")
set(MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0")
set(MACOSX_BUNDLE_BUNDLE_VERSION "1.0")
set(MACOSX_BUNDLE_COPYRIGHT "© 2025 Heber Diamond")


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt")
set(CMAKE_AUTOMOC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Collect all source and header files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/**/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_SOURCE_DIR}/src/**/*.hpp"
)

# Collect include directories
file(GLOB_RECURSE HEADER_PATHS "${CMAKE_SOURCE_DIR}/src/*.hpp")

set(INCLUDE_DIRS "")
foreach(header ${HEADER_PATHS})
    get_filename_component(dir ${header} DIRECTORY)
    list(APPEND INCLUDE_DIRS ${dir})
endforeach()
list(REMOVE_DUPLICATES INCLUDE_DIRS)

# Create executable
add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SOURCES} src/icon.icns)

set(MACOSX_BUNDLE_ICON_FILE icon.icns)
set_source_files_properties(src/icon.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")


# Add include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})

# Link Qt libraries
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

target_precompile_headers(PegQt PRIVATE src/helper/pch.hpp)
