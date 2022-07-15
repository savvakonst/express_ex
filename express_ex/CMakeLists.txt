
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


#add_subdirectory(test)


set(LIBRARY_SOURCES
        lib/common/common.cpp

        lib/ifs/AsyncParameter.cpp
        lib/ifs/ParameterIO.cpp
        lib/ifs/SyncParameter.cpp
        lib/ifs/express_ex.cpp
        lib/ifs/printer.cpp
        lib/ifs/ErrorsIo.cpp

        lib/KexParser/KexParser.cpp
        lib/KexParser/treeShapeListener.cpp
        lib/KexParser/errors.cpp


        lib/parser/basic.cpp
        lib/parser/body.cpp
        lib/parser/bodyTemplate.cpp

        lib/parser/line.cpp
        lib/parser/ExValue.cpp
        #lib/parser/operations.cpp

        lib/operations/FunctionCall/CallRecursiveFunction.cpp
        lib/operations/FunctionCall/call.cpp

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
        lib/operations/NeighborPointOperation.h

        #lib/jit/CL_generator.cpp
        lib/jit/IR_generator.cpp
        lib/jit/getWin32Intrinsinc.cpp
        lib/jit/table.cpp
        lib/operations/FunctionCall/CallTemplate.cpp
        lib/operations/NeighborPointOperation.cpp
        )


set(LIBRARY_HEADERS
        include/ifs/AsyncParameter.h
        include/ifs/SyncParameter.h
        include/ifs/config.h
        include/ifs/express_ex.h
        include/ifs/parameterIO.h
        include/ifs/parameterIfs.h
        lib/ifs/printer.h
        include/ifs/ExStream.h

        lib/KexParser/treeShapeListener.h


        include/parser/KexParser.h
        include/operations/ExValue.h
        include/parser/basic.h
        include/parser/body.h
        include/parser/bodyTemplate.h

        include/parser/callTemplate.h
        include/common/common.h
        include/parser/defWarningIgnore.h
        include/parser/line.h
        include/operations/operations.h
        include/parser/parsingException.h


        include/common/types_jty.h
        include/common/undefWarningIgnore.h

        lib/operations/FunctionCall/CallRecursiveFunction.h
        lib/operations/ArithmeticOperation.h
        lib/operations/BuiltInCallOperation.h
        lib/operations/ComparisonOperation.h
        lib/operations/ConvolveOperation.h
        lib/operations/IntegrateOperation.h
        include/operations/Operation_ifs.h
        lib/operations/SelectOperation.h
        lib/operations/SmallArrayDefOperation.h
        lib/operations/StoreToBufferOperation.h
        lib/operations/TypeCastOperation.h
        lib/operations/FunctionCall/call.h
        lib/operations/NeighborPointOperation.h


        #include/jit/CL_generator.h
        include/jit/IR_generator.h
        include/jit/buffer.h
        include/jit/context.h
        include/jit/llvmHdrs.h
        include/jit/orcjit.h
        include/jit/table.h
        include/ifs/ExStreamIfs.h
        #include/operations/ExValue_ifs.h
        )

