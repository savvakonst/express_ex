#ifndef PARAMETERIFS_H
#define PARAMETERIFS_H
#include <algorithm>
#include <complex>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "config.h"
#include "ExStreamIfs.h"


#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 5208)
#    pragma warning(disable : 4251)
#    pragma warning(disable : 4100)
#    pragma warning(disable : 4189)
#endif

enum class PrmTypesEn : uint64_t
{
    PRM_TYPE_U08 = 0x0001,
    PRM_TYPE_U16 = 0x0002,
    PRM_TYPE_U32 = 0x0004,
    PRM_TYPE_U64 = 0x0008,
    PRM_TYPE_I08 = 0x0011,
    PRM_TYPE_I16 = 0x0012,
    PRM_TYPE_I32 = 0x0014,
    PRM_TYPE_I64 = 0x0018,
    PRM_TYPE_F32 = 0x0024,
    PRM_TYPE_F64 = 0x0028,

    PRM_TYPE_U08_T = 0x1001,
    PRM_TYPE_U16_T = 0x1002,
    PRM_TYPE_U32_T = 0x1004,
    PRM_TYPE_U64_T = 0x1008,
    PRM_TYPE_I08_T = 0x1011,
    PRM_TYPE_I16_T = 0x1012,
    PRM_TYPE_I32_T = 0x1014,
    PRM_TYPE_I64_T = 0x1018,
    PRM_TYPE_F32_T = 0x1024,
    PRM_TYPE_F64_T = 0x1028,

    PRM_TYPE_U08_TU = 0x5001,
    PRM_TYPE_U16_TU = 0x5002,
    PRM_TYPE_U32_TU = 0x5004,
    PRM_TYPE_U64_TU = 0x5008,
    PRM_TYPE_I08_TU = 0x5011,
    PRM_TYPE_I16_TU = 0x5012,
    PRM_TYPE_I32_TU = 0x5014,
    PRM_TYPE_I64_TU = 0x5018,
    PRM_TYPE_F32_TU = 0x5024,
    PRM_TYPE_F64_TU = 0x5028,

    PRM_TYPE_UNKNOWN = 0xffff
};

struct TimeInterval {
    double bgn = 0.0;
    double end = 0.0;
};

struct DataInterval {
    union {
        int64_t int_type_representation;
        PrmTypesEn type;
    };
    int64_t offs;
    int64_t size;
    double frequency;
    double val_max;
    double val_min;
    TimeInterval time_interval;
    std::string file_name;
    bool local;
};

struct Dot {
    double val_max;
    double val_min;
    double time;
};

inline size_t sizeOfTy(const PrmTypesEn arg) { return ((uint64_t)arg) & 0xf; }

inline size_t sizeOfTimeTy(PrmTypesEn arg) {
    if (0x5000 == (((uint64_t)arg) & 0xf000)) return 0x8;
    else
        return 0x4;
}

// TypeEn      PRMType2JITType(PrmTypesEn arg);
// PrmTypesEn  JITType2PRMType(TypeEn arg);

DLL_EXPORT std::string toString(PrmTypesEn arg);

inline std::stringstream& stream(std::stringstream& OS, const DataInterval& di, std::string offset) {
    OS << offset << "DataInterval{\n";
    OS << offset << "  type: " << toString(di.type) << "\n";
    OS << offset << "  offs: " << di.offs << "\n";
    OS << offset << "  size: " << di.size << "\n";
    OS << offset << "  frequency: " << di.frequency << "\n";
    OS << offset << "  time_interval.end: " << di.time_interval.end << "\n";
    OS << offset << "  time_interval.bgn: " << di.time_interval.bgn << "\n";
    OS << offset << "  file_name: " << di.file_name << "\n";
    OS << offset << "}\n";
    return OS;
}

inline bool isEmpty(TimeInterval i) { return i.bgn >= i.end; }

inline double stdmax(double a, double b) { return a > b ? a : b; }
inline double stdmin(double a, double b) { return a < b ? a : b; }

inline TimeInterval operator&(DataInterval a, DataInterval b) {
    TimeInterval ret = {stdmax(a.time_interval.bgn, b.time_interval.bgn),
                        stdmin(a.time_interval.end, b.time_interval.end)};
    return ret;
}

