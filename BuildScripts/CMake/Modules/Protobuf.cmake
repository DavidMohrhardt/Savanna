# This defines a series of helper functions to enqueue execution of Protobuf compilations prior to
# Compiling C++ source code.
cmake_minimum_required(VERSION 3.26 FATAL_ERROR)

function(generate_protobuf_source_files TARGET IN_PROTOBUF_SRCS PROTOBUF_LANG_GEN IN_PROTOBUF_ARGS)
    include(FindProtobuf)

    # Try to find the protobuf package
    find_package(Protobuf QUIET)

    # # If the package was not found, use FetchContent to download and build it
    if(NOT Protobuf_FOUND)
        # set(BUILD_protobuf TRUE)
        include(FetchContent)
        message(CHECK_START "Fetching Protobuf")
        list(APPEND CMAKE_MESSAGE_INDENT "  ")
        set(protobuf_BUILD_TESTS OFF)
        set(protobuf_BUILD_EXPORT OFF)
        set(protobuf_MSVC_STATIC_RUNTIME OFF)
        FetchContent_Declare(protobuf
            GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
            GIT_TAG        v23.4
            SOURCE_SUBDIR  cmake
            FIND_PACKAGE_ARGS NAMES protobuf)

        FetchContent_MakeAvailable(protobuf)

        # find_package(protobuf CONFIG REQUIRED)
        # find_package(protobuf REQUIRED)
    endif()

    if(NOT BUILD_Protobuf)
        find_package(Protobuf REQUIRED)
    endif()
    if(NOT TARGET protobuf::libprotobuf)
        message(FATAL_ERROR "Target protobuf::libprotobuf not available.")
    endif()

    ## Get Protobuf include dir
    get_target_property(protobuf_dirs protobuf::libprotobuf INTERFACE_INCLUDE_DIRECTORIES)
    foreach(dir IN LISTS protobuf_dirs)
    if ("${dir}" MATCHES "BUILD_INTERFACE")
        message(STATUS "Adding proto path: ${dir}")
        list(APPEND PROTO_DIRS "--proto_path=${dir}")
    endif()
    endforeach()

    foreach(PROTO_FILE IN LISTS proto_files)
        #message(STATUS "protoc proto(cc): ${PROTO_FILE}")
        get_filename_component(PROTO_DIR ${PROTO_FILE} DIRECTORY)
        get_filename_component(PROTO_NAME ${PROTO_FILE} NAME_WE)
        set(PROTO_HDR ${PROJECT_BINARY_DIR}/${PROTO_DIR}/${PROTO_NAME}.pb.h)
        set(PROTO_SRC ${PROJECT_BINARY_DIR}/${PROTO_DIR}/${PROTO_NAME}.pb.cc)
        #message(STATUS "protoc hdr: ${PROTO_HDR}")
        #message(STATUS "protoc src: ${PROTO_SRC}")
        add_custom_command(
            OUTPUT ${PROTO_SRC} ${PROTO_HDR}
            COMMAND protobuf::protoc
            "--proto_path=${PROJECT_SOURCE_DIR}"
            ${PROTO_DIRS}
            "--cpp_out=${PROJECT_BINARY_DIR}"
            ${PROTO_FILE}
            DEPENDS ${PROTO_FILE} protobuf::protoc
            COMMENT "Generate C++ protocol buffer for ${PROTO_FILE}"
            VERBATIM)
        list(APPEND PROTO_HDRS ${PROTO_HDR})
        list(APPEND PROTO_SRCS ${PROTO_SRC})
    endforeach()

    # if (${PROTOBUF_LANG_GEN} STREQUAL "CPP") # C++

    #     message(STATUS "[${TARGET}]: Generating Protobuf Native Source Files...")
    #     protobuf_generate_cpp(PROTO_SOURCES PROTO_HEADERS ${IN_PROTOBUF_SRCS} ${IN_PROTOBUF_ARGS})

    #     message ("PROTO_SOURCES: ${PROTO_SOURCES}")
    #     message ("PROTO_SOURCES: ${PROTO_HEADERS}")

    # # elseif(${PROTOBUF_LANG_GEN} STREQUAL "PYTHON") # Python
    # else()
    #     message(FATAL_ERROR "Unknown language option ${PROTOBUF_LANG_GEN}. Currently commands only support CPP and PYTHON while this engine only uses CPP.")
    # endif()

    # target_include_directories(${TARGET} PRIVATE ${Protobuf_INCLUDE_DIRS})
    # target_link_libraries(${TARGET} PRIVATE ${Protobuf_LIBRARIES})

    target_sources(${TARGET} PUBLIC ${PROTO_SOURCES} ${PROTO_HEADERS})

endfunction(generate_protobuf_source_files)
