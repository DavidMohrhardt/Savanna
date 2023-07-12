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

set(SEPARATE_ENGINE_MODULES_TO_LIBRARIES OFF CACHE BOOL "Separate engine modules to libraries" FORCE)
set(ALLOW_GLOBBING_SUBDIRS_WITH_CMAKELISTS OFF CACHE BOOL "Allow globbing subdirectories with CMakeLists.txt files" FORCE)

if (SEPARATE_ENGINE_MODULES_TO_LIBRARIES)
    message(STATUS "Separating engine modules to libraries")
    # Code Modules
    add_subdirectory(Core)
    add_subdirectory(Concurrency)
    # add_subdirectory(Application)
    # add_subdirectory(Engine)
    add_subdirectory(Entities)
    add_subdirectory(Gfx)
    add_subdirectory(IO)
    # add_subdirectory(Memory)
    # TODO @DavidMohrhardt - Move profiling to a module
    # add_subdirectory(Profiling)

    add_subdirectory(OS)
else()
    message(STATUS "Engine Modules as one universal binary")
    set(core_libs "")
    set(core_include_dirs "")
    set(external_libs "")
    set(external_include_dirs "")
    DECLARE_CORE_LIBRARY(Savanna "${core_libs}" "${core_include_dirs}" "${external_libs}" "${external_include_dirs}")
endif()
