cmake_minimum_required(VERSION 3.0)

project(W_crypt)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories (./include)

aux_source_directory(./src SRC_LIST)

add_executable(CrytoTool ${SRC_LIST})

# add_subdirectory(src)