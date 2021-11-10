cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 14)

project(p2pd)

# --- main application ---

aux_source_directory(src SRC_FILES)
aux_source_directory(src/api SRC_FILES)
aux_source_directory(src/api/protocol SRC_FILES)
aux_source_directory(src/api/service SRC_FILES)
aux_source_directory(src/common SRC_FILES)
aux_source_directory(src/engine SRC_FILES)
aux_source_directory(src/json SRC_FILES)
aux_source_directory(src/websocket SRC_FILES)
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME}
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
target_compile_definitions(${PROJECT_NAME}
    PRIVATE SOURCE_ROOT="${CMAKE_CURRENT_SOURCE_DIR}/src"
)

include("cmake/macros.cmake")
add_boost(${PROJECT_NAME})
add_private_library(${PROJECT_NAME} "json-c")
add_private_library(${PROJECT_NAME} "libtorrent-rasterbar")
