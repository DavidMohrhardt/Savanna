cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

set(LIBRARY_NAME IO)

set(UNDEFINED OFF)

if (UNDEFINED)
    message("Detected default static library template in use. Ensure that you have set LIBRARY_NAME set and marked UNDEFINED as OFF in your CMAKE file.")
    return()
endif()

# Include all modules
file(GLOB_RECURSE modules ${PROJECT_SOURCE_DIR}/Buildscripts/CMake/Modules/*.cmake)
foreach(module ${modules})
    include("${module}")
endforeach()

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(ENGINE_MODULES_DIR ${CMAKE_SOURCE_DIR}/Modules)

# Acquire all subdirectories, This is evil for the record)
file(GLOB_RECURSE src
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/*.c
    ${CMAKE_CURRENT_LIST_DIR}/*.h
    ${CMAKE_CURRENT_LIST_DIR}/*.hpp)

# Filter any directory that contains a '~' in it's path.
list(FILTER src EXCLUDE REGEX "${CMAKE_CURRENT_LIST_DIR}/(.+)~(.+)")

# Sort the files into Filters based on Folder Hierarchy
SOURCE_GROUP_BY_FOLDER()

# Set the target static libraries
set(target_static_libs Core)

# Add Library
add_library(${LIBRARY_NAME} STATIC ${src})

target_include_directories(
    ${LIBRARY_NAME}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${ENGINE_MODULES_DIR}/Core)

target_link_libraries(${app_name} ${target_static_libs}) #${target_shared_libs})
add_dependencies(${LIBRARY_NAME} ${target_static_libs}) #${target_shared_libs})

# Install Binaries in Build Folder
install(TARGETS ${LIBRARY_NAME}
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
