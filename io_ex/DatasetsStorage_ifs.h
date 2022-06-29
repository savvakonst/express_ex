#ifndef DATASETSSTORAGE_IFS
#define DATASETSSTORAGE_IFS
#include <cstddef>
#include <cstdint>


#ifdef _WIN32
#    ifdef DS_STORAGE_TO_EXPORT
#        define DS_STORAGE_EXPORTS __declspec(dllexport)
#    else
#        define DS_STORAGE_EXPORTS __declspec(dllimport)
#    endif
#else
#    define DS_STORAGE_EXPORTS
#endif


class DatasetsStorage_ifs {
   public:
    enum
    { kDefaultId = -1 };


    using id_t = std::int64_t;
    using ds_ssize_t = std::int64_t;
    using ds_usize_t = std::uint64_t;

    virtual ~DatasetsStorage_ifs() = default;

    virtual bool create(const char* path) = 0;
    virtual bool open(const char* path, bool rw = false) = 0;
    virtual bool flush() = 0;
    virtual bool close() = 0;


    virtual bool datasetExists(const char* name) = 0;
    virtual bool removeDataset(const char* name) = 0;

    /**
     * \deprecated \attention  it deprecated use openDataset(const char* name) instead
     * \returns dataset id, or
     * value less than 0 if an error occurred.
     */
    [[deprecated("it deprecated use openDataset(const char* name) instead")]] virtual id_t createDataset(
        const char* name) = 0;


    /**
     * \returns dataset id, or
     * value less than 0 if an error occurred.
     */
    virtual id_t openDataset(const char* name) = 0;


    virtual bool closeDataset(id_t id) = 0;


    /**
     * \returns the current size of dataset, otherwise return -1.
     */
    virtual ds_ssize_t getDatasetSize(id_t id) = 0;


    virtual bool seek(id_t id, ds_usize_t pos) = 0;


    /**
     * \returns the number of bytes that were actually written, or
     * value less than 0 if an error occurred.
     */
    virtual std::ptrdiff_t writeToDataset(id_t id, const char* data, std::size_t count) = 0;

    /**
     * \returns the number of bytes read. If the end of data has already been reached it returns 0;
     * If an error occurs, such as when attempting to read from a device opened in WriteOnly mode, this
     * function returns a value less than 0.
     */
    virtual std::ptrdiff_t readFromDataset(id_t id, char* data, std::size_t count) = 0;
};

DS_STORAGE_EXPORTS std::unique_ptr<DatasetsStorage_ifs> openDatasetsStorage(const std::string& filename, bool rw);

#endif