message(STATUS "[${SAVANNA_ACTIVE_TARGET}] Processing Module [${SAVANNA_ACTIVE_TARGET}::OS]")

collect_sources_in_current_dir(src RECURSIVE_LIMITED)

target_sources(${SAVANNA_ACTIVE_TARGET} PUBLIC ${src})
target_include_directories(${SAVANNA_ACTIVE_TARGET} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# Ensure the windows specific code gets added
if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Windows)
endif()
