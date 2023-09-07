message(STATUS "[${SAVANNA_ACTIVE_TARGET}] Processing Module [Savanna::Gfx]")

collect_native_sources_in_current_dir(src RECURSIVE_LIMITED)

target_sources(${SAVANNA_ACTIVE_TARGET} PUBLIC ${src})
target_include_directories(${SAVANNA_ACTIVE_TARGET} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

message(STATUS "[${SAVANNA_ACTIVE_TARGET}::Gfx] Processing GFX API subdirectories...")

# add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Materials)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Vulkan)
