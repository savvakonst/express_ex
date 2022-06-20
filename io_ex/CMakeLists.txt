

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

target_link_libraries(io_ex Qt5::Widgets)