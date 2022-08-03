cmake_minimum_required(VERSION 3.10)

# Specify clang as the compiler to ensure supported language standards
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang")

# Defines project settings
project(Savanna VERSION 0.1)

# Set Universal variables
set(module_path ${PROJECT_SOURCE_DIR}/../Buildscripts/CMake/Modules/*.cmake)

# Include all modules
file(GLOB_RECURSE modules ${module_path})
foreach(module ${modules})
    include("${module}")
endforeach()

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set General Standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# set(CMAKE_C_STANDARD 20)
# Set requirement to false due to symbol error with fmt when seeking c20
# set(CMAKE_C_STANDARD_REQUIRED False)

# Command to output information to the console
# Useful for displaying errors, warnings, and debugging
message ("\tcxx Flags:" ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_DEBUGGING_FLAGS})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../Builds/archives")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../Builds/libs")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../Builds/bin")

# Windows specific settings
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Core Projects
add_subdirectory(Core)

# Editor Project
add_subdirectory(Editor)

# Modules
add_subdirectory(Modules)

# Outside Application Testing Projects
add_subdirectory(Applications/DX12Renderer)
add_subdirectory(Applications/VulkanRenderer)
