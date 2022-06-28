#include "ifs/SyncParameter.h"

#include <set>
#include <utility>

#include "Intervals.h"



SyncParameter::SyncParameter(std::string name, const std::vector<ExDataInterval>& interval_list, bool save_file_names) {
    name_ = std::move(name);

    interval_list_ = interval_list;

    if (!save_file_names) {
        for (auto& i : interval_list_) {
            i.file_name = "";
        }
    }

    if (!interval_list_.empty()) {
        sizeof_data_type_ = sizeOfTy(interval_list_.front().type);
        type_ = interval_list_.front().type;
        calc_min_max_ptr_ = g_calcMinMax_select(type_);
    }

    auto bgn = interval_list_.front().ti.time;
    auto end = interval_list_.back().getEndTime();

    time_interval_ = {bgn, end};
    calcExtendedInfo();

    const ExDataInterval* last_interval = nullptr;
    for (auto& i : interval_list_) {
        if (last_interval) {
            ExDataInterval bare_interval(type_);
            bare_interval.setProperties(i.ti.time, last_interval->getEndTime(), frequency_, 0);
            if (bare_interval.size) current_chunk_->addNextChunk(new BareChunk(bare_interval.size));
        }

        BareChunk* chunk = new Chunk(&i);

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

SyncParameter::SyncParameter(std::string name, const ExTimeInterval& time_interval,
                             const std::vector<ExDataInterval>& interval_list, bool save_file_names)
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



bool SyncParameter::open(bool open_to_write) {
    if (opened_ ) return false;
    opened_ = true;

    return opened_;
}

inline bool SyncParameter::close() {
    if (!opened_ ) return false;

    opened_ = false;
    return true;
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
    else return (uint64_t)(frequency_ * (time_interval_.time - timeFromDouble(time_interval_.duration)));
}



ParameterIfs* SyncParameter::intersection(ParameterIfs* prm, PrmTypesEn target_ty, const std::string& name) {
    auto parameter_a = this;
    auto parameter_b = (SyncParameter*)prm;

    if (parameter_a == parameter_b) return this;

    if (parameter_a->frequency_ != parameter_b->frequency_) {
        error_info_ = "different frequenters is not supported yet ";
        return nullptr;
    }

    if (parameter_a->frequency_ <= 0) {
        error_info_ = "sync parameter is has zero frequency";
        return nullptr;
    }

    auto frequency = parameter_a->frequency_;
    std::vector<ExDataInterval> ret;


    auto list_a = parameter_a->interval_list_;
    auto list_c = parameter_b->interval_list_;

    auto a_i = list_a.begin(), c_i = list_c.begin();

    uint64_t offset = 0;
    auto subIntersection = [target_ty, frequency](auto& forward_i, auto& backward_i, uint64_t offset) {
        auto &fo = *forward_i, &ba = *backward_i;

        ExDataInterval target(target_ty);
        auto ba_end = ba.getEndTime();
        if (fo.ti.time < ba_end) {
            /*they intersect*/
            auto fo_end = fo.getEndTime();
            if (ba_end < fo_end) {
                target.setProperties(fo.ti.time, ba_end, frequency, offset);
                ++backward_i;
            } else {
                target.setProperties(fo, offset);
                ++forward_i;
            }
        } else { /*they don't intersect*/
            ++backward_i;
        }
        return target;
    };


    while ((a_i != list_a.end()) || (c_i != list_c.end())) {
        ExDataInterval di =
            (a_i->ti.time >= c_i->ti.time) ? subIntersection(a_i, c_i, offset) : subIntersection(c_i, a_i, offset);
        offset += di.size;
        ret.push_back(std::move(di));
    }


    return new SyncParameter(name, time_interval_, ret);
}

SyncParameter* SyncParameter::enlargeFrequency(int64_t freq_factor, PrmTypesEn target_ty,
                                               const std::string& name) const {
    auto parameter_a = this;

    std::vector<ExDataInterval> data_interval;

    if ((-1 <= freq_factor) && (freq_factor <= 1)) return nullptr;
    if ((freq_factor < -1) && (int64_t(this->frequency_) % freq_factor)) return nullptr;


    uint64_t offset = 0;
    for (const ExDataInterval& src : parameter_a->interval_list_) {
        ExDataInterval interval(src.type);

        int64_t dst_frequency;
        if (freq_factor > 1) {
            dst_frequency = src.frequency * freq_factor;
        } else if (freq_factor < -1) {
            dst_frequency = src.frequency / freq_factor;
        } else {
            dst_frequency = src.frequency;
        }

        interval.setProperties(src.ti.time, src.getEndTime(), dst_frequency, offset);

        offset += interval.size;
        data_interval.push_back(std::move(interval));
    }

    return new SyncParameter(name, time_interval_, data_interval);
}

ParameterIfs* SyncParameter::retyping(PrmTypesEn target_ty, const std::string& name) {
    if (getPrmType() == target_ty) return this;

    std::vector<ExDataInterval> data_interval;
    uint64_t offset = 0;
    for (const ExDataInterval& src : interval_list_) {
        ExDataInterval interval(target_ty);

        interval.setProperties(src.ti.time, src.getEndTime(), src.frequency, offset);

        offset += interval.size;
        data_interval.push_back(std::move(interval));
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
