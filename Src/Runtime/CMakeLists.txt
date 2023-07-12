cmake_minimum_required(VERSION 3.21)

# Include all modules
file(GLOB_RECURSE modules ${PROJECT_SOURCE_DIR}/Buildscripts/CMake/Modules/*.cmake)
foreach(module ${modules})
    include("${module}")
endforeach()

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Windows specific settings
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# set(SEPARATE_ENGINE_MODULES_TO_LIBRARIES OFF CACHE BOOL "Separate engine modules to libraries" FORCE)
# set(ALLOW_GLOBBING_SUBDIRS_WITH_CMAKELISTS OFF CACHE BOOL "Allow globbing subdirectories with CMakeLists.txt files" FORCE)

message(STATUS "Separating engine modules to libraries")
# Code Modules
# add_subdirectory(Concurrency)
# add_subdirectory(Entities)
# add_subdirectory(Gfx)
# add_subdirectory(IO)
# add_subdirectory(OS)
