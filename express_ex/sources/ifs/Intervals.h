#ifndef CHUNK_H
#define CHUNK_H

#include <set>
#include <type_traits>

#include "ifs/ParameterIO.h"
#include "llvm/Support/JSON.h"
#include "parser/common.h"
#include "parser/defWarningIgnore.h"
#include "parser/undefWarningIgnore.h"

class BareChunk {
   public:
    BareChunk(ex_size_t size) : size_(size), data_to_read_(size) {}

    virtual ~BareChunk() = default;

    virtual ex_size_t read(char* ptr, ex_size_t size) {
        ex_size_t readed_data = getReadedData(size);
        memset(ptr, 0, readed_data);
        return readed_data;
    }

    virtual ex_size_t write(char* ptr, ex_size_t size, calcMinMaxTy calcMinMaxPtr = nullptr) {
        return getReadedData(size);
    }

    virtual BareChunk* addNextChunk(BareChunk* next_chunk) {
        next_chunk_ = next_chunk;
        return next_chunk_;
    }

    BareChunk* getNextIfNoDataToRead() {
        if (data_to_read_ == 0) return next_chunk_;
        else
            return this;
    }

    BareChunk* getNext() { return next_chunk_; }

   protected:
    inline ex_size_t getReadedData(ex_size_t size) {
        if (size > data_to_read_) {
            ex_size_t readed_data = data_to_read_;
            data_to_read_ = 0;
            return readed_data;
        } else {
            data_to_read_ -= size;
            return size;
        }
    }

    ex_size_t size_ = 0;
    ex_size_t data_to_read_ = 0;

    BareChunk* next_chunk_ = nullptr;
};

class Chunk : public BareChunk {
   public:
    Chunk(DataInterval* di, const std::string* work_directory)
        : BareChunk(di->size), di_(di), work_directory_(work_directory) {
        size_of_word_ = sizeOfTy(di_->type);
    }

    ~Chunk() override { close(); }

    ex_size_t read(char* ptr, ex_size_t size) override {
        if (size_ == data_to_read_) openToRead();

        ex_size_t readed_data = getReadedData(size);
        ifs_->read(ptr, readed_data);

        if (data_to_read_ == 0) close();
        return readed_data;
    }

    ex_size_t write(char* ptr, ex_size_t size, calcMinMaxTy calcMinMaxPtr = nullptr) override {
        bool is_new_interval = false;
        if (size_ == data_to_read_) {
            openToWrite();
            is_new_interval = true;
        }
        ex_size_t readed_data = getReadedData(size);
        ifs_->write(ptr, readed_data);

        calcMinMaxPtr(ptr, size / size_of_word_, di_->val_max, di_->val_min, is_new_interval);
        if (data_to_read_ == 0) close();
        return readed_data;
    }

   private:
    void close() {
        if (ifs_) {
            ifs_->close();
            delete ifs_;
            ifs_ = nullptr;
        }
    }

    void openToRead() {
        close();

        std::string file_name = (di_->local ? *work_directory_ + "/" : "") + di_->file_name;
        ifs_ = new std::fstream(file_name, std::ios::in | std::ios::binary);
        ifs_->seekg(di_->offs);
    }

    void openToWrite() {
        close();

        std::string file_name = (di_->local ? *work_directory_ + "/" : "") + di_->file_name;
        if (di_->offs == 0) ifs_ = new std::fstream(file_name, std::ios::out | std::ios::binary);
        else
            ifs_ = new std::fstream(file_name, std::ios::out | std::ios::binary | std::ios::app);
    }

    ex_size_t size_of_word_ = 0;
    const std::string* work_directory_ = nullptr;
    std::fstream* ifs_ = nullptr;
    DataInterval* di_ = nullptr;
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