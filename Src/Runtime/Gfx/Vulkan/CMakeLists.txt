message(STATUS "[${SAVANNA_ACTIVE_TARGET}::Gfx] Processing Module [Savanna::Gfx::Vk]")

collect_sources_in_current_dir(src RECURSIVE_LIMITED)

# TODO Also need to include the vulkan headers...

target_sources(${SAVANNA_ACTIVE_TARGET} PUBLIC ${src})
target_include_directories(${SAVANNA_ACTIVE_TARGET} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

find_package(Vulkan REQUIRED)
target_link_libraries(${SAVANNA_ACTIVE_TARGET} PUBLIC Vulkan::Vulkan)

# set(external_deps
#     Vulkan-Headers)

# foreach(dependency IN ITEMS ${external_deps})
#     message(STATUS "[${SAVANNA_ACTIVE_TARGET}::Gfx::Vk] Adding external dependency: ${dependency}...")
#     add_subdirectory(${CMAKE_SOURCE_DIR}/External/${dependency} ExternalProject)
# endforeach()

# target_include_directories(${SAVANNA_ACTIVE_TARGET} PUBLIC ${CMAKE_SOURCE_DIR}/External/Vulkan-Headers/include)

# TODO Some of this code has OS specific code in it. Ensure it's properly excluded.
