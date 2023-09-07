message(STATUS "[${SAVANNA_ACTIVE_TARGET}::Gfx] Processing Module [Savanna::Gfx::Vk]")

collect_native_sources_in_current_dir(src RECURSIVE_LIMITED)

find_package(Vulkan REQUIRED)
target_include_directories(${SAVANNA_ACTIVE_TARGET} PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${SAVANNA_ACTIVE_TARGET} PUBLIC Vulkan::Vulkan)

target_sources(${SAVANNA_ACTIVE_TARGET} PUBLIC ${src})
target_include_directories(${SAVANNA_ACTIVE_TARGET} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# Ensure the windows specific code gets added
if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Windows)
endif()

# Define runtime vulkan
# set(external_deps
#     Vulkan-Headers)

# foreach(dependency IN ITEMS ${external_deps})
#     message(STATUS "[${SAVANNA_ACTIVE_TARGET}::Gfx::Vk] Adding external dependency: ${dependency}...")
#     add_subdirectory(${CMAKE_SOURCE_DIR}/External/${dependency} ExternalProject)
# endforeach()

# target_include_directories(${SAVANNA_ACTIVE_TARGET} PUBLIC ${CMAKE_SOURCE_DIR}/External/Vulkan-Headers/include)

# TODO Some of this code has OS specific code in it. Ensure it's properly excluded.
