# Using the source list, create a source group for each
# Will essentially help recreate the folder structure in
# Visual Studio instead of all cpp files being in one folder.
function(SOURCE_GROUP_BY_FOLDER)
    message(STATUS "Setting up source groups")
    file(GLOB_RECURSE _source_list *.cpp* *.h* *.inl*)
    # message("Processing Source list: ${_source_list}")
    foreach(_source IN ITEMS ${_source_list})
        get_filename_component(_source_path "${_source}" PATH)
        string(REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_source_path}")
        string(REPLACE "/" "\\" _group_path "${_group_path}")
        source_group("${_group_path}" FILES "${_source}")
    endforeach()
endfunction(SOURCE_GROUP_BY_FOLDER)
