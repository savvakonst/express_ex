#ifndef FOLDER_IO_INTERFACE
#define FOLDER_IO_INTERFACE

#include "DatasetsStorage_ifs.h"
#include <vector>
#include <string>

class QFile;

#define DS_STORAGE_EXPORTS
#ifdef _WIN32
    #ifdef DS_STORAGE_TO_EXPORT
        #define DS_STORAGE_EXPORTS __declspec(dllexport)
    #else
        #define DS_STORAGE_EXPORTS __declspec(dllimport)
    #endif
#endif


class DS_STORAGE_EXPORTS FolderIoInterface : public DatasetsStorage_ifs {
public:

    FolderIoInterface() = default;


    ~FolderIoInterface() override;


    bool create(const char *path) override;

    bool open(const char *path, bool rw = false) override;

    bool flush() override { return true; }

    bool close() override;


    bool datasetExists(const char *name) override { return false; }

    id_t createDataset(const char *name) override;

    id_t openDataset(const char *name) override;

    bool closeDataset(id_t index) override;

    bool removeDataset(const char *name) override;

    ds_ssize_t getDatasetSize(id_t index) override;

    bool seek(id_t index, ds_usize_t pos) override { return false; } // unused
    std::ptrdiff_t writeToDataset(id_t index, const char *data, std::size_t count) override;

    std::ptrdiff_t readFromDataset(id_t index, char *data, std::size_t count) override;

private:


    static std::string getNameByPtr(const QFile *obj_id);


    std::string folder_name_;


    std::vector<QFile *> datasets_;

    bool rw_ = false;
};


#endif
