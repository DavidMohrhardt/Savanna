cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

FIND_VULKAN_OR_FAIL()
set(core_libs SavannaIO SavannaConcurrency SavannaCore)
message("Core libs: ${core_libs}")
set(core_include_dirs
    ${CMAKE_SOURCE_DIR}/Public
    ${CMAKE_SOURCE_DIR}/Modules/Core
    ${CMAKE_SOURCE_DIR}/Modules/Concurrency
    ${CMAKE_SOURCE_DIR}/Modules/IO
    ${CMAKE_SOURCE_DIR}/Modules/Gfx)
set(external_libs ${VULKAN_LIB_LIST} ${HELPER_LIB_LIST})
set(external_include_dirs "")
DECLARE_CORE_LIBRARY(SavannaVk "${core_libs}" "${core_include_dirs}" "${external_libs}" "${external_include_dirs}")
