message(STATUS "[${SAVANNA_ACTIVE_TARGET}::Gfx] Processing Module [Savanna::Gfx::Vk2]")

collect_native_sources_in_current_dir(src RECURSIVE_LIMITED)

find_package(Vulkan REQUIRED)
target_include_directories(${SAVANNA_ACTIVE_TARGET} PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${SAVANNA_ACTIVE_TARGET} PUBLIC Vulkan::Vulkan)

target_sources(${SAVANNA_ACTIVE_TARGET} PUBLIC ${src})
target_include_directories(${SAVANNA_ACTIVE_TARGET} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# message(STATUS "[${SAVANNA_ACTIVE_TARGET}::Gfx] Building Shared Library [Savanna::Gfx::Vk2]")

# set(SAVANNA_PREVIOUS_TARGET ${SAVANNA_ACTIVE_TARGET})
# set(SAVANNA_ACTIVE_TARGET SavannaVk2)

# # Create a dynamic library target for the module
# add_library(${SAVANNA_ACTIVE_TARGET} SHARED "")

# # Add the source files to the target
# collect_native_sources_in_current_dir(src RECURSIVE_LIMITED)
# target_sources(${SAVANNA_ACTIVE_TARGET} PUBLIC ${src})
# target_include_directories(${SAVANNA_ACTIVE_TARGET} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# # Add the Vulkan SDK
# find_package(Vulkan REQUIRED)
# target_include_directories(${SAVANNA_ACTIVE_TARGET} PUBLIC ${Vulkan_INCLUDE_DIRS})
# target_link_libraries(${SAVANNA_ACTIVE_TARGET} PUBLIC Vulkan::Vulkan)

# # Link against Savanna.dll which is built in the root directory
# message (STATUS "[${SAVANNA_ACTIVE_TARGET}] Linking against [${SAVANNA_PREVIOUS_TARGET}]")
# add_dependencies(${SAVANNA_ACTIVE_TARGET} ${SAVANNA_PREVIOUS_TARGET})
# target_link_libraries(${SAVANNA_ACTIVE_TARGET} PUBLIC ${SAVANNA_PREVIOUS_TARGET})
# target_include_directories(${SAVANNA_ACTIVE_TARGET} PUBLIC ${CMAKE_SOURCE_DIR}/Runtime)

# # Sort the files into Filters based on Folder Hierarchy
# SOURCE_GROUP_BY_FOLDER()

# set_target_properties(${SAVANNA_ACTIVE_TARGET} PROPERTIES
#     RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}"
# )

# # Reset the active target
# set(SAVANNA_ACTIVE_TARGET ${SAVANNA_PREVIOUS_TARGET})