set(LLVM_COMPONENTS
        Core
        ExecutionEngine
        Interpreter
        MC
        MCJIT
        Support
        nativecodegen
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


find_package(LLVM QUIET CONFIG)

get_property(IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

if (NOT LLVM_FOUND  #[[OR DEFINED LLVM_BUILD_DIR]] )

    set(LLVM_SOURCE_SUBDIR llvm)

    set(EXTERNAL_LLVM_SOURCE_DIR ${EXTERNAL_PROJECTS_DIRECTORY__}/llvm-project)
    set(EXTERNAL_LLVM_PROJECT_DIR ${EXTERNAL_LLVM_SOURCE_DIR}/${LLVM_SOURCE_SUBDIR})


    if (NOT EXISTS ${EXTERNAL_LLVM_PROJECT_DIR}/CMakeLists.txt)
        set(LLVM_GIT_REPOSITORY https://github.com/llvm/llvm-project.git)
        find_package(Git REQUIRED)
        set(LLVM_DOWNLOAD_COMMAND DOWNLOAD_COMMAND
                ${GIT_EXECUTABLE} clone
                --depth 1 --config core.autocrlf=false --branch llvmorg-12.0.1 "${LLVM_GIT_REPOSITORY}" "${EXTERNAL_LLVM_SOURCE_DIR}")
    else ()
        set(LLVM_DOWNLOAD_COMMAND)
    endif ()


    include(FetchContent)
    set(FETCHCONTENT_QUIET FALSE)
    FetchContent_Declare(
            llvm_prj
            ${LLVM_DOWNLOAD_COMMAND}
            SOURCE_DIR ${EXTERNAL_LLVM_SOURCE_DIR}
            SOURCE_SUBDIR ${LLVM_SOURCE_SUBDIR}

            GIT_PROGRESS TRUE
            LOG_CONFIGURE TRUE
    )

    FetchContent_GetProperties(llvm_prj)
    if (NOT llvm_prj_POPULATED)
        FetchContent_Populate(llvm_prj)

        set(LLVM_ADDITIONAL_ARGS "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}" "-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}")

        if (NOT IS_MULTI_CONFIG)
            set(LLVM_BUILD_TYPE -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})
        endif ()

        if (CMAKE_VS_PLATFORM_NAME)
            if (CMAKE_VS_PLATFORM_NAME STREQUAL "x64")
                set(LLVM_ADDITIONAL_ARGS -A x64 -Thost=x64)
            else ()
                set(LLVM_ADDITIONAL_ARGS -A Win32 -Thost=x64)
            endif ()
        elseif (CMAKE_CXX_COMPILER MATCHES "x64/cl.exe")
        elseif (CMAKE_CXX_COMPILER MATCHES "x86/cl.exe")
        elseif (CMAKE_CXX_FLAGS MATCHES "-m64")
            set(LLVM_ADDITIONAL_ARGS ${LLVM_ADDITIONAL_ARGS} "-DCMAKE_CXX_FLAGS=-m64" "-DCMAKE_C_COMPILER=-m64")
        elseif (CMAKE_CXX_FLAGS MATCHES "-m32")
            set(LLVM_ADDITIONAL_ARGS ${LLVM_ADDITIONAL_ARGS} "-DCMAKE_CXX_FLAGS=-m32" "-DCMAKE_C_COMPILER=-m32")
        endif ()


        message("run:\n${CMAKE_COMMAND} -G \"${CMAKE_GENERATOR}\" ${LLVM_BUILD_TYPE} ${LLVM_ADDITIONAL_ARGS} -DLLVM_INCLUDE_TESTS=OFF  -DLLVM_INCLUDE_BENCHMARKS=OFF -DLLVM_INCLUDE_TOOLS=OFF -DLLVM_INCLUDE_DOCS=OFF -DLLVM_TARGETS_TO_BUILD=X86  ${EXTERNAL_LLVM_PROJECT_DIR}"
                )
        execute_process(
                COMMAND ${CMAKE_COMMAND} "-G${CMAKE_GENERATOR}" ${LLVM_BUILD_TYPE} ${LLVM_ADDITIONAL_ARGS} -DLLVM_INCLUDE_TESTS=OFF -DLLVM_INCLUDE_BENCHMARKS=OFF -DLLVM_INCLUDE_TOOLS=OFF -DLLVM_INCLUDE_DOCS=OFF -DLLVM_TARGETS_TO_BUILD=X86 ${EXTERNAL_LLVM_PROJECT_DIR}
                RESULT_VARIABLE result
                ${outputOptions}
                WORKING_DIRECTORY "${llvm_prj_BINARY_DIR}"
        )


        set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${llvm_prj_BINARY_DIR})
        find_package(LLVM REQUIRED CONFIG)

        set(LLVM_BUILD_DIR ${llvm_prj_BINARY_DIR} CACHE STRING
                "cached name of available python module" FORCE)

    endif ()

endif ()


add_library(express_ex SHARED
        ${LIBRARY_SOURCES}
        ${LIBRARY_HEADERS}
        )


target_compile_options(express_ex PRIVATE
        ${EXPRESS_EX_COMPILE_OPTIONS__}
        )


target_include_directories(express_ex
        PUBLIC include
        PRIVATE ${LLVM_INCLUDE_DIRS}
        )

target_link_directories(express_ex PUBLIC
        ${OPENCL_LIBDIR__}
        )

llvm_map_components_to_libnames(LLVM_LIBS__
        ${LLVM_COMPONENTS}
        )

target_link_libraries(express_ex
        PUBLIC io_ex

        PRIVATE antlr4_shared
        PRIVATE EGrammar
        PRIVATE ${LLVM_LIBS__}
        )


if (DEFINED LLVM_BUILD_DIR)

    get_interface_link_libraries_recursively(LLVM_ALL_DEPENDENCIES ${LLVM_LIBS__})
    get_library_paths(LLVM_LIB_PATHS ${LLVM_ALL_DEPENDENCIES})

    message("${CMAKE_COMMAND} --build \"${LLVM_BUILD_DIR}\" --target ${LLVM_LIBS__} --config $<CONFIG>")

    add_custom_target(build_llvm
            ${CMAKE_COMMAND} --build "${LLVM_BUILD_DIR}" --target ${LLVM_LIBS__} --config $<CONFIG>
            BYPRODUCTS ${LLVM_LIB_PATHS}
            USES_TERMINAL
            #WORKING_DIRECTORY "${LLVM_BUILD_DIR}"
            )

    add_dependencies(express_ex build_llvm)

endif ()


separate_arguments(LLVM_DEFINITIONS_LIST__ NATIVE_COMMAND ${LLVM_DEFINITIONS})


target_compile_definitions(express_ex PRIVATE
        ${EXPRESS_EX_DEFINITIONS__}
        ${LLVM_DEFINITIONS_LIST__}
        DLL_express_ex_EXPORT
        )

if (PRINT_EX_DEBUG_INFO)
    message("PRINT_EX_DEBUG_INFO ${PRINT_EX_DEBUG_INFO}")
    target_compile_definitions(express_ex PUBLIC
            "$<$<CONFIG:Debug>:EX_DEBUG>"
            )
endif ()

add_subdirectory(app)
#set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "express_ex") ???