cmake_minimum_required(VERSION 3.10)

include(FetchContent)

# this was a neat idea but it doesn't work the way I want it to
macro(declare_and_fetch)

    set(options)
    set(oneValueArgs CONTENT_ID GIT_REPOSITORY GIT_TAG GIT_SHALLOW)
    set(multiValueArgs)
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" ${ARGN} )

    message("Fetching content id ${ARGS_CONTENT_ID} repo ${ARGS_GIT_REPOSITORY} tag ${ARGS_GIT_TAG} shallow op ${ARGS_GIT_SHALLOW}")

    # Copied from https://jonathanhamberg.com/post/2019-01-27-cmake-dependencies-fetch-content/
    # Thanks Jonathan!
    FetchContent_Declare(
        ${ARGS_CONTENT_ID}
        GIT_REPOSITORY ${ARGS_GIT_REPOSITORY}
        GIT_TAG ${ARGS_GIT_TAG}
        GIT_SHALLOW ${ARGS_GIT_SHALLOW}
    )

    FetchContent_GetProperties(${ARGS_CONTENT_ID})

    if(NOT ${ARGS_CONTENT_ID}_POPULATED)
        FetchContent_Populate(${ARGS_CONTENT_ID})

        set(${ARGS_CONTENT_ID}_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the ${ARGS_CONTENT_ID} example programs")
        set(${ARGS_CONTENT_ID}_BUILD_TESTS OFF CACHE INTERNAL "Build the ${ARGS_CONTENT_ID} test programs")
        set(${ARGS_CONTENT_ID}_BUILD_DOCS OFF CACHE INTERNAL "Build the ${ARGS_CONTENT_ID} documentation")
        set(${ARGS_CONTENT_ID}INSTALL OFF CACHE INTERNAL "Generate installation target")

        add_subdirectory(${${ARGS_CONTENT_ID}_SOURCE_DIR} ${${ARGS_CONTENT_ID}_BINARY_DIR})
    endif()
endmacro()
