#include <set>
#include <utility>

#include "Intervals.h"
#include "ifs/parameterIO.h"
#include "ifs/printer.h"
#include "llvm/Support/JSON.h"
#include "parser/common.h"
#include "parser/defWarningIgnore.h"
#include "parser/undefWarningIgnore.h"

SyncParameter::SyncParameter(std::string name, const std::vector<DataInterval>& interval_list, bool save_file_names) {
    name_ = std::move(name);
    // for (auto i : interval_list)
    //     interval_list_.push_back(i);

    interval_list_ = interval_list;
    number_of_intervals_ = interval_list.size();
    if (save_file_names == false) {
        for (auto& i : interval_list_) {
            i.file_name = "";
            i.local = true;
        }
    }

    if (interval_list_.size()) {
        sizeof_data_type_ = sizeOfTy(interval_list_.front().type);
        type_ = interval_list_.front().type;
        calc_min_max_ptr_ = g_calcMinMax_select(type_);
    }

    auto bgn = interval_list_.front().time_interval.bgn;
    auto end = interval_list_.back().time_interval.end;

    time_interval_ = {bgn, end};
    calcExtendedInfo();

    const DataInterval* last_interval = nullptr;
    for (auto& i : interval_list_) {
        if (last_interval) {
            ex_size_t size =
                (uint64_t)((i.time_interval.bgn - last_interval->time_interval.bgn) * frequency_) * sizeof_data_type_ -
                last_interval->time_interval.bgn;

            if (size) current_chunk_->addNextChunk(new BareChunk(size));
        }

        // std::stringstream stream;
        //::stream(stream, i, "    ");
        // llvm::outs() << stream.str();

        BareChunk* chunk = new Chunk(&i, &work_directory_);

        if (chunk_ == nullptr) {
            chunk_ = chunk;
            current_chunk_ = chunk;
        } else {
            current_chunk_ = current_chunk_->addNextChunk(chunk);
        }

        last_interval = &i;
    }
    if (current_chunk_) current_chunk_->addNextChunk(new FinishChunk());
    current_chunk_ = chunk_;
}

SyncParameter::SyncParameter(std::string name, const TimeInterval& time_interval,
                             const std::vector<DataInterval>& interval_list, bool save_file_names)
    : SyncParameter(std::move(name), interval_list, save_file_names) {
    time_interval_ = time_interval;
    calcExtendedInfo();
}

SyncParameter::~SyncParameter() {
    current_chunk_ = chunk_;

    while (current_chunk_) {
        chunk_ = current_chunk_->getNext();
        delete current_chunk_;
        current_chunk_ = chunk_;
    }
}

// is not supported yet

// is not supported yet

inline std::vector<int64_t> SyncParameter::read_dots(Dot* dot_buffer, size_t max_point_number, double from, double to) {
    return {-1};
}

inline std::vector<int64_t> SyncParameter::read_dots(double* top_buffer_ptr, double* bottom_buffer_ptr,
                                                     double* time_buffer_ptr, double from, double to,
                                                     size_t max_point_number_to_read) {
    return {-1};
}

inline bool SyncParameter::open(bool open_to_write) {
    if (opened_to_read_ | opened_to_write_) return false;

    opened_to_read_ = !open_to_write;
    opened_to_write_ = open_to_write;
    return opened_to_read_;
}

inline bool SyncParameter::close() {
    if (opened_to_read_ | opened_to_write_) {
        opened_to_read_ = false;
        opened_to_write_ = false;
        return true;
    }

    return false;
}

bool SyncParameter::seek(int64_t point_umber) { return false; }

uint64_t SyncParameter::write(char* data_buffer_ptr, uint64_t point_number) {
    ex_size_t bytes_to_write = point_number * sizeof_data_type_;
    ex_size_t written_bytes = 0;

    while ((bytes_to_write != 0) && (current_chunk_ != nullptr)) {
        written_bytes = current_chunk_->write(data_buffer_ptr, bytes_to_write, calc_min_max_ptr_);
        bytes_to_write -= written_bytes;
        current_chunk_ = current_chunk_->getNextIfNoDataToRead();
    }
    return written_bytes;
}
      
uint64_t SyncParameter::read(char* data_buffer_ptr, uint64_t point_number) {

    ex_size_t bytes_to_read = point_number * sizeof_data_type_;
    ex_size_t readed_bytes = 0;

    while ((bytes_to_read != 0) && (current_chunk_ != nullptr)) {
        readed_bytes = current_chunk_->read(data_buffer_ptr, bytes_to_read);
        data_buffer_ptr += readed_bytes;
        bytes_to_read -= readed_bytes;
        current_chunk_ = current_chunk_->getNextIfNoDataToRead();
    }
    return readed_bytes;
}
 

