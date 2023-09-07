cmake_minimum_required(VERSION 3.26 FATAL_ERROR)

macro(ADD_CORE_LIBRARY library_name library_sources)
    if (MULTI_COMPILE_LIBRARIES)
        message (STATUS "Multi-compile core library: ${library_name}")
        add_library(${library_name}_static STATIC ${library_sources})
        set_target_properties(${library_name}_static PROPERTIES OUTPUT_NAME ${library_name})

        add_library(${library_name}_shared SHARED ${library_sources})
        set_target_properties(${library_name}_shared PROPERTIES OUTPUT_NAME ${library_name})
    else()
        message (STATUS "Adding core library: ${library_name} as ${CORE_LIBRARY_KIND}")
        add_library(${library_name} ${CORE_LIBRARY_KIND} ${library_sources})
    endif()
endmacro()

macro(SET_TARGET_INCLUDE_DIRS target_name accessor include_dirs)
    if (MULTI_COMPILE_LIBRARIES)
        target_include_directories(${target_name}_static ${accessor} ${include_dirs})
        target_include_directories(${target_name}_shared ${accessor} ${include_dirs})
    else()
        target_include_directories(${target_name} ${accessor} ${include_dirs})
    endif()
endmacro()

macro(SET_TARGET_CORE_DEPENDENCIES target_name dependencies)
    if (MULTI_COMPILE_LIBRARIES)
        # Create empty list
        set(static_Dependencies "")
        set(shared_Dependencies "")
        # Fill empty list with dependencies
        foreach (dependency IN ITEMS ${dependencies})
            target_link_libraries(${target_name}_static ${dependency}_static)
            add_dependencies(${target_name}_static ${dependencies}_static)

            target_link_libraries(${target_name}_shared ${dependency}_shared)
            add_dependencies(${target_name}_shared ${dependencies}_shared)
        endforeach()
    else()
        if (SEPARATE_ENGINE_MODULES_TO_LIBRARIES)
            target_link_libraries(${target_name} ${dependencies})
            add_dependencies(${target_name} ${dependencies})
        else()
            target_link_libraries(${target_name} "Savanna")
            add_dependencies(${target_name} "Savanna")
        endif()
    endif()
endmacro()

