#ifndef PARAMETERIFS_H
#define PARAMETERIFS_H
#include <algorithm>
#include <complex>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "DatasetsStorage_ifs.h"
#include "ExStreamIfs.h"
#include "config.h"


typedef int64_t ex_time_t;
typedef int64_t ex_size_t;


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


bool isAsync(PrmTypesEn arg) {
    if (arg == PrmTypesEn::PRM_TYPE_UNKNOWN) return false;
    return (bool)(0x1000 & (uint64_t)arg);
}

inline ex_size_t sizeOfTy(const PrmTypesEn arg) { return ex_size_t(arg) & 0xf; }

inline ex_size_t sizeOfTimeTy(PrmTypesEn arg) {
    auto time_part = (ex_size_t(arg) & 0xf000);
    if (0x5000 == time_part) return 0x8;
    else if (0x1000 == time_part) return 0x4;
    else return 0x0;
}

DLL_EXPORT std::string toString(PrmTypesEn arg);


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
    DatasetsStorage_ifs* ds = nullptr;
};



inline ex_time_t timeFromDouble(double arg) { return ex_time_t(arg * (1ll << 10)) << 22; }

struct ExTimeInterval {
    // ExTimeInterval() : time(0), duration(0) {}

    [[maybe_unused]] ExTimeInterval(ex_time_t begin, double duration) : time(begin), duration(duration) {}
    [[maybe_unused]] ExTimeInterval(double begin, double duration) : time(timeFromDouble(begin)), duration(duration) {}


    //! Do not use this function in critical places? mabe for printing.
    [[maybe_unused]] ExTimeInterval(ex_time_t begin, ex_time_t duration)
        : time(begin), duration(double(begin) / double(1ll << 32)) {}


    ExTimeInterval& operator=(const ExTimeInterval& i) {
        if (this == &i) return *this;
        time = i.time;
        duration = i.duration;
        return *this;
    }

    union {
        ex_time_t time; /*!< full time. One step equal 1.0/(1<<32) sec sec */
        struct {
            int32_t time_int{};   /*!< integet part of time. One step equal 1 sec */
            uint32_t time_frac{}; /*!< fractional part of time. One step equal 1.0/(1<<32) sec */
        };
    };

    /**
     * It is time offset from first to last element   duration = first_point_time - last_point_time.
     * If interval contains one point: then the duration = 0.
     * It member doesn't play significant role, using double for compatibility with ui api
     */
    double duration;
};


struct ExDataInterval {
    explicit ExDataInterval(PrmTypesEn type_arg) : type(type_arg) {}


    ExTimeInterval ti = {0ll, 0.0};
    uint64_t offset = 0;
    uint64_t size = 0;
    int64_t frequency = 0; /*!< this is a signed variable for possible expansion to fractional frequencies */
    PrmTypesEn type = PrmTypesEn::PRM_TYPE_UNKNOWN;


    double val_max = 0;
    double val_min = 0;
    DatasetsStorage_ifs* ds = nullptr;
    std::string file_name;


    /**
     * @return if sync returns the time of the point which would follow the end point
     * if async returns time of the end point.
     */
    ex_time_t getEndTime() const {
        return isAsync(type) ? ex_time_t(ti.time + timeFromDouble(ti.duration))
                             : ti.time + ((ex_time_t((size / sizeOfTy(type)) << 24) / frequency) << 8);
    };

    /**
     * applicable for both async and sync
     * @return return amount of points
     */
    uint64_t getPointsNumber() const { return size / (sizeOfTy(type) + sizeOfTimeTy(type)); }


    /**
     * it is useful only for sync parameters,
     * It doesn't set the fields: val_max, val_min ,ds , file_name, they must be set manually
     * @return return true if successful
     */
    bool setProperties(ex_time_t begin, ex_time_t end, int64_t freq, uint64_t offs) {
        if (isAsync(type)) return false;

        offset = offs;
        frequency = freq;
        size = ((((end - begin) >> 8) * freq) >> 24) * sizeOfTy(type);
        ti = ExTimeInterval(begin, end - begin);

        return true;
    }

