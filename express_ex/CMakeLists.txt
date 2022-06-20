
cmake_minimum_required(VERSION 3.16)

# set the project name
project(express_ex)

SET(IS_SHARED false)

if (NOT CMAKE_CL_64)
    set(SPlatform x86)
    set(APlarform Win32)
else ()
    set(SPlatform x64)
    set(APlarform x64)
endif ()


set(CLHPP__ "C:/Users/SVK/Desktop/openCl/khronos/OpenCL-CLHPP/include")
set(CUDAPATH__ "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.0")
set(OPENCL_INCLUDEDIR__ ${CUDAPATH__}/include)
set(OPENCL_LIBDIR__ ${CUDAPATH__}/lib/${APlarform})





set(LIBRARY_SOURCES

        lib/ifs/AsyncParameter.cpp
        lib/ifs/ParameterIO.cpp
        lib/ifs/SyncParameter.cpp
        lib/ifs/express_ex.cpp
        lib/ifs/printer.cpp
        lib/ifs/ErrorsIo.cpp

        lib/KexParser/KexParser.cpp
        lib/KexParser/treeShapeListener.cpp

        lib/parser/basic.cpp
        lib/parser/body.cpp
        lib/parser/bodyTemplate.cpp
        lib/parser/call.cpp
        lib/common/common.cpp
        #app/main.cpp
        lib/KexParser/errors.cpp
        lib/parser/line.cpp
        lib/parser/newOp.cpp
        lib/parser/operations.cpp

        lib/parser/variable.cpp


        lib/operations/ArithmeticOperation.cpp
        lib/operations/BuiltInCallOperation.cpp
        lib/operations/ComparisonOperation.cpp
        lib/operations/ConvolveOperation.cpp
        lib/operations/IntegrateOperation.cpp
        lib/operations/Operation_ifs.cpp
        lib/operations/SelectOperation.cpp
        lib/operations/SmallArrayDefOperation.cpp
        lib/operations/StoreToBufferOperation.cpp
        lib/operations/TypeCastOperation.cpp


        lib/jit/CL_generator.cpp
        lib/jit/IR_generator.cpp
        lib/jit/getWin32Intrinsinc.cpp
        lib/jit/table.cpp


        )


set(LIBRARY_HEADERS
        include/ifs/AsyncParameter.h
        include/ifs/SyncParameter.h
        include/ifs/config.h
        include/ifs/express_ex.h
        include/ifs/parameterIO.h
        include/ifs/parameterIfs.h
        include/ifs/printer.h
        include/ifs/ExStream.h

        lib/KexParser/treeShapeListener.h

        include/parser/KexParser.h
        include/parser/variable.h
        include/parser/basic.h
        include/parser/body.h
        include/parser/bodyTemplate.h
        include/parser/call.h
        include/parser/callTemplate.h
        include/common/common.h
        include/parser/defWarningIgnore.h
        include/parser/line.h
        include/parser/operations.h
        include/parser/parsingException.h


        include/common/errors.h
        include/common/errors.h
        include/common/types_jty.h
        include/common/undefWarningIgnore.h


        include/operations/ArithmeticOperation.h
        include/operations/BuiltInCallOperation.h
        include/operations/ComparisonOperation.h
        include/operations/ConvolveOperation.h
        include/operations/IntegrateOperation.h
        include/operations/Operation_ifs.h
        include/operations/SelectOperation.h
        include/operations/SmallArrayDefOperation.h
        include/operations/StoreToBufferOperation.h
        include/operations/TypeCastOperation.h

        include/jit/CL_generator.h
        include/jit/IR_generator.h
        include/jit/buffer.h
        include/jit/context.h
        include/jit/llvmHdrs.h
        include/jit/orcjit.h
        include/jit/table.h
        include/ifs/ExStreamIfs.h
        )



if (MSVC_VERSION)
    SET(EXPRESS_EX_DEFINITIONS__
            WIN32
            _WINDOWS
            UNICODE
            _UNICODE
            )
    SET(EXPRESS_EX_COMPILE_OPTIONS__
            /wd4804
            /wd4018
            /wd5208
            )

endif ()

#set(CMAKE_CXX_STACK_SIZE "10000000")

find_package(LLVM REQUIRED CONFIG)


add_library(express_ex SHARED
        ${LIBRARY_SOURCES}
        ${LIBRARY_HEADERS}
        )







target_compile_options(express_ex PRIVATE
        ${EXPRESS_EX_COMPILE_OPTIONS__}
        )

target_include_directories(express_ex
        PRIVATE include
        PRIVATE ${LLVM_INCLUDE_DIRS}
        PRIVATE ${OPENCL_INCLUDEDIR__}
        PRIVATE ${OPENCL_LIBDIR__}
        )

target_link_directories(express_ex PUBLIC
        ${OPENCL_LIBDIR__}
        )

separate_arguments(LLVM_DEFINITIONS_LIST__ NATIVE_COMMAND ${LLVM_DEFINITIONS} )

llvm_map_components_to_libnames(llvm_libs
        Core
        ExecutionEngine
        Interpreter
        MC
        MCJIT
        Support
        nativecodegen
        )

target_compile_definitions(express_ex PRIVATE
        ${EXPRESS_EX_DEFINITIONS__}
        ${LLVM_DEFINITIONS_LIST__}
        DLL_express_ex_EXPORT
        )

target_link_libraries(express_ex PRIVATE
        OpenCL
        antlr4_shared
        EGrammar
        ${llvm_libs}
        )



#set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "express_ex") ???


