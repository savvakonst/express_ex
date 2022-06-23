



find_package(Python COMPONENTS Interpreter REQUIRED)

message("CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}")
add_test(NAME python_test
        COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/ex_test.py
        )