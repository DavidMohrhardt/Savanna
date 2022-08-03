cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

set(LIBRARY_NAME SavannaVulkan)

# Include all modules
file(GLOB_RECURSE modules ${PROJECT_SOURCE_DIR}/Buildscripts/CMake/Modules/*.cmake)
foreach(module ${modules})
    include("${module}")
endforeach()

# Find Vulkan with expanded macro
FIND_VULKAN_OR_FAIL()

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

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
set(target_static_libs
    Core
    ${VULKAN_LIB_LIST}
	${HELPER_LIB_LIST})

# Add Library
message("src ${CMAKE_SOURCE_DIR}/Rendering")

# add_library(${LIBRARY_NAME}_shared OBJECT ${src})
# target_include_directories(${LIBRARY_NAME}_shared PUBLIC
#     ${CMAKE_CURRENT_LIST_DIR}
#     ${CMAKE_SOURCE_DIR}/Rendering)
# target_link_libraries(${LIBRARY_NAME}_shared ${target_static_libs})

# add_library(${LIBRARY_NAME}_static STATIC ${src})
# target_include_directories(${LIBRARY_NAME}_shared PUBLIC
#     ${CMAKE_CURRENT_LIST_DIR}
#     ${CMAKE_SOURCE_DIR}/Rendering)
# target_link_libraries(${LIBRARY_NAME}_static ${target_static_libs})

add_library(${LIBRARY_NAME} SHARED ${src})

target_include_directories(
    ${LIBRARY_NAME}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_SOURCE_DIR}/Rendering)

target_link_libraries(${LIBRARY_NAME} ${target_static_libs})

set(LIBRARIES ${LIBRARY_NAME})#_static ${LIBRARY_NAME}_shared)

# Install Binaries in Build Folder
install(TARGETS ${LIBRARIES}
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
