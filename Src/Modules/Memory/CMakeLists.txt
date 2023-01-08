cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(LIBRARY_NAME Memory)

set(UNDEFINED OFF)

if (UNDEFINED)
    message("Detected default static library template in use. Ensure that you have set LIBRARY_NAME and marked UNDEFINED as OFF in your CMAKE file.")
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

# Acquire all source files in this directory, This is evil for the record)
file(GLOB src
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/*.c
    ${CMAKE_CURRENT_LIST_DIR}/*.h
    ${CMAKE_CURRENT_LIST_DIR}/*.hpp)

message ("before filter")
message ("${src}")

# Filter any directory that contains a '~' in it's path.
list(FILTER src EXCLUDE REGEX "${CMAKE_CURRENT_LIST_DIR}/(.+)~(.+)")

message ("after filter")
message ("${src}")

# file(GLOB_RECURSE cmake_lists ${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt)

# message ("after recurse")
# message ("${src}")

# message("Removing ${CMAKE_CURRENT_LIST_FILE} from list")
# list(REMOVE_ITEM cmake_lists "${CMAKE_CURRENT_LIST_FILE}")
# cmake_path(GET cmake_lists PARENT_PATH cmake_containing_directories)

# message ("after remove cmakes")
# message ("${src}")

# # # Filter Subfolders with explicit cmake txt files
# list(FILTER src EXCLUDE REGEX "${cmake_containing_directories}/.+")

# message ("after remove src from subfolders")
# message ("${src}")

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

# Install Binaries in Build Folder
install(TARGETS Memory
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