    /**
     * copies and adapts properties from the parent to its own type,
     * by size recalculation.
     * It doesn't set the fields: val_max, val_min ,ds , file_name, they must be set manually
     * @return return true if successful
     */
    bool setProperties(const ExDataInterval& parent, uint64_t offs) {
        offset = offs;
        frequency = parent.frequency;
        size = parent.getPointsNumber() * (sizeOfTy(type) + sizeOfTimeTy(type));
        ti = parent.ti;
        return true;
    }
};



// TypeEn      PRMType2JITType(PrmTypesEn arg);
// PrmTypesEn  JITType2PRMType(TypeEn arg);



inline std::stringstream& stream(std::stringstream& s, const ExTimeInterval& time_interval, const std::string& offset) {
    s << offset << "ti.time_int: " << time_interval.time_int << "\n";
    s << offset << "ti.time_frac " << std::setfill('0') << std::setw(5) << std::right << "0x" << std::hex
      << time_interval.time_frac << "\n"
      << std::dec;
    s << offset << "ti.duration: " << std::dec << time_interval.duration << "\n";
}

inline std::stringstream& stream(std::stringstream& OS, const ExDataInterval& di, const std::string& offset) {
    OS << offset << "DataInterval{\n";
    OS << offset << "  type: " << toString(di.type) << "\n";
    OS << offset << "  offs: " << di.offset << "\n";
    OS << offset << "  size: " << di.size << "\n";
    OS << offset << "  frequency: " << di.frequency << "\n";
    ::stream(OS, di.ti, offset + "  ");
    OS << offset << "  file_name: " << di.file_name << "\n";
    OS << offset << "}\n";
    return OS;
}


inline bool isEmpty(ExTimeInterval i) { return i.duration <= 0; }



typedef void (*calcMinMaxTy)(char* carg, int64_t Number, double& dmax, double& dmin, bool init);

class BareChunk;

class DLL_EXPORT ParameterIfs {
   public:
    virtual ~ParameterIfs() = default;

    const std::string& getName() const { return name_; }

    ExTimeInterval getMainTimeInterval() const { return time_interval_; }

    const std::vector<ExDataInterval>& getDataIntervalList() const { return interval_list_; }

    virtual PrmTypesEn getPrmType() {
        if (!interval_list_.empty()) type_ = interval_list_[0].type;
        return type_;
    }

    virtual uint64_t getVirtualSize() const = 0;

    virtual bool isAsync() = 0;


    virtual bool open(bool open_to_write = false) = 0;
    virtual bool close() = 0;

    virtual uint64_t write(char* data_buffer_ptr, uint64_t point_number) = 0;
    virtual uint64_t read(char* data_buffer_ptr, uint64_t point_number) = 0;

    virtual ParameterIfs* intersection(ParameterIfs* b, PrmTypesEn target_ty = PrmTypesEn::PRM_TYPE_UNKNOWN,
                                       const std::string& name = "") = 0;

    virtual ParameterIfs* retyping(PrmTypesEn target_ty = PrmTypesEn::PRM_TYPE_UNKNOWN,
                                   const std::string& name = "") = 0;

    virtual void setName(const std::string& name, const std::string& extension = ".dat") {
        name_ = name;

        if ((interval_list_.size() == 1)) {
            for (auto& i : interval_list_) i.file_name = name + ".dat";
            return;
        }
    }



    void addInterval(const ExDataInterval& interval) { interval_list_.push_back(interval); }

    virtual ParameterIfs* newParameter() = 0;

    virtual std::stringstream& stream(std::stringstream& OS, const std::string& offset = "") const {
        OS << offset << "ParameterInfo{\n";
        OS << offset << "  parameter_name: " << name_ << "\n";
        ::stream(OS, time_interval_, offset + "  ");
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



    std::string name_;


    ExTimeInterval time_interval_ = {0.0, 0.0};



    std::vector<ExDataInterval> interval_list_;

    const double additional_time_ =
        0.0009765625;  // TODO this is very bad idea, but it will be here  until nikolay fixes
                       //  the problem with interval boundaries
    PrmTypesEn type_ = PrmTypesEn::PRM_TYPE_UNKNOWN;

    int64_t current_interval_index_ = 0;


    int64_t sizeof_data_type_ = 0;





    std::string error_info_;
};



inline ExStreamIfs& operator<<(ExStreamIfs& stream, const ParameterIfs& arg) {
    std::stringstream std_stream;
    stream << arg.stream(std_stream).str() << " ";
    return stream;
}


#endif
