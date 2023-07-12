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
