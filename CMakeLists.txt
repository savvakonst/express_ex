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


find_package(Doxygen
        REQUIRED dot
        OPTIONAL_COMPONENTS mscgen dia)

if (Doxygen_FOUND)
    message("Doxygen_FOUND")
    set(DOXYGEN_HTML_EXTRA_STYLESHEET ${CMAKE_CURRENT_SOURCE_DIR}/external_projects/doxygen-awesome-css/doxygen-awesome.css)
    #set(DOXYGEN_HTML_EXTRA_STYLESHEET ${CMAKE_CURRENT_SOURCE_DIR}/external_projects/doxygen_theme_flat_design/src/doxygen-style.css)

    set(DOXYGEN_DOT_IMAGE_FORMAT svg)
    set(DOXYGEN_EXTRACT_ALL NO)
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc)
    doxygen_add_docs(gen_doc_express_ex
            express_ex/include/ifs io_ex #syntaxhighlighter
            ALL)
endif ()