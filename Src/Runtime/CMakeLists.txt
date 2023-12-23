cmake_minimum_required(VERSION 3.26 FATAL_ERROR)
include(GenerateExportHeader)

set(SAVANNA_ACTIVE_TARGET "Savanna")
message(STATUS "Building [${SAVANNA_ACTIVE_TARGET}] ${SAVANNA_CORE_LIBRARY_TYPE} Library")

add_library(${SAVANNA_ACTIVE_TARGET} ${SAVANNA_CORE_LIBRARY_TYPE})

# Ensure the input header is produced
CONFIG_INPUT_HEADER(${SAVANNA_ACTIVE_TARGET})

# Setup the export definition
target_compile_definitions(${SAVANNA_ACTIVE_TARGET} PRIVATE SAVANNA_EXPORTS)

# Get the native source files
collect_native_sources_in_current_dir(src RECURSIVE_LIMITED)

target_sources(${SAVANNA_ACTIVE_TARGET} PUBLIC ${src})
target_include_directories(${SAVANNA_ACTIVE_TARGET} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

set(subdir_list
    Concurrency
    Entities
    Gfx
    IO
    OS
    Scripting)

foreach(subdir IN ITEMS ${subdir_list})
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/${subdir})
endforeach()

# Sort the files into Filters based on Folder Hierarchy
SOURCE_GROUP_BY_FOLDER()

set_target_properties(${SAVANNA_ACTIVE_TARGET} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}"
)

install(TARGETS ${SAVANNA_ACTIVE_TARGET}
        EXPORT ${SAVANNA_ACTIVE_TARGET}Targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
