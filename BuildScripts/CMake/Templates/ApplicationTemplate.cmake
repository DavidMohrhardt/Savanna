cmake_minimum_required(VERSION 3.26 FATAL_ERROR)

# Set Application Name
set(app_name SavannaDX12)

message("Generating ${app_name}")

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

# Add Executable
add_executable(${app_name} WIN32 ${src})

set(target_static_libs
    Core)

target_include_directories(${app_name} PUBLIC ${CMAKE_SOURCE_DIR}/Core)
target_link_libraries(${app_name} ${target_static_libs})

# Install Binaries in Build Folder
install(TARGETS ${app_name} RUNTIME DESTINATION bin)
