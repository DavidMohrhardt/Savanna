message(STATUS "[${SAVANNA_ACTIVE_TARGET}] Processing Module [savanna::Entities]")

collect_native_sources_in_current_dir(src RECURSIVE_LIMITED)

target_sources(${SAVANNA_ACTIVE_TARGET} PUBLIC ${src})
target_include_directories(${SAVANNA_ACTIVE_TARGET} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
