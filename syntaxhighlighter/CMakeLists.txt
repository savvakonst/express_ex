

if (CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif ()


find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(LIBRARY_SOURCES
        sources/highlighter.cpp
        sources/highlightListener.cpp
        sources/mainWindow.cpp
        sources/textEdit.cpp
        sources/tree.cpp
        )

set(LIBRARY_HEADERS
        include/highlighter.h
        include/highlightListener.h
        include/mainwindow.h
        include/textEdit.h
        include/tree.h
        )


add_library(syntaxhighlighter SHARED
        ${LIBRARY_SOURCES}
        ${LIBRARY_HEADERS}
        )


target_compile_definitions(syntaxhighlighter PRIVATE QUILIB_LIBRARY)
target_include_directories(syntaxhighlighter PRIVATE include)
#Qt5Core Qt5Widgets Qt5Gui
target_link_libraries(syntaxhighlighter
        PRIVATE antlr4_shared
        PRIVATE EGrammar
        PRIVATE Qt5::Widgets)


