




add_executable(express_ex_app
        main.cpp
        )


target_link_libraries(express_ex_app
        PRIVATE express_ex
        PRIVATE antlr4_shared
        PRIVATE EGrammar)