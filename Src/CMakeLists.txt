cmake_minimum_required(VERSION 3.21)

# Defines project settings
project(Savanna VERSION 0.1)

# TODO @DavidMohrhardt - Make multiple scripts for different platforms
# If on windows then run Prepare.ps1 which exists in the root directory
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    # make sure to execute process from the root directory
    execute_process(COMMAND powershell -ExecutionPolicy Bypass -File ${PROJECT_SOURCE_DIR}/../Prepare.ps1)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Specify clang as the compiler to ensure supported language standards
# TODO @DavidMohrhardt - For some reason this triggers a configuration change that causes the project to rebuild every time. Disabling for now.
# set(CMAKE_C_COMPILER "llvm")
# set(CMAKE_CXX_COMPILER "llvm++")

# Set Global Variables
# set(CORE_LIBRARY_KIND "STATIC" CACHE STRING "Set the kind of library to build (STATIC, SHARED, MODULE)")
set(CORE_LIBRARY_KIND "SHARED" CACHE STRING "Set the kind of library to build (STATIC, SHARED, MODULE)")
set(MULTI_COMPILE_LIBRARIES OFF CACHE BOOL "Set to ON to compile object libraries for each library and then create both static and shared libraries from the object libraries. Set to OFF to compile each library with the definition of ${CORE_LIBRARY_KIND}.")

if (CORE_LIBRARY_KIND STREQUAL "SHARED")
    set(EXPORT_ALL_SYMBOLS ON CACHE BOOL "Export all symbols for dynamic libraries")
else()
    set(EXPORT_ALL_SYMBOLS OFF CACHE BOOL "Export all symbols for dynamic libraries")
endif()

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
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

# Command to output information to the console
# Useful for displaying errors, warnings, and debugging
message ("\tcxx Flags:" ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_DEBUGGING_FLAGS})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../Builds/archives")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../Builds/libs")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../Builds/bin")

# Editor Project

# Modules
add_subdirectory(Modules)

# Tests
add_subdirectory(Tests)

# Outside Application Testing Projects
# add_subdirectory(Applications/DX12Renderer)
add_subdirectory(Applications/VulkanRenderer)
