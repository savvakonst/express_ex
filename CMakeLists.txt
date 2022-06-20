cmake_minimum_required(VERSION 3.16)

project(express_ex_env)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CONFIGURATION_TYPES Debug Release)

#set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
if (NOT hasParent)
    set(VENV_PATH bin)

    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${VENV_PATH})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${VENV_PATH})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${VENV_PATH})
endif ()

add_subdirectory(io_ex)
add_subdirectory(EGrammar)
add_subdirectory(express_ex)
add_subdirectory(syntaxhighlighter)