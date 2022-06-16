
cmake_minimum_required(VERSION 3.10)

# set the project name
project(express_ex)

SET(IS_SHARED false)

if( NOT  CMAKE_CL_64 )
    set(SPlatform  x86)
    set(APlarform  Win32)
else()
    set(SPlatform  x64) 
    set(APlarform  x64)
endif()
 
# specify the C++ standard 
set(CMAKE_CXX_STANDARD 17)


set(CMAKE_CXX_STANDARD_REQUIRED True)


set(CLHPP       "C:/Users/SVK/Desktop/openCl/khronos/OpenCL-CLHPP/include")
set(cudaPath    "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.0")
set(openCL_includeDir  ${cudaPath}/include)

set(EGrammarDir ../EGrammar)
set(llvmDir     ../../common/llvm-project)



#include(CMakeMacro.txt)

include_directories(${CLHPP})
include_directories(${openCL_includeDir})

include_directories(${llvmDir}/llvm/include)
include_directories(${llvmDir}/build_${SPlatform}/include)



message("CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH}")

#set(CMAKE_PREFIX_PATH ../../libs/antlr4/runtime/Cpp/install/debug_x64 C:/Qt/5.12.8/msvc2017_64)




add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../EGrammar     ${CMAKE_CURRENT_BINARY_DIR}/EGrammar EXCLUDE_FROM_ALL)
add_subdirectory(io_ex)




set(CMAKE_CONFIGURATION_TYPES Debug Release)


set(LIBRARY_SOURCES
    sources/ifs/AsyncParameter.cpp
    sources/ifs/ParameterIO.cpp
    sources/ifs/SyncParameter.cpp
    sources/ifs/express_ex.cpp
    sources/ifs/printer.cpp

    sources/parser/basic.cpp
    sources/parser/body.cpp
    sources/parser/bodyTemplate.cpp
    sources/parser/call.cpp
    sources/parser/common.cpp
    sources/parser/errors.cpp
    sources/parser/line.cpp
    sources/parser/newOp.cpp
    sources/parser/operations.cpp
    sources/parser/treeShapeListener.cpp
    sources/parser/variable.cpp


    sources/operations/ArithmeticOperation.cpp
    sources/operations/BuiltInCallOperation.cpp
    sources/operations/ComparisonOperation.cpp
    sources/operations/ConvolveOperation.cpp
    sources/operations/IntegrateOperation.cpp
    sources/operations/Operation_ifs.cpp
    sources/operations/SelectOperation.cpp
    sources/operations/SmallArrayDefOperation.cpp
    sources/operations/StoreToBufferOperation.cpp
    sources/operations/TypeCastOperation.cpp


    sources/jit/CL_generator.cpp
    sources/jit/IR_generator.cpp
    sources/jit/getWin32Intrinsinc.cpp
    sources/jit/table.cpp
)





set(LIBRARY_HEADERS
    include/ifs/AsyncParameter.h
    include/ifs/SyncParameter.h
    include/ifs/config.h
    include/ifs/express_ex.h
    include/ifs/parameterIO.h
    include/ifs/parameterIfs.h
    include/ifs/printer.h


    include/parser/basic.h
    include/parser/body.h
    include/parser/bodyTemplate.h
    include/parser/call.h
    include/parser/callTemplate.h
    include/parser/common.h
    include/parser/defWarningIgnore.h
    include/parser/line.h
    include/parser/operations.h
    include/parser/parsingException.h
    include/parser/treeShapeListener.h
    include/parser/types_jty.h
    include/parser/undefWarningIgnore.h
    include/parser/variable.h

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
)



set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})



add_library(express_ex SHARED
    ${LIBRARY_SOURCES}
    ${LIBRARY_HEADERS}
)

target_include_directories(express_ex PUBLIC include)





target_link_libraries(express_ex io_ex)
target_link_libraries(express_ex EGrammar)


set(CMAKE_CXX_STACK_SIZE "10000000")

