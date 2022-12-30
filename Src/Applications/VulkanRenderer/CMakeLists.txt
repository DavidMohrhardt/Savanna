cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
include(FetchContent)

# Set Application Name
set(app_name SavannaVulkanRenderer)

# Define specific app modules
set(app_modules ${CMAKE_CURRENT_LIST_DIR}/*.cmake)

message("Generating ${app_name}")

# Find Vulkan with expanded macro
FIND_VULKAN_OR_FAIL()

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Acquire all subdirectories, This is evil for the record
file(GLOB_RECURSE src
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/*.c
    ${CMAKE_CURRENT_LIST_DIR}/*.h
    ${CMAKE_CURRENT_LIST_DIR}/*.hpp)

# Filter any directory that contains a '~' in it's path.
list(FILTER src EXCLUDE REGEX "${CMAKE_CURRENT_LIST_DIR}/(.+)~(.+)")

# Sort the files into Filters based on Folder Hierarchy
SOURCE_GROUP_BY_FOLDER()

# Add Executable
add_executable(${app_name} ${src})

# declare_and_fetch(CONTENT_ID "glfw" GIT_REPOSITORY https://github.com/glfw/glfw GIT_TAG "3.3.6" GIT_SHALLOW ON)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
)

FetchContent_GetProperties(glfw)
if(NOT glfw_POPULATED)
    FetchContent_Populate(glfw)

    set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
    set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
    set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
    set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")

    add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif()

set (HELPER_LIB_LIST glfw)

set(target_static_libs
    Core
	# SavannaVulkan_static
    ${VULKAN_LIB_LIST}
	${HELPER_LIB_LIST})

# Set the shared libs as well
set(target_shared_libs
	SavannaVk)

set(core_include_dirs
	# Savanna Core
	${CMAKE_SOURCE_DIR}/Core
	${CMAKE_SOURCE_DIR}/Modules/Rendering
	${CMAKE_SOURCE_DIR}/Modules/Rendering/Vulkan)

# TODO change when unpacking artifacts is normalized
add_subdirectory(
	${PROJECT_SOURCE_DIR}/../External/glm-0.9.9.8/glm
	${PROJECT_SOURCE_DIR}/../External/bin/glm)

message("Vulkan libs ${VULKAN_PATH}/Include")

target_include_directories(
	${app_name}
	PUBLIC
	# Core Libraries
	${core_include_dirs}
	# Vulkan
	${VULKAN_PATH}/Include
	# glm
	${PROJECT_SOURCE_DIR}/../External/glm-0.9.9.8/glm/glm
	# glfw3
	${PROJECT_SOURCE_DIR}/../External/glfw-3.3.6.bin.WIN64/glfw-3.3.6.bin.WIN64/include)

target_link_libraries(${app_name} ${target_static_libs} ${target_shared_libs})

# Define project properties
set_property(TARGET ${Recipe_Name} PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/binaries)
set_property(TARGET ${Recipe_Name} PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/binaries)
set_property(TARGET ${Recipe_Name} PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/binaries)
set_property(TARGET ${Recipe_Name} PROPERTY RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_CURRENT_SOURCE_DIR}/binaries)
set_property(TARGET ${Recipe_Name} PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}/binaries)

# Define C++ version to be used for building the project
set_property(TARGET ${Recipe_Name} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${Recipe_Name} PROPERTY CXX_STANDARD_REQUIRED ON)

# Define C version to be used for building the project
set_property(TARGET ${Recipe_Name} PROPERTY C_STANDARD 99)
set_property(TARGET ${Recipe_Name} PROPERTY C_STANDARD_REQUIRED ON)

# Install Binaries in Build Folder
install(TARGETS ${app_name} RUNTIME DESTINATION bin)
