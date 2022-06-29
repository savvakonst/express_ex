

if (CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif ()


message("HDF5_FOUND ${HDF5_FOUND}")


include(FindHDF5)
find_package(Qt5 COMPONENTS Widgets REQUIRED)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


if (HDF5_FOUND)
    set(DS_HDF5_IO_INTERFACE_SOURCES
            Hdf5IoInterface.cpp
            Hdf5IoInterface.h
            )

    set(DS_HDF5_C_DEFINITIONS ${HDF5_C_DEFINITIONS} HDF5_IO_INTERFACE_ENABLE)
    set(DS_HDF5_C_LIBRARIES ${HDF5_C_LIBRARIES})
endif ()


add_library(io_ex SHARED
        DatasetsStorage_ifs.h
        DatasetsStorage_ifs.cpp
        FolderIoInterface.cpp
        FolderIoInterface.h

        ${DS_HDF5_IO_INTERFACE_SOURCES}
        )


target_include_directories(io_ex INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${HDF5_INCLUDE_DIRS})

target_link_libraries(io_ex PRIVATE Qt5::Widgets ${DS_HDF5_C_LIBRARIES})

target_compile_definitions(io_ex PRIVATE DS_STORAGE_TO_EXPORT ${DS_HDF5_C_DEFINITIONS})