if(MSVC)





    target_compile_options(express_ex PRIVATE
        /wd4804
        /wd4018
        /wd5208
    )


    SET(ObjNames
        LLVMAggressiveInstCombine
        LLVMAnalysis
        LLVMAsmPrinter
        LLVMBinaryFormat
        LLVMBitReader
        LLVMBitWriter
        LLVMBitstreamReader
        LLVMCFGuard
        LLVMCodeGen
        LLVMCore
        LLVMDebugInfoCodeView
        LLVMDebugInfoDWARF
        LLVMDebugInfoMSF
        LLVMDemangle
        LLVMExecutionEngine
        LLVMGlobalISel
        LLVMInstCombine
        LLVMInterpreter
        LLVMMC
        LLVMMCDisassembler
        LLVMMCJIT
        LLVMMCParser
        LLVMObject
        LLVMProfileData
        LLVMRemarks
        LLVMRuntimeDyld
        LLVMScalarOpts
        LLVMSelectionDAG
        LLVMSupport
        LLVMTarget
        LLVMTextAPI
        LLVMTransformUtils
        LLVMX86CodeGen
        LLVMX86Desc
        LLVMX86Info
        #LLVMX86Utils
        OpenCL
    )


    set(openCL_libDir ${cudaPath}/lib/${APlarform})
    set(llvmLibPath   ${llvmDir}/build_${SPlatform}/$<CONFIG>/lib)
    #set(antlrLibPath  ${antlr4Dir}/bin/vs-2019/${SPlatform}/$<CONFIG>\ DLL)

    target_link_directories(express_ex PUBLIC
        #${antlrLibPath}
        ${llvmLibPath}
        ${openCL_libDir}
    )


    target_link_libraries(express_ex ${ObjNames}  )




    SET(common_MSVC_definitions
        WIN32
        _WINDOWS
        _CRT_SECURE_NO_DEPRECATE
        _CRT_SECURE_NO_WARNINGS
        _CRT_NONSTDC_NO_DEPRECATE
        _CRT_NONSTDC_NO_WARNINGS
        _SCL_SECURE_NO_DEPRECATE
        _SCL_SECURE_NO_WARNINGS
        UNICODE
        _UNICODE
        __STDC_CONSTANT_MACROS
        __STDC_FORMAT_MACROS
        __STDC_LIMIT_MACROS
    )




    if(IS_SHARED)
    #if(false)
        set_property(TARGET express_ex APPEND PROPERTY
            COMPILE_DEFINITIONS ${common_MSVC_definitions} DLL_express_ex_EXPORT
        )

        set(FILE_NAMES_TO_COPY "express_ex.*" "syntaxhighlighter.*" "topsyntaxhighlighter.*" "antlr4-runtime.*")


        foreach( TARGET_CONFIG  Debug Release)

            foreach( FILE_NAME_MASK ${FILE_NAMES_TO_COPY})

                #set(FILES_TO_COPY_PATTERN ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${FILE_NAME_MASK} )
                file(GLOB FILES_TO_COPY "${SPlatform}/${TARGET_CONFIG}/${FILE_NAME_MASK}" )

                #add_custom_command(TARGET express_ex POST_BUILD COMMAND ${CMAKE_COMMAND} -E echo "-------------------------FILES_TO_COPY: $<FILES_TO_COPY>")

                foreach( FILE_NAME_VAR ${FILES_TO_COPY})
                    add_custom_command(TARGET express_ex POST_BUILD COMMAND ${CMAKE_COMMAND} -E echo "-------------------------FILE_NAME_VAR: ${FILE_NAME_VAR}")

                    add_custom_command(TARGET express_ex POST_BUILD
                        COMMAND  ${CMAKE_COMMAND} -E copy_if_different
                            ${FILE_NAME_VAR}
                            //nas_sy/Common/svk/express_ex/${APlarform}/${TARGET_CONFIG}.Shared
                        )
                endforeach( FILE_NAME_VAR )
            endforeach( FILE_NAME_MASK )
        endforeach( TARGET_CONFIG )




    else()
        set_property(TARGET express_ex APPEND PROPERTY
            COMPILE_DEFINITIONS ${common_MSVC_definitions} CONSOLE_APP_
        )
    endif()



    #set_property(GLOBAL PROPERTY VS_STARTUP_PROJECT "express_ex")
    set_property (DIRECTORY PROPERTY VS_STARTUP_PROJECT "express_ex")
else()
    message(FATAL_ERROR  "compilers other than MSVC are not supported" )   
endif()





#COMMAND $<$<CONFIG:Release>:strip>  
