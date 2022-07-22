#include "ifs/parameterIO.h"

#include <set>
#include <type_traits>

#include "common/common.h"
#include "ifs/AsyncParameter.h"
#include "ifs/SyncParameter.h"
#include "llvm/Support/JSON.h"



ExDataInterval getDataInterval(llvm::json::Value &data_fragment, llvm::json::Array &data_files_list);

calcMinMaxTy gCalcMinMaxSelect(PrmTypesEn arg) {
    int code = 0xFF & ((int)arg);

#define CALC_CASE_OP(T, SUB_T)         \
    case (sizeof(T) | ((SUB_T) << 4)): \
        return calcMinMax<T>

    switch (code) {
        CALC_CASE_OP(int8_t, 1);
        CALC_CASE_OP(int16_t, 1);
        CALC_CASE_OP(int32_t, 1);
        CALC_CASE_OP(int64_t, 1);
        CALC_CASE_OP(float, 2);
        CALC_CASE_OP(double, 2);
    default:
        return nullptr;
    };
#undef CALC_CASE_OP
}

TypeEn PRMType2JITType(PrmTypesEn arg) {
    switch (arg) {
    case PrmTypesEn::PRM_TYPE_U08:
        return TypeEn::unknown_jty;
    case PrmTypesEn::PRM_TYPE_U16:
        return TypeEn::unknown_jty;
    case PrmTypesEn::PRM_TYPE_U32:
        return TypeEn::unknown_jty;
    case PrmTypesEn::PRM_TYPE_U64:
        return TypeEn::unknown_jty;
    case PrmTypesEn::PRM_TYPE_I08:
        return TypeEn::int8_jty;
    case PrmTypesEn::PRM_TYPE_I16:
        return TypeEn::int16_jty;
    case PrmTypesEn::PRM_TYPE_I32:
        return TypeEn::int32_jty;
    case PrmTypesEn::PRM_TYPE_I64:
        return TypeEn::int64_jty;
    case PrmTypesEn::PRM_TYPE_F32:
        return TypeEn::float_jty;
    case PrmTypesEn::PRM_TYPE_F64:
        return TypeEn::double_jty;
    case PrmTypesEn::PRM_TYPE_U08_T:
        return TypeEn::unknown_jty;
    case PrmTypesEn::PRM_TYPE_U16_T:
        return TypeEn::unknown_jty;
    case PrmTypesEn::PRM_TYPE_U32_T:
        return TypeEn::unknown_jty;
    case PrmTypesEn::PRM_TYPE_U64_T:
        return TypeEn::unknown_jty;
    case PrmTypesEn::PRM_TYPE_I08_T:
        return TypeEn::int8_jty;
    case PrmTypesEn::PRM_TYPE_I16_T:
        return TypeEn::int16_jty;
    case PrmTypesEn::PRM_TYPE_I32_T:
        return TypeEn::int32_jty;
    case PrmTypesEn::PRM_TYPE_I64_T:
        return TypeEn::int64_jty;
    case PrmTypesEn::PRM_TYPE_F32_T:
        return TypeEn::float_jty;
    case PrmTypesEn::PRM_TYPE_F64_T:
        return TypeEn::double_jty;
    default:
        return TypeEn::unknown_jty;
    }
}

PrmTypesEn toPrmType(TypeEn arg) {
    if (isFloating(arg)) return (PrmTypesEn)(0x20 | sizeOfTy(arg));
    if (isInteger(arg)) return (PrmTypesEn)(0x10 | sizeOfTy(arg));
    return PrmTypesEn::PRM_TYPE_UNKNOWN;
}

std::string toString(PrmTypesEn arg) {
#define CASE_OP(NAME)      \
    case PrmTypesEn::NAME: \
        return #NAME
    switch (arg) {
        CASE_OP(PRM_TYPE_U08);
        CASE_OP(PRM_TYPE_U16);
        CASE_OP(PRM_TYPE_U32);
        CASE_OP(PRM_TYPE_U64);
        CASE_OP(PRM_TYPE_I08);
        CASE_OP(PRM_TYPE_I16);
        CASE_OP(PRM_TYPE_I32);
        CASE_OP(PRM_TYPE_I64);
        CASE_OP(PRM_TYPE_F32);
        CASE_OP(PRM_TYPE_F64);
        CASE_OP(PRM_TYPE_U08_T);
        CASE_OP(PRM_TYPE_U16_T);
        CASE_OP(PRM_TYPE_U32_T);
        CASE_OP(PRM_TYPE_U64_T);
        CASE_OP(PRM_TYPE_I08_T);
        CASE_OP(PRM_TYPE_I16_T);
        CASE_OP(PRM_TYPE_I32_T);
        CASE_OP(PRM_TYPE_I64_T);
        CASE_OP(PRM_TYPE_F32_T);
        CASE_OP(PRM_TYPE_F64_T);
    default:
        return "TypeEn::unknown_jty";
    }
#undef CASE_OP
}


inline int64_t &transformToInt64(uint64_t &arg) { return *((int64_t *)&arg); }

