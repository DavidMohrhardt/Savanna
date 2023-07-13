message(STATUS "Including Concurrency Module in ${PROJECT_NAME}")

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

collect_sources_in_current_dir(src RECURSIVE_LIMITED)

message("${src}")
