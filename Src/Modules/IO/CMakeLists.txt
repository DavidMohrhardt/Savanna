cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

set(core_libs SavannaCore) #SavannaGfx)
set(core_include_dirs ${CMAKE_SOURCE_DIR}/Modules/Core)
    # ${CMAKE_SOURCE_DIR}/Modules/Gfx)
set(external_libs )
set(external_include_dirs )
DECLARE_CORE_LIBRARY(SavannaIO "${core_libs}" "${core_include_dirs}" "${external_libs}" "${external_include_dirs}")
