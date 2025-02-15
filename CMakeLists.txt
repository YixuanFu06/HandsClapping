cmake_minimum_required(VERSION 3.15)

# Project name and languages
project(HandsClappingGame LANGUAGES CXX)
set(CMAKE_BUILD_TYPE Release)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set pybind11 and python path
set(Python3_EXECUTABLE "D:/Program Files/Python/Python313/python.exe") # Replace it with your python executable path
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
set(pybind11_DIR  "C:/Users/yixua/AppData/Roaming/Python/Python313/site-packages/pybind11/share/cmake/pybind11") # Replace it with your pybind11 path
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
set(AI_DIR "${SOURCE_DIR}/AI")
set(AI_IDIOT_DIR "${AI_DIR}/Idiot")
set(AI_HDP_DIR "${AI_DIR}/HDP")
set(GAME_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/build/bin/game")
set(PYINSTALLER_OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin/online")
set(AI_IDIOT_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/build/bin/AI/Idiot")
set(AI_HDP_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/build/bin/AI/HDP")
set(AI_API_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/build/bin/AI/api")
set(DATA_DIR "${CMAKE_SOURCE_DIR}/data")

# Collect files
file(GLOB_RECURSE GAME_SOURCES "${GAME_DIR}/*.cpp")
file(GLOB_RECURSE ONLINE_SOURCES "${ONLINE_DIR}/*.py")
file(GLOB_RECURSE IDIOT_SOURCES "${AI_IDIOT_DIR}/include/*.cpp")
file(GLOB_RECURSE HDP_SOURCES "${AI_HDP_DIR}/include/*.cpp")

# Main programs
set(Main_Game "${APP_DIR}/handsclapping_game.cpp")
set(Game_Debug_Mode "${APP_DIR}/handsclapping_game_debug.cpp")
set(Referee_Mode "${APP_DIR}/handsclapping_referee.cpp")

set(Binding_File "${SOURCE_DIR}/handsclapping_pylib.cpp")

set(Idiot_Training "${AI_IDIOT_DIR}/app/training.cpp")
set(Idiot_Demo "${AI_IDIOT_DIR}/app/demo.cpp")
set(Idiot_1v1 "${AI_IDIOT_DIR}/app/game_1v1.cpp")
set(Idiot_Analysis "${AI_IDIOT_DIR}/app/tools.cpp")
set(Idiot_Hybrid_Training "${AI_IDIOT_DIR}/app/hybrid_training.cpp")
set(Idiot_Group_Training "${AI_IDIOT_DIR}/app/group_training.cpp")

set(HDP_Training "${AI_HDP_DIR}/app/training.cpp")
set(HDP_1v1 "${AI_HDP_DIR}/app/game_1v1.cpp")

set(AI_Demo "${AI_DIR}/app/demo.cpp")
set(AI_1v1 "${AI_DIR}/app/game_1v1.cpp")

# Generate python binding
pybind11_add_module(handsclapping ${Binding_File} ${GAME_SOURCES})
target_include_directories(handsclapping PRIVATE ${ONLINE_DIR} ${GAME_DIR})
set_target_properties(handsclapping PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/lib")

# Main game executable file
add_executable(handsclapping_game ${Main_Game} ${GAME_SOURCES})
target_include_directories(handsclapping_game PRIVATE ${APP_DIR} ${GAME_DIR})
set_target_properties(handsclapping_game PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${GAME_OUTPUT_DIR}")

# Debug mode executable file
add_executable(handsclapping_game_debug ${Game_Debug_Mode} ${GAME_SOURCES})
target_include_directories(handsclapping_game_debug PRIVATE ${APP_DIR} ${GAME_DIR})
set_target_properties(handsclapping_game_debug PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${GAME_OUTPUT_DIR}")

# Referee mode executable file
add_executable(handsclapping_referee ${Referee_Mode} ${GAME_SOURCES})
target_include_directories(handsclapping_referee PRIVATE ${APP_DIR} ${GAME_DIR})
set_target_properties(handsclapping_referee PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${GAME_OUTPUT_DIR}")

# Idiot training executable file
add_executable(idiot_training ${Idiot_Training} ${GAME_SOURCES} ${IDIOT_SOURCES})
target_include_directories(idiot_training PRIVATE ${AI_IDIOT_DIR} ${GAME_DIR})
set_target_properties(idiot_training PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${AI_IDIOT_OUTPUT_DIR}")

# Idiot demo executable file
add_executable(idiot_demo ${Idiot_Demo} ${GAME_SOURCES} ${IDIOT_SOURCES})
target_include_directories(idiot_demo PRIVATE ${AI_IDIOT_DIR} ${GAME_DIR})
set_target_properties(idiot_demo PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${AI_IDIOT_OUTPUT_DIR}")

# Idiot 1v1 game executable file
add_executable(idiot_game_1v1 ${Idiot_1v1} ${GAME_SOURCES} ${IDIOT_SOURCES})
target_include_directories(idiot_game_1v1 PRIVATE ${AI_IDIOT_DIR} ${GAME_DIR})
set_target_properties(idiot_game_1v1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${AI_IDIOT_OUTPUT_DIR}")

# Idiot analysis tool executable file
add_executable(idiot_tools ${Idiot_Analysis} ${GAME_SOURCES} ${IDIOT_SOURCES})
target_include_directories(idiot_tools PRIVATE ${AI_IDIOT_DIR} ${GAME_DIR})
set_target_properties(idiot_tools PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${AI_IDIOT_OUTPUT_DIR}")

# Idiot hybrid training executable file
add_executable(idiot_hybrid_training ${Idiot_Hybrid_Training} ${GAME_SOURCES} ${IDIOT_SOURCES})
target_include_directories(idiot_hybrid_training PRIVATE ${AI_IDIOT_DIR} ${GAME_DIR})
set_target_properties(idiot_hybrid_training PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${AI_IDIOT_OUTPUT_DIR}")

# Idiot asymmetric training executable file
add_executable(idiot_group_training ${Idiot_Group_Training} ${GAME_SOURCES} ${IDIOT_SOURCES})
target_include_directories(idiot_group_training PRIVATE ${AI_IDIOT_DIR} ${GAME_DIR})
set_target_properties(idiot_group_training PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${AI_IDIOT_OUTPUT_DIR}")

# HDP training executable file
add_executable(hdp_training ${HDP_Training} ${GAME_SOURCES} ${HDP_SOURCES})
target_include_directories(hdp_training PRIVATE ${AI_HDP_DIR} ${GAME_DIR})
set_target_properties(hdp_training PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${AI_HDP_OUTPUT_DIR}")

# HDP 1v1 game executable file
add_executable(hdp_game_1v1 ${HDP_1v1} ${GAME_SOURCES} ${HDP_SOURCES})
target_include_directories(hdp_game_1v1 PRIVATE ${AI_HDP_DIR} ${GAME_DIR})
set_target_properties(hdp_game_1v1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${AI_HDP_OUTPUT_DIR}")

# AI demo executable file
add_executable(ai_demo ${AI_Demo} ${GAME_SOURCES} ${IDIOT_SOURCES} ${HDP_SOURCES})
target_include_directories(ai_demo PRIVATE ${AI_DIR} ${GAME_DIR})
set_target_properties(ai_demo PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${AI_API_OUTPUT_DIR}")

# AI 1v1 game executable file
add_executable(ai_1v1 ${AI_1v1} ${GAME_SOURCES} ${IDIOT_SOURCES} ${HDP_SOURCES})
target_include_directories(ai_1v1 PRIVATE ${AI_DIR} ${GAME_DIR})
set_target_properties(ai_1v1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${AI_API_OUTPUT_DIR}")

# PyInstaller for online sources
foreach(script ${ONLINE_SOURCES})
    get_filename_component(script_name ${script} NAME_WE)
    add_custom_command(
        TARGET handsclapping POST_BUILD
        COMMAND ${Python3_EXECUTABLE} -m PyInstaller --onefile
                --distpath ${PYINSTALLER_OUTPUT_DIR}
                --workpath ${CMAKE_BINARY_DIR}/online_tmp
                ${script}
        COMMENT "Running PyInstaller to package ${script} into an executable"
    )
endforeach()

# Install targets
set(CPACK_PACKAGE_NAME "HandsClappingGame")
set(CPACK_PACKAGE_VERSION "1.1.0")
set(CPACK_PACKAGE_CONTACT "yixuanfu89@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The electronic version of the game of Handsclapping.")
set(CPACK_PACKAGE_VENDOR "Yixuan Fu, Zhourui Zhang")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-Windows-x86_64")
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_IGNORE_FILES "/build/;/\\\\.git/;/\\\\.svn/;/\\\\.hg/;/\\\\.bzr/;/_CPack_Packages/;/CMakeFiles/;CMakeCache.txt;\\\\.cmake$;\\\\.swp$;\\\\.bak$;\\\\.orig$;\\\\.tmp$;\\\\.log$;\\\\.DS_Store$;\\\\.directory$")

include(CPack)

install(TARGETS handsclapping DESTINATION HandsClapping/lib)
install(FILES ${ONLINE_SOURCES} DESTINATION HandsClapping/python)
install(DIRECTORY ${GAME_OUTPUT_DIR} DESTINATION HandsClapping/bin)
install(DIRECTORY ${PYINSTALLER_OUTPUT_DIR} DESTINATION HandsClapping/bin)
install(DIRECTORY ${AI_IDIOT_OUTPUT_DIR} DESTINATION HandsClapping/bin/AI)
install(DIRECTORY ${AI_HDP_OUTPUT_DIR} DESTINATION HandsClapping/bin/AI)
install(DIRECTORY ${AI_API_OUTPUT_DIR} DESTINATION HandsClapping/bin/AI)
install(DIRECTORY ${DATA_DIR} DESTINATION HandsClapping)
install(FILES "${CMAKE_SOURCE_DIR}/LICENSE" DESTINATION share/licenses)
install(FILES "${CMAKE_SOURCE_DIR}/README.md" DESTINATION share/doc)
install(FILES "${CMAKE_SOURCE_DIR}/src/README.md" DESTINATION share/doc RENAME "TUTORIAL.md")
install(DIRECTORY "${CMAKE_SOURCE_DIR}/assets" DESTINATION share)