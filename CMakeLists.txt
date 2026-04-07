cmake_minimum_required(VERSION 3.20)
project(isim LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_executable(isim ${SRC_FILES})

target_include_directories(isim PRIVATE include)

target_link_libraries(isim
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)

target_compile_options(isim PRIVATE
    $<$<CONFIG:Release>:-O3>
)