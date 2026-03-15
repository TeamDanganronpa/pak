cmake_minimum_required(VERSION 3.30)
project(pak
        DESCRIPTION "Simple, easy-to-use PAK interactions library"
        LANGUAGES C
)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)

add_subdirectory("third_party/bytepicker")

add_library(pak)
target_include_directories(pak PUBLIC "src")
target_link_libraries(pak PRIVATE bytepicker)
target_compile_definitions(pak PUBLIC PAK_COMPILATION)
target_sources(pak PRIVATE
        "src/pak/read/read.c"
        "src/pak/write/write.c"
)

if (BUILD_SHARED_LIBS)
    target_compile_definitions(pak PUBLIC PAK_DLL)
endif ()