#ifndef CHUNK_H
#define CHUNK_H

#include <set>
#include <type_traits>

#include "common/common.h"
#include "ifs/ExStreamIfs.h"
#include "ifs/parameterIO.h"

class BareChunk {
   public:
    BareChunk(ex_size_t size) : size_(size), data_to_process_(size) {}

    virtual ~BareChunk() = default;

    virtual ex_size_t read(char* ptr, ex_size_t size) {
        ex_size_t readed_data = getDataSizeToProcess(size);
        memset(ptr, 0, readed_data);
        return readed_data;
    }

    virtual ex_size_t write(char* ptr, ex_size_t size, calcMinMaxTy calcMinMaxPtr = nullptr) {
        return getDataSizeToProcess(size);
    }

    virtual BareChunk* addNextChunk(BareChunk* next_chunk) {
        next_chunk_ = next_chunk;
        return next_chunk_;
    }

    BareChunk* getNextIfNoDataToRead() {
        if (data_to_process_ == 0) return next_chunk_;
        else return this;
    }

    BareChunk* getNext() { return next_chunk_; }

   protected:
    inline ex_size_t getDataSizeToProcess(ex_size_t size) {
        if (size > data_to_process_) {
            ex_size_t processed_data = data_to_process_;
            data_to_process_ = 0;
            return processed_data;
        } else {
            data_to_process_ -= size;
            return size;
        }
    }

    ex_size_t size_ = 0;
    ex_size_t data_to_process_ = 0;

    BareChunk* next_chunk_ = nullptr;
};

class Chunk : public BareChunk {
   public:
    Chunk(ExDataInterval* di, ExStreamIfs* err_stream = nullptr)
        : BareChunk(di->size), di_(di), err_stream_(err_stream) {
        size_of_word_ = sizeOfTy(di_->type);
    }

    ~Chunk() override { close(); }

    ex_size_t read(char* ptr, ex_size_t size) override {
        if (size_ == data_to_process_) openToRead();

        ex_size_t data_size_to_process = getDataSizeToProcess(size);
        di_->ds->readFromDataset(id_, ptr, data_size_to_process);

        if (data_to_process_ == 0) close();
        return data_size_to_process;
    }

    ex_size_t write(char* ptr, ex_size_t size, calcMinMaxTy calcMinMaxPtr = nullptr) override {
        bool is_new_interval = false;
        if (size_ == data_to_process_) {
            openToWrite();
            is_new_interval = true;
        }

        ex_size_t data_size_to_process = getDataSizeToProcess(size);
        di_->ds->writeToDataset(id_, ptr, data_size_to_process);

        calcMinMaxPtr(ptr, size / size_of_word_, di_->val_max, di_->val_min, is_new_interval);
        if (data_to_process_ == 0) close();
        return data_size_to_process;
    }

   private:
    void close() {
        if (id_ >= 0) {
            di_->ds->closeDataset(id_);
            id_ = DatasetsStorage_ifs::kDefaultId;
        }
    }

    void openToRead() {
        close();

        id_ = di_->ds->openDataset(di_->file_name.c_str());
        if ((id_ < 0) && err_stream_)
            *err_stream_ << ExColors::RED << "error: " << ExColors::RESET << "can't open data set for reading/n";

        if (di_->offset != 0) di_->ds->seek(id_, di_->offset);
    }

    void openToWrite() {
        close();

        id_ = di_->ds->openDataset(di_->file_name.c_str());
        if ((id_ < 0) && err_stream_)
            *err_stream_ << ExColors::RED << "error: " << ExColors::RESET << "can't open data set for writing/n";

        // if (di_->offset != 0) di_->ds->seek(id_, di_->offset);
        if (di_->offset != 0) di_->ds->seek(id_, di_->ds->getDatasetSize(id_));
    }

    ex_size_t size_of_word_ = 0;

    DatasetsStorage_ifs::id_t id_ = DatasetsStorage_ifs::kDefaultId;
    ExDataInterval* di_ = nullptr;
    ExStreamIfs* err_stream_ = nullptr;
};

class FinishChunk : public BareChunk {
   public:
    FinishChunk() : BareChunk(1) { next_chunk_ = nullptr; }

    ~FinishChunk() override = default;

    virtual ex_size_t read(char* ptr, ex_size_t size) override {
        memset(ptr, 0, size);
        return size;
    }

    virtual ex_size_t write(char* ptr, ex_size_t size, calcMinMaxTy calcMinMaxPtr = nullptr) override { return size; }

    BareChunk* addNextChunk(BareChunk* next_chunk) override { return nullptr; }
};

#endif