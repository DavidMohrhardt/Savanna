cmake_minimum_required(VERSION 3.26 FATAL_ERROR)

set(SAVANNA_ACTIVE_TARGET "Savanna")
CONFIG_INPUT_HEADER(${SAVANNA_ACTIVE_TARGET})

collect_sources_in_current_dir(src RECURSIVE_LIMITED)

add_library(${SAVANNA_ACTIVE_TARGET} SHARED ${src})

target_include_directories(${SAVANNA_ACTIVE_TARGET} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

set(subdir_list
    Concurrency
    Entities
    Gfx
    IO)

foreach(subdir IN ITEMS ${subdir_list})
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/${subdir})
endforeach()
