#ifndef ASYNC_PARAMETER_H
#define ASYNC_PARAMETER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "ifs/ParameterIfs.h"

class DLL_EXPORT AsyncParameter : public ParameterIfs {
   public:
    AsyncParameter(const AsyncParameter& prm);
    AsyncParameter(const std::string& name, const std::vector<ExDataInterval>& interval_list, bool save_file_names);
    AsyncParameter(const std::string& name, const ExTimeInterval& time_interval,
                   const std::vector<ExDataInterval>& interval_list, bool save_file_names = true);

   private:
    /**
     * it is called by constructor
     */
    void init();

   public:
    ~AsyncParameter() override;

    bool isAsync() override { return true; }

    bool open(bool open_to_write = false) override;
    bool close() override;
    uint64_t write(char* data_buffer_ptr, uint64_t points_to_write) override;
    uint64_t read(char* data_buffer_ptr, uint64_t points_to_read) override;

    uint64_t getVirtualSize() const override;

    ParameterIfs* intersection(ParameterIfs* b, PrmTypesEn target_ty = PrmTypesEn::PRM_TYPE_UNKNOWN,
                               const std::string& name = "") override;

    ParameterIfs* retyping(PrmTypesEn target_ty = PrmTypesEn::PRM_TYPE_UNKNOWN, const std::string& name = "") override;

    ParameterIfs* newParameter() override;


    friend class ParametersDB;

   protected:
    bool isOpened() { return (id_ >= 0) && ds_storage_; }
    // TODO
    const size_t prm_time_size_ = 4;

    void readFromBuffer(char* data_buffer_ptr, uint64_t points_to_read);
    void readRawData(uint64_t points_to_read);



    const ExTimeInterval& getTimeInterval(int64_t interval_index) const {
        return interval_list_[(size_t)interval_index].ti;
    }

    const ExDataInterval& getCurrentInterval() { return interval_list_[(size_t)current_interval_index_]; }

    inline void openNewInterval();

    struct DLL_EXPORT IntermediateBuffer {
        char* base_ptr_;
        char* current_ptr_;
        size_t size_;

        IntermediateBuffer() : base_ptr_(nullptr), current_ptr_(nullptr), size_(0) {}
        ~IntermediateBuffer() { clear(); }

        void clear() {
            delete[] base_ptr_;
            base_ptr_ = nullptr;
            current_ptr_ = nullptr;
        }

        void resize(size_t new_size) {
            if (new_size > size_) {
                clear();
                size_ = new_size;
                base_ptr_ = new char[new_size];
                current_ptr_ = base_ptr_ + size_;
            }
        }

        void replaceLastData() {
            if (current_ptr_ == base_ptr_)  // bad idea
                return;
            size_t size_to_replace = base_ptr_ + size_ - current_ptr_;
            std::memcpy(base_ptr_, current_ptr_, size_to_replace);
            current_ptr_ = base_ptr_ + size_to_replace;
        }

        void resetPos() { current_ptr_ = base_ptr_; }
    };

    int64_t current_interval_index_ = 0;

    IntermediateBuffer intermediate_buffer_ = IntermediateBuffer();
    IntermediateBuffer time_buffer_ = IntermediateBuffer();


    size_t unused_points_in_current_interval_ = 0;
    size_t data_size_ = 0;
    size_t data_size_factor_ = 0;
    bool is_new_interval_ = true;
    AsyncParameter* parent_parameter_ = nullptr;


    DatasetsStorage_ifs::id_t id_ = DatasetsStorage_ifs::kDefaultId;

    /**
     * pointer to current , this class doesn't own it
     */
    DatasetsStorage_ifs* ds_storage_ = nullptr;

    typedef void (*buffer_access_t)(AsyncParameter*, char*, uint64_t);

    buffer_access_t copyToBuffer_ = nullptr;
    buffer_access_t copyFromBuffer_ = nullptr;

    template <typename DATA_T, typename TT>
    friend void copyToBuffer(AsyncParameter* p, char* untyped_data_buffer_ptr, uint64_t points_to_read);

    template <typename DATA_T, typename TT>
    friend void copyFromBuffer(AsyncParameter* p, char* untyped_ata_buffer_ptr, uint64_t points_to_read);
};


template <typename DATA_T, typename TT>
void copyToBuffer(AsyncParameter* p, char* untyped_data_buffer_ptr, uint64_t points_to_read) {
    DATA_T* data_buffer_ptr = (DATA_T*)untyped_data_buffer_ptr;
    while (points_to_read) {
        *((TT*)p->time_buffer_.current_ptr_) = *((TT*)p->intermediate_buffer_.current_ptr_);
        p->time_buffer_.current_ptr_ += sizeof(TT);
        p->intermediate_buffer_.current_ptr_ += sizeof(TT);
        *data_buffer_ptr = *((DATA_T*)p->intermediate_buffer_.current_ptr_);
        p->intermediate_buffer_.current_ptr_ += sizeof(DATA_T);
        ++data_buffer_ptr;
        points_to_read--;
    }
}

template <typename DATA_T, typename TT>
void copyFromBuffer(AsyncParameter* p, char* untyped_data_buffer_ptr, uint64_t points_to_read) {
    DATA_T* data_buffer_ptr = (DATA_T*)untyped_data_buffer_ptr;
    AsyncParameter::IntermediateBuffer& parent_time_buffer = p->parent_parameter_->time_buffer_;
    while (points_to_read) {
        *((TT*)p->intermediate_buffer_.current_ptr_) = *((TT*)parent_time_buffer.current_ptr_);
        // this side effect is important for IntermediateBuffer::replaceLastData() void
        parent_time_buffer.current_ptr_ += sizeof(TT);

        p->intermediate_buffer_.current_ptr_ += sizeof(TT);
        *((DATA_T*)p->intermediate_buffer_.current_ptr_) = *data_buffer_ptr;
        p->intermediate_buffer_.current_ptr_ += sizeof(DATA_T);
        ++data_buffer_ptr;
        points_to_read--;
    }
}

#endif