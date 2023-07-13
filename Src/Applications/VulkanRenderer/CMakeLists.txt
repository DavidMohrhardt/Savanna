cmake_minimum_required(VERSION 3.26 FATAL_ERROR)
include(FetchContent)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
)
FetchContent_Declare(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm
)
FetchContent_MakeAvailable(glfw glm)

FIND_VULKAN_OR_FAIL()

# set(core_libs SavannaVk  SavannaConcurrency SavannaIO SavannaCore)
# set(core_libs SavannaCore SavannaIO SavannaConcurrency SavannaVk)
set(core_libs SavannaVk SavannaIO SavannaConcurrency SavannaCore)
set(core_include_dirs
        ${CMAKE_SOURCE_DIR}/Public
        ${CMAKE_SOURCE_DIR}/Modules/Core
        ${CMAKE_SOURCE_DIR}/Modules/IO
        ${CMAKE_SOURCE_DIR}/Modules/Application
        ${CMAKE_SOURCE_DIR}/Modules/Concurrency
        ${CMAKE_SOURCE_DIR}/Modules/Gfx/Vulkan)

set(external_libs glfw glm)
set(external_include_dirs)

CREATE_APP_WITH_LIBS(SavannaVkRenderer "${core_libs}" "${core_include_dirs}" "${external_libs}" "${external_include_dirs}")

# Copy Assets folder to install directory
add_custom_command(TARGET SavannaVkRenderer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_LIST_DIR}/Assets $<TARGET_FILE_DIR:SavannaVkRenderer>/Assets)


# Install the executable
install(TARGETS ${target_name} RUNTIME DESTINATION bin/Debug)

