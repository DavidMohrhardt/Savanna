cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

# Module dependencies
set(core_libs )
set(core_include_dirs "")
    # ${CMAKE_SOURCE_DIR}/Modules/Core
    # ${CMAKE_SOURCE_DIR}/Modules/IO)
    # ${CMAKE_SOURCE_DIR}/Modules/Gfx)
# set(external_libs ${VULKAN_LIB_LIST} ${HELPER_LIB_LIST})
# set(external_include_dirs "")
DECLARE_CORE_LIBRARY(SavannaCoreModule "${core_libs}" "${core_include_dirs}" "${external_libs}" "${external_include_dirs}")
