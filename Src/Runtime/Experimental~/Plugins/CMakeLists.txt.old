cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

# Module dependencies
set(core_libs SavannaCore SavannaOS)
set(core_include_dirs "${CMAKE_SOURCE_DIR}/Modules/Core" "${CMAKE_SOURCE_DIR}/Modules/OS")
set(external_libs "")
set(external_include_dirs "")

if (TARGET_WIN)
    set (external_libs ${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION})
    message(STATUS "Windows Kits 11 dir: ${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}")
endif()

DECLARE_CORE_LIBRARY(SavannaPlugin "${core_libs}" "${core_include_dirs}" "${external_libs}" "${external_include_dirs}")
