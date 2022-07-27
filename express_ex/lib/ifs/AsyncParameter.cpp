#include "ifs/AsyncParameter.h"

#include <set>

#include "common/common.h"
#include "ifs/parameterIO.h"


AsyncParameter::AsyncParameter(const AsyncParameter& prm)
    : ParameterIfs(prm),
      current_interval_index_(prm.current_interval_index_),
      intermediate_buffer_(prm.intermediate_buffer_),
      time_buffer_(prm.time_buffer_),
      unused_points_in_current_interval_(prm.unused_points_in_current_interval_),
      data_size_(prm.data_size_),
      data_size_factor_(prm.data_size_factor_),
      is_new_interval_(prm.is_new_interval_),
      parent_parameter_(this),
      id_(DatasetsStorage_ifs::kDefaultId),
      ds_storage_(nullptr) {
    init();
}

AsyncParameter::AsyncParameter(const std::string& name, const std::vector<ExDataInterval>& interval_list,
                               bool save_file_names) {
    name_ = name;
    parent_parameter_ = this;
    interval_list_ = interval_list;

    if (!save_file_names) {
        for (auto& i : interval_list_) {
            i.file_name = "";
        }
    }

    // if (!interval_list_.empty()) {
    data_size_ = sizeOfTy(getPrmType());
    data_size_factor_ = (data_size_ + prm_time_size_);
    type_ = interval_list_.front().type;
    calc_min_max_ptr_ = gCalcMinMaxSelect(type_);

    //}

    auto bgn = interval_list_.front().ti.time;
    auto last_ti = interval_list_.back().ti;
    auto end = last_ti.duration + double(last_ti.time) / double(1ll << 32);

    time_interval_ = {bgn, end};
    init();
}

AsyncParameter::AsyncParameter(const std::string& name, const ExTimeInterval& time_interval,
                               const std::vector<ExDataInterval>& interval_list, bool save_file_names)
    : AsyncParameter(name, interval_list, save_file_names) {
    parent_parameter_ = this;
    time_interval_ = time_interval;
}

void AsyncParameter::init() {
#define CASE(DATA_TYPE, TIME_TYPE)                                \
    case sizeof(DATA_TYPE): {                                     \
        copyFromBuffer_ = ::copyFromBuffer<DATA_TYPE, TIME_TYPE>; \
        copyToBuffer_ = ::copyToBuffer<DATA_TYPE, TIME_TYPE>;     \
        break;                                                    \
    }

#define SWITCH(TIME_TYPE)          \
    switch (data_size_) {          \
        CASE(uint64_t, TIME_TYPE); \
        CASE(uint32_t, TIME_TYPE); \
        CASE(uint16_t, TIME_TYPE); \
        CASE(uint8_t, TIME_TYPE);  \
    default:                       \
        break;                     \
    }

    if (prm_time_size_ == 8) {
        SWITCH(uint64_t);
    } else if (prm_time_size_ == 4) {
        SWITCH(uint32_t);
    }
#undef SWITCH
#undef CASE
}


AsyncParameter::~AsyncParameter() = default;

void AsyncParameter::readFromBuffer(char* data_buffer_ptr, uint64_t points_to_read) {
    ExDataInterval& di = interval_list_[current_interval_index_];
    if (calc_min_max_ptr_) calc_min_max_ptr_(data_buffer_ptr, points_to_read, di.val_max, di.val_min, is_new_interval_);
    copyFromBuffer_(this, data_buffer_ptr, points_to_read);
    /*
    if (data_size_ == 8) copyFromBuffer((uint64_t*)data_buffer_ptr, points_to_read);
    else if (data_size_ == 4) copyFromBuffer((uint32_t*)data_buffer_ptr, points_to_read);
    else if (data_size_ == 2) copyFromBuffer((uint16_t*)data_buffer_ptr, points_to_read);
    else if (data_size_ == 1) copyFromBuffer((uint8_t*)data_buffer_ptr, points_to_read);
     */
}

inline bool AsyncParameter::open(bool open_to_write) {
    unused_points_in_current_interval_ = interval_list_[current_interval_index_].getPointsNumber();

    if (isOpened()) return false;

    openNewInterval();
    return true;
}

bool AsyncParameter::close() {
    if (isOpened()) {
        ds_storage_->closeDataset(id_);
        id_ = DatasetsStorage_ifs::kDefaultId;
        return true;
    }
    return false;
}

uint64_t AsyncParameter::write(char* data_buffer_ptr, uint64_t points_to_write) {
    if (!isOpened()) return points_to_write;


    intermediate_buffer_.resize(points_to_write * data_size_factor_);
    intermediate_buffer_.resetPos();

    uint64_t base = points_to_write;
    auto* ptr = (uint8_t*)data_buffer_ptr;

    if (interval_list_.size() <= current_interval_index_) return 0;

    while (points_to_write) {
        if (points_to_write >= unused_points_in_current_interval_) {
            readFromBuffer(data_buffer_ptr, unused_points_in_current_interval_);
            current_interval_index_++;
            points_to_write -= unused_points_in_current_interval_;
            if (interval_list_.size() > current_interval_index_) {
                unused_points_in_current_interval_ = interval_list_[current_interval_index_].size / data_size_factor_;
                openNewInterval();
            } else {
                // it is error case (impossible case)
                break;
            }
            is_new_interval_ = true;
        } else {
            readFromBuffer(data_buffer_ptr, points_to_write);
            unused_points_in_current_interval_ -= points_to_write;
            points_to_write = 0;
            is_new_interval_ = false;
        }
    }

    ds_storage_->writeToDataset(id_, intermediate_buffer_.base_ptr_, (base - points_to_write) * data_size_factor_);

    return base - points_to_write;
}

