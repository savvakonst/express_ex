cmake_minimum_required(VERSION 3.16)

project(express_ex_env)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()


if (NOT EXTERNAL_PROJECTS_DIRECTORY)
    set(EXTERNAL_PROJECTS_DIRECTORY__ ${CMAKE_SOURCE_DIR}/external_projects)
elseif ()
    if (IS_ABSOLUTE ${EXTERNAL_PROJECTS_DIRECTORY})
        set(EXTERNAL_PROJECTS_DIRECTORY__ ${EXTERNAL_PROJECTS_DIRECTORY})
    elseif ()
        set(EXTERNAL_PROJECTS_DIRECTORY__ ${CMAKE_SOURCE_DIR}/${EXTERNAL_PROJECTS_DIRECTORY})
    endif ()
endif ()

include(ExpressUtils.cmake)


get_directory_property(hasParent PARENT_DIRECTORY)
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
add_subdirectory(doc)


