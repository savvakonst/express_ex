#ifndef ASYNC_PARAMETER_H
#define ASYNC_PARAMETER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "ifs/parameterIfs.h"

class DLL_EXPORT AsyncParameter : public ParameterIfs {
   public:
    AsyncParameter(const AsyncParameter& c) : ParameterIfs(c) { parent_parameter_ = this; }
    AsyncParameter() { parent_parameter_ = this; }
    AsyncParameter(const std::string &name, const std::vector<DataInterval>& interval_list, bool save_file_names);
    AsyncParameter(const std::string& name, const TimeInterval& time_interval,
                   const std::vector<DataInterval>& interval_list,
                   bool save_file_names = true);
     
    ~AsyncParameter() override;

    bool isAsync() override { return true; }
    // is not supported yet
    std::vector<int64_t> read_dots(Dot* dot_buffer, size_t max_point_number, double from, double to) override;

    // is not supported yet
    std::vector<int64_t> read_dots(double* top_buffer_ptr, double* bottom_buffer_ptr, double* time_buffer_ptr,
                                   double from, double to, size_t max_point_number_to_read) override;

    bool open(bool open_to_write = false) override;
    bool close() override;
    uint64_t write(char* data_buffer_ptr, uint64_t points_to_write) override;
    uint64_t read(char* data_buffer_ptr, uint64_t points_to_read) override;

    uint64_t getVirtualSize() const override;

    ParameterIfs* intersection(ParameterIfs* b, PrmTypesEn target_ty = PrmTypesEn::PRM_TYPE_UNKNOWN,
                               const std::string& name = "") override;
    ParameterIfs* enlargeFrequency(int64_t arg, PrmTypesEn target_ty = PrmTypesEn::PRM_TYPE_UNKNOWN,
                                   const std::string& name = "");
    ParameterIfs* retyping(PrmTypesEn target_ty = PrmTypesEn::PRM_TYPE_UNKNOWN, const std::string& name = "") override;

    virtual ParameterIfs* newParameter() override;

    friend void readParametersList(std::string databaseFName, std::vector<ParameterIfs>& parameterList);
    friend class ParametersDB;

   protected:
    const size_t prm_time_size_ = 4;

    void readFromBuffer(char* data_buffer_ptr, uint64_t points_to_read);
    void readRawData(uint64_t points_to_read);

    template <typename T>
    void copyToBuffer(T* data_buffer_ptr, uint64_t points_to_read) {
        while (points_to_read) {
            *((float*)time_buffer_.current_ptr_) = *((float*)intermediate_buffer_.current_ptr_);
            time_buffer_.current_ptr_ += sizeof(float);
            intermediate_buffer_.current_ptr_ += sizeof(float);
            *data_buffer_ptr = *((T*)intermediate_buffer_.current_ptr_);
            intermediate_buffer_.current_ptr_ += sizeof(T);
            ++data_buffer_ptr;
            points_to_read--;
        }
    }

    template <typename T>
    void copyFromBuffer(T* data_buffer_ptr, uint64_t points_to_read) {
        IntermediateBuffer& parent_time_buffer = parent_parameter_->time_buffer_;
        while (points_to_read) {
            *((float*)intermediate_buffer_.current_ptr_) = *((float*)parent_time_buffer.current_ptr_);
            // this side effect is impotant for IntermediateBuffer::replaceLastData() void
            parent_time_buffer.current_ptr_ += sizeof(float);

            intermediate_buffer_.current_ptr_ += sizeof(float);
            *((T*)intermediate_buffer_.current_ptr_) = *data_buffer_ptr;
            intermediate_buffer_.current_ptr_ += sizeof(T);
            ++data_buffer_ptr;
            points_to_read--;
        }
    }

    const TimeInterval& getTimeInterval(int64_t interval_index) const {
        return interval_list_[(size_t)interval_index].time_interval;
    }

    const DataInterval& getCurrentInterval() { return interval_list_[(size_t)current_interval_index_]; }

    inline void openNewInterval();

    struct DLL_EXPORT IntermediateBuffer {
        char* base_ptr_;
        char* current_ptr_;
        size_t size_;

        IntermediateBuffer() : base_ptr_(nullptr), current_ptr_(nullptr), size_(0) {}
        ~IntermediateBuffer() { clear(); }

        void clear() {
            delete[] base_ptr_;
            base_ptr_    = nullptr;
            current_ptr_ = nullptr;
        }

        void resize(size_t new_size) {
            if (new_size > size_) {
                clear();
                size_        = new_size;
                base_ptr_    = new char[new_size];
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

    IntermediateBuffer intermediate_buffer_ = IntermediateBuffer();
    IntermediateBuffer time_buffer_         = IntermediateBuffer();

    size_t unused_points_in_current_interval_ = 0;
    size_t data_size_                         = 0;
    size_t data_size_factor_                  = 0;
    bool is_new_interval_                     = true;
    AsyncParameter* parent_parameter_         = nullptr;
};

#endif