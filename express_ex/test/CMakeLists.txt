cmake_minimum_required(VERSION 3.14)



find_package(Python COMPONENTS Interpreter REQUIRED)

message("CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}")



enable_testing()


#[[ it is not suitable
add_test(NAME python_async_test
        COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/ex_test.py    --db="sync_base.express" -i="single_arg_convolve.kex"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/async
        )


add_test(NAME python_sync_test
        COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/ex_test.py  --db="sync_base.express" -i="single_arg_convolve.kex"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/sync
        )
]]