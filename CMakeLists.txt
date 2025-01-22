cmake_minimum_required(VERSION 3.15)

# Project name and languages
project(HandsClappingGame LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set pybind11 path
set(Python_EXECUTABLE "D:/Program Files/Python/Python313/python.exe") # Replace it with your python executable path
set(Python_LIBRARIES "D:/Program Files/Python/Python313/libs") # Replace it with your python library path
set(Python_INCLUDE_DIRS "D:/Program Files/Python/Python313/include") # Replace it with your python include path
find_package(Python REQUIRED COMPONENTS Interpreter Development)
set(pybind11_DIR C:/Users/yixua/AppData/Roaming/Python/Python313/site-packages/pybind11/share/cmake/pybind11) # Replace it with your pybind11 path
find_package(pybind11 REQUIRED)

# Set compiler options for UTF-8
if(MSVC)
    add_compile_options(/source-charset:utf-8 /execution-charset:utf-8)
else()
    add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()

# Source and head files
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(GAME_DIR "${SOURCE_DIR}/game")
set(APP_DIR "${SOURCE_DIR}/app")
set(ONLINE_DIR "${SOURCE_DIR}/online")

# Collect files
file(GLOB_RECURSE GAME_SOURCES "${GAME_DIR}/*.cpp")

# Main progams
set(Main_Game "${APP_DIR}/handsclapping_game.cpp")
set(Game_Debug_Mode "${APP_DIR}/handsclapping_game_debug.cpp")
set(Referee_Mode "${APP_DIR}/handsclapping_referee.cpp")
set(Binding_File "${SOURCE_DIR}/handsclapping_pylib.cpp")

# Generate python binding
pybind11_add_module(handsclapping ${Binding_File} ${GAME_SOURCES})
target_include_directories(handsclapping PRIVATE ${ONLINE_DIR} ${GAME_DIR})
set_target_properties(handsclapping PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/lib")

# Main game executable file
add_executable(handsclapping_game ${Main_Game} ${GAME_SOURCES})
target_include_directories(handsclapping_game PRIVATE ${APP_DIR} ${GAME_DIR})

# Debug mode executable file
add_executable(handsclapping_game_debug ${Game_Debug_Mode} ${GAME_SOURCES})
target_include_directories(handsclapping_game_debug PRIVATE ${APP_DIR} ${GAME_DIR})

# Referee mode executable file
add_executable(handsclapping_referee ${Referee_Mode} ${GAME_SOURCES})
target_include_directories(handsclapping_referee PRIVATE ${APP_DIR} ${GAME_DIR})