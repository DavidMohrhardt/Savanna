message(STATUS "[${SAVANNA_ACTIVE_TARGET}::Gfx::Vk] Processing Module [${SAVANNA_ACTIVE_TARGET}::Gfx::Vk::Windows]")

collect_sources_in_current_dir(src RECURSIVE_LIMITED)

target_sources(${SAVANNA_ACTIVE_TARGET} PUBLIC ${src})
