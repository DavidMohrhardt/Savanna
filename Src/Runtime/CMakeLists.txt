cmake_minimum_required(VERSION 3.26 FATAL_ERROR)

set(name "Savanna")
CONFIG_INPUT_HEADER(${name})

# Add the runtime folders

collect_sources_in_current_dir(src RECURSIVE)

message("${src}")

add_library(${name} SHARED ${src})
