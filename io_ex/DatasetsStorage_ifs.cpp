


#include <QFileInfo>
#include <memory>

#ifdef HDF5_IO_INTERFACE_ENABLE
#    include "Hdf5IoInterface.h"
#endif

#include "FolderIoInterface.h"


std::unique_ptr<DatasetsStorage_ifs> openDatasetsStorage(const std::string& filename, bool rw) {
    std::unique_ptr<DatasetsStorage_ifs> ds_storage;

    QFileInfo file_info(filename.c_str());

    if (file_info.suffix() == "h5"){
    #ifdef HDF5_IO_INTERFACE_ENABLE
        ds_storage = std::make_unique<Hdf5IoInterface>();
    #endif
    }
    else ds_storage = std::make_unique<FolderIoInterface>();

    if (ds_storage->open(filename.c_str(), rw)) return ds_storage;

    return nullptr;
}