macro(INSTALL_CORE_LIB library_name)
    if (MULTI_COMPILE_LIBRARIES)
        install(TARGETS ${LIBRARY_NAME}_shared ${LIBRARY_NAME}_static
            EXPORT ${LIBRARY_NAME}Targets
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    else()
        install(TARGETS ${LIBRARY_NAME}
            EXPORT ${LIBRARY_NAME}Targets
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    endif()
endmacro()

macro(SET_CORE_LIBRARIES library_names out_library_names)
    # Create empty list
    set(processed_library_names "")
    if (MULTI_COMPILE_LIBRARIES)
        foreach (library_name IN ITEMS ${library_names})
            message(STATUS "Processing core library: ${library_name} as ${CORE_LIBRARY_KIND}")
            if (CORE_LIBRARY_KIND STREQUAL "STATIC")
                set(processed_library_names ${processed_library_names} ${library_name}_static)
            elseif (CORE_LIBRARY_KIND STREQUAL "SHARED")
                set(processed_library_names ${processed_library_names} ${library_name}_shared)
            else()
                message(FATAL_ERROR "Unknown core library kind: ${CORE_LIBRARY_KIND}")
            endif()
        endforeach()
    else()
        set(processed_library_names "${library_names}")
    endif()
    set(out_library_names "${processed_library_names}")
endmacro()

macro(FIND_SOURCE_FILES_AND_SUBDIRS)
    # Acquire all source files in this directory, This is evil for the record)
    file(GLOB_RECURSE src
        ${CMAKE_CURRENT_LIST_DIR}/*.cpp
        ${CMAKE_CURRENT_LIST_DIR}/*.c
        ${CMAKE_CURRENT_LIST_DIR}/*.h
        ${CMAKE_CURRENT_LIST_DIR}/*.hpp)

    SUBDIRLIST(subdirs ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR})
    foreach (subdir IN ITEMS ${subdirs})
        if (${subdir} STREQUAL ${CMAKE_CURRENT_LIST_DIR})
            continue()
        endif()
        # Check if the subdirectory contains a ~ in it's path
        if (${subdir} MATCHES "~")
            list(FILTER src EXCLUDE REGEX ${subdir})
            continue()
        endif()
        if (ALLOW_GLOBBING_SUBDIRS_WITH_CMAKELISTS)
            # Check if the subdirectory has a CMakeLists.txt file
            file(GLOB CMakeLists ${subdir}/CMakeLists.txt)
            if (CMakeLists)
                # Add the subdirectory
                add_subdirectory(${subdir})
                # Filter out the subdirectory from the source list
                list(FILTER src EXCLUDE REGEX ${subdir})
            endif()
        endif()
    endforeach()
endmacro(FIND_SOURCE_FILES_AND_SUBDIRS)

macro(ADD_MODULE_DEPENDENCY target_name module_name)
    SET_CORE_LIBRARIES(${module_name} module_name)
    SET_TARGET_CORE_DEPENDENCIES(${target_name} ${module_name})
    set(module_dir ${CMAKE_SOURCE_DIR}/Modules)
    target_include_directories(${target_name} PUBLIC ${module_dir})
endmacro()

macro(ADD_LIBS_FOR_BUILD target_name core_libs core_include_dirs external_libs external_include_dirs)
    # if core include dirs are specified, add them
    message(STATUS "Adding include directories for ${target_name}")

    message(STATUS "Adding current directory to target includes: ${CMAKE_CURRENT_SOURCE_DIR}")
    # Add the current directory to the include directories privately.
    target_include_directories(${target_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

    set (USE_MODULE_DEPENDENCY_MACRO FALSE)

    # if core libs are specified, link them
    if (core_libs)
        if (USE_MODULE_DEPENDENCY_MACRO)
            ADD_MODULE_DEPENDENCY(${target_name} ${core_libs})
        else()
            message(STATUS "Adding core dependencies: ${core_libs}")
            SET_CORE_LIBRARIES(${core_libs} core_libs)
            SET_TARGET_CORE_DEPENDENCIES(${target_name} ${core_libs})

            if (core_include_dirs)
                message(STATUS "Adding core include directories: ${core_include_dirs}")
                SET_TARGET_INCLUDE_DIRS(${target_name} PUBLIC "${core_include_dirs}")
            endif()
        endif()
    endif()

    # if additional libs are specified, link them
    if (external_libs)
        message(STATUS "Adding external dependencies: ${external_libs}")
        target_link_libraries(${target_name} ${external_libs})
    endif()

    # if additional include dirs are specified, add them
    if (external_include_dirs)
        message(STATUS "Adding external include directories: ${external_include_dirs}")
        target_include_directories(${target_name} PUBLIC ${external_include_dirs})
    endif()
endmacro()

macro(DECLARE_CORE_LIBRARY target_name core_libs core_include_dirs external_libs external_include_dirs)
    message(STATUS "Processing: ${target_name}")

    # Turn on the ability to create folders to organize projects (.vcproj)
    # It creates "CMakePredefinedTargets" folder by default and adds CMake
    # defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Find all source files and subdirectories
    FIND_SOURCE_FILES_AND_SUBDIRS()

    # Sort the files into Filters based on Folder Hierarchy
    SOURCE_GROUP_BY_FOLDER()

    # Add the library
    ADD_CORE_LIBRARY(${target_name} "${src}")

    # Add the libraries for the build
    ADD_LIBS_FOR_BUILD(${target_name} "${core_libs}" "${core_include_dirs}" "${external_libs}" "${external_include_dirs}")

    # Install the library
    INSTALL_CORE_LIB(${target_name})

    message (STATUS "Finished Processing ${target_name}\n")
endmacro()

macro(CREATE_APP_WITH_LIBS target_name core_libs core_include_dirs external_libs external_include_dirs)
    message(STATUS "Processing: ${target_name}")

    # Turn on the ability to create folders to organize projects (.vcproj)
    # It creates "CMakePredefinedTargets" folder by default and adds CMake
    # defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Find all source files and subdirectories
    FIND_SOURCE_FILES_AND_SUBDIRS()

    # Sort the files into Filters based on Folder Hierarchy
    SOURCE_GROUP_BY_FOLDER()

	# Add Executable
	add_executable(${target_name} ${src})

    # Add the libraries for the build
    message (STATUS "Adding libraries for build")
    ADD_LIBS_FOR_BUILD(${target_name} "${core_libs}" "${core_include_dirs}" "${external_libs}" "${external_include_dirs}")

	# Define project properties
	set_property(TARGET ${Recipe_Name} PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/binaries)
	set_property(TARGET ${Recipe_Name} PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/binaries)
	set_property(TARGET ${Recipe_Name} PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/binaries)
	set_property(TARGET ${Recipe_Name} PROPERTY RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_CURRENT_SOURCE_DIR}/binaries)
	set_property(TARGET ${Recipe_Name} PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}/binaries)

    message (STATUS "Finished Processing ${target_name}\n")
endmacro()

function(CONFIG_INPUT_HEADER cur_name)
    message(STATUS "Configuring input header for ${cur_name}")

    file(READ "${CMAKE_CURRENT_LIST_DIR}/${cur_name}_BuildConfig.json" module_json)
    string(JSON name_var GET "${module_json}" Name)
    string(JSON config_array_var GET "${module_json}" Configurations)

    set (config_name "Release")
    if(CMAKE_BUILD_TYPE MATCHES Debug)
        set(config_name "Debug")
    endif()

    string(JSON config_array_length LENGTH  "${config_array_var}")
    math(EXPR config_count "${config_array_length} - 1")
    foreach (i RANGE ${config_count})
        string (JSON config_item GET "${config_array_var}" ${i})
        string (JSON config_item_name GET "${config_item}" Name)
        if (${config_item_name} STREQUAL ${config_name})
            # Iterate over another array of items
            string(JSON config_item_array_var GET "${config_item}" Values)
            string(JSON config_item_array_length LENGTH  "${config_item_array_var}")
            math(EXPR config_item_count "${config_item_array_length} - 1")
            foreach(j RANGE ${config_item_count})
                # Get the Name and Value of each item
                string(JSON config_item_item GET "${config_item_array_var}" ${j})
                string(JSON config_item_item_name GET "${config_item_item}" Name)
                string(JSON config_item_item_value GET "${config_item_item}" Value)
                # Set the variable
                set(${config_item_item_name} ${config_item_item_value})
                message(STATUS "Set ${config_item_item_name} to ${config_item_item_value}")
            endforeach()

        endif()
    endforeach()

    # In the current directory, find the *.h.in file
    file(GLOB input_header "${CMAKE_CURRENT_LIST_DIR}/*.h.in")

    # The output header is the input header with the .in removed
    string(REPLACE ".h.in" ".gen.h" output_header "${input_header}")

    # Configure the input header
    configure_file(${input_header} ${output_header} @ONLY)

endfunction()
