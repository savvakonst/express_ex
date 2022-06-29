#ifndef HDF5_IO_INTERFACE_H
#define HDF5_IO_INTERFACE_H
#include <hdf5.h>

#include <string>
#include <vector>

#include "DatasetsStorage_ifs.h"


class DS_STORAGE_EXPORTS Hdf5IoInterface : public DatasetsStorage_ifs {
   public:
    Hdf5IoInterface() = default;

    ~Hdf5IoInterface() override;


    bool create(const char* path) override;
    bool open(const char* path, bool rw = false) override;
    bool flush() override { return true; }
    bool close() override;

    bool datasetExists(const char* name) override;  // unused
    id_t createDataset(const char* name) override;
    id_t openDataset(const char* name) override;
    bool closeDataset(id_t index) override;
    bool removeDataset(const char* name) override;
    ds_ssize_t getDatasetSize(id_t index) override;
    bool seek(id_t index, ds_usize_t pos) override;
    std::ptrdiff_t writeToDataset(id_t index, const char* data, std::size_t count) override;
    std::ptrdiff_t readFromDataset(id_t index, char* data, std::size_t count) override;

   private:
    bool initVariables();
    void closeHandles();

    static std::string getNameByHid(hid_t obj_id);

    hid_t file_id_ = H5I_BADID;
    hid_t file_space_id_ = H5I_BADID;
    hid_t mem_space_id_ = H5I_BADID;
    hid_t prop_list_id_ = H5I_BADID;



    struct dataset_t {
        hid_t id_ = H5I_BADID;
        ds_usize_t offset_ = 0;
        ds_usize_t size_ = 0;
    };

    std::vector<dataset_t> datasets_;

    bool rw_ = false;
};

#endif
