cmake_minimum_required(VERSION 3.10)

# macro to add all directories in result
macro(SUBDIRLIST result firstdir curdir)
    file(GLOB endF6_SRC_TOP RELATIVE
        ${curdir} ${curdir}/*)
    file(GLOB_RECURSE endF6_SRC_NESTED ${curdir}/*)
    set(children ${endF6_SRC_TOP} ${endF6_SRC_NESTED})

    set(dirlist "${firstdir}")
    foreach(child ${children})
        IF(IS_DIRECTORY ${curdir}/${child})
            LIST(APPEND dirlist ${curdir}/${child})
        endIF()
    endforeach()
    set(${result} ${dirlist})
endmacro()

function(include_all_files)
    SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

    foreach(subdir ${SUBDIRS})
        include_directories(${subdir})
    endforeach()
endfunction()

macro(GLOB_TARGET_INCLUDES includes_target scope dir)
    #argv[0] - result variable, argv[1] - add relative paths, set "" if you don't want it
    #argv[2] - path to folder with folders
    SUBDIRLIST(SUBDIRS ${dir} ${dir})

    foreach(subdir ${SUBDIRS})
        target_include_directories(${includes_target} ${scope} subdir)
    endforeach(subdir ${SUBDIRS})
endmacro()

macro(SOURCE_GROUP_BY_FOLDER)
    file(GLOB_RECURSE _source_list *.cpp* *.h* *.hpp*)
    foreach(_source IN ITEMS ${_source_list})
        get_filename_component(_source_path "${_source}" PATH)
        string(REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_source_path}")
        string(REPLACE "/" "\\" _group_path "${_group_path}")
        source_group("${_group_path}" FILES "${_source}")
    endforeach()
endmacro(SOURCE_GROUP_BY_FOLDER)

macro(SOURCE_GROUP_BY_FOLDER)
    file(GLOB_RECURSE _source_list *.cpp* *.h* *.hpp*)
    foreach(_source IN ITEMS ${_source_list})
        get_filename_component(_source_path "${_source}" PATH)
        string(REPLACE "${CMAKE_CURRENT_LIST_DIR}" "" _group_path "${_source_path}")
        string(REPLACE "/" "\\" _group_path "${_group_path}")
        source_group("${_group_path}" FILES "${_source}")
    endforeach()
endmacro(SOURCE_GROUP_BY_FOLDER)

macro(SOURCE_GROUP_BY_DIRECTORY_HIERARCHY dir)
    file(GLOB_RECURSE _source_path *.cpp* *.c* *.h* *.hpp*)
    foreach(_source IN ITEMS ${_source_list})
        get_filename_component(_source_path "${_source}" PATH)
        string(REPLACE "${dir}" "" _group_path "${_source_path}")
        string(REPLACE "/" "\\" _group_path "${_group_path}")
        source_group("${_group_path}" FILES "${_source}")
    endforeach()
endmacro(SOURCE_GROUP_BY_DIRECTORY_HIERARCHY)

# function(add_out_of_source_dependency_proj input output)
#     # argv[0] - input directory,
#     # argv[1] - output directory
#     add_subdirectory(${input} ${output})
# endfunction()
