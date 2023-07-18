message(STATUS "[${SAVANNA_ACTIVE_TARGET}::OS] Processing Module [${SAVANNA_ACTIVE_TARGET}::OS::Windows]")

collect_sources_in_current_dir(src RECURSIVE_LIMITED)

target_sources(${SAVANNA_ACTIVE_TARGET} PUBLIC ${src})
