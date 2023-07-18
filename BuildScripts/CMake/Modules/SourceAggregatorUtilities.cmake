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
# out_src_list: The output source C++ files collected by this function.
# directory: The input directory to search.
# [MODE]: Indicates the MODE in which to collect source files. Can be
# any of the following values:
#   - <NONE>: Will only attempt to get the source files at the current directory.
#   this is the default mode.
#   - RECURSIVE: Will attempt to get all source files in the current directory and subdirectory
#   only ignoring directories that contain a '~' (IE ./foo/Deprecated~).
#   - RECURSIVE_LIMITED: Will attempt to get all source files in the current directory and
#   its subdirectories ignoring any subdirectories that contain a CMakeLists.txt file and
#   directories that contain a '~' (IE ./foo/Deprecated~).
function(collect_sources_in_dir out_src_list directory)
    set(options RECURSIVE RECURSIVE_LIMITED)
    set(oneValueArgs)
    set(multiValueArgs)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(recursion_requested FALSE)
    if (ARG_RECURSIVE)
        set(recursion_requested TRUE)
    elseif(ARG_RECURSIVE_LIMITED)
        set(recursion_requested TRUE)
    endif()

    if (recursion_requested)
        # Acquire all source files in this directory, This is evil for the record)
        file(GLOB_RECURSE src_list
            ${directory}/*.c*
            ${directory}/*.h*)

        SUBDIRLIST(subdirs ${directory} ${directory})

        foreach (subdir IN ITEMS ${subdirs})
            if (${subdir} STREQUAL ${directory})
                continue()
            endif()

            # Check if the subdirectory contains a ~ in it's path
            if (${subdir} MATCHES "~")
                list(FILTER src_list EXCLUDE REGEX ${subdir})
                continue()
            endif()

            if (ARG_RECURSIVE_LIMITED)
                # # Check if the subdirectory has a CMakeLists.txt file
                file(GLOB CMakeLists ${subdir}/CMakeLists.txt)
                if (CMakeLists)
                    # Filter out the subdirectory from the source list
                    list(FILTER src_list EXCLUDE REGEX ${subdir})
                endif() # CMakeLists
            endif() # ARG_RECURSIVE_LIMITED

        endforeach()
    else() # Not recursive,
        file(GLOB src_list
            ${directory}/*.c*
            ${directory}/*.h*)
    endif()

    set(${out_src_list} ${src_list} PARENT_SCOPE)
endfunction(collect_sources_in_dir)

# Collects all source files in the current directory and potentially subdirectories.
# Calls collect_sources_in_dir with the ${CMAKE_CURRENT_LIST_DIR} argument as the
# directory argument.
#
# Args
# out_src_list: The output source C++ files collected by this function.
# directory: The input directory to search.
# [MODE]: Indicates the MODE in which to collect source files. Can be
# any of the following values:
#   - <NONE>: Will only attempt to get the source files at the current directory.
#   this is the default mode.
#   - RECURSIVE: Will attempt to get all source files in the current directory and subdirectory
#   only ignoring directories that contain a '~' (IE ./foo/Deprecated~).
#   - RECURSIVE_LIMITED: Will attempt to get all source files in the current directory and
#   its subdirectories ignoring any subdirectories that contain a CMakeLists.txt file and
#   directories that contain a '~' (IE ./foo/Deprecated~).
function(collect_sources_in_current_dir out_src_list)
    set(options RECURSIVE RECURSIVE_LIMITED)
    set(oneValueArgs)
    set(multiValueArgs)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (ARG_RECURSIVE)
        collect_sources_in_dir(src_list ${CMAKE_CURRENT_LIST_DIR} RECURSIVE)
    elseif(ARG_RECURSIVE_LIMITED)
        collect_sources_in_dir(src_list ${CMAKE_CURRENT_LIST_DIR} RECURSIVE_LIMITED)
    else() # Not recursive
        collect_sources_in_dir(src_list ${CMAKE_CURRENT_LIST_DIR})
    endif()

    set(${out_src_list} ${src_list} PARENT_SCOPE)
endfunction(collect_sources_in_current_dir)

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
    collect_sources_in_current_dir(_source_list RECURSIVE)
    # message("Processing Source list: ${_source_list}")
    foreach(_source IN ITEMS ${_source_list})
        get_filename_component(_source_path "${_source}" PATH)
        string(REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_source_path}")
        string(REPLACE "/" "\\" _group_path "${_group_path}")
        source_group("${_group_path}" FILES "${_source}")
    endforeach()
endfunction(SOURCE_GROUP_BY_FOLDER)
