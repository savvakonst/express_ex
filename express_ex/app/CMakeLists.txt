




add_executable(express_ex_app
        main.cpp
        )


target_include_directories(express_ex_app
        PRIVATE ${LLVM_INCLUDE_DIRS}
        )


llvm_map_components_to_libnames(LLVM_LIBS_FOR_APP_
        Support
        )

target_link_libraries(express_ex_app
        PRIVATE express_ex
        ${LLVM_LIBS_FOR_APP_}
        )