inline TimeInterval operator||(DataInterval a, DataInterval b) {
    auto c = a & b;
    if (c.bgn <= c.end)
        return {stdmin(a.time_interval.bgn, b.time_interval.bgn), stdmax(a.time_interval.end, b.time_interval.end)};
    return {0.0};
}

inline DataInterval createSyncIntervalByFrequency(TimeInterval time_interval, int64_t frequency, PrmTypesEn target_ty,
                                                  int64_t offset = 0, const std::string& filename = "",
                                                  bool local = true) {
    DataInterval interval{};

    interval.type = target_ty;
    interval.offs = 0;

    constexpr auto tick_size = 1024;
    auto begin = int64_t(time_interval.bgn * tick_size);
    auto end = int64_t(time_interval.end * tick_size);

    auto tick_begin = tick_size * (begin / tick_size);

    if (frequency > tick_size) {
        interval.size = int64_t(sizeOfTy(target_ty)) * (end - begin + 1) * (frequency / tick_size);
        std::cout << "interval.size: " << interval.size << ": " << (end - begin + 1);
    } else {
        auto tmp = (end - tick_begin + 1) * frequency;
        auto len = tmp / tick_size + ((tmp % tick_size) > 0);

        tmp = (begin % tick_size) * frequency;
        len -= tmp / tick_size + ((tmp % tick_size) > 0);

        interval.size = int64_t(sizeOfTy(target_ty)) * len;
    }

    interval.frequency = double(frequency);
    interval.time_interval = time_interval;

    interval.file_name = filename;
    interval.local = local;

    return interval;
}

inline DataInterval createSyncInterval(const DataInterval& src, int64_t freq_factor, const std::string& filename = "",
                                       bool local = true) {
    DataInterval interval{};

    int64_t dst_frequency;
    auto src_frequency = int64_t(src.frequency);

    if (freq_factor > 1) {
        dst_frequency = src_frequency * freq_factor;
        interval.size = src.size * freq_factor;
        interval.offs = src.offs * freq_factor;
    } else if (freq_factor < -1) {
        dst_frequency = src_frequency / freq_factor;
        interval.size = src.size / freq_factor;
        interval.offs = src.offs / freq_factor;
    } else {
        dst_frequency = src_frequency;
        interval.size = src.size;
        interval.offs = src.offs;
    }

    interval.type = src.type;

    interval.frequency = double(dst_frequency);
    interval.time_interval = src.time_interval;

    interval.file_name = filename;
    interval.local = local;

    return interval;
}

inline DataInterval createSyncInterval(const DataInterval& src, PrmTypesEn target_ty, const std::string& filename = "",
                                       bool local = true) {
    DataInterval interval{};

    interval.type = target_ty;
    interval.offs = 0;

    interval.size = src.size / sizeOfTy(src.type) * sizeOfTy(target_ty);
    interval.frequency = src.frequency;
    interval.time_interval = src.time_interval;

    interval.file_name = filename;
    interval.local = local;

    return interval;
}

inline DataInterval createAsyncIntervalBySize(TimeInterval time_interval, int64_t size, PrmTypesEn target_ty,
                                              const std::string& filename = "", bool local = true) {
    DataInterval interval{};

    interval.type = target_ty;
    interval.offs = 0;
    interval.size = (int64_t)size;
    interval.frequency = 0;
    interval.time_interval = time_interval;

    interval.file_name = filename;
    interval.local = local;

    return interval;
}

/* std::pair<uint64_t, uint64_t> getBorders(const DataInterval& di) {
    constexpr auto tick_size = 1024;

    auto begin = int64_t(di.time_interval.bgn * tick_size);
    auto frequency = int64_t(di.frequency);

    if (frequency > tick_size) {
        begin + sizeOfTy(di.type) * frequency / tick_size;
    }

    auto offset = begin % tick_size

                  auto end = di.size / int64_t(sizeOfTy(di.type));

    return {
        begin,
    };
}*/

typedef void (*calcMinMaxTy)(char* carg, int64_t Number, double& dmax, double& dmin, bool init);

class BareChunk;

class DLL_EXPORT ParameterIfs {
   public:
    virtual ~ParameterIfs() = default;

    const std::string& getName() const { return name_; }

