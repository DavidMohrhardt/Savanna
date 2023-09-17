cmake_minimum_required(VERSION 3.26 FATAL_ERROR)

set(SAVANNA_ACTIVE_TARGET SavannaRendererVulkan)

message(STATUS "Building [${SAVANNA_ACTIVE_TARGET}] Application")

include(FetchContent)

message (STATUS "Fetching glfw")
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw
)

message (STATUS "Fetching glm")
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
)

FetchContent_MakeAvailable(glfw glm)
    #protobuf)

collect_native_sources_in_current_dir(src RECURSIVE_LIMITED)

# Sort the files into Filters based on Folder Hierarchy
SOURCE_GROUP_BY_FOLDER()

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add Executable
add_executable(${SAVANNA_ACTIVE_TARGET} ${src})

# Link libraries
# TODO Make find Vulkan QUIET and enable the runtime loader for vulkan in the case it's not just defined
find_package(Vulkan REQUIRED)

# add_dependencies(   ${SAVANNA_ACTIVE_TARGET}
#                     Savanna)

target_link_libraries(  ${SAVANNA_ACTIVE_TARGET}
                        Savanna)

target_link_libraries(  ${SAVANNA_ACTIVE_TARGET}
                        glfw
                        glm
                        Vulkan::Vulkan)
                        # protobuf::libprotobuf)

include_savanna_directories(${SAVANNA_ACTIVE_TARGET}
                            ${CMAKE_SOURCE_DIR}/Runtime)

target_include_directories( ${SAVANNA_ACTIVE_TARGET}
                            PUBLIC ${Vulkan_INCLUDE_DIRS})
                            # PUBLIC ${CMAKE_SOURCE_DIR}/Runtime)

# Copy Assets folder to install directory
add_custom_command( TARGET ${SAVANNA_ACTIVE_TARGET} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_CURRENT_LIST_DIR}/Assets/Shaders/SPIRV $<TARGET_FILE_DIR:${SAVANNA_ACTIVE_TARGET}>/Assets/SPIRV)

# Install the executable
install(TARGETS ${SAVANNA_ACTIVE_TARGET}
            EXPORT ${SAVANNA_ACTIVE_TARGET}Targets
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
