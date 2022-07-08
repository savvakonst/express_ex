

if (CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif ()


message("HDF5_FOUND ${HDF5_FOUND}")


include(FindHDF5)
find_package(Qt5 COMPONENTS Widgets REQUIRED)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


if(TARGET CONAN_PKG::hdf5)
    message("TARGET CONAN_PKG::hdf5")
    set(DS_HDF5_IO_INTERFACE_SOURCES
            Hdf5IoInterface.cpp
            Hdf5IoInterface.h
            )

    set(DS_HDF5_C_DEFINITIONS HDF5_IO_INTERFACE_ENABLE)
    set(DS_HDF5_C_LIBRARIES CONAN_PKG::hdf5)


elseif (HDF5_FOUND)
    set(DS_HDF5_IO_INTERFACE_SOURCES
            Hdf5IoInterface.cpp
            Hdf5IoInterface.h
            )

    set(DS_HDF5_C_DEFINITIONS ${HDF5_C_DEFINITIONS} HDF5_IO_INTERFACE_ENABLE)
    set(DS_HDF5_C_LIBRARIES ${HDF5_C_LIBRARIES})

    message("HDF5_C_LIBRARIES ${HDF5_C_LIBRARIES}")

    add_custom_target(deploy_hdf_5
            ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE_DIR:${HDF5_C_LIBRARIES}>/$<TARGET_FILE_NAME:${HDF5_C_LIBRARIES}>" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<TARGET_FILE_NAME:${HDF5_C_LIBRARIES}>"
            USES_TERMINAL
            )

endif ()


add_library(io_ex SHARED
        DatasetsStorage_ifs.h
        DatasetsStorage_ifs.cpp
        FolderIoInterface.cpp
        FolderIoInterface.h

        ${DS_HDF5_IO_INTERFACE_SOURCES}
        )


get_target_property(TEMP_VAR io_ex INTERFACE_LINK_LIBRARIES)

message("IMPORTED_LOCATION_DEBUG ${TEMP_VAR}")

target_include_directories(io_ex INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${HDF5_INCLUDE_DIRS})

target_link_libraries(io_ex PRIVATE Qt5::Widgets ${DS_HDF5_C_LIBRARIES})

target_compile_definitions(io_ex PRIVATE DS_STORAGE_TO_EXPORT ${DS_HDF5_C_DEFINITIONS})

if (TARGET deploy_hdf_5)
    add_dependencies(io_ex deploy_hdf_5)
endif ()