uint64_t SyncParameter::getVirtualSize() const {
    if (interval_list_.size() == 1) return ((uint64_t)interval_list_.front().size) / sizeof_data_type_;
    else
        return (uint64_t)(frequency_ * (time_interval_.end - time_interval_.bgn + additional_time_));
}

ParameterIfs* SyncParameter::intersection(ParameterIfs* prm, PrmTypesEn target_ty, const std::string& name) {
    auto parameter_a = this;
    auto parameter_b = (SyncParameter*)prm;

    if (parameter_a == parameter_b) return this;

    if (parameter_a->frequency_ != parameter_b->frequency_) {
        error_info_ = "different frequenters is not supported yet ";
        return nullptr;
    }

    if (parameter_a->frequency_ <= 0.0) {
        error_info_ = "async parameter is not supported yet";
        return nullptr;
    }

    auto frequency = int64_t(parameter_a->frequency_);
    std::vector<DataInterval> ret;

    for (const auto& a : parameter_a->interval_list_) {
        for (const auto& b : parameter_b->interval_list_) {
            auto t = a & b;
            if (!isEmpty(t)) {
                auto interval = createSyncIntervalByFrequency(t, frequency, target_ty);
                if (!ret.empty()) {
                    auto t2 = ret.back() || interval;
                    if (!isEmpty(t)) ret.back() = createSyncIntervalByFrequency(t2, frequency, target_ty);
                    else
                        ret.push_back(interval);
                } else
                    ret.push_back(interval);
            }
        }
    }

    int64_t offset = 0;
    int64_t size = 0;
    for (auto& i : ret) {
        i.offs = offset + size;
        offset = i.offs;
        size = i.size;
    }

    return new SyncParameter(name, time_interval_, ret);
}

SyncParameter* SyncParameter::enlargeFrequency(int64_t freq_factor, PrmTypesEn target_ty,
                                               const std::string& name) const {
    auto parameter_a = this;

    std::vector<DataInterval> data_interval;

    if ((-1 <= freq_factor) && (freq_factor <= 1)) return nullptr;
    if ((freq_factor < -1) && (int64_t(this->frequency_) % freq_factor)) return nullptr;

    for (const DataInterval& a : parameter_a->interval_list_) {
        auto interval = createSyncInterval(a, freq_factor);
        data_interval.push_back(interval);
    }

    return new SyncParameter(name, time_interval_, data_interval);
}

ParameterIfs* SyncParameter::retyping(PrmTypesEn target_ty, const std::string& name) {
    if (getPrmType() == target_ty) return this;

    std::vector<DataInterval> data_interval;
    for (const DataInterval& a : interval_list_) {
        auto interval = createSyncInterval(a, target_ty);
        data_interval.push_back(interval);
    }
    return new SyncParameter(name, time_interval_, data_interval);
}

ParameterIfs* SyncParameter::newParameter() {
    return new SyncParameter("", this->getMainTimeInterval(), this->getDataIntervalList(), false);
}

inline bool SyncParameter::calcExtendedInfo() {
    if (interval_list_.size()) {
        frequency_ = interval_list_.front().frequency;
        for (auto i : interval_list_)
            if (frequency_ != i.frequency) {
                error_info_ = "calcExtendedInfo - different frequencys";
                return false;
            }
    }
    return true;
}

inline void SyncParameter::openNewInterval(double di_index) {
    if (ifs_) {
        current_interval_index_ = (int64_t)di_index;
        ifs_->close();
        delete ifs_;
        ifs_ = nullptr;
    }

    const DataInterval& current_interval = getCurrentInterval();
    std::string file_name = (current_interval.local ? work_directory_ + "/" : "") + current_interval.file_name;

    if (opened_to_read_) {
        ifs_ = new std::fstream(file_name, std::ios::in | std::ios::binary);
        ifs_->seekg(current_interval.offs);
    } else if (opened_to_write_) {
        if (current_interval.offs == 0) ifs_ = new std::fstream(file_name, std::ios::out | std::ios::binary);
        else
            ifs_ = new std::fstream(file_name, std::ios::out | std::ios::binary | std::ios::app);
    }
    // std::ios::app |  std::ios::trunc
}

// 23152640
// 225988608
