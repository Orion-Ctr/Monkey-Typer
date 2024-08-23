cmake_minimum_required(VERSION 3.28)

project(TypingGame)

set(CMAKE_CXX_STANDARD 17)

# Fetch SFML using FetchContent
include(FetchContent)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML
        GIT_TAG 2.6.1
)
FetchContent_MakeAvailable(SFML)

add_executable(TypingGame src/main.cpp)

target_link_libraries(TypingGame sfml-graphics sfml-window sfml-system)