bool fromJSON(const llvm::json::Value &DataFragment, ExDataInterval &dataInterval) {
    llvm::json::Path::Root root("bare");
    llvm::json::ObjectMapper o(DataFragment, root);
    bool ret = true;

    ret &= o.map("Frequency", dataInterval.frequency);



    ret &= o.map("Offset", transformToInt64(dataInterval.size));
    ret &= o.map("Size", transformToInt64(dataInterval.size));

    ret &= o.map("Type", *((int64_t *)&dataInterval.type));

    double begin_time = 0., end_time = 0.;
    ret &= o.map("sTime.Begin", begin_time);
    ret &= o.map("sTime.End", end_time);
    dataInterval.ti = ExTimeInterval(begin_time, end_time - begin_time);
    return ret;
}
/*
static std::string convertUtf8PathToWinLocate(const std::string& str){
    std::wstring wstr;
    bool status = llvm::ConvertUTF8toWide(str, wstr);

    std::stringstream ss;
    ss << wstr.c_str();

    return ss.str();
}
*/
ExDataInterval getDataInterval(llvm::json::Value &data_fragment, llvm::json::Array &data_files_list) {
    ExDataInterval data_interval(PrmTypesEn::PRM_TYPE_UNKNOWN);
    fromJSON(data_fragment, data_interval);

    int64_t data_index;
    llvm::json::Path::Root root("bare");
    llvm::json::ObjectMapper O(data_fragment, root);
    O.map("Index", data_index);

    for (auto i : data_files_list) {
        llvm::json::Path::Root subroot("bare");
        llvm::json::ObjectMapper O(i, subroot);
        int64_t file_data_index;
        O.map("Index", file_data_index);

        if (data_index == file_data_index) {
            // std::string tmp;
            O.map("Name", data_interval.file_name);
            // TODO : need to threat "Local" field
            // O.map("Local", data_interval.local);
            // dataInterval.file_name = convertUtf8PathToWinLocate(tmp);
        }
    }

    return data_interval;
}

bool readParametersList(DatasetsStorage_ifs *storage, const std::string &express_f_name,
                        std::vector<ParameterIfs *> &parameter_list) {
    std::ifstream ifs(express_f_name);
    auto id = storage->openDataset(express_f_name.c_str());
    if (id < 0) return false;

    auto size = storage->getDatasetSize(id);

    std::unique_ptr<char[]> content = std::make_unique<char[]>(size);

    storage->readFromDataset(id, content.get(), size);


    auto e = llvm::json::parse(llvm::StringRef(content.get(), size));

    // llvm::outs()<<e.takeError();
    if (auto E = e.takeError()) {
        llvm::errs() << "Error " << toString(std::move(E)) << "\n";
    }
    llvm::json::Value &j_value = e.get();

    llvm::json::Object *j_object = j_value.getAsObject();
    llvm::json::Array *parameters_list = (*j_object)["Parameters.List"].getAsArray();

    (*parameters_list)[0].getAsObject();

    for (auto i : *parameters_list) {
        llvm::json::Object p_object = *i.getAsObject();

        if (p_object.find("Data.Fragments.List") != p_object.end()) {
            llvm::json::Array &data_files_list = *(p_object["Data.Files.List"].getAsArray());
            llvm::json::Array &data_fragments_list = *(p_object["Data.Fragments.List"].getAsArray());

            std::vector<ExDataInterval> interval_list;

            for (auto k : data_fragments_list) {
                auto interval = getDataInterval(k, data_files_list);
                interval.ds = storage;
                interval_list.push_back(std::move(interval));
            }
            llvm::json::Path::Root root("bare");
            llvm::json::ObjectMapper O(i, root);

            std::string name;


            bool ret = true;

            ret &= O.map(" Name", name);
            double begin_time = 0., end_time = 0.;
            ret &= O.map("sTime.Begin", begin_time);
            ret &= O.map("sTime.End", end_time);
            ExTimeInterval time_interval(begin_time, end_time - begin_time);
            int64_t int_type_representation = 0;
            ret &= O.map("Data.Type", int_type_representation);

            ParameterIfs *parameter = nullptr;
            if (isAsync(PrmTypesEn(int_type_representation)))
                parameter = new AsyncParameter(name, time_interval, interval_list);
            else parameter = new SyncParameter(name, time_interval, interval_list);
            // parameterList.size()==47
            parameter_list.push_back(parameter);
        }
    }
    return true;
}

ParameterIfs *retyping(ParameterIfs *a, PrmTypesEn target_ty, const std::string &name) {
    if (a == nullptr) return nullptr;
    return a->retyping(target_ty, name);
}

ParameterIfs *newParameter(const std::string &name, const std::vector<ExDataInterval> &interval_list,
                           bool save_fnames) {
    if (!interval_list.empty()) {
        if (isAsync(interval_list.front().type)) return new AsyncParameter(name, interval_list, save_fnames);
        else return new SyncParameter(name, interval_list, save_fnames);
    }
    return nullptr;
}

ParameterIfs *newParameter(const std::string &name, const ExTimeInterval &time_interval,
                           const std::vector<ExDataInterval> &interval_list, bool save_fnames) {
    if (!interval_list.empty()) {
        if (isAsync(interval_list.front().type)) return new AsyncParameter(name, time_interval, interval_list);
        else return new SyncParameter(name, time_interval, interval_list);
    }
    return nullptr;
}

ParameterIfs *intersection(ParameterIfs *a, ParameterIfs *b, PrmTypesEn target_ty, const std::string &name) {
    if (a == nullptr) return b;
    if (b == nullptr) return a;
    return a->intersection(b, target_ty, name);
}

ParameterIfs *intersection(std::vector<ParameterIfs *> arg_list, PrmTypesEn target_ty, const std::string &name) {
    std::set<ParameterIfs *> parameter_set;
    ParameterIfs *p = nullptr;
    for (auto i : arg_list) {
        p = intersection(p, i, target_ty, name);
        parameter_set.insert(p);
    }

    for (auto i : arg_list) parameter_set.erase(i);

    parameter_set.erase(nullptr);
    // if(p)
    parameter_set.erase(p);

    for (auto i : parameter_set) delete i;

    return p;
}
