

if (CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif ()


find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)




add_library(io_ex SHARED
        DatasetsStorage_ifs.h
        FolderIoInterface.cpp
        FolderIoInterface.h
	#Hdf5IoInterface.cpp  #require hdf5 library
	#Hdf5IoInterface.h
        )



target_include_directories(io_ex INTERFACE  ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(io_ex PRIVATE Qt5::Widgets)

target_compile_definitions(io_ex PRIVATE DS_STORAGE_TO_EXPORT)