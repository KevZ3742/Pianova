cmake_minimum_required(VERSION 3.20)
project(Pianova LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Build SFML as static libs, and skip the parts we don't need
set(BUILD_SHARED_LIBS OFF)
set(SFML_BUILD_AUDIO OFF)    # we use miniaudio instead
set(SFML_BUILD_NETWORK OFF)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON)
FetchContent_MakeAvailable(SFML)

# miniaudio is a single header; we only want to download it, not build it
FetchContent_Declare(miniaudio
    GIT_REPOSITORY https://github.com/mackron/miniaudio.git
    GIT_TAG 0.11.21
    GIT_SHALLOW ON
    SOURCE_SUBDIR does_not_exist)
FetchContent_MakeAvailable(miniaudio)

add_executable(pianova
    src/main.cpp
    src/Note.cpp
    src/MiniaudioImpl.cpp)
target_include_directories(pianova PRIVATE include ${miniaudio_SOURCE_DIR})
target_link_libraries(pianova PRIVATE SFML::Graphics)