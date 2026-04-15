cmake_minimum_required(VERSION 3.20)
project(isim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

if(APPLE AND NOT OpenMP_ROOT AND EXISTS "/opt/homebrew/opt/libomp")
    set(OpenMP_ROOT "/opt/homebrew/opt/libomp")
endif()

find_package(OpenMP REQUIRED)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_executable(isim ${SRC_FILES})

target_include_directories(isim PRIVATE include)

target_link_libraries(isim
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    OpenMP::OpenMP_CXX
)

target_compile_options(isim PRIVATE
    $<$<CONFIG:Release>:
        -O3
        -march=native          # instructions CPU spécifiques à ta machine (AVX, SSE4, etc.)
        -ffast-math            # flottants moins stricts mais BEAUCOUP plus rapides
        -funroll-loops         # déroulage de boucles automatique
        -fomit-frame-pointer   # libère un registre supplémentaire
        -DNDEBUG               # désactive les assert() en release
    >
)