    const TimeInterval& getMainTimeInterval() const { return time_interval_; }

    const std::vector<DataInterval>& getDataIntervalList() const { return interval_list_; }

    virtual PrmTypesEn getPrmType() {
        if (interval_list_.size()) type_ = interval_list_[0].type;
        return type_;
    }

    virtual uint64_t getVirtualSize() const = 0;

    virtual bool isAsync() = 0;

    // is not supported yet
    virtual std::vector<int64_t> read_dots(Dot* dot_buffer, size_t max_point_number, double from, double to) = 0;

    // is not supported yet
    virtual std::vector<int64_t> read_dots(double* top_buffer_ptr, double* bottom_buffer_ptr, double* time_buffer_ptr,
                                           double from, double to, size_t max_point_number_to_read) = 0;

    virtual bool open(bool open_to_write = false) = 0;
    virtual bool close() = 0;

    virtual uint64_t write(char* data_buffer_ptr, uint64_t point_number) = 0;
    virtual uint64_t read(char* data_buffer_ptr, uint64_t point_number) = 0;

    virtual ParameterIfs* intersection(ParameterIfs* b, PrmTypesEn target_ty = PrmTypesEn::PRM_TYPE_UNKNOWN,
                                       const std::string& name = "") = 0;
    virtual ParameterIfs* retyping(PrmTypesEn target_ty = PrmTypesEn::PRM_TYPE_UNKNOWN,
                                   const std::string& name = "") = 0;

    virtual void setName(const std::string& name, bool single_file = true) {
        name_ = name;

        if ((interval_list_.size() == 1) || single_file) {
            for (auto& i : interval_list_) i.file_name = name + ".dat";
            return;
        }

        int64_t index = 0;
        for (auto& i : interval_list_) i.file_name = name + "_" + std::to_string(index++) + ".dat";
    }

    int64_t getDataIntervalIndex(double time) {
        for (int64_t i = (current_interval_index_ < 0 ? 0 : current_interval_index_); i < number_of_intervals_; i++) {
            DataInterval& a = interval_list_[(size_t)i];
            if ((a.time_interval.bgn <= time) && (time < (a.time_interval.end + additional_time_))) return i;
            // this is very bad idea, but it will be here  until Nikolai fixes the problem with interval boundaries
        }
        return -1;
    }

    void setPath(std::string dirname) { work_directory_ = dirname; }
    void setLocal(bool val = true) {
        for (auto& i : interval_list_) i.local = val;
    }
    void addInterval(const DataInterval& interval) { interval_list_.push_back(interval); }

    virtual ParameterIfs* newParameter() = 0;

    virtual std::stringstream& stream(std::stringstream& OS, std::string offset = "") const {
        OS << offset << "ParameterInfo{\n";
        OS << offset << "  parameter_name: " << name_ << "\n";
        OS << offset << "  time_interval.end: " << time_interval_.end << "\n";
        OS << offset << "  time_interval.bgn: " << time_interval_.bgn << "\n";
        OS << offset << "  interval_list: ["
           << "\n";
        for (auto interval : interval_list_) ::stream(OS, interval, "    ");
        OS << offset << "  ]\n";
        OS << offset << "}\n";
        return OS;
    }

    friend class ParametersDB;

   protected:
    calcMinMaxTy calc_min_max_ptr_ = nullptr;

    std::fstream* ifs_ = nullptr;

    std::string name_ = "";
    TimeInterval time_interval_;
    std::vector<DataInterval> interval_list_;

    const double additional_time_ = 0.0009765625;  // this is very bad idea, but it will be here  until nikolay fixes
                                                   // the problem with interval boundaries
    PrmTypesEn type_ = PrmTypesEn::PRM_TYPE_UNKNOWN;

    int64_t current_interval_index_ = 0;
    int64_t number_of_intervals_ = 0;

    int64_t sizeof_data_type_ = 0;
    int64_t point_number_ = 0;

    bool opened_to_read_ = false;
    bool opened_to_write_ = false;

    std::string work_directory_ = "";
    std::string error_info_ = "";
};




inline ExStreamIfs &operator<<(ExStreamIfs &stream,const ParameterIfs & arg) {
    std::stringstream std_stream;
    stream << arg.stream(std_stream).str() << " ";
    return stream;
}


#endif
