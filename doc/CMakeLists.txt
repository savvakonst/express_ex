find_package(Doxygen
        OPTIONAL_COMPONENTS mscgen dia dot)


if (Doxygen_FOUND)
    message("Doxygen_FOUND")
    set(DOXYGEN_HTML_EXTRA_STYLESHEET ${EXTERNAL_PROJECTS_DIRECTORY__}/doxygen-awesome-css/doxygen-awesome.css)

    set(DOXYGEN_DOT_IMAGE_FORMAT svg)
    set(DOXYGEN_GENERATE_HTML NO)
    set(DOXYGEN_GENERATE_XML YES)
    set(DOXYGEN_EXTRACT_ALL NO)
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    doxygen_add_docs(gen_doxygen_doc
            ../express_ex/include/ifs ../io_ex #syntaxhighlighter
            ALL)


    find_program(SPHINX_EXECUTABLE
            NAMES sphinx-build
            DOC "Path to sphinx-build executable"
            NO_CACHE)

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(gen_sphinx_doc
            "Failed to find sphinx-build executable"
            SPHINX_EXECUTABLE)

    if (gen_sphinx_doc_FOUND)

        set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)
        set(SPHINX_BUILD ${CMAKE_CURRENT_SOURCE_DIR}/build)

        add_custom_target(gen_sphinx_doc ALL
                COMMAND
                ${SPHINX_EXECUTABLE} -b html
                -Dbreathe_projects.exrt=${DOXYGEN_OUTPUT_DIRECTORY}/xml
                ${SPHINX_SOURCE} ${SPHINX_BUILD}
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Generating documentation with Sphinx")

        add_dependencies(gen_sphinx_doc gen_doxygen_doc)
    endif ()
endif ()