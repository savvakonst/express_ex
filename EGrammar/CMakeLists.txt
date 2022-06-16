cmake_minimum_required(VERSION 3.17)

# TODO: Add custom file generation with antlr4
# antlr4 -Dlanguage=Cpp EGrammar.g4  -o ./gen


set(PROJECT_NAME EGrammar)
set(LIBRARY_NAME EGrammar)

project(${PROJECT_NAME} VERSION 1.0 LANGUAGES CXX)



find_package(antlr4-runtime QUIET)

if(MSVC_VERSION)
    set(ANTLR_SUPPRESS_ERRORS
            /wd4275
            /wd4251
            )
endif()


if(NOT antlr4-runtime_FOUND)

    set(ANTLR_BUILD_CPP_TESTS OFF)
    set(ANTLR4_INSTALL ON)
    set(WITH_STATIC_CRT OFF)

    set(antlr_4_SOURCE_SUBDIR runtime/Cpp)

    include(FetchContent)
    FetchContent_Declare( # TODO: need to set up a source code destination directory to avoid source code duplication
            antlr_4
            GIT_REPOSITORY https://github.com/antlr/antlr4.git
            GIT_TAG 9cf7e96a2adbf71021bc2c1ccf2917a574040b08
            SOURCE_SUBDIR ${antlr_4_SOURCE_SUBDIR}
            )




    if (MSVC_VERSION) # it is to suppress cl : Command line warning D9025 : overriding '/W3' with '/W4'
        string(REGEX REPLACE "/W[1-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    endif()

    FetchContent_MakeAvailable(antlr_4)

    target_compile_options(antlr4_shared PRIVATE ${ANTLR_SUPPRESS_ERRORS})
    if (MSVC_VERSION)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
    endif()


    set(ANTLR4_INCLUDE_DIR  ${antlr_4_SOURCE_DIR}/${antlr_4_SOURCE_SUBDIR}/runtime/src)
endif()


set(LIBRARY_HEADERS_DIR .)
set(LIBRARY_SOURCES_DIR .)

set(LIBRARY_HEADERS
  ${LIBRARY_HEADERS_DIR}/EGrammarBaseListener.h
  ${LIBRARY_HEADERS_DIR}/EGrammarLexer.h
  ${LIBRARY_HEADERS_DIR}/EGrammarListener.h
  ${LIBRARY_HEADERS_DIR}/EGrammarParser.h
)

set(LIBRARY_SOURCES
  ${LIBRARY_SOURCES_DIR}/EGrammarBaseListener.cpp
  ${LIBRARY_SOURCES_DIR}/EGrammarLexer.cpp
  ${LIBRARY_SOURCES_DIR}/EGrammarListener.cpp
  ${LIBRARY_SOURCES_DIR}/EGrammarParser.cpp
)

add_library(${LIBRARY_NAME} STATIC
  ${LIBRARY_HEADERS}
  ${LIBRARY_SOURCES}
)

message("ANTLR4_INCLUDE_DIR ${ANTLR4_INCLUDE_DIR}")

set_target_properties(${LIBRARY_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_compile_options(${LIBRARY_NAME} PRIVATE ${ANTLR_SUPPRESS_ERRORS})
target_include_directories(${LIBRARY_NAME} PRIVATE ${ANTLR4_INCLUDE_DIR})
target_link_libraries(${LIBRARY_NAME} antlr4_shared)
