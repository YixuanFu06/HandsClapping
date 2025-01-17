cmake_minimum_required(VERSION 3.10)

# Project name and languages
project(HandsClappingGame LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Source and head files
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src/app")
set(GAME_DIR "${CMAKE_SOURCE_DIR}/src/game")

# Collect files
file(GLOB_RECURSE GAME_SOURCES "${GAME_DIR}/*.cpp")
file(GLOB_RECURSE GAME_HEADERS "${GAME_DIR}/*.h")

# Main progams
set(Main_Game "${SRC_DIR}/handsclapping_game.cpp")
set(Game_Debug_Mode "${SRC_DIR}/handsclapping_game_debug.cpp")
set(Referee_Mode "${SRC_DIR}/handsclapping_referee.cpp")

# Main game executable file
add_executable(handsclapping_game ${Main_Game} ${GAME_SOURCES})
target_include_directories(handsclapping_game PRIVATE ${SRC_DIR} ${GAME_DIR})

# Debug mode executable file
add_executable(handsclapping_game_debug ${Game_Debug_Mode} ${GAME_SOURCES})
target_include_directories(handsclapping_game_debug PRIVATE ${SRC_DIR} ${GAME_DIR})

# Referee mode executable file
add_executable(handsclapping_referee ${Referee_Mode} ${GAME_SOURCES})
target_include_directories(handsclapping_referee PRIVATE ${SRC_DIR} ${GAME_DIR})