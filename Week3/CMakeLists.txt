cmake_minimum_required(VERSION 3.0)

project(main)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(./include)

aux_source_directory(./src SRC_LIST)

add_executable(main ${SRC_LIST})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(main ${OpenCV_LIBS})