# Finds Vulkan installation or fails with a warning message
macro(FIND_VULKAN_OR_FAIL)
    # Reference from https://github.com/PacktPublishing/Learning-Vulkan/blob/master/Chapter%2003/HandShake/CMakeLists.txt
    # AUTO_LOCATE_VULKAN - accepted value ON or OFF
    # ON  - Use CMake to auto locate the Vulkan SDK.
    # OFF - Vulkan SDK path can be specified manually. This is helpful to test the build on various Vulkan version.
    # option(AUTO_LOCATE_VULKAN "AUTO_LOCATE_VULKAN" ON) # doesn't work for whatever reason
    set(AUTO_LOCATE_VULKAN ON)

    if(AUTO_LOCATE_VULKAN)
        message(STATUS "Auto Locate Vulkan")

        # Find Vulkan Path using CMake's Vulkan Module
        # This will return Boolean 'Vulkan_FOUND' indicating the status of find as success(ON) or fail(OFF).
        # Include directory path - 'Vulkan_INCLUDE_DIRS' and 'Vulkan_LIBRARY' with required libraries.
        find_package(Vulkan)

        # Try extracting VulkanSDK path from ${Vulkan_INCLUDE_DIRS}
        if (NOT ${Vulkan_INCLUDE_DIRS} STREQUAL "")
            set(VULKAN_PATH ${Vulkan_INCLUDE_DIRS})
            STRING(REGEX REPLACE "/Include" "" VULKAN_PATH ${VULKAN_PATH})
        endif()

        if(NOT Vulkan_FOUND)
            # CMake may fail to locate the libraries but could be able to
            # provide some path in Vulkan SDK include directory variable
            # 'Vulkan_INCLUDE_DIRS', try to extract path from this.
            message(STATUS "Failed to locate Vulkan SDK, retrying again...")
            if(EXISTS "${VULKAN_PATH}")
                message(STATUS "Successfully located the Vulkan SDK: ${VULKAN_PATH}")
            else()
                message("Error: Unable to locate Vulkan SDK.")
            endif()
        endif()
    endif()
    if (NOT AUTO_LOCATE_VULKAN OR NOT Vulkan_FOUND)
        message(STATUS "Attempting to locate Vulkan SDK using manual path......")
        set(VULKAN_SDK "C:/dev/sdks/Vulkan")
        set(VULKAN_VERSION "1.3.216.0")
        set(VULKAN_PATH "${VULKAN_SDK}/${VULKAN_VERSION}")
        message(STATUS "Using manual specified path: ${VULKAN_PATH}")

        # Check if manual set path exists
        if(NOT EXISTS "${VULKAN_PATH}")
            message("Error: Unable to locate this Vulkan SDK path VULKAN_PATH: ${VULKAN_PATH}.")
        return()
        endif()
    endif()

    # Add any required preprocessor definitions here
    add_definitions(-DVK_USE_PLATFORM_WIN32_KHR)

    # vulkan-1 library for build Vulkan application.
    set(VULKAN_LIB_LIST "vulkan-1")

    if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        # Include Vulkan header files from Vulkan SDK
        include_directories(AFTER ${VULKAN_PATH}/Include)

        # Link directory for vulkan-1
        link_directories(${VULKAN_PATH}/Bin;${VULKAN_PATH}/Lib;)
    endif()
    # End of Reference from https://github.com/PacktPublishing/Learning-Vulkan/blob/master/Chapter%2003/HandShake/CMakeLists.txt
endmacro()
