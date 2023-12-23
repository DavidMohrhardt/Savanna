message(STATUS "[${SAVANNA_ACTIVE_TARGET}::Gfx] Processing Module [savanna::Gfx::Vk2]")

collect_native_sources_in_current_dir(src RECURSIVE_LIMITED)

target_sources(${SAVANNA_ACTIVE_TARGET} PUBLIC ${src})
target_include_directories(${SAVANNA_ACTIVE_TARGET} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_include_directories(${SAVANNA_ACTIVE_TARGET} PUBLIC ${SAVANNA_EXTERNAL_FOLDER}/Vulkan-Headers/include)