void AsyncParameter::readRawData(uint64_t points_to_read) {
    ds_storage_->readFromDataset(id_, intermediate_buffer_.current_ptr_, points_to_read * data_size_factor_);
    intermediate_buffer_.current_ptr_ += points_to_read * data_size_factor_;
}

inline void AsyncParameter::openNewInterval() {
    if (isOpened()) {
        ds_storage_->closeDataset(id_);
        id_ = DatasetsStorage_ifs::kDefaultId;
    }

    const ExDataInterval& current_interval = getCurrentInterval();
    auto file_name = current_interval.file_name;
    ds_storage_ = current_interval.ds;


    id_ = ds_storage_->openDataset(file_name.c_str());

    if (current_interval.offset > 0) ds_storage_->seek(id_, current_interval.offset);


    if (id_ < 0) {
        close();
        error_info_ = file_name + "can't be opened.";
        print_IR_error(error_info_);
    }
}

uint64_t AsyncParameter::read(char* data_buffer_ptr, uint64_t points_to_read) {
    if (!isOpened()) AsyncParameter::open();

    intermediate_buffer_.resize(points_to_read * data_size_factor_);
    intermediate_buffer_.resetPos();
    time_buffer_.resize(points_to_read * prm_time_size_);

    uint64_t base = points_to_read;

    if (interval_list_.size() <= current_interval_index_) return 0;

    while (points_to_read) {
        if (points_to_read >= unused_points_in_current_interval_) {
            readRawData(unused_points_in_current_interval_);
            current_interval_index_++;
            points_to_read -= unused_points_in_current_interval_;
            if (interval_list_.size() > current_interval_index_) {
                unused_points_in_current_interval_ = interval_list_[current_interval_index_].size / data_size_factor_;
                openNewInterval();

            } else {
                //////////////////
                // fill the buffer with zero values
                //////////////////
                break;
            }
        } else {
            readRawData(points_to_read);
            unused_points_in_current_interval_ -= points_to_read;
            points_to_read = 0;
        }
    }

    time_buffer_.replaceLastData();
    intermediate_buffer_.resetPos();

    copyToBuffer_(this, data_buffer_ptr, base);
    /*
    if (data_size_ == 8) copyToBuffer((uint64_t*)data_buffer_ptr, base);
    else if (data_size_ == 4) copyToBuffer((uint32_t*)data_buffer_ptr, base);
    else if (data_size_ == 2) copyToBuffer((uint16_t*)data_buffer_ptr, base);
    else if (data_size_ == 1) copyToBuffer((uint8_t*)data_buffer_ptr, base);
    */
    time_buffer_.resetPos();

    return base - points_to_read;
}

inline uint64_t AsyncParameter::getVirtualSize() const {
    uint64_t total_size = 0;
    for (auto& interval : interval_list_) total_size = total_size + interval.size;

    auto s = ((ParameterIfs*)this)->getPrmType();

    total_size = total_size / (sizeOfTy(s) + prm_time_size_);
    return total_size;
}

ParameterIfs* AsyncParameter::intersection(ParameterIfs* b, PrmTypesEn target_ty, const std::string& name) {
    auto parameter_a = this;
    auto parameter_b = (AsyncParameter*)b;
    target_ty = PrmTypesEn(0x1000 | (uint64_t)target_ty);

    if (parameter_a == parameter_b) return this;

    if (parameter_a->interval_list_.size() == parameter_b->interval_list_.size()) {
        auto il_size = parameter_a->interval_list_.size();
        for (size_t n = 0; n < il_size; n++) {
            ExDataInterval& interval_a = parameter_a->interval_list_[n];
            ExDataInterval& interval_b = parameter_b->interval_list_[n];

            if ((interval_a.size != interval_b.size)) {
                error_info_ = "different sizes of intervals.";
                return nullptr;
            }
        }
    } else {
        error_info_ = "different number os intervals.";
        return nullptr;
    }

    std::vector<ExDataInterval> data_interval;
    uint64_t offset = 0;
    for (const auto& a : interval_list_) {
        auto interval = ExDataInterval(a.type);
        interval.setProperties(a, offset);
        offset += interval.size;

        data_interval.push_back(std::move(interval));
    }

    auto ret = new AsyncParameter(name, time_interval_, data_interval);
    ret->parent_parameter_ = parent_parameter_;

    return ret;
}



ParameterIfs* AsyncParameter::retyping(PrmTypesEn target_ty, const std::string& name) {
    target_ty = PrmTypesEn(0x1000 | (uint64_t)target_ty);

    if (getPrmType() == target_ty) return this;

    std::vector<ExDataInterval> data_interval;
    uint64_t offset = 0;
    for (const auto& a : interval_list_) {
        auto interval = ExDataInterval(target_ty);
        interval.setProperties(a, offset);
        offset += interval.size;

        data_interval.push_back(std::move(interval));
    }

    auto ret = new AsyncParameter(name, time_interval_, data_interval);
    ret->parent_parameter_ = parent_parameter_;

    return ret;
}

ParameterIfs* AsyncParameter::newParameter() {
    auto* p = new AsyncParameter("", this->getMainTimeInterval(), this->getDataIntervalList(), false);
    p->parent_parameter_ = parent_parameter_;
    return p;
}
