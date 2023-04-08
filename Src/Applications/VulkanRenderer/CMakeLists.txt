cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
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

set(core_libs SavannaApplication SavannaVk SavannaIO SavannaCore)
set(core_include_dirs ${CMAKE_SOURCE_DIR}/Modules/Core/ ${CMAKE_SOURCE_DIR}/Modules/IO ${CMAKE_SOURCE_DIR}/Modules/Gfx/Vulkan/)
set(external_libs glfw glm)
set(external_include_dirs)
CREATE_APP_WITH_LIBS(SavannaVkRenderer "${core_libs}" "${core_include_dirs}" "${external_libs}" "${external_include_dirs}")
