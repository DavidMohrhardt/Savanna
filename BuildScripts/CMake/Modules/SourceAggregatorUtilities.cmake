cmake_minimum_required(VERSION 3.26 FATAL_ERROR)

# function to add all directories in result
function(SUBDIRLIST result firstdir curdir)
    file(GLOB endF6_SRC_TOP RELATIVE
        ${curdir} ${curdir}/*)
    file(GLOB_RECURSE endF6_SRC_NESTED ${curdir}/*)
    set(children ${endF6_SRC_TOP} ${endF6_SRC_NESTED})

    set(dirlist "${firstdir}")
    foreach(child ${children})
        if(IS_DIRECTORY ${curdir}/${child})
            LIST(APPEND dirlist ${curdir}/${child})
        endIF()
    endforeach()
    set(${result} ${dirlist} PARENT_SCOPE)
endfunction()

# Collects all source files in the given directory and potentially subdirectories
#
# Args
# OUT_SRC_LIST: The output source C++ files collected by this function.
# DIRECTORY: The input directory to search.
# [MODE]: Indicates the MODE in which to collect source files. Can be
# any of the following values:
#   - NONE: Will only attempt to get the source files at the current directory.
#   this is the default mode.
#   - RECURSIVE: Will attempt to get all source files in the current directory and subdirectory
#   only ignoring directories that contain a '~' (IE ./foo/Deprecated~).
#   - RECURSIVE_LIMITED: Will attempt to get all source files in the current directory and
#   its subdirectories ignoring any subdirectories that contain a CMakeLists.txt file and
#   directories that contain a '~' (IE ./foo/Deprecated~).
function(collect_sources_in_dir OUT_SRC_LIST DIRECTORY MODE)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs REGEXES)
    cmake_parse_arguments(COLLECT_SOURCES "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(recursion_requested FALSE)
    set(limited_recursion FALSE)
    if (${MODE} STREQUAL "RECURSIVE")
        set(recursion_requested TRUE)
    elseif(${MODE} STREQUAL "RECURSIVE_LIMITED")
        set(recursion_requested TRUE)
        set(limited_recursion TRUE)
    elseif(${MODE} STREQUAL "NONE")
    else()
        message(FATAL_ERROR "MODE argument must be either RECURSIVE, RECURSIVE_LIMITED, or NONE but was ${MODE}.")
    endif()

    if (recursion_requested)
        # Acquire all source files in this directory, This is evil for the record)
        foreach(regex ${COLLECT_SOURCES_REGEXES})
            file(GLOB_RECURSE regex_src_list ${DIRECTORY}/*${regex})
            list(APPEND src_list ${regex_src_list})
        endforeach()

        # filter all directories with a '~' as a general rule.
        list(FILTER src_list EXCLUDE REGEX "(.+)(~)(.+)")

        if (limited_recursion)
            # Get all subdirectories
            file(GLOB_RECURSE subdirs RELATIVE ${DIRECTORY} ${DIRECTORY}/*)

            # Filter out non-directories
            list(FILTER subdirs INCLUDE REGEX "/$")

            # Remove trailing slash from directory names
            list(TRANSFORM subdirs REPLACE "/$" "")

            # Exclude source files that are in subdirectories with a CMakeLists.txt file
            foreach(subdir ${subdirs})
                if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/CMakeLists.txt")
                    list(FILTER src_list EXCLUDE REGEX "^${subdir}/")
                endif()
            endforeach()
        endif(limited_recursion)

    else() # Not recursive,
        foreach(regex ${COLLECT_SOURCES_REGEXES})
            file(GLOB regex_src_list ${DIRECTORY}/*${regex})
            list(APPEND src_list ${regex_src_list})
        endforeach()
    endif()

    set(${OUT_SRC_LIST} ${src_list} PARENT_SCOPE)
endfunction(collect_sources_in_dir)

# Collects all source files in the current directory and potentially subdirectories.
# Calls collect_sources_in_dir with the ${CMAKE_CURRENT_LIST_DIR} argument as the
# directory argument.
#
# Args
# OUT_SRC_LIST: The output source C++ files collected by this function.
# directory: The input directory to search.
# [MODE]: Indicates the MODE in which to collect source files. Can be
# any of the following values:
#   - NONE: Will only attempt to get the source files at the current directory.
#   this is the default mode.
#   - RECURSIVE: Will attempt to get all source files in the current directory and subdirectory
#   only ignoring directories that contain a '~' (IE ./foo/Deprecated~).
#   - RECURSIVE_LIMITED: Will attempt to get all source files in the current directory and
#   its subdirectories ignoring any subdirectories that contain a CMakeLists.txt file and
#   directories that contain a '~' (IE ./foo/Deprecated~).
function(collect_sources_in_current_dir OUT_SRC_LIST MODE)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs REGEXES)
    cmake_parse_arguments(COLLECT_SOURCES "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    collect_sources_in_dir(src_list ${CMAKE_CURRENT_LIST_DIR} ${MODE} REGEXES ${COLLECT_SOURCES_REGEXES})

    set(${OUT_SRC_LIST} ${src_list} PARENT_SCOPE)
endfunction(collect_sources_in_current_dir)

# Collects all CPP/C source files in the current directory and potentially subdirectories.
# Calls collect_sources_in_dir with the ${CMAKE_CURRENT_LIST_DIR} argument as the
# directory argument.
#
# Args
# OUT_SRC_LIST: The output source C++ files collected by this function.
# directory: The input directory to search.
# [MODE]: Indicates the MODE in which to collect source files. Can be
# any of the following values:
#   - NONE: Will only attempt to get the source files at the current directory.
#   this is the default mode.
#   - RECURSIVE: Will attempt to get all source files in the current directory and subdirectory
#   only ignoring directories that contain a '~' (IE ./foo/Deprecated~).
#   - RECURSIVE_LIMITED: Will attempt to get all source files in the current directory and
#   its subdirectories ignoring any subdirectories that contain a CMakeLists.txt file and
#   directories that contain a '~' (IE ./foo/Deprecated~).
function(collect_native_sources_in_current_dir OUT_SRC_LIST MODE)
    collect_sources_in_current_dir(src_list ${MODE} REGEXES *.cpp *.h *.c *.cc *.hpp)
    set(${OUT_SRC_LIST} ${src_list} PARENT_SCOPE)
endfunction(collect_native_sources_in_current_dir)

# Calls target_include_directory on all directories with a CMakeLists.txt in Src/Runtime folder
# of the Savanna project. This is a highly specialized function and should not be used for any
# other purposes
#
# Args
# in_target: The input target to include the Savanna core includes directories.
function(include_savanna_directories TARGET DIRECTORY)
    # Special case to grab the DIRECTORY as well if needs be
    if(EXISTS "${DIRECTORY}/CMakeLists.txt")
        message (STATUS "Including ${DIRECTORY} in ${TARGET}")
        target_include_directories(${TARGET} PRIVATE ${DIRECTORY})
    endif()
    file(GLOB_RECURSE CMAKELISTS_FILES "${DIRECTORY}/*/CMakeLists.txt")
    foreach(CMAKELISTS_FILE ${CMAKELISTS_FILES})
        get_filename_component(SUBDIRECTORY ${CMAKELISTS_FILE} DIRECTORY)
        message (STATUS "Including ${SUBDIRECTORY} in ${TARGET}")
        target_include_directories(${TARGET} PRIVATE ${SUBDIRECTORY})
    endforeach()
endfunction()

# Using the source list, create a source group for each
# Will essentially help recreate the folder structure in
# Visual Studio instead of all cpp files being in one folder.
function(SOURCE_GROUP_BY_FOLDER)
    message(STATUS "Setting up source groups")
    # TODO: Maybe collect any kind of source instead of just CPP/C files
    collect_native_sources_in_current_dir(_source_list RECURSIVE)
    # message("Processing Source list: ${_source_list}")
    foreach(_source IN ITEMS ${_source_list})
        get_filename_component(_source_path "${_source}" PATH)
        string(REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_source_path}")
        string(REPLACE "/" "\\" _group_path "${_group_path}")
        source_group("${_group_path}" FILES "${_source}")
    endforeach()
endfunction(SOURCE_GROUP_BY_FOLDER)
