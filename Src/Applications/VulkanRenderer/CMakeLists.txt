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

set(SAVANNA_ACTIVE_TARGET SavannaVkRenderer)

message(STATUS "Building [${SAVANNA_ACTIVE_TARGET}] Application")

# set(core_libs Savanna)
# set(external_libs glfw glm)
# set(external_include_dirs)

message(STATUS "Processing: ${SAVANNA_ACTIVE_TARGET}")

collect_sources_in_current_dir(src RECURSIVE_LIMITED)

# Sort the files into Filters based on Folder Hierarchy
SOURCE_GROUP_BY_FOLDER()

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add Executable
add_executable(${SAVANNA_ACTIVE_TARGET} ${src})

# Link libraries
find_package(Vulkan REQUIRED)

add_dependencies(${SAVANNA_ACTIVE_TARGET} Savanna)
target_link_libraries(${SAVANNA_ACTIVE_TARGET} Savanna glfw glm Vulkan::Vulkan)
include_savanna_directories(${SAVANNA_ACTIVE_TARGET} ${CMAKE_SOURCE_DIR}/Runtime)
target_include_directories(${SAVANNA_ACTIVE_TARGET} PUBLIC ${Vulkan_INCLUDE_DIRS})

# Copy Assets folder to install directory
add_custom_target(TARGET )
add_custom_command(TARGET ${SAVANNA_ACTIVE_TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_LIST_DIR}/Assets/Shaders/SPIRV $<TARGET_FILE_DIR:${SAVANNA_ACTIVE_TARGET}>/Assets/SPIRV)

# Install the executable
install(TARGETS ${SAVANNA_ACTIVE_TARGET}
            EXPORT ${SAVANNA_ACTIVE_TARGET}Targets
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
